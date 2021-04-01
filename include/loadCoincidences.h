/**
 * @file loadCoincidences.h
 * @author Ming Fang
 * @brief Get coincident events from a pair of channels.
 * @date 2021-03-30
 */

#pragma once

#include "TH1D.h"

#include "getInput.h"
#include "loadEvents.h"
#include "loadChannel.h"

/**
 * Holds the coincidences from two channels.
 * 
 */
class CoincidenceChannel
{
private:
    /* data */
    // User setting for coincidence selection.
    const CoincidenceSetting& coincidenceSetting;
    /**
     * @brief Get the histogram of the time differences between all coincidences.
     * 
     * @param timeDiff Holds the time differences between all pairs of coincidences.
     * @param TOF Histogram object.
     * @param plotname Name of the histogram.
     * @return int 
     */
    int getTOF(const std::vector<Double_t>& timeDiff, TH1D& TOF, const std::string& plotname);
public:
    // CoincidenceChannel(const InputParameters& input): coincidenceSetting(input.coincidenceSetting) {}
    /**
     * @brief Construct a new Coincidence Channel object.
     * 
     * @param input User setting for coincidence selection.
     * @param ch0 Channel object holding all events in the first channel.
     * @param ch1 Channel object holding all events in the second channel.
     */
    CoincidenceChannel(const CoincidenceSetting& input, const Channel& ch0, const Channel& ch1): coincidenceSetting(input), channel0(ch0), channel1(ch1) {}
    // Channel object holding all events in the first channel.
    const Channel& channel0;
    // Channel object holding all events in the second channel.
    const Channel& channel1;
    // Indices of all pair of coindences.
    std::vector<std::pair<UInt_t, UInt_t>> pairIndices;
    /**
     * @brief Select coincidences between the two channels.
     * 
     * @return int 
     */
    int findCoincidences();
    // Holds the differences of the time stamps of coincidences, calculated by digitizer.
    std::vector<Double_t> timeDiffHeader;
    // Holds the differences of the time stamps of coincidences, calculated using DIACFD or DCFD.
    std::vector<Double_t> timeDiffDIACFD;
    
    // Histogram of the time differences between all coincidences. Time stamps are calculated by digitizer.
    TH1D TOFDIACFD;
    /**
     * @brief Get the histogram of the time differences between all coincidences. Time stamps are calculated by digitizer.
     * 
     * @param plotname Name of the histogram.
     * @return int 
     */
    int getTOFDIACFD(const std::string& plotname);
    
    // Histogram of the time differences between all coincidences. Time stamps are calculated by digitizer.
    TH1D TOFHeader;
    /**
     * @brief Get the histogram of the time differences between all coincidences. Time stamps are calculated by digitizer.
     * 
     * @param plotname Name of the histogram.
     * @return int 
     */
    int getTOFHeader(const std::string& plotname);
    // ~CoincidenceChannel();

    int saveTOFHeader(const std::string& filename);
    int saveTOFDIACFD(const std::string& filename);
};
