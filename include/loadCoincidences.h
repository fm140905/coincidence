/*
 * @Description: Load coincidences from input
 * @Author: Ming Fang
 * @Date: 2021-03-29 01:14:19
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-30 14:02:29
 */

#pragma once

#include "TH1D.h"

#include "getInput.h"
#include "loadEvents.h"
#include "loadChannel.h"

class CoincidenceChannel
{
private:
    /* data */
    const CoincidenceSetting& coincidenceSetting;
    int getTOF(const std::vector<Double_t>& timeDiff, TH1D& TOF, const std::string& plotname);
public:
    // CoincidenceChannel(const InputParameters& input): coincidenceSetting(input.coincidenceSetting) {}
    CoincidenceChannel(const CoincidenceSetting& input, const Channel& ch0, const Channel& ch1): coincidenceSetting(input), channel0(ch0), channel1(ch1) {}
    const Channel& channel0;
    const Channel& channel1;
    std::vector<std::pair<UInt_t, UInt_t>> pairIndices;
    int findCoincidences();
    std::vector<Double_t> timeDiffHeader;
    std::vector<Double_t> timeDiffDIACFD;
    
    //TODO
    TH1D TOFDIACFD;
    int getTimeDiffDIACFD();
    int getTOFDIACFD(const std::string& plotname);
    
    TH1D TOFHeader;
    int getTOFHeader(const std::string& plotname);
    // ~CoincidenceChannel();
};
