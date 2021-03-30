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
    const std::string settingFilePath("/media/ming/DATA/coincidence/test/loadCoincidencesTest.json");
    const InputParameters settings(settingFilePath);
    
    // const Channel channel0(settings->channelSettings[0]);
    // channel0.loadEvents();
    // const Channel channel1(settings->channelSettings[1]);
    // channel1.loadEvents();
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    // myapp->Run();
    return 0;
}
