/*
 * @Description: Get input parameters from a json file
 * @Author: Ming Fang
 * @Date: 2021-03-26 12:41:05
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-26 23:28:00
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "TFile.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>


struct ChannelSettings
{
    std::string path;
    uint channelNumber=0;
    bool processOutput=false;
    
    ULong64_t maxNumPulses=-1;
    std::string polarity;
    std::vector<int> headers;
    std::vector<bool> saveHeaders;
    int sampleSize=4;
    int length=288;
    double dynamicRange=0.5;
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
    double cfdFraction=0.2;
    int timeDelay=6;
    int interpolationPoints=1;
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