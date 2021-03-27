/*
 * @Description: Test getInput function from json file
 * @Author: Ming Fang
 * @Date: 2021-03-26 20:12:42
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-26 23:29:15
 */

#include <gtest/gtest.h>
#include <string>
#include "../include/getInput.h"

TEST(getInputTest, cannotFindInput)
{
    std::string f("inputTest.json");
    EXPECT_THROW({
        try
        {
            const InputParameters settings(f);
        }
        catch( const std::invalid_argument& e )
        {
            EXPECT_STREQ( "Input file does not exit.", e.what() );
            throw;
        }
    }, std::invalid_argument );
}


TEST(getInputTest, getSpecificChannelSetting)
{
    std::string f("/home/mingf2/projects/coincidence/test/inputTest.json");
    const InputParameters settings(f);
    EXPECT_EQ(settings.channelSettings.size(), 2);
    const ChannelSettings& ch0 = settings.channelSettings[0];
    const ChannelSettings& ch1 = settings.channelSettings[1];

    EXPECT_EQ(ch0.channelNumber, 0);
    EXPECT_STREQ(ch0.path.c_str(), "/home/ming/Downloads/pals/test/channel0.bin");
    EXPECT_TRUE(ch0.processOutput);
    EXPECT_EQ(ch0.maxNumPulses, 100);
    EXPECT_EQ(ch0.channelNumber, 0);
    EXPECT_STREQ(ch0.polarity.c_str(), "positive");

    EXPECT_DOUBLE_EQ(ch0.dynamicRange, 1.0);
    EXPECT_EQ(ch0.timestep, 2);
    EXPECT_EQ(ch0.resolution, 14);
    std::vector<int> headers={2,2,8,2,2,4,4};
    EXPECT_EQ(ch0.headers.size(), headers.size());
    EXPECT_EQ(ch0.saveHeaders.size(), headers.size());
    for (std::size_t i = 0; i < headers.size(); i++)
    {
        EXPECT_EQ(ch0.headers[i], headers[i]);
        EXPECT_FALSE(ch0.saveHeaders[i]);
    }
    EXPECT_EQ(ch0.sampleSize, 2);
    EXPECT_EQ(ch0.length, 208);
    EXPECT_DOUBLE_EQ(ch0.dcOffset, 0.2);
    EXPECT_EQ(ch0.offset, 8);
    EXPECT_EQ(ch0.savePulses, 0);

    EXPECT_EQ(ch0.preTrig, 48);
    EXPECT_EQ(ch0.preGate, 50);
    EXPECT_EQ(ch0.shortGate, 100);
    EXPECT_EQ(ch0.longGate, 100);
    
    EXPECT_TRUE(ch0.calibrationHeight);
    EXPECT_DOUBLE_EQ(ch0.calibrationHeightCoefficient, 100.0);
    EXPECT_FALSE(ch0.calibrationIntegral);
    EXPECT_DOUBLE_EQ(ch0.calibrationIntegralCoefficient, 1.0);

    EXPECT_FALSE(ch0.rejection);
    EXPECT_TRUE(ch0.zeroSupression);
    EXPECT_DOUBLE_EQ(ch0.lowVoltageThreshold, 0.05);
    EXPECT_TRUE(ch0.clipRejection);
    
    EXPECT_FALSE(ch0.energyCut);
    EXPECT_DOUBLE_EQ(ch0.EnergyLowerThreshold, 0.0);
    EXPECT_DOUBLE_EQ(ch0.EnergyUpperThreshold, 500.0);
    
    EXPECT_FALSE(ch0.PSDCut);
    EXPECT_TRUE(ch0.lazyCut);
    EXPECT_DOUBLE_EQ(ch0.PSDLowerThreshold, 0.0);
    EXPECT_DOUBLE_EQ(ch0.PSDUpperThreshold, 1.0);
    EXPECT_FALSE(ch0.quadraticCut);
    std::vector<double> coefs={0.0,0.0,0.5};
    EXPECT_EQ(ch0.quadraticCoefficients.size(), coefs.size());
    for (std::size_t i = 0; i < coefs.size(); i++)
    {
        EXPECT_DOUBLE_EQ(ch0.quadraticCoefficients[i], coefs[i]);
    }
    
    EXPECT_FALSE(ch0.timing);
    EXPECT_STREQ(ch0.timingMethod.c_str(), "DIACFD");
    EXPECT_DOUBLE_EQ(ch0.cfdFraction, 0.4);
    EXPECT_EQ(ch0.timeDelay, 4);
    EXPECT_EQ(ch0.interpolationPoints, 7);


    // channel 1
    EXPECT_STREQ(ch1.path.c_str(), "/home/ming/Downloads/pals/test/channel1.bin");
    EXPECT_TRUE(ch1.processOutput);
    EXPECT_EQ(ch1.maxNumPulses, 10000);
    EXPECT_EQ(ch1.channelNumber, 1);
    EXPECT_STREQ(ch1.polarity.c_str(), "negative");
    
    EXPECT_DOUBLE_EQ(ch1.dynamicRange, 2.0);
    EXPECT_EQ(ch1.timestep, 2);
    EXPECT_EQ(ch1.resolution, 14);
    EXPECT_EQ(ch1.headers.size(), headers.size());
    EXPECT_EQ(ch1.saveHeaders.size(), headers.size());
    for (std::size_t i = 0; i < headers.size(); i++)
    {
        EXPECT_EQ(ch1.headers[i], headers[i]);
        EXPECT_FALSE(ch1.saveHeaders[i]);
    }
    EXPECT_EQ(ch1.sampleSize, 2);
    EXPECT_EQ(ch1.length, 208);
    EXPECT_DOUBLE_EQ(ch1.dcOffset, 0.2);
    EXPECT_EQ(ch1.offset, 8);
    EXPECT_EQ(ch1.savePulses, 0);

    EXPECT_EQ(ch1.preTrig, 48);
    EXPECT_EQ(ch1.preGate, 20);
    EXPECT_EQ(ch1.shortGate, 100);
    EXPECT_EQ(ch1.longGate, 300);
    
    EXPECT_FALSE(ch1.calibrationHeight);
    EXPECT_DOUBLE_EQ(ch1.calibrationHeightCoefficient, 1.0);
    EXPECT_TRUE(ch1.calibrationIntegral);
    EXPECT_DOUBLE_EQ(ch1.calibrationIntegralCoefficient, 1.0);

    EXPECT_FALSE(ch1.rejection);
    EXPECT_TRUE(ch1.zeroSupression);
    EXPECT_DOUBLE_EQ(ch1.lowVoltageThreshold, 0.05);
    EXPECT_TRUE(ch1.clipRejection);
    
    EXPECT_FALSE(ch1.energyCut);
    EXPECT_DOUBLE_EQ(ch1.EnergyLowerThreshold, 0.0);
    EXPECT_DOUBLE_EQ(ch1.EnergyUpperThreshold, 500.0);
    
    EXPECT_TRUE(ch1.PSDCut);
    EXPECT_FALSE(ch1.lazyCut);
    EXPECT_DOUBLE_EQ(ch1.PSDLowerThreshold, 0.0);
    EXPECT_DOUBLE_EQ(ch1.PSDUpperThreshold, 1.0);
    EXPECT_TRUE(ch1.quadraticCut);
    coefs=std::vector<double>({0.1,0.01,0.2});
    EXPECT_EQ(ch1.quadraticCoefficients.size(), coefs.size());
    for (std::size_t i = 0; i < coefs.size(); i++)
    {
        EXPECT_DOUBLE_EQ(ch1.quadraticCoefficients[i], coefs[i]);
    }
    
    EXPECT_TRUE(ch1.timing);
    EXPECT_STREQ(ch1.timingMethod.c_str(), "DCFD");
    EXPECT_DOUBLE_EQ(ch1.cfdFraction, 0.4);
    EXPECT_EQ(ch1.timeDelay, 4);
    EXPECT_EQ(ch1.interpolationPoints, 7);    
}

TEST(getInputTest, getCoincidenceSetting)
{
    std::string f("/home/mingf2/projects/coincidence/test/inputTest.json");
    const InputParameters settings(f);
    const CoincidenceSetting& coinset = settings.coincidenceSetting;
    EXPECT_TRUE(coinset.coincidence);
    EXPECT_EQ(coinset.coincidentChannels.first, 0);
    EXPECT_EQ(coinset.coincidentChannels.second, 8);
    EXPECT_DOUBLE_EQ(coinset.timeWindow, 200);
    EXPECT_TRUE(coinset.plotTOF);
    EXPECT_DOUBLE_EQ(coinset.xmin, -10);
    EXPECT_DOUBLE_EQ(coinset.xmax, 10);
    EXPECT_EQ(coinset.bins, 100);
}

