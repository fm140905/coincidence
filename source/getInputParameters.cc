#include "getInputParameters.h"

std::string trim(const std::string &str,
                 const std::string &whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string removeComments(const std::string &str,
                           const std::string &begChar)
{
    const auto strBegin = str.find_first_of(begChar);
    if (strBegin == std::string::npos)
        return str; // no comment

    if (strBegin == 0)
        return ""; // no content

    return str.substr(0, strBegin);
}

int getInputParameters(std::string filepath, Parameters &setting)
{
    std::ifstream fileptr;
    //std::vector<std::string> tags
    //std::ofstream testout("testout.txt");
    fileptr.open(filepath, std::ios::in);
    if (!fileptr.is_open())
    {
        std::cout << "can't open file " << filepath << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::string> lines;
    std::vector<std::string> newlines;
    std::string tag[] = {"NHeaders: ", "Headersize: ", "NSamples: ",
                         "Samplesize: ", "Dynamicrange: ", "Resolultion: ",
                         "Offset: ", "Delt: ", "SavePulses: ", "Filter bad: ",
                         "Zerosupression: ", "MinVoltage: ", "Clipped: ",
                         "MaxVoltage: ", "Save bad: ", "Filter piled-up: ",
                         "Save piled-up: ", "Save S_time: ", "Save S_frequency: ",
                         "PHD: ", "SavePHD: ", "PID: ", "Pre: ", "Post: ",
                         "SavePID: ", "TailvsTotal: ", "Tailstart: ",
                         "SaveTailvsTotal: ", "MaxNumPulses: ", "Time stamp: ",
                         "SaveTimeStamp: ", "Directory: ", "Folders: ",
                         "Channels: ", "Filetype: ", "Coincidence: ",
                         "Coincidence Channels: ", "Time Window: ", "Filter Width: ",
                         "Fraction: ","Time Delay: ","Interpolation: ", "SaveDT: ",
                         "TOF: ", "SaveTOF: ", "Calibration: ", "EnergyCut: ",
                         "EnergyLow: ","EnergyHigh: ","PSDCut: ","PSDLow: ",
                         "PSDHigh: ","SavePH: ","SavePI: ", "Polarity: "};
    while (getline(fileptr, line))
    {
        lines.push_back(line);
    }
    UInt_t lineindex(0);
    while (lineindex < lines.size())
    {
        line = lines[lineindex];
        line = trim(line); //remove all leading and trailing " \t".
        if (line.length() > 1)
        {
            //remove all comments (starts with "#")
            line = removeComments(line, "#");
        }
        if (line.length() > 1)
        {
            //testout << line << std::endl;
            newlines.push_back(line);
        }
        lineindex++;
    }
    lines.clear();

    for (int i = 0; i < newlines.size(); i++)
    {
        line = newlines[i];
        // 1st
        std::size_t taglength(0);
        if (line.find(tag[0]) == 0)
        {
            taglength = tag[0].length();
            setting.NHeaders = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 2nd
        else if (line.find(tag[1]) == 0)
        {
            taglength = tag[1].length();
            // setting.Headersize = stoi(line.substr(taglength, std::string::npos));
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.Headersize.push_back(stoi(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 3rd
        else if (line.find(tag[2]) == 0)
        {
            taglength = tag[2].length();
            setting.NSamples = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 4th
        else if (line.find(tag[3]) == 0)
        {
            taglength = tag[3].length();
            setting.Samplesize = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 5th
        else if (line.find(tag[4]) == 0)
        {
            taglength = tag[4].length();
            setting.DynamicRange = stof(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 6th, resolution
        else if (line.find(tag[5]) == 0)
        {
            taglength = tag[5].length();
            setting.Resolution = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 7th, offset
        else if (line.find(tag[6]) == 0)
        {
            taglength = tag[6].length();
            setting.Offset = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 8th, delta t
        else if (line.find(tag[7]) == 0)
        {
            taglength = tag[7].length();
            setting.Delt = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 9th
        else if (line.find(tag[8]) == 0)
        {
            taglength = tag[8].length();
            setting.SavePulses = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 10th
        else if (line.find(tag[9]) == 0)
        {
            taglength = tag[9].length();
            setting.FilterBad = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 11th
        else if (line.find(tag[10]) == 0)
        {
            taglength = tag[10].length();
            setting.ZeroSupression = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 12th
        else if (line.find(tag[11]) == 0)
        {
            taglength = tag[11].length();
            setting.MinVoltage = stof(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 13th
        else if (line.find(tag[12]) == 0)
        {
            taglength = tag[12].length();
            setting.Clipped = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 14th
        else if (line.find(tag[13]) == 0)
        {
            taglength = tag[13].length();
            setting.MaxVoltage = stof(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 15th
        else if (line.find(tag[14]) == 0)
        {
            taglength = tag[14].length();
            setting.SaveBad = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 16th
        else if (line.find(tag[15]) == 0)
        {
            taglength = tag[15].length();
            setting.FilterPiledup = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 17th
        else if (line.find(tag[16]) == 0)
        {
            taglength = tag[16].length();
            setting.SavePiledup = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 18th
        else if (line.find(tag[17]) == 0)
        {
            taglength = tag[17].length();
            setting.SaveSTime = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 19th
        else if (line.find(tag[18]) == 0)
        {
            taglength = tag[18].length();
            setting.SaveSFrequency = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 20th
        else if (line.find(tag[19]) == 0)
        {
            taglength = tag[19].length();
            setting.PHD = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 21st
        else if (line.find(tag[20]) == 0)
        {
            taglength = tag[20].length();
            setting.SavePHD = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 22nd
        else if (line.find(tag[21]) == 0)
        {
            taglength = tag[21].length();
            setting.PID = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 23rd
        else if (line.find(tag[22]) == 0)
        {
            taglength = tag[22].length();
            setting.Pre = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 24th
        else if (line.find(tag[23]) == 0)
        {
            taglength = tag[23].length();
            setting.Post = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 25th
        else if (line.find(tag[24]) == 0)
        {
            taglength = tag[24].length();
            setting.SavePID = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 26th
        else if (line.find(tag[25]) == 0)
        {
            taglength = tag[25].length();
            setting.TailvsTotal = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 27th
        else if (line.find(tag[26]) == 0)
        {
            taglength = tag[26].length();
            setting.TailStart = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 28th
        else if (line.find(tag[27]) == 0)
        {
            taglength = tag[27].length();
            setting.SaveTailvsTotal = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 29th
        else if (line.find(tag[28]) == 0)
        {
            taglength = tag[28].length();
            setting.MaxNumPulses = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        //30th
        else if (line.find(tag[29]) == 0)
        {
            taglength = tag[29].length();
            setting.TimeStamp = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 31th
        else if (line.find(tag[30]) == 0)
        {
            taglength = tag[30].length();
            setting.SaveTimeStamp = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 32th
        else if (line.find(tag[31]) == 0)
        {
            taglength = tag[31].length();
            setting.Directory = line.substr(taglength, std::string::npos);
            setting.Directory = setting.Directory.substr(0, setting.Directory.length() - 1);
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 33th
        else if (line.find(tag[32]) == 0)
        {
            taglength = tag[32].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.Folders.push_back(stoi(substr));
            }

            if (setting.Folders.size() == 1)
            {
                setting.Folders.push_back(setting.Folders[0]);
            }

            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 34th
        else if (line.find(tag[33]) == 0)
        {
            taglength = tag[33].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.Channels.push_back(stoi(substr));
            }

            // if (setting.Channels.size() == 1)
            // {
            //     setting.Channels.push_back(setting.Channels[0]);
            // }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 35th
        else if (line.find(tag[34]) == 0)
        {
            taglength = tag[34].length();
            setting.Filetype = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 36th
        else if (line.find(tag[35]) == 0)
        {
            taglength = tag[35].length();
            setting.Coincidence = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 37th
        else if (line.find(tag[36]) == 0)
        {
            taglength = tag[36].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.CoincidenceChannels.push_back(stoi(substr));
            }
        }
        // 38th
        else if (line.find(tag[37]) == 0)
        {
            taglength = tag[37].length();
            setting.TimeWindow = stof(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 39th
        else if (line.find(tag[38]) == 0)
        {
            taglength = tag[38].length();
            setting.FilterWidth = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 40th
        else if (line.find(tag[39]) == 0)
        {
            taglength = tag[39].length();
            setting.Fraction = stof(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 41th
        else if (line.find(tag[40]) == 0)
        {
            taglength = tag[40].length();
            setting.TimeDelay = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 42th
        else if (line.find(tag[41]) == 0)
        {
            taglength = tag[41].length();
            setting.Interpolation = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 43th
        else if (line.find(tag[42]) == 0)
        {
            taglength = tag[42].length();
            setting.SaveDT = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 44th
        else if (line.find(tag[43]) == 0)
        {
            taglength = tag[43].length();
            setting.TOF = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 45th
        else if (line.find(tag[44]) == 0)
        {
            taglength = tag[44].length();
            setting.SaveTOF = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 46th
        else if (line.find(tag[45]) == 0)
        {
            taglength = tag[45].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.Calicoefs.push_back(stof(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 47th
        else if (line.find(tag[46]) == 0)
        {
            taglength = tag[46].length();
            // setting.EnergyCut = stoi(line.substr(taglength, std::string::npos));
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.EnergyCut.push_back(stoi(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 48th
        else if (line.find(tag[47]) == 0)
        {
            taglength = tag[47].length();
            // setting.EnergyLoW = stof(line.substr(taglength, std::string::npos));
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.EnergyLoW.push_back(stof(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 49th
        else if (line.find(tag[48]) == 0)
        {
            taglength = tag[48].length();
            // setting.EnergyHigh = stof(line.substr(taglength, std::string::npos));
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.EnergyHigh.push_back(stof(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 50th
        else if (line.find(tag[49]) == 0)
        {
            taglength = tag[49].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.PSDCut.push_back(stoi(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 51th
        else if (line.find(tag[50]) == 0)
        {
            taglength = tag[50].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.PSDLow.push_back(stof(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 52th
        else if (line.find(tag[51]) == 0)
        {
            taglength = tag[51].length();
            std::string strTemp = line.substr(taglength, std::string::npos);
            std::stringstream ss(strTemp);
            while (ss.good())
            {
                std::string substr;
                getline(ss, substr, ',');
                setting.PSDHigh.push_back(stof(substr));
            }
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 53th
        else if (line.find(tag[52]) == 0)
        {
            taglength = tag[52].length();
            setting.SavePH = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 54th
        else if (line.find(tag[53]) == 0)
        {
            taglength = tag[53].length();
            setting.SavePI = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // 55rd
        else if (line.find(tag[54]) == 0)
        {
            taglength = tag[54].length();
            setting.Polarity = stoi(line.substr(taglength, std::string::npos));
            //std::cout << line.substr(taglength,std::string::npos) << std::endl;
        }
        // add more options here.
    }
    // std::cout << setting.Filetype <<'\n' 
    //         << setting.Folders[0] <<'\t' << setting.Folders[1]<<"\n"
    //         << setting.Channels.size() << '\n' 
    //         << setting.NHeaders <<"\t"<< setting.Headersize <<"\n"
    //         << setting.NSamples <<"\t"<< setting.Samplesize <<"\n"
    //         << setting.DynamicRange <<"\n"<< setting.Resolution <<"\n"
    //         << setting.Offset <<"\n" << setting.Delt<<"\n"
    //         << setting.SavePulses <<"\n" <<setting.FilterBad <<"\n"
    //         << setting.ZeroSupression<<"\n" << setting.MinVoltage<<"\n"
    //         << setting.Clipped <<"\n" << setting.MaxVoltage<<"\n"
    //         << setting.SaveBad <<"\n" <<setting.FilterPiledup<<"\n" 
    //         << setting.SavePiledup<<"\n" <<setting.SaveSFrequency<<"\n" 
    //         << setting.SaveSTime<<"\n" <<setting.PHD<<"\n" 
    //         << setting.SavePHD<<"\n" <<setting.PID<<"\n" 
    //         << setting.Pre<<"\n" <<setting.Post<<"\n" 
    //         << setting.SavePID<<"\n" <<setting.TailvsTotal<<"\n" 
    //         << setting.TailStart<<"\n" <<setting.SaveTailvsTotal<<"\n" 
    //         << setting.Coincidence <<"\t" << setting.CoincidenceChannels[0] <<"\t"
    //         << setting.CoincidenceChannels[1] <<"\n" << setting.TimeWindow << '\n'
    //         << setting.FilterWidth << '\t' << setting.Fraction << '\t'
    //         << setting.TimeDelay << '\t' << std::endl;
    // testout.close();
    return 0;
}