/*
 * @Description: Simple example on using the library
 * @Author: Ming Fang
 * @Date: 2021-03-30 16:13:41
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-04-06 16:39:57
 */

#include <fstream>
#include <chrono>
#include <sys/stat.h>

#include "TCanvas.h"
#include "TError.h"

#include "../include/getInput.h"
#include "../include/loadEvents.h"
#include "../include/loadChannel.h"
#include "../include/loadCoincidences.h"

int main(int argc, char** argv)
{
    // supress root mesg
    gErrorIgnoreLevel = kWarning;
    // create output directory
    struct stat st = {0};
    // create directories to save the output files and pics
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }
    auto startTime = std::chrono::high_resolution_clock::now();
    const std::string settingFilePath(argv[1]);
    const InputParameters settings(settingFilePath);
    std::vector<std::shared_ptr<Channel>> coincidenceChannels;
    
    // process each channel
    for (std::size_t i = 0; i < settings.channelSettings.size(); i++)
    {
        const ChannelSettings& chseti = settings.channelSettings[i];
        if (!chseti.processOutput)
        {
            continue;
        }

        
        std::shared_ptr<Channel> channelI(new Channel(chseti));
        channelI->loadEvents();
        std::string plotname;
        std::string outname;

        if (chseti.plotGoodPulses)
        {
            plotname = "Good pulses_channel " + std::to_string(chseti.channelNumber);
            std::unique_ptr<TCanvas> goodPulseCanvasCHI(new TCanvas(plotname.c_str(), "Good pulses", 200, 10, 700, 500));
            channelI->getGoodPulseGraph(plotname);
            channelI->goodPulseGraph.DrawClone("A PLC PMC");
            goodPulseCanvasCHI->DrawClone();
            outname = "output/"+plotname + ".png";
            goodPulseCanvasCHI->SaveAs(outname.c_str());
        }

        if (chseti.plotBadPulses)
        {
            plotname = "Rejected pulses_channel " + std::to_string(chseti.channelNumber);
            std::unique_ptr<TCanvas> badPulseCanvasCHI(new TCanvas(plotname.c_str(), "Rejected pulses", 200, 10, 700, 500));
            channelI->getBadPulseGraph(plotname);
            channelI->badPulseGraph.DrawClone("A PLC PMC");
            badPulseCanvasCHI->DrawClone();
            outname = "output/"+plotname + ".png";
            badPulseCanvasCHI->SaveAs(outname.c_str());
        }
        
        if (chseti.plotPHD)
        {
            plotname = "PHD_channel " + std::to_string(chseti.channelNumber);
            std::unique_ptr<TCanvas> PHDCanvasCHI(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
            channelI->getPHD(plotname);
            channelI->PHD.DrawClone();
            PHDCanvasCHI->DrawClone();
            outname = "output/"+plotname + ".png";
            PHDCanvasCHI->SaveAs(outname.c_str());
        }

        if (chseti.plotPID)
        {
            plotname = "PID_channel " + std::to_string(chseti.channelNumber);
            std::unique_ptr<TCanvas> PIDCanvasCHI(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
            channelI->getPID(plotname);
            channelI->PID.DrawClone();
            PIDCanvasCHI->DrawClone();
            outname = "output/"+plotname + ".png";
            PIDCanvasCHI->SaveAs(outname.c_str());
        }

        if (chseti.plotPSD)
        {
            plotname = "PSD_channel " + std::to_string(chseti.channelNumber);
            std::unique_ptr<TCanvas> PSDCanvasCHI(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
            channelI->getPSD(plotname);
            channelI->PSD.DrawClone("COLZ");
            PSDCanvasCHI->DrawClone();
            outname = "output/"+plotname + ".png";
            PSDCanvasCHI->SaveAs(outname.c_str());
        }

        if (chseti.savePulses)
        {
            outname = "output/Good pulses_channel " + std::to_string(chseti.channelNumber);
            channelI->saveGoodPulses(outname);
            outname = "output/Rejected pulses_channel "+ std::to_string(chseti.channelNumber);
            channelI->saveBadPulses(outname);
        }
        

        outname = "output/PH_channel " + std::to_string(chseti.channelNumber);
        channelI->savePH(outname);
        outname = "output/PI_channel " + std::to_string(chseti.channelNumber);
        channelI->savePI(outname);
        outname = "output/Pulse integral_channel " + std::to_string(chseti.channelNumber);
        channelI->saveIntegrals(outname);
        outname = "output/Time stamp_channel " + std::to_string(chseti.channelNumber);
        channelI->saveTimeStamp(outname);

        if (settings.coincidenceSetting.coincidence)
        {
            if (chseti.channelNumber == settings.coincidenceSetting.coincidentChannels.first || 
                chseti.channelNumber == settings.coincidenceSetting.coincidentChannels.second )
            {
                coincidenceChannels.push_back(channelI);
            }
        }
    }
    
    // coincidence
    // two-channel, AND
    if (settings.coincidenceSetting.coincidence)
    {
        if (coincidenceChannels.size() < 2)
        {
           throw std::invalid_argument("One or more coincidence channels could not be found."); 
        }

        const CoincidenceSetting& coinset = settings.coincidenceSetting;
        CoincidenceChannel coin(coinset, *coincidenceChannels[0], *coincidenceChannels[1]);
        coin.findCoincidences();
        std::cout << coin.pairIndices.size() << " coincidences found within a " << coinset.timeWindow << " ns time window." << std::endl;
        std::string plotname;
        std::string outname;

        if (coinset.plotTOF)
        {
            plotname = "TOF_Header";
            std::unique_ptr<TCanvas> TOFCanvas(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
            coin.getTOFHeader(plotname);
            coin.TOFHeader.DrawClone();
            TOFCanvas->DrawClone();
            TOFCanvas->SaveAs("output/TOF_Header.png");

            // check if CFD timing enabled for both channels
            if (coin.channel0.channelSetting.timing && coin.channel1.channelSetting.timing)
            {
                plotname = "TOF_CFD";
                std::unique_ptr<TCanvas> TOFCanvas(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
                coin.getTOFDIACFD(plotname);
                coin.TOFDIACFD.DrawClone();
                TOFCanvas->DrawClone();
                TOFCanvas->SaveAs("output/TOF_CFD.png");
            }
        }

        // save time differences in txt
        if (coinset.saveTimeDifference)
        {
            outname = "output/TOF_Header";
            coin.saveTOFHeader(outname);
            if (coin.channel0.channelSetting.timing && coin.channel1.channelSetting.timing)
            {
                outname = "output/TOF_CFD";
                coin.saveTOFDIACFD(outname);
            }
        }
    }
    
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    // myapp->Run();
    return 0;
}
