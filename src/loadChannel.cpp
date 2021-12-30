/**
 * @file loadChannel.cpp
 * @author Ming Fang
 * @brief Process events of a single channel from a binary input file
 * @date 2021-03-30
 */

#include "../include/loadChannel.h"

int Channel::loadEvents()
{
    if (!channelSetting.processOutput)
    {
        std::cout << "Warning: Pulse processing of channel " << channelSetting.channelNumber << "is disabled." << std::endl;
        return 0;
    }
    
    voltage.resize(channelSetting.length, 0);
    // buffer size = 64MB
    ULong_t bufSize = 1024*1024*channelSetting.bufferSize / channelSetting.eventSize;
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
                timing(newPulse);
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
        std::cout << " Total counts = " << currentNumber << '\t'
                  << " good counts = " << goodCounts << '\t'
                  << " rejected counts = " << badCounts << '\n';
    }
    fileptr.close();

    delete[] buffer;
    return 0;
}

int Channel::getInterpPulse(std::vector<Double_t>& interpolated)
{
    for (std::size_t i = 0; i < interpolated.size(); i++)
    {
        interpolated[i] = getInterpPoint(i);
    }
    
    return 0;
}

int Channel::getBipolarPulse(std::vector<Double_t>& interpolated)
{
    for (std::size_t i = channelSetting.timeDelay; i < interpolated.size(); i++)
    {
        interpolated[i] = getBipolarPoint(i);
    }
    
    return 0;
}

double Channel::getBipolarPoint(const int&i)
{
    return channelSetting.cfdFraction * getInterpPoint(i) - getInterpPoint(i-channelSetting.timeDelay);
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
    if (channelSetting.timing)
    {
        // std::vector<double> interpolated(channelSetting.windowSize * channelSetting.interpolationPoints, 0);
        // // getInterpPulse(interpolated);
        // getBipolarPulse(interpolated);
        // interpPulses.push_back(interpolated);
        if (channelSetting.timingMethod=="DIACFD")
        {
            timingDIACFD(event);
        }
        else if (channelSetting.timingMethod=="DCFD")
        {
            timingDCFD(event);
        }
        else
        {
            throw std::invalid_argument(channelSetting.timingMethod + "is not available.");
        }
        
    }
    return 0;
}

int Channel::timingDCFD(Event& event) 
{
    // // DCFD
    double ymax(0);
    int imax(0);
    getInterpMax(ymax, imax);

    // solve y[i] - f * ymax = 0
    ymax *= channelSetting.cfdFraction;
    int low = findInterp(0, imax, ymax);

    // coarse time stamp at startIndex
    ULong64_t coarseTimeStamp = (event.timeStampHeader / 2000  - channelSetting.preTrig + channelSetting.startIndex) * 2;
    ymax =  (getInterpPoint(low) - ymax) / (getInterpPoint(low) - getInterpPoint(low + 1)) + low; 
    event.timeStampDACFD = ymax * channelSetting.findTimeReso + coarseTimeStamp;
    return 0;
}

int Channel::timingDIACFD(Event& event)
{
    // // DIACFD
    double ymax(0);
    int imax(0);
    getInterpMax(ymax, imax);

    // solve f * y[i] - y[i+\delta] = 0
    int low;
    if (getBipolarPoint(imax) > 0)
    {
        // zero crossing point inside (imax, imax + delta)
        // binary search
        low = findZeroBipolar(imax, imax + channelSetting.timeDelay);
    }
    else
    {
        low = findZeroBipolar(channelSetting.timeDelay, imax);
    }

    // coarse time stamp at startIndex
    ULong64_t coarseTimeStamp = (event.timeStampHeader / 2000  - channelSetting.preTrig + channelSetting.startIndex) * 2;
    ymax =  getBipolarPoint(low) / (getBipolarPoint(low) - getBipolarPoint(low + 1)) + low; 
    event.timeStampDACFD = ymax * channelSetting.findTimeReso + coarseTimeStamp;
    return 0;
}

int Channel::findInterp(int low, int high, const double& value)
{
    // base case
    if (high - low < 2)
    {
        return low;
    }
    int mid = (low + high) / 2;
    if (getInterpPoint(mid) < value)
    {
        return findInterp(mid, high, value);
    }
    else
    {
        return findInterp(low, mid, value);
    }
}

int Channel::findZeroBipolar(int low, int high)
{
    // base case
    if (high - low < 2)
    {
        return low;
    }
    int mid = (low + high) / 2;
    if (getBipolarPoint(mid) > 0)
    {
        return findZeroBipolar(mid, high);
    }
    else
    {
        return findZeroBipolar(low, mid);
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

int Channel::saveGoodPulses(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + "cannot be written to.");
    }
    
    const uint outN = (channelSetting.savePulses < goodPulses.size()) ? channelSetting.savePulses : goodPulses.size();
    for (std::size_t i = 0; i < outN; i++)
    {
        for (std::size_t j = 0; j < channelSetting.length; j++)
        {
            outf << goodPulses[i][j] << '\t';
        }
        outf << '\n';
    }
    outf.close();
    
    return 0;
}
int Channel::saveBadPulses(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + "cannot be written to.");
    }
    
    const uint outN = (channelSetting.savePulses < badPulses.size()) ? channelSetting.savePulses : badPulses.size();
    for (std::size_t i = 0; i < outN; i++)
    {
        for (std::size_t j = 0; j < channelSetting.length; j++)
        {
            outf << badPulses[i][j] << '\t';
        }
        outf << '\n';
    }
    outf.close();

    return 0;
}
int Channel::savePH(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + "cannot be written to.");
    }

    outf << "Pulse height (V)\n";
    for (std::size_t i = 0; i < events.size(); i++)
    {
        outf << events[i].height << '\n';
    }
    outf.close();

    return 0;
}
int Channel::savePI(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + " cannot be written to.");
    }

    outf << "Total int.(V*ns/dt)\n";
    for (std::size_t i = 0; i < events.size(); i++)
    {
        outf << events[i].totalIntegral << '\n';
    }
    outf.close();

    return 0;
}
int Channel::saveTimeStamp(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + "cannot be written to.");
    }
    outf << std::fixed << std::setprecision(5);
    if (channelSetting.timing && channelSetting.timingMethod=="DIACFD")
    {
        outf << "Header (ps)\tDIACFD(ns)\n";
        for (std::size_t i = 0; i < events.size(); i++)
        {
            outf << events[i].timeStampHeader << '\t'  << events[i].timeStampDACFD << '\n';
        }
    }
    else if (channelSetting.timing && channelSetting.timingMethod=="DCFD")
    {
        outf << "Header (ps)\tDCFD(ns)\n";
        for (std::size_t i = 0; i < events.size(); i++)
        {
            outf << events[i].timeStampHeader << '\t' << events[i].timeStampDACFD << '\n';
        }
    }
    else
    {
        outf << "Header (ps)\t CFD disabled\n";
        for (std::size_t i = 0; i < events.size(); i++)
        {
            outf << events[i].timeStampHeader << '\t' << 0 << '\n';
        }
    }
    outf.close();

    return 0;
}
int Channel::saveIntegrals(const std::string& filename)
{
    std::ofstream outf;
    outf.open(filename);
    if (!outf.good())
    {
        throw std::invalid_argument(filename + "cannot be written to.");
    }

    outf << "Total int.\tTail int.(V*ns/dt)\n";
    for (std::size_t i = 0; i < events.size(); i++)
    {
        outf << events[i].totalIntegral << '\t' << events[i].tailIntegral << '\n';
    }
    outf.close();

    return 0;
}