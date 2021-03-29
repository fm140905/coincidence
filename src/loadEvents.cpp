/*
 * @Description: Load pulses from binary files
 * @Author: Ming Fang
 * @Date: 2021-03-26 23:41:11
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-29 01:04:07
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
            std::vector<double> voltage(channelSetting.length, 0);
            Event newPulse(channelSetting);
            newPulse.parse(buffer, bufIndex, voltage);
            currentNumber++;
            if (newPulse.isGood)
            {
                events.push_back(newPulse);
                goodCounts++;
                if (goodCounts <= channelSetting.goodNumber ||
                    goodCounts <= channelSetting.savePulses)
                {
                    goodPulses.push_back(voltage);
                }
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

int Channel::getPulseGraph(const std::vector<std::vector<double_t>>& pulses, const UInt_t& pulseNum, 
                           TMultiGraph* multiGraph, const std::string& plotName)
{
    const UInt_t& N = channelSetting.length;
    Double_t x_vals[N];
    Double_t gateHeight(0);
    for (int i = 0; i < N; i++)
    {
        x_vals[i] = channelSetting.timestep * i;
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