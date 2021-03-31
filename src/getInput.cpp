/**
 * @file getInput.cpp
 * @author Ming Fang
 * @brief Get input parameters from a json file
 * @date 2021-03-30
 */

#include "getInput.h"

int ChannelSettings::getSampleNumber()
{
    std::ifstream fileptr;
    fileptr.open(path, std::ios::in | std::ios::binary);
    if (!fileptr.good())
    {
        throw std::invalid_argument(path + "does not exit.");
    }
    while (!fileptr.eof())
    {
        //read header
        fileptr.ignore(std::accumulate(headers.begin(), headers.end()-1, 0));

        fileptr.read((char *)&(length), headers[6]);
        break;
    }
    fileptr.close();

    return 0;
}

int ChannelSettings::timingSetup()
{
    interpolationPoints++;
    startIndex = preTrig - windowSize / 2;
    startIndex = (startIndex > 0) ? startIndex : 0;
    timeDelay = timeDelay / timestep * interpolationPoints;
    findTimeReso = timestep;
    findTimeReso /= interpolationPoints;

    // get tSinc coeff
    Double_t temp(0);
    double_t phi(0);
    sincCoefs.push_back(1);
    for(int j = 1; j < tsincWidth * interpolationPoints; j++)
    {
        phi = j * M_PI;
        phi /= interpolationPoints;
        temp = j;
        temp /= taperConst;
        temp = sin(phi) / phi * exp(-temp * temp);
        sincCoefs.push_back(temp);
    }
    return 0;
}
InputParameters::InputParameters(const std::string fpath)
{
    // read the JSON input file
    std::ifstream ifs(fpath);
    if(!ifs.good()){
        throw std::invalid_argument("Input file does not exit.");
    }
    rapidjson::IStreamWrapper isw(ifs);
    jsonInput.ParseStream<rapidjson::kParseCommentsFlag>(isw);

    getDefaultChannelSetting();
    getSpecificChannelSettings();
    getCoincidenceSetting();
}

int InputParameters::getChannelSetting(const rapidjson::Value& v, ChannelSettings& chset)
{
    if (v.HasMember("ChannelNumber"))
        chset.channelNumber = v["ChannelNumber"].GetUint();
    if (v.HasMember("ON"))
        chset.processOutput = v["ON"].GetBool();
    if (v.HasMember("Path"))
        chset.path = v["Path"].GetString();
    
    if (v.HasMember("MaxNumPulses"))
        chset.maxNumPulses = v["MaxNumPulses"].GetUint64();
    if (v.HasMember("Polarity"))
        chset.polarity = v["Polarity"].GetString();
    if (v.HasMember("Dynamicrange"))
        chset.dynamicRange =v["Dynamicrange"].GetDouble();
    if (v.HasMember("TimeStep"))
        chset.timestep = v["TimeStep"].GetUint();
    if (v.HasMember("Resolution"))
        chset.resolution = v["Resolution"].GetUint();
    if (v.HasMember("Headers"))
    {
        chset.headers.clear();
        for (auto& i : v["Headers"].GetArray()){
            chset.headers.push_back(i.GetInt());
        }
    }
    if (v.HasMember("SaveHeaders"))
    {
        chset.saveHeaders.clear();
        for (auto& i : v["SaveHeaders"].GetArray()){
            chset.saveHeaders.push_back(i.GetBool());
        }
    }
    if (v.HasMember("SampleSize"))
        chset.sampleSize = v["SampleSize"].GetInt();
    if (v.HasMember("RecordLength"))
        chset.length = v["RecordLength"].GetInt();
    if (v.HasMember("DCOffset"))
        chset.dcOffset = v["DCOffset"].GetDouble();
    if (v.HasMember("BaselineSamples"))
        chset.offset = v["BaselineSamples"].GetInt();
    if (v.HasMember("SavePulses"))
        chset.savePulses = v["SavePulses"].GetInt();

    if (v.HasMember("PreTrigger"))
        chset.preTrig = v["PreTrigger"].GetInt();
    if (v.HasMember("Gates"))
    {   
        if (v["Gates"].HasMember("PreGate")){
            chset.preGate = v["Gates"]["PreGate"].GetInt();
        }
        if (v["Gates"].HasMember("ShortGate")){
            chset.shortGate = v["Gates"]["ShortGate"].GetInt();
        }
        if (v["Gates"].HasMember("LongGate")){
            chset.longGate = v["Gates"]["LongGate"].GetInt();
        }
        
    }
    if (v.HasMember("Calibration")){
        if (v["Calibration"].HasMember("Height"))
        {
            if (v["Calibration"]["Height"].HasMember("ON"))
            {
                chset.calibrationHeight = v["Calibration"]["Height"]["ON"].GetBool();
            }
            if (v["Calibration"]["Height"].HasMember("Coefficient"))
            {
                chset.calibrationHeightCoefficient = v["Calibration"]["Height"]["Coefficient"].GetDouble();
            }
        }
        if (v["Calibration"].HasMember("Integral"))
        {
            if (v["Calibration"]["Integral"].HasMember("ON"))
            {
                chset.calibrationIntegral = v["Calibration"]["Integral"]["ON"].GetBool();
            }
            if (v["Calibration"]["Integral"].HasMember("Coefficient"))
            {
                chset.calibrationIntegralCoefficient = v["Calibration"]["Integral"]["Coefficient"].GetDouble();
            }
        }
    }
    
    if (v.HasMember("Rejection")){
        if (v["Rejection"].HasMember("ON"))
            chset.rejection = v["Rejection"]["ON"].GetBool();
        if (v["Rejection"].HasMember("ZeroSupression"))
            chset.zeroSupression = v["Rejection"]["ZeroSupression"].GetBool();
        if (v["Rejection"].HasMember("LowVoltageThreshold"))
            chset.lowVoltageThreshold = v["Rejection"]["LowVoltageThreshold"].GetDouble();
        if (v["Rejection"].HasMember("ClipRejection"))
            chset.clipRejection = v["Rejection"]["ClipRejection"].GetBool();
    }

    if (v.HasMember("EnergyCut")){
        if (v["EnergyCut"].HasMember("ON"))
            chset.energyCut = v["EnergyCut"]["ON"].GetBool();
        if (v["EnergyCut"].HasMember("LowerThreshold"))
            chset.EnergyLowerThreshold = v["EnergyCut"]["LowerThreshold"].GetDouble();
        if (v["EnergyCut"].HasMember("UpperThreshold"))
            chset.EnergyUpperThreshold = v["EnergyCut"]["UpperThreshold"].GetDouble();
    }

    if (v.HasMember("PSDCut")){
        if (v["PSDCut"].HasMember("ON"))
            chset.PSDCut = v["PSDCut"]["ON"].GetBool();
        if (v["PSDCut"].HasMember("LazyCut"))
            chset.lazyCut = v["PSDCut"]["LazyCut"].GetBool();
        if (v["PSDCut"].HasMember("LowerThreshold"))
            chset.PSDLowerThreshold = v["PSDCut"]["LowerThreshold"].GetDouble();
        if (v["PSDCut"].HasMember("UpperThreshold"))
            chset.PSDUpperThreshold = v["PSDCut"]["UpperThreshold"].GetDouble();
        if (v["PSDCut"].HasMember("QuadraticCut"))
            chset.quadraticCut = v["PSDCut"]["QuadraticCut"].GetBool();
        if (v["PSDCut"].HasMember("QuadracticCoefficients")){
            assert(v["PSDCut"]["QuadracticCoefficients"].Size() == 3);
            chset.quadraticCoefficients.clear();
            for (auto& v : v["PSDCut"]["QuadracticCoefficients"].GetArray()){
                chset.quadraticCoefficients.push_back(v.GetDouble());
            }
        }
    }

    if (v.HasMember("Timing")){
        if (v["Timing"].HasMember("ON"))
            chset.timing = v["Timing"]["ON"].GetBool();
        if (v["Timing"].HasMember("Method"))
            chset.timingMethod = v["Timing"]["Method"].GetString();
        if (v["Timing"].HasMember("Interpolation"))
            chset.interpolationPoints = v["Timing"]["Interpolation"].GetInt();
        if (v["Timing"].HasMember("Fraction"))
            chset.cfdFraction = v["Timing"]["Fraction"].GetDouble();
        if (v["Timing"].HasMember("TimeDelay"))
            chset.timeDelay = v["Timing"]["TimeDelay"].GetInt();
    }

    if (v.HasMember("PlotPulses")){
        if (v["PlotPulses"].HasMember("Good pulses"))
        {
            if (v["PlotPulses"]["Good pulses"].HasMember("ON"))
            {
                chset.plotGoodPulses = v["PlotPulses"]["Good pulses"]["ON"].GetBool();
            }
            if (v["PlotPulses"]["Good pulses"].HasMember("Number"))
            {
                chset.goodNumber = v["PlotPulses"]["Good pulses"]["Number"].GetInt();
            }
        }
        if (v["PlotPulses"].HasMember("Rejected pulses"))
        {
            if (v["PlotPulses"]["Rejected pulses"].HasMember("ON"))
            {
                chset.plotBadPulses = v["PlotPulses"]["Rejected pulses"]["ON"].GetBool();
            }
            if (v["PlotPulses"]["Rejected pulses"].HasMember("Number"))
            {
                chset.badNumber = v["PlotPulses"]["Rejected pulses"]["Number"].GetInt();
            }
        }
    }

    if (v.HasMember("Plot Pulse Height Distribution")){
        if (v["Plot Pulse Height Distribution"].HasMember("ON"))
            chset.plotPHD = v["Plot Pulse Height Distribution"]["ON"].GetBool();
        if (v["Plot Pulse Height Distribution"].HasMember("Calibrated"))
            chset.plotPHDCalibrated = v["Plot Pulse Height Distribution"]["Calibrated"].GetBool();
        if (v["Plot Pulse Height Distribution"].HasMember("xmin"))
            chset.PHDXmin = v["Plot Pulse Height Distribution"]["xmin"].GetDouble();
        if (v["Plot Pulse Height Distribution"].HasMember("xmax"))
            chset.PHDXmax = v["Plot Pulse Height Distribution"]["xmax"].GetDouble();
        if (v["Plot Pulse Height Distribution"].HasMember("Bins"))
            chset.PHDBins = v["Plot Pulse Height Distribution"]["Bins"].GetInt();
    }

    if (v.HasMember("Plot Pulse Integral Distribution")){
        if (v["Plot Pulse Integral Distribution"].HasMember("ON"))
            chset.plotPID = v["Plot Pulse Integral Distribution"]["ON"].GetBool();
        if (v["Plot Pulse Integral Distribution"].HasMember("Calibrated"))
            chset.plotPIDCalibrated = v["Plot Pulse Integral Distribution"]["Calibrated"].GetBool();
        if (v["Plot Pulse Integral Distribution"].HasMember("xmin"))
            chset.PIDXmin = v["Plot Pulse Integral Distribution"]["xmin"].GetDouble();
        if (v["Plot Pulse Integral Distribution"].HasMember("xmax"))
            chset.PIDXmax = v["Plot Pulse Integral Distribution"]["xmax"].GetDouble();
        if (v["Plot Pulse Integral Distribution"].HasMember("Bins"))
            chset.PIDBins = v["Plot Pulse Integral Distribution"]["Bins"].GetInt();
    }

    if (v.HasMember("Plot PSD")){
        if (v["Plot PSD"].HasMember("ON"))
            chset.plotPSD = v["Plot PSD"]["ON"].GetBool();
        if (v["Plot PSD"].HasMember("Calibrated"))
            chset.plotPSDCalibrated = v["Plot PSD"]["Calibrated"].GetBool();
        if (v["Plot PSD"].HasMember("xmin"))
            chset.PSDXmin = v["Plot PSD"]["xmin"].GetDouble();
        if (v["Plot PSD"].HasMember("xmax"))
            chset.PSDXmax = v["Plot PSD"]["xmax"].GetDouble();
        if (v["Plot PSD"].HasMember("XBins"))
            chset.PSDXBins = v["Plot PSD"]["XBins"].GetInt();
        if (v["Plot PSD"].HasMember("ymin"))
            chset.PSDYmin = v["Plot PSD"]["ymin"].GetDouble();
        if (v["Plot PSD"].HasMember("ymax"))
            chset.PSDYmax = v["Plot PSD"]["ymax"].GetDouble();
        if (v["Plot PSD"].HasMember("YBins"))
            chset.PSDYBins = v["Plot PSD"]["YBins"].GetInt();
    }

    return 0;
}
int InputParameters::getDefaultChannelSetting()
{
    assert(jsonInput.HasMember("ChannelSettings"));
    assert(jsonInput["ChannelSettings"].HasMember("Defaults"));
    
    const rapidjson::Value& defaults_=jsonInput["ChannelSettings"]["Defaults"];
    getChannelSetting(defaults_, defaultChannelSetting);
    // std::cout << defaultChannelSetting.maxNumPulses << ' ' << defaultChannelSetting.resolution << std::endl;
    return 0;
}

int InputParameters::getSpecificChannelSettings()
{
    const rapidjson::Value& channelSpecs_=jsonInput["ChannelSettings"]["ChannelSpecific"];
    assert(channelSpecs_.IsArray());
    for (rapidjson::SizeType i = 0; i < channelSpecs_.Size(); i++){
        ChannelSettings channelI = defaultChannelSetting;
        getChannelSetting(channelSpecs_[i], channelI);
        channelI.getSampleNumber();
        // std::cout << channelI.length << "Samples." << std::endl;

        channelI.eventSize = std::accumulate(channelI.headers.begin(), 
           channelI.headers.end(), 0) + channelI.length * channelI.sampleSize;

        if (channelI.polarity == "negative")
        {
            channelI.polarityCoef = -1;
        }
        else
        {
            channelI.polarityCoef = 1;
        }
        
        channelI.lsb2voltage = channelI.dynamicRange / (pow(2, channelI.resolution) - 1);
        
        channelI.offset = (channelI.offset > channelI.length) ? channelI.length : channelI.offset;

        channelI.preTrig = channelI.preTrig / channelI.timestep;
        
        channelI.preGate = channelI.preGate / channelI.timestep;
        channelI.preGate = channelI.preTrig - channelI.preGate;
        if(channelI.preGate < 0){
            channelI.preGate = 0;
        }
        
        channelI.longGate = channelI.longGate  / channelI.timestep;
        channelI.longGate += channelI.preGate;
        if(channelI.longGate > channelI.length){
            channelI.longGate = channelI.length;
        }

        channelI.shortGate = channelI.shortGate / channelI.timestep;
        channelI.shortGate += channelI.preGate;
        if(channelI.shortGate > channelI.length){
            channelI.shortGate = channelI.length;
        }
        
        if (channelI.timing)
        {
            channelI.timingSetup();
        }
        
        channelSettings.push_back(channelI);
    }

    return 0;
}

int InputParameters::getCoincidenceSetting()
{
    const rapidjson::Value& coinSets_=jsonInput["CoincidenceSettings"];
    coincidenceSetting.coincidence=coinSets_["ON"].GetBool();
    coincidenceSetting.coincidence=coinSets_["ON"].GetBool();
    // now only support finding coincidences between two channels
    assert(coinSets_["CoincidenceChannels"].Size() == 2);
    coincidenceSetting.coincidentChannels.first = coinSets_["CoincidenceChannels"][0].GetUint();
    coincidenceSetting.coincidentChannels.second = coinSets_["CoincidenceChannels"][1].GetUint();
    
    coincidenceSetting.timeWindow = coinSets_["TimeWindow"].GetUint();
    coincidenceSetting.saveTimeDifference = coinSets_["SaveTimeDifference"].GetBool();

    coincidenceSetting.plotTOF = coinSets_["PlotTOF"]["ON"].GetBool();
    coincidenceSetting.xmin = coinSets_["PlotTOF"]["xmin"].GetDouble();
    coincidenceSetting.xmax = coinSets_["PlotTOF"]["xmax"].GetDouble();
    coincidenceSetting.bins = coinSets_["PlotTOF"]["Bins"].GetInt();

    return 0;
}