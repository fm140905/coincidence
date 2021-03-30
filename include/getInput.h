/*
 * @Description: Get input parameters from a json file
 * @Author: Ming Fang
 * @Date: 2021-03-26 12:41:05
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-29 20:30:40
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>

#include "TFile.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>


struct ChannelSettings
{
private:
    /*data*/
public:
    int getSampleNumber();
    std::string path;
    uint channelNumber=0;
    bool processOutput=false;
    
    ULong64_t maxNumPulses=-1;
    std::string polarity;
    int polarityCoef=-1;
    std::vector<int> headers;
    std::vector<bool> saveHeaders;
    int sampleSize=4;
    int eventSize=0;
    int length=288;
    double dynamicRange=0.5;
    double lsb2voltage=1;
    int timestep=4;
    int resolution=12;
    int offset=16;
    double dcOffset=0.1;
    int savePulses=1;

    int preTrig=4;
    int preGate=2;
    int shortGate=10;
    int longGate=30;

    bool calibrationIntegral=false;
    double calibrationIntegralCoefficient=1.0;
    bool calibrationHeight=false;
    double calibrationHeightCoefficient=1.0;

    bool rejection=false;
    bool zeroSupression=false;
    double lowVoltageThreshold=0.01;
    bool clipRejection=false;

    bool energyCut=false;
    double EnergyLowerThreshold=0.0;
    double EnergyUpperThreshold=1.0;

    bool PSDCut=false;
    bool lazyCut=true;
    double PSDLowerThreshold=0.0;
    double PSDUpperThreshold=1.0;
    bool quadraticCut=false;
    std::vector<double> quadraticCoefficients;
    
    bool timing=false;
    std::string timingMethod="DIACFD";
    int windowSize=16; // 16 samples
    int startIndex=0; // start point for interpolation
    double cfdFraction=0.2;
    int timeDelay=6;
    int interpolationPoints=1;
    int tsincWidth=6; // 6 lobes of tsinc function
    int taperConst=30; // tapering constant
    double findTimeReso=0; // time resolution after interpolation
    std::vector<double> sincCoefs;

    bool plotGoodPulses=false;
    int goodNumber=0;
    bool plotBadPulses=false;
    int badNumber=0;

    bool plotPHD=false;
    bool plotPHDCalibrated=false;
    double PHDXmin=0.0;
    double PHDXmax=0.0;
    int PHDBins=1;
    bool plotPID=false;
    bool plotPIDCalibrated=false;
    double PIDXmin=0.0;
    double PIDXmax=0.0;
    int PIDBins=1;

    bool plotPSD=false;
    bool plotPSDCalibrated=false;
    double PSDXmin=0.0;
    double PSDXmax=0.0;
    double PSDYmin=0.0;
    double PSDYmax=0.0;
    int PSDXBins=1;
    int PSDYBins=1;

    int timingSetup();
};

struct CoincidenceSetting
{
    bool coincidence=false;
    std::pair<uint, uint> coincidentChannels;
    uint timeWindow=100;
    bool saveTimeDifference=false;
    bool plotTOF=false;
    double xmin=-20;
    double xmax=20;
    int bins=200;
};


class InputParameters
{
private:
    /* data */
    rapidjson::Document jsonInput;
    std::string inputFilepath;
    ChannelSettings defaultChannelSetting;
    int getChannelSetting(const rapidjson::Value& v, ChannelSettings& chset);
    int getDefaultChannelSetting();
    int getSpecificChannelSettings();
    int getCoincidenceSetting();
public:
    std::vector<ChannelSettings> channelSettings;
    CoincidenceSetting coincidenceSetting;
    InputParameters(const std::string fpath);

    // ~getInput() {}
};


inline std::string makeString(const char *x)
{
    return x;
}