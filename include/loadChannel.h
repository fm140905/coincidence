/*
 * @Description: read single channel
 * @Author: Ming Fang
 * @Date: 2021-03-30 13:52:07
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-30 13:56:33
 */

#pragma once


#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include "getInput.h"
#include "loadEvents.h"
#include "event.h"

class Channel
{
private:
    const ChannelSettings& channelSetting;
    int getPulseGraph(const std::vector<std::vector<double_t>>& pulses, const UInt_t& pulseNum, 
                           TMultiGraph* multiGraph, const std::string& plotName,
                           const int& pts=1);

    std::vector<Double_t> sincCoeffs;
    int getInterpPulse(std::vector<Double_t>& interpolated);
    double getInterpPoint(const int& i);

    int getBipolarPulse(std::vector<Double_t>& interpolated);
    double getBipolarPoint(const int&i);

    int getInterpMax(double& ymax, int& imax);
    int findMax(int low, int high);
    int findInterp(int low, int high, const double& value);
    int findZeroBipolar(int low, int high);
public:
    std::vector<double> voltage;
    std::vector<Event> events;
    Channel(const ChannelSettings& sett): channelSetting(sett) {}
    // ~Channel();
    int loadEvents();

    int timing(Event& event);
    int timingDCFD(Event& event);
    int timingDIACFD(Event& event);
    
    std::vector<std::vector<double_t>> goodPulses;
    std::vector<std::vector<double_t>> badPulses;
    TMultiGraph goodPulseGraph;
    int getGoodPulseGraph(const std::string& plotName);
    TMultiGraph badPulseGraph;
    int getBadPulseGraph(const std::string& plotName);
    // for bedug
    std::vector<std::vector<double_t>> interpPulses;
    TMultiGraph interpPulseGraph;
    int getinterpPulseGraph(const std::string& plotName);
    
    TH1D PHD;
    int getPHD(const std::string& plotName);
    TH1D PID;
    int getPID(const std::string& plotName);

    TH2D PSD;
    int getPSD(const std::string& plotName);
};