/**
 * @file loadEvents.cpp
 * @author Ming Fang
 * @brief Load single event. Perform calibration and rejection.
 * @date 2021-03-30
 */

#include "loadEvents.h"


int Event::parse(const char* buffer, ULong64_t& bufIndex, std::vector<Double_t>& voltage)
{
    // read headers

    // read digitizer time stamp
    bufIndex += channelSetting.headers[0] + channelSetting.headers[1];
    std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[2],
            reinterpret_cast<char *>(&timeStampHeader));
    bufIndex += std::accumulate(channelSetting.headers.begin()+2, channelSetting.headers.end(), 0);
    // bufIndex += channelSetting.headers[2];

    // // skip other headers
    // std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[3],
    //         reinterpret_cast<char *>(&energyLongHeader));
    // bufIndex += channelSetting.headers[3];
    // std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[4],
    //         reinterpret_cast<char *>(&energyShortHeader));
    // bufIndex += channelSetting.headers[4];
    // std::copy(&buffer[bufIndex], &buffer[bufIndex] + channelSetting.headers[5],
    //         reinterpret_cast<char *>(&flagHeader));
    // bufIndex = bufIndex + channelSetting.headers[5] + channelSetting.headers[6];

    // read waveform samples
    std::vector<uint16_t> waveform;
    waveform.resize(channelSetting.length);
    std::memcpy(waveform.data(), &buffer[bufIndex], channelSetting.sampleSize*channelSetting.length);
    bufIndex = bufIndex + channelSetting.sampleSize * channelSetting.length;

    // processing

    //calculate the baseline.
    Double_t baseline = 0;
    if (channelSetting.reversebaseline)
    {
        baseline = std::accumulate(waveform.rbegin(), 
                                waveform.rbegin()+channelSetting.offset, 0);
    }
    else
    {
        baseline = std::accumulate(waveform.begin(), 
                                waveform.begin()+channelSetting.offset, 0);
    }
    
    
    baseline = baseline / channelSetting.offset;

    //convert unit to voltage
    for (int k = 0; k < channelSetting.length; k++)
    {
        voltage[k] = channelSetting.polarityCoef *
                     (waveform[k] - baseline) * channelSetting.lsb2voltage;
    }

    // calculate pulse height and height index
    Int_t heightindex; 
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

int Event::getPulseHeight(const std::vector<Double_t>& v, Int_t& heightindex)
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

int Event::rejection(const std::vector<Double_t>& v, const Int_t& i)
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

        if (isGood && channelSetting.pileupRejection)
        {
            pileupRejection(v, i);
        }
    }
    return 0;
}

int Event::pileupRejection(const std::vector<Double_t>& v, const Int_t heightIndex)
{
    const int dpjump = channelSetting.riseTime;
    const float dpf = channelSetting.peakRatioLowerThreshold; // double pulse fraction
    const float threshV = channelSetting.peakHeightLowerThreshold; // the noise level
    float deltaV = 0;
        
    for (int j = heightIndex; j < channelSetting.length - dpjump; j++)
    {
        // detect the rising edge of the second pulse after the trigger pulse
        deltaV = v[j+dpjump] - v[j];
        // threshV = 
        if (deltaV > dpf * height && deltaV > threshV)
        {
            isGood = false;
            return 0;
        }
    }
    for (int j = 0; j < heightIndex - 2 * dpjump; j++)
    {
        // detect the rising edge of the second pulse before the trigger pulse
        deltaV = v[j+dpjump] - v[j];
        if (deltaV > dpf * height && deltaV > threshV)
        {
            isGood=false;
            return 0;
        }
    }
    return 0;
}

int Event::energyCut()
{
    if (isGood && channelSetting.energyCut)
    {
        if (energy < channelSetting.EnergyLowerThreshold ||
            energy > channelSetting.EnergyUpperThreshold)
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
        if (channelSetting.lazyCut)
        {
            double psdRatio = tailIntegral / totalIntegral;
            if (psdRatio < channelSetting.PSDLowerThreshold ||
                psdRatio > channelSetting.PSDUpperThreshold)
            {
                isGood = false;
            }
        }
        else if (channelSetting.quadraticCut)
        {
            double tmp = (channelSetting.quadraticCoefficients[0] * totalIntegral +
                          channelSetting.quadraticCoefficients[1]) * totalIntegral +
                         channelSetting.quadraticCoefficients[2];
            if (tailIntegral < tmp)
            {
                isGood = false; // gamma pulses
            }
        }
    }
    return 0;
}
