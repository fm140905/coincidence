/*
 * @Description: Test loading coincidences
 * @Author: Ming Fang
 * @Date: 2021-03-29 10:25:37
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-29 20:41:27
 */

#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "TCanvas.h"

#include "../include/getInput.h"
#include "../include/loadCoincidences.h"

class CoincidenceTest : public ::testing::Test{
public:
    void SetUp() override{
        f = "/home/mingf2/projects/coincidence/test/inputCoincidencesTest.json";
        settings = new InputParameters(f);
        channel0 = new Channel(settings->channelSettings[0]);
        channel0->loadEvents();
        channel1 = new Channel(settings->channelSettings[1]);
        channel1->loadEvents();
        coin = new CoincidenceChannel(settings->coincidenceSetting, *channel0, *channel1);
        coin->findCoincidences();
    }
    void TearDown( ) { 
        delete settings;
        delete channel0;
        delete channel1;
        delete coin;
   }
    std::string f;
    InputParameters* settings;
    Channel* channel0;
    Channel* channel1;
    CoincidenceChannel* coin;
};

TEST_F(CoincidenceTest, findCoincidences)
{
    std::cout << coin->pairIndices.size() << std::endl;
    EXPECT_EQ(coin->pairIndices.size(), 100000);
}

TEST_F(CoincidenceTest, getTOFheader)
{
    std::string plotname = "TOF - Header";
    std::unique_ptr<TCanvas> TOFCanvas(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    coin->getTOFHeader(plotname);
    coin->TOFHeader.DrawClone();
    TOFCanvas->DrawClone();
    TOFCanvas->SaveAs("/home/mingf2/projects/coincidence/test/testdata/TOF - Header.png");
}

TEST_F(CoincidenceTest, getTOFDIACFD)
{
    std::string plotname = "TOF - DIACFD";
    std::unique_ptr<TCanvas> TOFCanvas(new TCanvas(plotname.c_str(), plotname.c_str(), 200, 10, 700, 500));
    coin->getTOFDIACFD(plotname);
    coin->TOFDIACFD.DrawClone();
    TOFCanvas->DrawClone();
    TOFCanvas->SaveAs("/home/mingf2/projects/coincidence/test/testdata/TOF - DIACFD.png");
}