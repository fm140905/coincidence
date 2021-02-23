#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <iostream>
#include "TFile.h"

struct Event
{
    ULong64_t timeStampHeader = 0;
    Double_t timeStampDACFD = 0;

    std::vector<Float_t> voltage;
    // std::vector<Short_t> samples;
    Float_t height = 0;
    UShort_t heightindex = 0;
    //UShort_t flags=0;
    Float_t totalIntegral = 0;
    Float_t energy=0;
    Float_t tailIntegral = 0;
    //Float_t tailRatio=0;
    Bool_t isBad = false;
    //Bool_t isPiled = false;
    //Bool_t isNeutron = false;
    //UShort_t pulseType=0;
};

#endif