/*
 * @Description: load single event
 * @Author: Ming Fang
 * @Date: 2021-03-26 12:31:48
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-30 13:54:17
 */

#pragma once


#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "TFile.h"
// #include "TCanvas.h"
// #include "TH1D.h"
// #include "TH2D.h"
// #include "TGraph.h"
// #include "TMultiGraph.h"

#include "getInput.h"

class Event
{
private:
    int getPulseHeight(const std::vector<Double_t>& v, UShort_t& heightindex);
    int getPulseIntegral(const std::vector<Double_t>& v);
    int calibration();
    int rejection(const std::vector<Double_t>& v, const UShort_t& i);
    int pileupRejection();
    int energyCut();
    int PSDCut();
    const ChannelSettings& channelSetting;
public:
    ULong64_t timeStampHeader = 0;
    uint16_t energyShortHeader=0;
    uint16_t energyLongHeader=0;
    uint32_t flagHeader=0;
    
    Double_t timeStampDACFD = 0;
    Double_t height = 0;
    Double_t totalIntegral = 0;
    Double_t energy=0;
    Double_t tailIntegral = 0;
    Bool_t isGood = true;
    Event(const ChannelSettings& sett): channelSetting(sett) {}
    // ~Event();
    int parse(const char* buffer, ULong64_t& bufIndex, std::vector<Double_t>& voltage);
};




