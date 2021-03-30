/*
 * @Description: Test getInput function from json file
 * @Author: Ming Fang
 * @Date: 2021-03-26 20:12:42
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-30 16:50:31
 */

#include <gtest/gtest.h>
#include <string>
#include "../include/getInput.h"

TEST(getInputTest, cannotFindInput)
{
    std::string f("notexist.json");
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
    std::string f("/media/ming/DATA/coincidence/test/getInputTest.json");
    const InputParameters settings(f);
    EXPECT_EQ(settings.channelSettings.size(), 2);
    const ChannelSettings& ch0 = settings.channelSettings[0];
    const ChannelSettings& ch1 = settings.channelSettings[1];

    EXPECT_EQ(ch0.channelNumber, 0);
    EXPECT_STREQ(ch0.path.c_str(), "/media/ming/DATA/coincidence/test/testdata/channel0.bin");
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
    EXPECT_EQ(ch0.length, 104);
    EXPECT_DOUBLE_EQ(ch0.dcOffset, 0.2);
    EXPECT_EQ(ch0.offset, 8);
    EXPECT_EQ(ch0.savePulses, 0);

    EXPECT_EQ(ch0.preTrig, 24);
    EXPECT_EQ(ch0.preGate, 0);
    EXPECT_EQ(ch0.shortGate, 50);
    EXPECT_EQ(ch0.longGate, 50);
    
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

    // channel 1
    EXPECT_STREQ(ch1.path.c_str(), "/media/ming/DATA/coincidence/test/testdata/channel1.bin");
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
    EXPECT_EQ(ch1.length, 104);
    EXPECT_DOUBLE_EQ(ch1.dcOffset, 0.2);
    EXPECT_EQ(ch1.offset, 8);
    EXPECT_EQ(ch1.savePulses, 0);

    EXPECT_EQ(ch1.preTrig, 24);
    EXPECT_EQ(ch1.preGate, 14);
    EXPECT_EQ(ch1.shortGate, 64);
    EXPECT_EQ(ch1.longGate, 104);
    
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
}

TEST(getInputTest, timingTest)
{
    std::string f("/media/ming/DATA/coincidence/test/getInputTest.json");
    const InputParameters settings(f);
    EXPECT_EQ(settings.channelSettings.size(), 2);
    const ChannelSettings& ch0 = settings.channelSettings[0];
    const ChannelSettings& ch1 = settings.channelSettings[1];

    EXPECT_TRUE(ch0.timing);
    EXPECT_STREQ(ch0.timingMethod.c_str(), "DIACFD");
    EXPECT_DOUBLE_EQ(ch0.cfdFraction, 0.4);
    EXPECT_EQ(ch0.timeDelay, 16);
    EXPECT_EQ(ch0.interpolationPoints, 8);
    EXPECT_EQ(ch0.tsincWidth, 6);
    EXPECT_EQ(ch0.windowSize, 16);
    EXPECT_EQ(ch0.taperConst, 30);
    EXPECT_DOUBLE_EQ(ch0.findTimeReso, 0.25);

    EXPECT_TRUE(ch1.timing);
    EXPECT_STREQ(ch1.timingMethod.c_str(), "DCFD");
    EXPECT_DOUBLE_EQ(ch1.cfdFraction, 0.4);
    EXPECT_EQ(ch1.timeDelay, 16);
    EXPECT_EQ(ch1.interpolationPoints, 8);
    EXPECT_EQ(ch1.tsincWidth, 6);
    EXPECT_EQ(ch1.windowSize, 16);
    EXPECT_EQ(ch1.taperConst, 30);
    EXPECT_DOUBLE_EQ(ch1.findTimeReso, 0.25);

    std::ofstream ofs("/media/ming/DATA/coincidence/test/testdata/sincCoef.txt");
    for (std::size_t i = 0; i < ch0.sincCoefs.size() / ch0.interpolationPoints; i++)
    {
        for (std::size_t j = 0; j < ch0.interpolationPoints; j++)
        {
            ofs << ch0.sincCoefs[i*ch0.interpolationPoints+j] << '\t';
            /* code */
        }
        ofs << '\n';
    }
    ofs.close();
    


}
TEST(getInputTest, plotPHDPID)
{
    std::string f("/media/ming/DATA/coincidence/test/getInputTest.json");
    const InputParameters settings(f);
    EXPECT_EQ(settings.channelSettings.size(), 2);
    const ChannelSettings& ch0 = settings.channelSettings[0];
    const ChannelSettings& ch1 = settings.channelSettings[1];
    
    EXPECT_TRUE(ch0.plotGoodPulses);
    EXPECT_TRUE(ch1.plotGoodPulses);
    EXPECT_TRUE(ch0.plotBadPulses);
    EXPECT_TRUE(ch1.plotBadPulses);
    EXPECT_EQ(ch0.goodNumber, 100);
    EXPECT_EQ(ch1.goodNumber, 100);
    EXPECT_EQ(ch0.badNumber, 100);
    EXPECT_EQ(ch1.badNumber, 100);

    EXPECT_TRUE(ch0.plotPHD);
    EXPECT_TRUE(ch0.plotPHDCalibrated);
    EXPECT_DOUBLE_EQ(ch0.PHDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch0.PHDXmax, 1000.0);
    EXPECT_EQ(ch0.PHDBins, 100);
    EXPECT_TRUE(ch0.plotPID);
    EXPECT_TRUE(ch0.plotPIDCalibrated);
    EXPECT_DOUBLE_EQ(ch0.PIDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch0.PIDXmax, 1000.0);
    EXPECT_EQ(ch0.PIDBins, 100);

    EXPECT_TRUE(ch1.plotPHD);
    EXPECT_TRUE(ch1.plotPHDCalibrated);
    EXPECT_DOUBLE_EQ(ch1.PHDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch1.PHDXmax, 1000.0);
    EXPECT_EQ(ch1.PHDBins, 100);
    EXPECT_TRUE(ch1.plotPID);
    EXPECT_TRUE(ch1.plotPIDCalibrated);
    EXPECT_DOUBLE_EQ(ch1.PIDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch1.PIDXmax, 1000.0);
    EXPECT_EQ(ch1.PIDBins, 100);

}

TEST(getInputTest, plotPSD)
{
    std::string f("/media/ming/DATA/coincidence/test/getInputTest.json");
    const InputParameters settings(f);
    EXPECT_EQ(settings.channelSettings.size(), 2);
    const ChannelSettings& ch0 = settings.channelSettings[0];
    const ChannelSettings& ch1 = settings.channelSettings[1];

    EXPECT_TRUE(ch0.plotPSD);
    EXPECT_TRUE(ch0.plotPSDCalibrated);
    EXPECT_DOUBLE_EQ(ch0.PSDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch0.PSDXmax, 1000.0);
    EXPECT_EQ(ch0.PSDXBins, 100);
    EXPECT_DOUBLE_EQ(ch0.PSDYmin, 0.0);
    EXPECT_DOUBLE_EQ(ch0.PSDYmax, 1.0);
    EXPECT_EQ(ch0.PSDYBins, 100);

    EXPECT_TRUE(ch1.plotPSD);
    EXPECT_TRUE(ch1.plotPSDCalibrated);
    EXPECT_DOUBLE_EQ(ch1.PSDXmin, 0.0);
    EXPECT_DOUBLE_EQ(ch1.PSDXmax, 1000.0);
    EXPECT_EQ(ch1.PSDXBins, 100);
    EXPECT_DOUBLE_EQ(ch1.PSDYmin, 0.0);
    EXPECT_DOUBLE_EQ(ch1.PSDYmax, 1.0);
    EXPECT_EQ(ch1.PSDYBins, 100);

}

TEST(getInputTest, getCoincidenceSetting)
{
    std::string f("/media/ming/DATA/coincidence/test/getInputTest.json");
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

