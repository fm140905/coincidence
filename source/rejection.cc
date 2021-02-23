#include "rejection.h"

Int_t rejection(const Parameters &setting, std::vector<Event> &events)
{
    //std::vector<Int_t> BadIndexes;
    Int_t badnumber(0);
    Int_t indexTemp(0);
    Float_t heightTemp(0);
    // const Float_t fraction = setting.Rejection;

    // std::cout << "Start filtering bad pulses." << std::endl;

    if (!setting.FilterBad)
    {
        std::cout << "You choose not to filter bad pulses." << std::endl;
        return 0;
    }

    // filter bad pulses
    for (int i = 0; i < events.size(); i++)
    {
        indexTemp = events[i].heightindex;
        heightTemp = events[i].voltage[indexTemp];

        //zero supression
        if (setting.ZeroSupression && heightTemp < setting.MinVoltage)
        {
            events[i].isBad = true;
        }

        // filter clipped pulses
        if (!events[i].isBad)
        {
            if (setting.Clipped &&
                (events[i].voltage[indexTemp - 1] == heightTemp ||
                 events[i].voltage[indexTemp + 1] == heightTemp))
            {
                events[i].isBad = true;
            }
        }

        // filter baseline-shifted pulses
        if (!events[i].isBad)
        {
            for (int j = 0; j < setting.Offset; j++)
            {
                if (events[i].voltage[j] > 0.1 /*setting.MinVoltage*/)
                {
                    events[i].isBad = true;
                    break;
                }
            }
        }

        // if (!events[i].isBad)
        // {
        //     if(events[i].voltage[indexTemp -1 ] >= fraction * events[i].voltage[indexTemp] || events[i].voltage[indexTemp + 1 ] >= fraction * events[i].voltage[indexTemp])
        //     {
        //         events[i].isBad = true;
        //     }
        // }

        if (events[i].isBad)
        {
            //BadIndexes.push_back(i);
            badnumber++;
        }
    }

    std::cout << " " << badnumber << " bad pulses are found." << std::endl;
    //save bad pulses
    /*
    if(setting.SaveBad)
    {
        std::ofstream badpulsesfile("BadPulses.txt");
        for(int i=0;i<BadIndexes.size();i++)
        {
            indexTemp = BadIndexes[i];
            for(int j=0;j<events[indexTemp].voltage.size();j++)
            {
                badpulsesfile << events[indexTemp].voltage[j] <<"  ";
            }
            badpulsesfile << std::endl;
        }
        badpulsesfile.close();
        std::cout << " Bad pulses were saved to file: BadPulses.txt."
                  << std::endl;
    }
*/
    // BadIndexes.clear();
    std::cout << "Finished filtering bad pulses." << std::endl;
    return 0;
}

Int_t energyCut(const Parameters &setting, std::vector< Event >& events, const int &channelI )
{
    if(setting.EnergyCut[channelI])
    {
        for(int i=0; i<events.size(); i++)
        {
            if (events[i].energy < setting.EnergyLoW[channelI] || events[i].energy > setting.EnergyHigh[channelI]) {
                events[i].isBad = true;
            }
            
        }
    }
    return 0;
}

Int_t PSDCut(const Parameters &setting, const UInt_t index, std::vector< Event >& events )
{
    Float_t psdtemp(0);
    if(setting.PSDCut[index])
    {
        for(int i=0; i<events.size(); i++)
        {
            psdtemp = events[i].tailIntegral / events[i].totalIntegral;
            if (psdtemp < setting.PSDLow[index] || psdtemp > setting.PSDHigh[index]) {
                events[i].isBad = true;
            }
            
        }
    }
    return 0;
}