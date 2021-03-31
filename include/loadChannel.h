/**
 * @file loadChannel.h
 * @author Ming Fang
 * @brief Process events of a single channel from a binary input file
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
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include "getInput.h"
#include "loadEvents.h"
#include "event.h"

/**
 * Holds events from a single channel.
 * 
 */
class Channel
{
private:
    // settings for processing data from this channel.
    const ChannelSettings& channelSetting;
    /**
     * @brief Get a graph of pulses
     * 
     * @param pulses Pulses to plot.
     * @param pulseNum Max number of pulses to plot.
     * @param multiGraph Graph object showing the pulses.
     * @param plotName Name of the graph.
     * @param pts Number of interpolation points. Default to 1.
     * @return 0 
     */
    int getPulseGraph(const std::vector<std::vector<double_t>>& pulses, const UInt_t& pulseNum, 
                           TMultiGraph* multiGraph, const std::string& plotName,
                           const int& pts=1);

    // std::vector<Double_t> sincCoeffs;
    /**
     * @brief Get an interpolated pulse based on tsinc interpolation.
     * 
     * @param interpolated Holds the interpolated pulses.
     * @return 0 
     */
    int getInterpPulse(std::vector<Double_t>& interpolated);
    /**
     * @brief Calculate the i-th point of the interpolated pulses.
     * 
     * @param i Index of the point of interest.
     * @return double
     */
    double getInterpPoint(const int& i);
    /**
     * @brief Get a bipolar pulse by delaying and inverting the interpolated pulse in CFD.
     * 
     * @param interpolated Holds the bipolar pulse.
     * @return int 
     */
    int getBipolarPulse(std::vector<Double_t>& interpolated);
    /**
     * @brief Calculate the i-th point of the bipolar pulse by delaying and inverting the interpolated pulse in CFD.
     * 
     * @param i Index of the point of interest.
     * @return double 
     */
    double getBipolarPoint(const int&i);
    /**
     * @brief Calcuate the height and index of the hightest point of the interpolated pulse.
     * 
     * @param ymax Height of the hightest point of the interpolated pulse.
     * @param imax Index of the hightest point of the interpolated pulse
     * @return int 
     */
    int getInterpMax(double& ymax, int& imax);
    /**
     * @brief Find the index of the highest point through binary searching.
     * 
     * @param low Lower boundary of the index range where the highest point resides.
     * @param high Upper boundary of the index range where the highest point resides.
     * @return int 
     */
    int findMax(int low, int high);
    /**
     * @brief Find the index of point whose value equals "value" through binary searching.
     * 
     * @param low Lower boundary of the index range where the point of interest locates.
     * @param high Upper boundary of the index range where the point of interest locates.
     * @param value Value of interest.
     * @return int 
     */
    int findInterp(int low, int high, const double& value);
    /**
     * @brief Find the index of the zero crossing point the bipolar pulse in CFD.
     * 
     * @param low Lower boundary of the index range where the zero-crossing locates.
     * @param high Upper boundary of the index range where the zero-crossing locates.
     * @return int 
     */
    int findZeroBipolar(int low, int high);
public:
    // tempory storage of the pulse when reading an event.
    std::vector<double> voltage;
    // holds all good events in the channel.
    std::vector<Event> events;
    /**
     * @brief Construct a new Channel object.
     * 
     * @param sett Settings for processing the data.
     */
    Channel(const ChannelSettings& sett): channelSetting(sett) {}
    // ~Channel();
    /**
     * @brief Read the events from the binary file specrified in the settings.
     * 
     * @return int 
     */
    int loadEvents();
    /**
     * @brief Perform timing of an event using DIACFD or DCFD.
     * 
     * @param event Event of interest.
     * @return int 
     */
    int timing(Event& event);
    /**
     * @brief Perform timing of an event using DCFD.
     * 
     * @param event Event of interest.
     * @return int 
     */
    int timingDCFD(Event& event);
    /**
     * @brief Perform timing of an event using DIACFD.
     * 
     * @param event Event of interest.
     * @return int 
     */
    int timingDIACFD(Event& event);
    
    // Holds a number of good pulses for plotting or exporting.
    std::vector<std::vector<double_t>> goodPulses;
    // Holds a number of bad pulses for plotting or exporting.
    std::vector<std::vector<double_t>> badPulses;
    // Graph showing the good pulses.
    TMultiGraph goodPulseGraph;
    /**
     * @brief Get the Graph object showing the Good Pulses.
     * 
     * @param plotName Name of the Graph.
     * @return int 
     */
    int getGoodPulseGraph(const std::string& plotName);
    // Graph showing the bad pulses.
    TMultiGraph badPulseGraph;
    /**
     * @brief Get the Graph object showing the Bad Pulses.
     * 
     * @param plotName Name of the Graph.
     * @return int 
     */
    int getBadPulseGraph(const std::string& plotName);

    // for bedug
    
    // Holds a number of interpolated pulses.
    std::vector<std::vector<double_t>> interpPulses;
    // Graph showing the interpolated pulses.
    TMultiGraph interpPulseGraph;
    /**
     * @brief Get the Graph object showing the Interpolated Pulses.
     * 
     * @param plotName Name of the Graph.
     * @return int 
     */
    int getinterpPulseGraph(const std::string& plotName);
    
    // 1D Histogram object holding the pulse height distribution.
    TH1D PHD;
    /**
     * @brief Get the pulse height distribution.
     * 
     * @param plotName Name of the histogram.
     * @return int 
     */
    int getPHD(const std::string& plotName);
    // 1D Histogram object holding the pulse integral distribution.
    TH1D PID;
    /**
     * @brief Get the pulse integral distribution.
     * 
     * @param plotName Name of the histogram.
     * @return int 
     */
    int getPID(const std::string& plotName);

    // 2D Histogram object holding the PSD
    TH2D PSD;
    /**
     * @brief Get the PSD.
     * 
     * @param plotName Name of the histogram.
     * @return int 
     */
    int getPSD(const std::string& plotName);
};