/*
 * @Description: Load pulses from binary files
 * @Author: Ming Fang
 * @Date: 2021-03-26 23:41:11
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-29 20:36:24
 */
#include "loadEvents.h"


int Event::parse(const char* buffer, ULong64_t& bufIndex, std::vector<Double_t>& voltage)
{
    // read headers
    bufIndex += channelSetting.headers[0] + channelSetting.headers[1];
    std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[2],
            reinterpret_cast<char *>(&timeStampHeader));
    bufIndex += channelSetting.headers[2];
    std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[3],
            reinterpret_cast<char *>(&energyLongHeader));
    bufIndex += channelSetting.headers[3];
    std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[4],
            reinterpret_cast<char *>(&energyShortHeader));
    bufIndex += channelSetting.headers[4];
    std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[5],
            reinterpret_cast<char *>(&flagHeader));
    bufIndex = bufIndex + channelSetting.headers[5] + channelSetting.headers[6];

    // read waveform samples
    std::vector<uint16_t> waveform;
    waveform.reserve(channelSetting.sampleSize*channelSetting.length);
    std::memcpy(waveform.data(), &buffer[bufIndex], channelSetting.sampleSize*channelSetting.length);
    bufIndex = bufIndex + channelSetting.sampleSize * channelSetting.length;

    // processing
    Double_t heightTemp(0);

    //calculate the baseline.
    Double_t baseline = 0;
    baseline = std::accumulate(waveform.begin(), 
                               waveform.begin()+channelSetting.offset, 0);
    baseline = baseline / channelSetting.offset;

    //convert to voltage
    for (int k = 0; k < channelSetting.length; k++)
    {
        voltage[k] = channelSetting.polarityCoef *
                     (waveform[k] - baseline) * channelSetting.lsb2voltage;
    }

    // calculate pulse height and height index
    UShort_t heightindex; 
    getPulseHeight(voltage, heightindex);
    
    // calculate pulse integral and psd ratio
    getPulseIntegral(voltage);

    // energy calibration()
    calibration();

    // energy cut
    energyCut();

    // psd cut
    PSDCut();

    // rejection: zero supression, clip, pile-up
    rejection(voltage, heightindex);
    
    return 0;
}

int Event::getPulseHeight(const std::vector<Double_t>& v, UShort_t& heightindex)
{
    heightindex = std::max_element(v.begin(), v.end()) - v.begin();
    height = *(v.begin() + heightindex);
    return heightindex;
}

int Event::getPulseIntegral(const std::vector<Double_t>& v)
{
    totalIntegral = std::accumulate(v.begin()+channelSetting.preGate, 
                                    v.begin()+channelSetting.longGate, 0.0);
                                    
    tailIntegral = std::accumulate(v.begin()+channelSetting.preGate, 
                                   v.begin()+channelSetting.shortGate, 0.0);
    tailIntegral = totalIntegral - tailIntegral;
    return 0;
}

int Event::calibration()
{
    if (channelSetting.calibrationHeight)
    {
        energy = height * channelSetting.calibrationHeightCoefficient;
    }
    else
    {
        energy = totalIntegral * channelSetting.calibrationIntegralCoefficient;
    }
    
    return 0;
}

int Event::rejection(const std::vector<Double_t>& v, const UShort_t& i)
{
    if (isGood && channelSetting.rejection)
    {
        if (channelSetting.zeroSupression)
        {
            isGood = (height < channelSetting.lowVoltageThreshold) ? false : true;
        }
        

        if (isGood && channelSetting.clipRejection)
        {
            if(std::abs(v[i - 1] - height) < channelSetting.lsb2voltage ||
               std::abs(v[i + 1] - height) < channelSetting.lsb2voltage) 
            {
                isGood = false;
            }
        }

        // TODO
        // if (isGood && channelSetting.pileupRejection)
        // {
        //     pileupRejection();
        // }
    }
    return 0;
}

int Event::energyCut()
{
    if (isGood && channelSetting.energyCut)
    {
        if (energy > channelSetting.EnergyLowerThreshold ||
            energy < channelSetting.EnergyUpperThreshold)
        {
            isGood = false;
        }
    }
    
    return 0;
}

int Event::PSDCut()
{
    if (isGood && channelSetting.PSDCut)
    {
        double psdRatio = tailIntegral / totalIntegral;
        if (channelSetting.lazyCut)
        {
            if (psdRatio > channelSetting.PSDLowerThreshold ||
                psdRatio < channelSetting.PSDUpperThreshold)
            {
                isGood = false;
            }
        }
        else if (channelSetting.quadraticCut)
        {
            double tmp = (channelSetting.quadraticCoefficients[0] * totalIntegral +
                          channelSetting.quadraticCoefficients[1]) * totalIntegral +
                         channelSetting.quadraticCoefficients[2];
            if (psdRatio < tmp)
            {
                isGood = false; // gamma pulses
            }
        }
    }
    return 0;
}



int Channel::loadEvents()
{
    voltage.resize(channelSetting.length, 0);
    // buffer size = 64MB
    ULong_t bufSize = 1024*1024*64 / channelSetting.eventSize;
    bufSize = bufSize * channelSetting.eventSize;
    char * buffer = new char[bufSize]; 

    UInt_t currentNumber = 0;
    UInt_t goodCounts = 0;
    UInt_t badCounts = 0;
    std::ifstream fileptr;
    fileptr.open(channelSetting.path, std::ios::in | std::ios::binary);
    std::cout << "Open file: " << channelSetting.path << std::endl;
    if (!fileptr.good())
    {
        throw std::invalid_argument(channelSetting.path + "does not exit.");
    }
    else
    {
        std::cout << " Reading data: " << std::endl;
    }
    
    ULong64_t bufIndex = 0;
    while (!fileptr.eof() && currentNumber < channelSetting.maxNumPulses)
    {
        fileptr.read(buffer, bufSize);
        bufSize = fileptr.gcount();
        bufSize = bufSize / channelSetting.eventSize;
        bufSize = bufSize * channelSetting.eventSize;
        bufIndex = 0;

        // extract pulses
        while (bufIndex < bufSize && currentNumber < channelSetting.maxNumPulses)
        {
            Event newPulse(channelSetting);
            newPulse.parse(buffer, bufIndex, voltage);
            currentNumber++;
            if (newPulse.isGood)
            {
                goodCounts++;
                if (goodCounts <= channelSetting.goodNumber ||
                    goodCounts <= channelSetting.savePulses)
                {
                    goodPulses.push_back(voltage);
                }
                if (channelSetting.timing)
                {
                    std::vector<double> interpolated(channelSetting.windowSize * channelSetting.interpolationPoints, 0);
                    sincInterpolation(interpolated);
                    // interpPulses.push_back(interpolated);
                    timing(newPulse);
                }
                events.push_back(newPulse);
            }
            else
            {
                badCounts++;
                if (badCounts <= channelSetting.badNumber ||
                    badCounts <= channelSetting.savePulses)
                {
                    badPulses.push_back(voltage);
                }
            }
        }
        std::cout <<  "Total counts = " << currentNumber << '\t'
                  << " good counts = " << goodCounts << '\t'
                  << " rejected counts = " << badCounts << '\n';
    }
    fileptr.close();

    delete[] buffer;
    return 0;
}

int Channel::sincInterpolation(std::vector<Double_t>& interpolated)
{
    for (std::size_t i = 0; i < interpolated.size(); i++)
    {
        interpolated[i] = getInterpPoint(i);
    }
    
    return 0;
}

double Channel::getInterpPoint(const int& i)
{
    int k = i % channelSetting.interpolationPoints;
    int j = i / channelSetting.interpolationPoints + channelSetting.startIndex;
    if (k==0)
    {
        return voltage[j];
    }
    else
    {
        double y(0);
        for (int l = 0; l < channelSetting.tsincWidth; l++)
        {
            y = y + voltage[j-l] * channelSetting.sincCoefs[l * channelSetting.interpolationPoints + k] 
                                      + voltage[j + 1 + l] * channelSetting.sincCoefs[(l + 1)*channelSetting.interpolationPoints - k];
        }
        return y;
    }
}

int Channel::getInterpMax(double& ymax, int& imax)
{
    imax = std::max_element(voltage.begin(), voltage.end()) - voltage.begin();
    int low = (voltage[imax - 1] > voltage[imax + 1]) ? (imax - 1) : imax;
    int high = (voltage[imax - 1] > voltage[imax + 1]) ? imax : (imax + 1);
    low = (low - channelSetting.startIndex) * channelSetting.interpolationPoints;
    high = (high - channelSetting.startIndex) * channelSetting.interpolationPoints;
    imax = findMax(low, high);
    ymax = getInterpPoint(imax);
    return 0;
}

int Channel::findMax(int low, int high)
{
    if (low > high)
    {
        return low;
    }
    if (high - low < 2) 
    {
        return (getInterpPoint(low) > getInterpPoint(high)) ? low : high;
    }
    int mid = (low+high) / 2;
    int left = findMax(low, mid-1);
    int right = findMax(mid+1, high);
    return (getInterpPoint(left) < getInterpPoint(mid)) && (getInterpPoint(right) < getInterpPoint(mid)) ? mid : (getInterpPoint(left) > getInterpPoint(right) ? left : right);
}

int Channel::timing(Event& event)
{
    // // DIACFD
    double ymax(0);
    int imax(0);
    getInterpMax(ymax, imax);
    const double& f = channelSetting.cfdFraction;
    const int& d = channelSetting.cfdFraction;

    // solve f * y[i] - y[i+\delta] = 0
    int low;
    if (f * ymax - getInterpPoint(imax + d) > 0)
    {
        // zero crossing point inside (imax, imax + delta)
        // binary search
        low = findZeroBinary(imax, imax +d);
    }
    else
    {
        low = findZeroBinary(0, imax);
    }
    // coarse time stamp at startIndex
    ULong64_t coarseTimeStamp = (event.timeStampHeader / 2000  - channelSetting.preGate + channelSetting.startIndex) * 2;
    ymax =  getInterpPoint(low) / (getInterpPoint(low) - getInterpPoint(low + 1)) + low; 
    event.timeStampDACFD = ymax * channelSetting.findTimeReso + coarseTimeStamp;
    return 0;
}

int Channel::findZeroBinary(int low, int high)
{
    // base case
    if (high - low < 2)
    {
        return low;
    }
    int mid = (low + high) / 2;
    if (getInterpPoint(mid) > 0)
    {
        return findZeroBinary(mid, high);
    }
    else
    {
        return findZeroBinary(low, mid);
    }
}
int Channel::getPulseGraph(const std::vector<std::vector<double_t>>& pulses, const UInt_t& pulseNum, 
                           TMultiGraph* multiGraph, const std::string& plotName, const int& pts)
{
    UInt_t N = channelSetting.length;
    if (pulses.size()!=0)
    {
        N = pulses[0].size();
        /* code */
    }
    
    Double_t x_vals[N];
    Double_t gateHeight(0);
    for (int i = 0; i < N; i++)
    {
        x_vals[i] = channelSetting.timestep * i;
        x_vals[i] /= pts;
    }
    
    const UInt_t& M = (pulseNum < pulses.size()) ? pulseNum : pulses.size();
    std::vector<TGraph> graphs;
    for (std::size_t i = 0; i < M; i++)
    {
        graphs.push_back(TGraph(N, x_vals, &(pulses[i][0])));
        Double_t tmp = *(std::max_element(pulses[i].begin(), pulses[i].end()));
        gateHeight = (gateHeight < tmp) ? tmp : gateHeight;
    }
    
    if (M == 0)
    {
        gateHeight = 1.0;
    }
    
    // Long gate
    Double_t longgate_vals[N];
    for (int i = 0; i < N; i++)
    {
        if(i < channelSetting.preGate || i >= channelSetting.longGate)
        {
            longgate_vals[i] = 0.7 * gateHeight;
        }
        else
        {
            longgate_vals[i] =  0.9 * gateHeight;
        }
    }
    graphs.push_back(TGraph(N, x_vals, &(longgate_vals[0])));

    // Short Gate
    Double_t shortgate_vals[N];
    for (int i = 0; i < N; i++)
    {
        if(i < channelSetting.preGate || i >= channelSetting.shortGate)
        {
            shortgate_vals[i] = 0.4 * gateHeight;
        }
        else
        {
            shortgate_vals[i] = 0.5 * gateHeight;
        }
    }
    graphs.push_back(TGraph(N, x_vals, &(shortgate_vals[0])));

    for (int i = 0; i < graphs.size(); i++)
    {
        multiGraph->Add((TGraph*)graphs[i].Clone(),"lp");
    }
    multiGraph->SetTitle(plotName.c_str());
    multiGraph->GetXaxis()->SetTitle("Time (ns)");
    multiGraph->GetYaxis()->SetTitle("Voltage (V)");

    return 0;

}

int Channel::getGoodPulseGraph(const std::string& plotName)
{
    getPulseGraph(goodPulses, channelSetting.goodNumber, &goodPulseGraph, plotName);
    return 0;
}

int Channel::getBadPulseGraph(const std::string& plotName)
{
    getPulseGraph(badPulses, channelSetting.badNumber, &badPulseGraph, plotName);

    return 0;
}

int Channel::getinterpPulseGraph(const std::string& plotName)
{
    // interpPulses = std::vector<std::vector<Double_t>>(2, channelSetting.sincCoefs);
    getPulseGraph(interpPulses, interpPulses.size(), &interpPulseGraph, plotName, channelSetting.interpolationPoints);
    return 0;
}

int Channel::getPHD(const std::string& plotName)
{
    if (!channelSetting.plotPHD)
    {
        return 0;
    }
    
    PHD = TH1D(plotName.c_str(), plotName.c_str(), 
               channelSetting.PHDBins, 
               channelSetting.PHDXmin, channelSetting.PHDXmax);
    if (channelSetting.plotPHDCalibrated)
    {
        PHD.GetXaxis()->SetTitle("Light Output (keVee)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PHD.Fill(events[i].height * channelSetting.calibrationHeightCoefficient);
        }
    }
    else
    {
        PHD.GetXaxis()->SetTitle("Pulse height (V)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PHD.Fill(events[i].height);
        }
    }
    PHD.GetYaxis()->SetTitle("Counts");
    
    return 0;
}

int Channel::getPID(const std::string& plotName)
{
    if (!channelSetting.plotPID)
    {
        return 0;
    }
    
    PID = TH1D(plotName.c_str(), plotName.c_str(), 
               channelSetting.PIDBins, 
               channelSetting.PIDXmin, channelSetting.PIDXmax);
    if (channelSetting.plotPIDCalibrated)
    {
        PID.GetXaxis()->SetTitle("Light Output (keVee)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PID.Fill(events[i].totalIntegral * channelSetting.calibrationIntegralCoefficient);
        }
    }
    else
    {
        PID.GetXaxis()->SetTitle("Total integral (V * ns / dt)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PID.Fill(events[i].totalIntegral);
        }
    }
    PID.GetYaxis()->SetTitle("Counts");

    return 0;
}

int Channel::getPSD(const std::string& plotName)
{
    if (!channelSetting.plotPSD)
    {
        return 0;
    }
    
    PSD = TH2D(plotName.c_str(), plotName.c_str(), 
            channelSetting.PSDXBins, channelSetting.PSDXmin, channelSetting.PSDXmax,
            channelSetting.PSDYBins, channelSetting.PSDYmin, channelSetting.PSDYmax);
    if (channelSetting.plotPSDCalibrated)
    {
        PSD.GetXaxis()->SetTitle("Light Output (keVee)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PSD.Fill(events[i].totalIntegral * channelSetting.calibrationIntegralCoefficient, 
                     events[i].tailIntegral / events[i].totalIntegral);  
        }
    }
    else
    {
        PSD.GetXaxis()->SetTitle("Total integral (V * ns / dt)");
        for (std::size_t i = 0; i < events.size(); i++)
        {
            PSD.Fill(events[i].totalIntegral, 
                     events[i].tailIntegral / events[i].totalIntegral);  
        }
    }
    PSD.GetYaxis()->SetTitle("Tail-Total ratio");
    return 0;
}