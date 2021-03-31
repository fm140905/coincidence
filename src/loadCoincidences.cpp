/**
 * @file loadCoincidences.cpp
 * @author Ming Fang
 * @brief Get coincident events from a pair of channels.
 * @date 2021-03-30
 */

#include "loadCoincidences.h"

int CoincidenceChannel::findCoincidences()
{
    const Long64_t timewindow = 1000 * coincidenceSetting.timeWindow; // in ps
    Long64_t timediff(0);
    UInt_t i(0), j(0);
    while (i < channel0.events.size() && j < channel1.events.size())
    {
        timediff = channel1.events[j].timeStampHeader - channel0.events[i].timeStampHeader;
        //timediff = (timestamps[chIndex0][i] / 2000 - timestamps[chIndex1][i] / 2000) * 2000;
        //testfile << timediff << '\n';
        if (timediff - timewindow > 0 )
        {
            i++;
        }

        else if (timediff + timewindow < 0)
        {
            j++;
        }

        else
        {
            pairIndices.push_back(std::make_pair(i, j));
            timeDiffHeader.push_back((Double_t)timediff / 1000); // ns
            timeDiffDIACFD.push_back(channel1.events[j].timeStampDACFD - channel0.events[i].timeStampDACFD);
            i++;
            j++;
        }
    }
    return 0;
}

int CoincidenceChannel::getTOF(const std::vector<Double_t>& timeDiff, TH1D& TOF, const std::string& plotname){
    TOF = TH1D(plotname.c_str(), plotname.c_str(), coincidenceSetting.bins, coincidenceSetting.xmin, coincidenceSetting.xmax);
    for (std::size_t i = 0; i < timeDiff.size(); i++)
    {
        TOF.Fill(timeDiff[i]);
    }
    TOF.GetXaxis()->SetTitle("Time Difference (ns)");
    TOF.GetYaxis()->SetTitle("Counts");
    
    TOF.SetFillColor(40);
    
    return 0;
}
int CoincidenceChannel::getTOFHeader(const std::string& plotname){
    if (coincidenceSetting.plotTOF)
    {
        getTOF(timeDiffHeader, TOFHeader, plotname);
    }
    
    return 0;
}
int CoincidenceChannel::getTOFDIACFD(const std::string& plotname){
    // TODO
    if (coincidenceSetting.plotTOF)
    {
        getTOF(timeDiffDIACFD, TOFDIACFD, plotname);
    }
    
    return 0;
}