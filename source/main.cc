#include <vector>
#include <ctime>
#include <sys/time.h>
#include <sys/stat.h>
#include <numeric>
/* ROOT */
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
// #include "TF1.h"
// #include "TGraph.h"
// #include "TMultiGraph.h"

#include "getInputParameters.h"
#include "events.h"
#include "output.h"
#include "readData.h"
#include "rejection.h"
#include "getTimeStamp.h"
#include "coincidence.h"
#include "getPlot.h"

int main(int argc, char *argv[])
{
    struct stat st = {0};
    // create directories to save the output files and pics, works in Linux
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    const std::string settingFilePath(argv[1]);
    Parameters setting;
    //const std::string dataFilePath("/mnt/d/Projects/PSD/1/wave4.dat");
    //std::vector<Event> pulses;
    // TApplication *myapp = new TApplication("myApp", 0, 0);

    // step 0 read input parameters
    getInputParameters(settingFilePath, setting);
    getSampleNum(setting);
    // return 0;

    //// step 1, read the data in binary/ASCII format, calculate the baseline, convert to voltage, calculate heightIndex.
    std::vector< std::vector< Event > > coincidentEvents;
    // coincidence
    if(setting.Coincidence)
    {
        std::cout << "*****Start Picking up Coincidences between Channel";
        for(int i=0;i<setting.CoincidenceChannels.size();i++)
        {
            std::cout << setting.CoincidenceChannels[i] << '\t';
        }
        std::cout <<"*****"<< std::endl;

        std::vector< std::vector<UShort_t> > flags;
        std::vector< std::vector<ULong64_t> > timeStampsHeader;
        readTimeStamp(setting, flags, timeStampsHeader);
        // std::cout<< timeStampsHeader[0].size() << '\t' << timeStampsHeader[1].size() << std::endl;

        std::vector< std::vector< UInt_t > > coincidentIndexes;
        find_coincidence(setting, timeStampsHeader, coincidentIndexes); // find coincidence between 2 channels
        std::cout << "*****Finished picking up coincidences. " << std::endl;
        // std::ofstream pairfile("pairs");
        // for(int i=0; i<indexpairs.size();i++)
        // {
        //     pairfile << indexpairs[i][0] << '\t' << indexpairs[i][1] << '\n';
        // }
        // pairfile.close();
        std::vector< std::vector<ULong64_t> >().swap(timeStampsHeader);
        std::vector< std::vector<UShort_t> >().swap(flags); 
        // std::cout << coincidentIndexes.size() << std::endl;
        if(coincidentIndexes.size() == 0)
        {
            std::cout << " No coincidences are found.*****" << std::endl;
            return 0;
        }
        else
        {
            std::cout << coincidentIndexes.size() <<" coincidences are found.*****" << std::endl;
            std::cout << "*****Start reading coincident pulses..*****" << std::endl;

            // read coincident pulses
            readCoincidences(setting, coincidentIndexes, coincidentEvents);
            // newPulseHeight(setting, coincidentEvents);
            // std::cout << "*****Finished reading coincident pulses!*****" << std::endl;
            std::cout << coincidentEvents[0].size() <<" coincidences are found.*****" << std::endl;
            std::vector< std::vector< UInt_t > >().swap(coincidentIndexes);

            //std::vector< std::vector<Float_t> > timeStampsDIACFD(coincidentEvents.size());
        }
    }
    else
    {
        // read data
        std::cout << "*****Start reading pulses in Channel" << '\t';
        for(int i=0;i<setting.Channels.size();i++)
        {
            std::cout << setting.Channels[i] << '\t';
        }
        std::cout <<"*****"<< std::endl;

        readEvents(setting, coincidentEvents);
        std::cout << "*****Finished reading pulses. ";
    }
    
    std::cout << "*****Start filtering bad pulses..*****" << std::endl;
    if (setting.FilterBad) {
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            rejection(setting, coincidentEvents[i]);
        }
    }
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        if(setting.EnergyCut[i])
        {
            energyCut(setting, coincidentEvents[i], i);
        }
    }
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        if(setting.PSDCut[i])
        {
            PSDCut(setting, i, coincidentEvents[i]);
        }
    }
    std::cout << "*****Finished filtering bad pulses!*****" << std::endl;

    std::string outname;
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        // plot 10 good pulses
        outname = "Pulse_CH" + std::to_string(setting.Channels[i]);
        plotPulse(setting, coincidentEvents[i], outname,[](Event pulse) { return !pulse.isBad; });
        std::cout << " Example pulses of CH" << std::to_string(i) << " are plotted! " << std::endl;
    }

    if(setting.SavePulses)
    {
        // save 100 good pulses
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/goodpulsesch" + std::to_string(setting.Channels[i]);
            savePulses(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
        }
    }
    if(setting.SaveBad)
    {
        // save 100 bad pulses
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/baddpulsesch" + std::to_string(setting.Channels[i]);
            savePulses(setting, coincidentEvents[i], outname, [](Event pulse) { return pulse.isBad; });
        }
    }
    if(setting.SavePH)
    {
        // save the pulse heights
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/PH_CH" + std::to_string(setting.Channels[i]);
            savePH(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse heights were saved to file: " << outname <<"."<< std::endl;
        }
    }
    if(setting.SavePI)
    {
        // save the pulse heights
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/PI_CH" + std::to_string(setting.Channels[i]);
            savePI(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse integrals were saved to file: " << outname <<"."<< std::endl;
        }
    }
    //std::vector<float_t> calicoef{1,1};
    if (setting.PHD)
    {
        // calicoef[0] = 478 / 0.6579;
        // calicoef[1] = 478 / 0.5479;
        for(int i=0;i<coincidentEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]);
            TCanvas *PHcanvas = new TCanvas(outname.c_str(), "PHD Canvas", 200, 10, 700, 500);
            outname = "Pulse_height_distribution_" + outname;
            TH1F *PHhisto = new TH1F(outname.c_str(), outname.c_str(), 200, 0., 2.000); // 2000 keVee or 2.0 V
            getPHD(PHhisto, PHcanvas, setting, coincidentEvents[i], setting.Calicoefs[i], [](Event pulse) { return !pulse.isBad; });
            outname = "output/" + outname +".png";
            PHcanvas->SaveAs(outname.c_str());
            if (setting.SavePHD)
            {
                outname = "output/" + outname +".txt";
                saveHisto(PHhisto, outname);
                std::cout << " Pulse height distribution was saved to file: " << outname <<"."<< std::endl;
            }
        }
    }
    if (setting.PID)
    {
        // calicoef[0] = 478 / 3.1440;
        // calicoef[1] = 478 / 3.1500;
        for(int i=0;i<coincidentEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]);
            TCanvas *PIcanvas = new TCanvas(outname.c_str(), "PID Canvas", 200, 10, 700, 500);
            outname = "Pulse_integral_distribution_" + outname;
            TH1F *PIhisto = new TH1F(outname.c_str(), outname.c_str(), 200, 0., 2000); //// 2000 kevee or 20.0 V
            getPID(PIhisto, PIcanvas, setting, coincidentEvents[i], setting.Calicoefs[i],[](Event pulse) { return !pulse.isBad; });
            outname = "output/" + outname +".png";
            PIcanvas->SaveAs(outname.c_str());
            if (setting.SavePID)
            {
                outname = "output/" + outname +".txt";
                saveHisto(PIhisto, outname);
                std::cout << " Pulse integral distribution was saved to file: " << outname <<"."<< std::endl;
            }
        }
    }

    if (setting.TimeStamp)
    {
        std::cout << "*****Start calculating time stamps of coincidences using DIACFD..*****" << std::endl;
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            getTimeStamp(setting, coincidentEvents[i]);
        }
        std::cout << "*****Finished calculaing time stamps of coincidences!*****" << std::endl;
        
        //// TBD: multichannel support
        if (setting.Coincidence && setting.SaveDT) 
        {
            // save time differences
            SaveDT(coincidentEvents, "output/timediff_Header", "Header");
            SaveDT(coincidentEvents, "output/timediff_DIACFD", "DIACFD");
        }
        if (setting.Coincidence && setting.TOF) 
        {
            TCanvas *TOFcanvas = new TCanvas("c3", "TOF Canvas", 200, 10, 700, 500);
            TH1F *TOFhisto = new TH1F("TOF", "Histogram of time differences", 800, -6.0, 10.0);
            getTOF(TOFhisto, TOFcanvas, setting, coincidentEvents);
            outname = "output/TOF_"+std::to_string(setting.Fraction) +"_"+std::to_string(setting.TimeDelay)+"_"+std::to_string(setting.Interpolation)+".png";
            TOFcanvas->SaveAs(outname.c_str());
            if (setting.SaveTOF) 
            {
                // save ToF
                outname = "output/TOF_"+std::to_string(setting.Fraction) +"_"+std::to_string(setting.TimeDelay)+"_"+std::to_string(setting.Interpolation)+".txt";
                saveTOF(TOFhisto, outname);
            }
        }
    }
    if(setting.SaveTimeStamp)
    {
        // save time stamps
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/timestamp_Header_CH" + std::to_string(setting.Channels[i]);
            saveTimestamp(coincidentEvents[i], outname, "Header", [](Event pulse) { return !pulse.isBad; });
            if(setting.TimeStamp)
            {
                outname = "output/timestamp_DIACFD_CH" + std::to_string(setting.Channels[i]);
                saveTimestamp(coincidentEvents[i], outname, "DIACFD", [](Event pulse) { return !pulse.isBad; });
            }
        }
    }

    if(setting.TailvsTotal)
    {
        for(int i=0;i<coincidentEvents.size();i++)
        {
            // outname = "channel" + std::to_string(i);
            // TCanvas *PSDcanvas = new TCanvas(outname.c_str(), "PSD Canvas", 200, 10, 700, 500);
            // outname = "PSD " + outname;
            // TGraph *PSDgraph = new TGraph();
            outname = "PSD_CH" + std::to_string(setting.Channels[i]);
            getPSD(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " PSD of CH" << std::to_string(i) << " was plotted." << std::endl;
            outname = "output/" + outname +".txt";
            if (setting.SaveTailvsTotal)
            {
                savePSD(coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
                std::cout << " PSD ratios of CH" << std::to_string(i) << " are saved to file " << outname << "." << std::endl;
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    // myapp->Run();
    return 0;
}