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
#include "TStyle.h"
#include "TApplication.h"

#include "../include/cmdParser.h"
#include "../include/getInput.h"
#include "../include/loadEvents.h"
#include "../include/loadChannel.h"
#include "../include/loadCoincidences.h"

int main(int argc, char** argv)
{
    // parse command line input
    CmdParser cmdinput(argc, argv);
    if(cmdinput.cmdOptionExists("-h")){
        std::cout << std::endl;
        std::cout << "---Script to process binary data saved by CoMPASS---" << std::endl;
        std::cout << std::endl;
        std::cout << "Usage: main [options]" << std::endl;
        std::cout << std::endl;
        std::cout << "Possible options are:" << std::endl;
        std::cout << "\t -h\n\t\tOutput this text; Script is not executed." << std::endl;
        std::cout << "\t -i FILE\n\t\tLoad parameters from FILE and prcoess data." << std::endl;
        std::cout << "\t -d\n\t\tDisplay results, including pulses, spectra, and PSD." << std::endl;
        std::cout << std::endl;
        return 0;
    }
    // get path of input json file 
    const std::string settingFilePath = cmdinput.getCmdOption("-i");
    if (settingFilePath.empty()){
        std::cout << "Invalid input json file path." << std::endl;
        return 1;
    }

    // supress root mesg
    gErrorIgnoreLevel = kWarning;
    // create output directory
    struct stat st = {0};
    // create directories to save the output files and pics
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }
    // set stat box positions
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    //Tapplication to display results
    TApplication* myapp = new TApplication("PALS", 0, 0);

    auto startTime = std::chrono::high_resolution_clock::now();
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
            TCanvas goodPulseCanvasCHI(plotname.c_str(), "Good pulses", 200, 10, 700, 500);
            auto cp = goodPulseCanvasCHI.DrawClone();
            channelI->getGoodPulseGraph(plotname);
            channelI->goodPulseGraph.DrawClone("A PLC PMC");
            outname = "output/"+plotname + ".png";
            cp->SaveAs(outname.c_str());
        }

        if (chseti.plotBadPulses)
        {
            plotname = "Rejected pulses_channel " + std::to_string(chseti.channelNumber);
            TCanvas badPulseCanvasCHI(plotname.c_str(), "Rejected pulses", 200, 10, 700, 500);
            auto cp = badPulseCanvasCHI.DrawClone();
            channelI->getBadPulseGraph(plotname);
            channelI->badPulseGraph.DrawClone("A PLC PMC");
            outname = "output/"+plotname + ".png";
            cp->SaveAs(outname.c_str());
        }
        
        if (chseti.plotPHD)
        {
            plotname = "PHD_channel " + std::to_string(chseti.channelNumber);
            TCanvas PHDCanvasCHI(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500);
            auto cp = PHDCanvasCHI.DrawClone();
            channelI->getPHD(plotname);
            channelI->PHD.DrawClone();
            outname = "output/"+plotname + ".png";
            cp->SaveAs(outname.c_str());
        }

        if (chseti.plotPID)
        {
            plotname = "PID_channel " + std::to_string(chseti.channelNumber);
            TCanvas PIDCanvasCHI(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500);
            auto cp = PIDCanvasCHI.DrawClone();
            channelI->getPID(plotname);
            channelI->PID.DrawClone();
            outname = "output/"+plotname + ".png";
            cp->SaveAs(outname.c_str());
        }

        if (chseti.plotPSD)
        {
            plotname = "PSD_channel " + std::to_string(chseti.channelNumber);
            TCanvas PSDCanvasCHI(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500);
            auto cp = PSDCanvasCHI.DrawClone();
            channelI->getPSD(plotname);
            channelI->PSD.DrawClone("COLZ");
            outname = "output/"+plotname + ".png";
            cp->SaveAs(outname.c_str());
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
            TCanvas TOFCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500);
            auto cp = TOFCanvas.DrawClone();
            coin.getTOFHeader(plotname);
            coin.TOFHeader.DrawClone();
            cp->SaveAs("output/TOF_Header.png");

            // check if CFD timing enabled for both channels
            if (coin.channel0.channelSetting.timing && coin.channel1.channelSetting.timing)
            {
                plotname = "TOF_CFD";
                TCanvas TOFCFDCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500);
                auto cp = TOFCFDCanvas.DrawClone();
                coin.getTOFDIACFD(plotname);
                coin.TOFDIACFD.DrawClone();
                cp->SaveAs("output/TOF_CFD.png");
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
    // display results if required
    if(cmdinput.cmdOptionExists("-d"))
    {
        std::cout << "Press ctrl+c to exit.." << std::endl;
        myapp->Run();
    }
    return 0;
}
