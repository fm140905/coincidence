#include "coincidence.h"

Int_t find_coincidence(const Parameters& setting, const std::vector< std::vector<ULong64_t> > &timestamps, std::vector< std::vector< UInt_t > > &pairs)
{
    const std::vector<Int_t> &channels = setting.CoincidenceChannels;
    //std::cout << "numbero of channels: " << channels.size() << std::endl;
    if (channels.size() < 2)
    {
        std::cout << "At least 2 channels are needed to perform coincidence measurement." << std::endl;
        exit(1);
    }

    // for(int i=0;i<channels.size;i++)
    // {
    //     if(std::find(channels.begin(),channels.end(),i) == channels.end())
    //     {
    //         std::cout << "Coincidence channel " << i << " is not in the input channels" << std::endl;
    //         exit(1);
    //     }
    // }
    const Int_t chIndex0 = setting.CoincidenceChannels[0];
    const Int_t chIndex1 = setting.CoincidenceChannels[1];
    // const Int_t chIndex1 = chIndex0 + 1;
    const Long64_t timewindow = 1000 * setting.TimeWindow;
    std::vector<UInt_t> pairstemp{0, 0};
    Long64_t timediff(0);
    //std::vector<Int_t> timeDiffs;
    //UInt_t j(0);
    UInt_t i = 0;
    UInt_t j = 0;
    //std::ofstream testfile("output/timediff_DCFD");
    //testfile << "i" << '\t' << "j" << '\t' << "timediff(ps)" << std::endl;
    while (i < timestamps[0].size() && j < timestamps[1].size())
    {
        timediff = timestamps[0][i] - timestamps[1][j];
        //timediff = (timestamps[chIndex0][i] / 2000 - timestamps[chIndex1][i] / 2000) * 2000;
        //testfile << timediff << '\n';
        if (timediff - timewindow > 0 )
        {
            j++;
        }

        else if (timediff + timewindow < 0)
        {
            i++;
        }

        else
        {
            pairstemp[0] = i;
            pairstemp[1] = j;
            pairs.push_back(pairstemp);
            //timeDiffs.push_back(timediff);
            //testfile << timestamps[chIndex0][i] << '\t' << timestamps[chIndex1][i] << '\t' << timediff << '\n';
            // testfile << timestamps[chIndex0][i] << '\t' << timestamps[chIndex1][j] << '\t' << timediff << '\n';
            i++;
            j++;
        }
    }
    //std::cout << mini << std::endl;
    //std::cout << i <<  " " << j << std::endl;

    // for(int i=0, j=0;i < timestamps[chIndex0].size() && j < timestamps[chIndex1].size();)
    // {
    //     if( timestamps[chIndex0][i]-timestamps[chIndex1][j] > timewindow * 1000 )
    //     {
    //         j++;
    //     }

    //     else if (timestamps[chIndex1][j]-timestamps[chIndex0][i] > timewindow * 1000)
    //     {
    //         i++;
    //     }

    //     else
    //     {
    //         pairstemp[0] = i;
    //         pairstemp[1] = j;
    //         pairs.push_back(pairstemp);
    //         i++;
    //         j++;
    //     }
    // }
    return 0;
}