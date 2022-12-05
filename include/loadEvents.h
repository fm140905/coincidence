/**
 * @file loadEvents.h
 * @author Ming Fang
 * @brief Load single event. Perform calibration and rejection.
 * @date 2021-03-30
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

/**
 * @brief A single event.
 * 
 */
class Event
{
private:
    /**
     * @brief Get the pulse height of an event.
     * 
     * @param v The digitized pulse in volts.
     * @param heightindex Index of the highest point of the pulse.
     * @return int 
     */
    int getPulseHeight(const std::vector<Double_t>& v, Int_t& heightindex);
    /**
     * @brief Get the total integral of an event.
     * 
     * @param v The digitized pulse in volts.
     * @return int 
     */
    int getPulseIntegral(const std::vector<Double_t>& v);
    /**
     * @brief Perform the energy calibration.
     * 
     * @return int 
     */
    int calibration();
    /**
     * @brief Perform pulse rejection, including zero-supression, clipped pulse rejection, and pile-up rejection.
     * 
     * @param v The digitized pulse in volts.
     * @param i Index of the highest point of the pulse.
     * @return int 
     */
    int rejection(const std::vector<Double_t>& v, const Int_t& i);
    /**
     * @brief Reject pil-up pulses.
     * 
     * @param v The digitized pulse in volts.
     * @param heightIndex Index of the highest point of the pulse.
     * @return int 
     */
    int pileupRejection(const std::vector<Double_t>& v, const Int_t heightIndex);
    /**
     * @brief Perform energy cut.
     * 
     * @return int 
     */
    int energyCut();
    /**
     * @brief Perform PSD cut.
     * 
     * @return int 
     */
    int PSDCut();
    // Users input for event data processing.
    const ChannelSettings& channelSetting;
public:
    // Digitizer time stamp, in ps.
    ULong64_t timeStampHeader = 0;

    // // Other headers are skipped.
    // uint16_t energyShortHeader=0;
    // uint16_t energyLongHeader=0;
    // uint32_t flagHeader=0;
    
    // Time stamp calculated by DIACFD or DCFD, in ns.
    Double_t timeStampDACFD = 0;
    // Pulse height, in volt.
    Double_t height = 0;
    // Pulse total integral, in V*ns/dt.
    Double_t totalIntegral = 0;
    // Light output (keVee) or energy (keV)
    Double_t energy=0;
    // Pulse tail integral, in V*ns/dt.
    Double_t tailIntegral = 0;
    // True if event is rejected.
    Bool_t isGood = true;
    // True is event is piledup
    Bool_t isPiledup = false;
    /**
     * @brief Construct a new Event object.
     * 
     * @param sett Users input for event data processing.
     */
    Event(const ChannelSettings& sett): channelSetting(sett) {}
    // ~Event();
    /**
     * @brief Read the event data stored in binary form in buffer.
     * 
     * @param buffer Buffer holding the binary event data.
     * @param bufIndex Starting index of the current event in buffer.
     * @param voltage Digitized pulse in volts.
     * @return int 
     */
    int parse(const char* buffer, ULong64_t& bufIndex, std::vector<Double_t>& voltage);
};




