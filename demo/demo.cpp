/*
 * @Description: Simple example on using the library
 * @Author: Ming Fang
 * @Date: 2021-03-30 16:13:41
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-30 16:51:18
 */

#include <fstream>
#include <chrono>
#include <sys/stat.h>

#include "TCanvas.h"

#include "../include/getInput.h"
#include "../include/loadEvents.h"
#include "../include/loadChannel.h"
#include "../include/loadCoincidences.h"

int main(int argc, char** argv)
{
    // create output directory
    struct stat st = {0};
    // create directories to save the output files and pics
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }
    auto startTime = std::chrono::high_resolution_clock::now();
    const std::string settingFilePath("/home/mingf2/projects/coincidence/demo/demo.json");
    const InputParameters settings(settingFilePath);
    
    Channel channel0(settings.channelSettings[0]);
    channel0.loadEvents();

    std::string plotname = "Good pulses - channel 0";
    std::unique_ptr<TCanvas> goodPulseCanvasCH0(new TCanvas(plotname.c_str(), "Good pulses", 200, 10, 700, 500));
    channel0.getGoodPulseGraph(plotname);
    channel0.goodPulseGraph.DrawClone("A PLC PMC");
    goodPulseCanvasCH0->DrawClone();
    goodPulseCanvasCH0->SaveAs("output/Good pulses - channel 0.png");

    plotname = "PHD - channel 0";
    std::unique_ptr<TCanvas> PHDCanvasCH0(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel0.getPHD(plotname);
    channel0.PHD.DrawClone();
    PHDCanvasCH0->DrawClone();
    PHDCanvasCH0->SaveAs("output/PHD - channel 0.png");

    plotname = "PID - channel0 ";
    std::unique_ptr<TCanvas> PIDCanvasCH0(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel0.getPID(plotname);
    channel0.PID.DrawClone();
    PIDCanvasCH0->DrawClone();
    PIDCanvasCH0->SaveAs("output/PID - channel 0.png");

    std::string outname = "output/PH - channel 0";
    channel0.savePH(outname);
    outname = "output/PI - channel 0";
    channel0.savePI(outname);
    outname = "output/Pulse integral - channel 0";
    channel0.saveIntegrals(outname);
    outname = "output/Time stamp - channel 0";
    channel0.saveTimeStamp(outname);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    // myapp->Run();
    return 0;
}
