#ifndef GETINPUTPARAMETERS_H
#define GETINPUTPARAMETERS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "TFile.h"

struct Parameters
{
    //Settings
    std::string Directory;
    std::vector < int > Folders;
    std::vector < int > Channels;
    int Filetype=0; // 0 for binary, 1 for ASCII
    int Polarity=-1; // polarity of the pulses
    int NHeaders=7;
    std::vector < int > Headersize; // bytes
    int NSamples=208;
    int Samplesize=2; //bytes
    float DynamicRange=2.0; // Volts
    int Resolution=14;
    int Offset=30; // number of samples to calculate the baseline
    int Delt=2; // time between 2 samples, unit: ns

    bool SavePulses=0;

    bool TimeStamp=0;
    bool SaveTimeStamp=0;

    // float Rejection=0.5;// in coincidence, reject bad shaping pulses

    // Bad pulses options
    bool FilterBad=1;
    bool ZeroSupression=1;
    float MinVoltage=0.05; // Volts
    bool Clipped=1;
    float MaxVoltage=2.0; // Volts
    bool SaveBad=0;

    // Piled-up pulses option
    bool FilterPiledup=1;
    bool SavePiledup=0;
    bool SaveSTime=0;
    bool SaveSFrequency=0;

    //energy cut
    std::vector < float_t> Calicoefs;
    std::vector<bool> EnergyCut;
    std::vector<float_t> EnergyHigh;
    std::vector<float_t> EnergyLoW;

    //PSD cut
    std::vector< bool > PSDCut;
    std::vector< float_t > PSDLow;
    std::vector< float_t >  PSDHigh;

    // Coincidences selection
    bool Coincidence=1;
    std::vector < int > CoincidenceChannels;
    float TimeWindow=8; // time window, unit: ns
    int Interpolation=0; // number of interpolation points between two samples
        // DIACFD settings
    int FilterWidth=1; // width of moving average filter, number of samples
    double Fraction=0.15; // constant fraction F
    int TimeDelay=6; // time delay, unit: ns
    bool SaveDT=1;
    bool TOF=0;
    bool SaveTOF=0;

    // Pulse height distribution and pulse integral distribution options
    bool SavePH=0;
    bool PHD=1;
    bool SavePHD=0;
    bool SavePI=0;
    bool PID=1;
    int Pre=4;
    int Post=120;
    bool SavePID=0;

    // Tail integral vs total integral
    bool TailvsTotal=1;
    int TailStart=22;
    int SaveTailvsTotal=0;

    //  Maximum number of pulses to process
    int MaxNumPulses=1;
};

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t");

std::string removeComments(const std::string& str,
                 const std::string& begChar);

int getInputParameters(std::string filepath, Parameters& setting);

#endif