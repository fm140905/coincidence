/*
 * @Description: Test load events from binary data
 * @Author: Ming Fang
 * @Date: 2021-03-28 17:40:06
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-29 19:26:56
 */

#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "TCanvas.h"

#include "../include/getInput.h"
#include "../include/loadEvents.h"

class ChannelTest : public ::testing::Test{
public:
    void SetUp() override{
        f = "/home/mingf2/projects/coincidence/test/testdata/input.json";
        settings = new InputParameters(f);
        channel0 = new Channel(settings->channelSettings[0]);
        channel0->loadEvents();
        channel1 = new Channel(settings->channelSettings[1]);
        channel1->loadEvents();
    }
    void TearDown( ) { 
        delete settings;
        delete channel0;
        delete channel1;
   }
    std::string f;
    InputParameters* settings;
    Channel* channel0;
    Channel* channel1;
};

TEST_F(ChannelTest, loadEvents)
{
    EXPECT_EQ(channel0->events.size(), 100);
    EXPECT_EQ(channel0->events[0].timeStampHeader, 48127163865);
    EXPECT_NEAR(channel0->events[0].height, 0.147469938, 1e-6);
    EXPECT_EQ(channel0->events[1].timeStampHeader, 48623174412);
    EXPECT_NEAR(channel0->events[1].height, 0.362876147, 1e-6);
    EXPECT_EQ(channel0->events[99].timeStampHeader, 1664117388189);
    EXPECT_NEAR(channel0->events[99].height, 0.0948391631, 1e-6);

    EXPECT_EQ(channel1->events.size(), 100);
    EXPECT_EQ(channel1->events[0].timeStampHeader, 48127162330);
    EXPECT_NEAR(channel1->events[0].height, 1.00167859, 1e-6);
    EXPECT_EQ(channel1->events[1].timeStampHeader, 48623176718);
    EXPECT_NEAR(channel1->events[1].height, 0.127174512, 1e-6);
    EXPECT_EQ(channel1->events[99].timeStampHeader, 1664117385619);
    EXPECT_NEAR(channel1->events[99].height, 0.988692522, 1e-6);

    EXPECT_EQ(channel0->goodPulses.size(), 100);
    EXPECT_EQ(channel1->goodPulses.size(), 100);
    EXPECT_EQ(channel0->badPulses.size(), 0);
    EXPECT_EQ(channel1->badPulses.size(), 0);
}

TEST_F(ChannelTest, plotPulsesTest)
{   
    std::string plotname = "Good pulses - channel " + std::to_string(settings->channelSettings[0].channelNumber);
    std::unique_ptr<TCanvas> goodPulseCanvasCH0(new TCanvas(plotname.c_str(), "Good pulses", 200, 10, 700, 500));
    channel0->getGoodPulseGraph(plotname);
    channel0->goodPulseGraph.DrawClone("A PLC PMC");
    goodPulseCanvasCH0->DrawClone();
    goodPulseCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/Good pulses - channel 0.png");

    plotname = "Good pulses - channel " + std::to_string(settings->channelSettings[1].channelNumber);
    std::unique_ptr<TCanvas> goodPulseCanvasCH1(new TCanvas(plotname.c_str(), "Good pulses", 200, 10, 700, 500));
    channel1->getGoodPulseGraph(plotname);
    channel1->goodPulseGraph.DrawClone("A PLC PMC");
    goodPulseCanvasCH1->DrawClone();
    goodPulseCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/Good pulses - channel 1.png");

    plotname = "Rejected pulses - channel " + std::to_string(settings->channelSettings[0].channelNumber);
    std::unique_ptr<TCanvas> badPulseCanvasCH0(new TCanvas(plotname.c_str(), "Rejected pulses", 200, 10, 700, 500));
    channel0->getBadPulseGraph(plotname);
    channel0->badPulseGraph.DrawClone("A PLC PMC");
    badPulseCanvasCH0->DrawClone();
    badPulseCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/rejected pulses - channel 0.png");

    plotname = "Rejected pulses - channel " + std::to_string(settings->channelSettings[1].channelNumber);
    std::unique_ptr<TCanvas> badPulseCanvasCH1(new TCanvas(plotname.c_str(), "Rejected pulses", 200, 10, 700, 500));
    channel1->getBadPulseGraph(plotname);
    channel1->badPulseGraph.DrawClone("A PLC PMC");
    badPulseCanvasCH1->DrawClone();
    badPulseCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/rejected pulses - channel 1.png");
}

TEST_F(ChannelTest, plotPHDTest)
{
    std::string plotname = "PHD - channel " + std::to_string(settings->channelSettings[0].channelNumber);
    std::unique_ptr<TCanvas> PHDCanvasCH0(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel0->getPHD(plotname);
    channel0->PHD.DrawClone();
    PHDCanvasCH0->DrawClone();
    PHDCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PHD - channel 0.png");

    plotname = "PHD - channel " + std::to_string(settings->channelSettings[1].channelNumber);
    std::unique_ptr<TCanvas> PHDCanvasCH1(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel1->getPHD(plotname);
    channel1->PHD.DrawClone();
    PHDCanvasCH1->DrawClone();
    PHDCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PHD - channel 1.png");
    
}

TEST_F(ChannelTest, plotPIDTest)
{
    std::string plotname = "PID - channel " + std::to_string(settings->channelSettings[0].channelNumber);
    std::unique_ptr<TCanvas> PIDCanvasCH0(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel0->getPID(plotname);
    channel0->PID.DrawClone();
    PIDCanvasCH0->DrawClone();
    PIDCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PID - channel 0.png");

    plotname = "PID - channel " + std::to_string(settings->channelSettings[1].channelNumber);
    std::unique_ptr<TCanvas> PIDCanvasCH1(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel1->getPID(plotname);
    channel1->PID.DrawClone();
    PIDCanvasCH1->DrawClone();
    PIDCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PID - channel 1.png");
}

TEST_F(ChannelTest, plotPSDTest)
{
    std::string plotname = "PSD - channel " + std::to_string(settings->channelSettings[0].channelNumber);
    std::unique_ptr<TCanvas> PSDCanvasCH0(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel0->getPSD(plotname);
    channel0->PSD.DrawClone();
    PSDCanvasCH0->DrawClone();
    PSDCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PSD - channel 0.png");

    plotname = "PSD - channel " + std::to_string(settings->channelSettings[1].channelNumber);
    std::unique_ptr<TCanvas> PSDCanvasCH1(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    channel1->getPSD(plotname);
    channel1->PSD.DrawClone("COLZ");
    PSDCanvasCH1->DrawClone();
    PSDCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/PSD - channel 1.png");
}

// TEST_F(ChannelTest, interpolationTest)
// {
//     std::string plotname = "Interpolated pulses - channel " + std::to_string(settings->channelSettings[0].channelNumber);
//     std::unique_ptr<TCanvas> goodPulseCanvasCH0(new TCanvas(plotname.c_str(), "Interpolated pulses", 200, 10, 700, 500));
//     channel0->getinterpPulseGraph(plotname);
//     channel0->interpPulseGraph.DrawClone("A PLC PMC");
//     goodPulseCanvasCH0->DrawClone();
//     goodPulseCanvasCH0->SaveAs("/home/mingf2/projects/coincidence/test/testdata/Interpolated pulses - channel 0.png");

//     plotname = "Interpolated pulses - channel " + std::to_string(settings->channelSettings[1].channelNumber);
//     std::unique_ptr<TCanvas> goodPulseCanvasCH1(new TCanvas(plotname.c_str(), "Interpolated pulses", 200, 10, 700, 500));
//     channel1->getinterpPulseGraph(plotname);
//     channel1->interpPulseGraph.DrawClone("A PLC PMC");
//     goodPulseCanvasCH1->DrawClone();
//     goodPulseCanvasCH1->SaveAs("/home/mingf2/projects/coincidence/test/testdata/Interpolated pulses - channel 1.png");

// }