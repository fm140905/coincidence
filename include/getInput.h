/**
 * @file getInput.h
 * @author Ming Fang
 * @brief Get input parameters from a json file
 * @date 2021-03-30
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

/**
 * Saves user input parameters for processing single channel data. 
 * 
 */
struct ChannelSettings
{
private:
    /*data*/
public:
    /**
     * @brief Get the number of waveform sample
     * 
     * @return int 
     */
    int getSampleNumber();
    int bufferSize=512; // size of data chunk to process, in MB
    std::string path; // path to binary file
    uint channelNumber=0; // channel number
    bool processOutput=false; // enable processing if true
    
    ULong64_t maxNumPulses=-1; // max number of pulses to process
    std::string polarity; // pulse polarity, either "negative" or "positive"
    int polarityCoef=-1; // pulse polarity, either -1 or 1
    std::vector<int> headers; // size of each header in bytes
    std::vector<bool> saveHeaders=std::vector<bool>(7, false);
    int sampleSize=4; // size of waveform sample in bytes
    int eventSize=0; // total size of a single event in bytes
    int length=288; // number of samples in a waveform
    double dynamicRange=0.5; // dynamic range of the digitizer
    double lsb2voltage=1; // conversion from LSB to volt
    int timestep=4; // digitizer time resolution in ns
    int resolution=12; // digitizer ADC resolution in bits
    int offset=16; // name of samples to calculate the baseline
    bool reversebaseline=false; // baseline location, head or tail of the pulse
    double dcOffset=0.1; // DC offset
    int savePulses=1; // number of pulses to be saved

    int preTrig=4; // CoMPASS PreTrigger, in ns
    int preGate=2; // CoMPASS PreGate, in ns
    int shortGate=10; // CoMPASS short date, in ns
    int longGate=30; // CoMPASS long gate, in ns

    bool calibrationIntegral=false; // calibration based on integral if true
    double calibrationIntegralCoefficient=1.0; // integral-to-energy calibration coefficient
    bool calibrationHeight=false; // calibration based on pulse amplitude if true
    double calibrationHeightCoefficient=1.0; // pulse amplitude-to-energy calibration coefficient

    bool rejection=false; // enable pulse rejection if true
    bool zeroSupression=false; // enable zero supression if true
    double lowVoltageThreshold=0.01; // minimum pulse amplitude for zero supression
    bool clipRejection=false; // reject clipped pulse if true

    bool energyCut=false; // enable energy cut if true
    double EnergyLowerThreshold=0.0; // lower energy threshold for energy cut
    double EnergyUpperThreshold=1.0; // upper energy threshold for energy cut

    bool PSDCut=false; // enable PSD cut if true
    bool lazyCut=true; // perform straight line cut if true
    double PSDLowerThreshold=0.0; // lower tail-total ratio threshold for lazy PSD cut 
    double PSDUpperThreshold=1.0; // upper tail-total ratio threshold for lazy PSD cut
    bool quadraticCut=false; // perform quadratic PSD cut if true
    // quadratic PSD line coefficients in order of 
    // quadratic coefficient, the linear coefficient and the constant or free term.
    std::vector<double> quadraticCoefficients;
    
    bool timing=false; // enable timing if true
    std::string timingMethod="DIACFD"; // timing method, either DIACFD or DCFD
    int windowSize=16; // approximately 2*pulse rise time, in number of samples
    int startIndex=0; // waveform index of start point for interpolation
    double cfdFraction=0.2; // CFD fraction, 0 to 1
    int timeDelay=6; // CFD delay, in ns
    int interpolationPoints=1; // number of parts a time step is divided into after interpolation
    int tsincWidth=6; // number of lobes of tsinc function used in interpolation, default to 6 
    int taperConst=30; // tapering constant, default=30
    double findTimeReso=0; // time resolution after interpolation, in ns
    std::vector<double> sincCoefs; // tsinc interpolation coefficients

    bool plotGoodPulses=false; // plot good pulses if true
    int goodNumber=0; // number of good pulses to plot
    bool plotBadPulses=false; // plot bad pulses if true
    int badNumber=0; // number of bad pulses to plot

    bool plotPHD=false; // plot pulse height distribution if true
    bool plotPHDCalibrated=false; // calibrate pulse height distribution if true
    double PHDXmin=0.0; // lower boundary of the pulse height distribution
    double PHDXmax=0.0; // upper boundary of the pulse height distribution
    int PHDBins=1; // number of bins in pulse height distribution
    bool plotPID=false; // plot pulse integral distribution if true
    bool plotPIDCalibrated=false; // calibrate pulse integral distribution if true
    double PIDXmin=0.0; // lower boundary of the pulse integral distribution
    double PIDXmax=0.0; // upper boundary of the pulse integral distribution
    int PIDBins=1; // number of bins in pulse integral distribution

    bool plotPSD=false; // plot PSD if true
    bool plotPSDCalibrated=false; // calibrate PSD if true
    double PSDXmin=0.0; // lower x boundary of the PSD
    double PSDXmax=0.0; // upper x boundary of the PSD
    double PSDYmin=0.0; // lower y boundary of the PSD
    double PSDYmax=0.0; // upper y boundary of the PSD
    int PSDXBins=1; // number of bins along x
    int PSDYBins=1; // number of bins along y

    /**
     * @brief set up parameters for timing
     * 
     * @return 0 
     */
    int timingSetup();
};

/**
 * Stores the user input parameters for coincidence selection
 * 
 */
struct CoincidenceSetting
{
    bool coincidence=false; // enable coincidence selection if true
    std::pair<uint, uint> coincidentChannels; // find coincidences between these two channels
    uint timeWindow=100; // time window, in ns
    bool saveTimeDifference=false; // save the time difference between each pair of coincident events if true
    bool plotTOF=false; // plot the histogram of time differences
    double xmin=-20; // lower x boundary for ToF plot
    double xmax=20; // lower x boundary for ToF plot
    int bins=200; // number of bins in histogram of time differences
};

/**
 * Saves all the user input parameters.
 * 
 */
class InputParameters
{
private:
    /* data */
    rapidjson::Document jsonInput; // user input file content
    std::string inputFilepath; // path to the user input file
    ChannelSettings defaultChannelSetting; // default channel settings in input file
    /**
     * @brief Get the Channel Setting from input file
     * 
     * @param v channel setting in input file
     * @param chset stores the channel setting
     * @return 0 
     */
    int getChannelSetting(const rapidjson::Value& v, ChannelSettings& chset);
    /**
     * @brief Get the Default Channel Setting from input file
     * 
     * @return 0 
     */
    int getDefaultChannelSetting();
    /**
     * @brief Get the Specific Channel Settings from input file
     * 
     * @return int 
     */
    int getSpecificChannelSettings();
    /**
     * @brief Get the Coincidence Setting from input file
     * 
     * @return int 
     */
    int getCoincidenceSetting();
public:
    // holds the settings of all channels
    std::vector<ChannelSettings> channelSettings;
    // holds the setting for coincidence selection
    CoincidenceSetting coincidenceSetting;
    /**
     * @brief Construct a new Input Parameters object
     * 
     * @param fpath path to user input file
     */
    InputParameters(const std::string fpath);

    // ~getInput() {}
};
