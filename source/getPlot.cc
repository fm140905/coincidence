#include "getPlot.h"

Int_t getTOF(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector< std::vector< Event > > &coincidentEvents)
{
    std::cout << "Start plotting the time difference histogram." << std::endl;

    // if(!setting.PHD)
    // {
    //     std::cout << "You choose not to plot the pulse height distribution." << std::endl;
    //     return 0;
    // }

    histo->GetXaxis()->SetTitle("Time Difference (ns)");
    histo->GetYaxis()->SetTitle("Counts");

    // calculate DT
    //std::vector<Float_t> timediffs;
    Float_t timediff(0);
    for(int i=0;i<coincidentEvents[0].size();i++)
    {
        if (!coincidentEvents[0][i].isBad &&
            !coincidentEvents[1][i].isBad
            )
        {
            timediff = (coincidentEvents[1][i].timeStampDACFD - coincidentEvents[0][i].timeStampDACFD);
            //timediffs.push_back(timediff);
            histo->Fill(timediff);
        }
    }
    histo->SetFillColor(40);

    // // 20191207 by ming
    // // fit the histogram
    // TF1 *TOFfunc = new TF1("TOFfunc","[0]*exp(-0.5*((x-[1])/[2])^2)",-2,4);
    // TOFfunc->SetParNames("Constant","Mean","Sigma");
    // TOFfunc->SetParameter(0, histo->GetMaximum());
    // TOFfunc->SetParLimits(0, histo->GetMaximum(), histo->GetMaximum());
    // int binmax = histo->GetMaximumBin(); 
    // double mean_est = histo->GetXaxis()->GetBinCenter(binmax);
    // TOFfunc->SetParameter(1, mean_est);
    // TOFfunc->SetParLimits(1, -4, 4);
    // TOFfunc->SetParameter(2, 0.3);
    // TOFfunc->SetParLimits(2, 0, 2);
    // histo->Fit("TOFfunc","W");
    // // histo -> GetXaxis()->SetRangeUser(0., 2.);
    histo->Draw();
    // TOFfunc->Draw("same");
    canvas->Draw();

    std::cout << " Time difference histogram is plotted." << std::endl;
    return 0;
}

Int_t getPHD(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector< Event > &events, const float_t calicoef, std::function<Bool_t(Event)> cond)
{
    std::cout << "Start plotting the pulse height distribution." << std::endl;

    // if(!setting.PHD)
    // {
    //     std::cout << "You choose not to plot the pulse height distribution." << std::endl;
    //     return 0;
    // }
    if(calicoef == 1)
    {
        histo->GetXaxis()->SetTitle("Voltage (V)");
        //histo->GetXaxis()->SetRangeUser(0.,maxheight);
    }
    else
    {
        histo->GetXaxis()->SetTitle("Light Output (keVee)");
    }
    histo->GetYaxis()->SetTitle("Counts");

    // calculate PHD
    Float_t heightTemp(0);
    for (int i = 0; i < events.size(); i++)
    {
        heightTemp = events[i].height * calicoef;
        //float_t dttemp = coincidentEvents[1][i].timeStampDACFD - coincidentEvents[0][i].timeStampDACFD;
        if (cond(events[i]))
        {
            histo->Fill(heightTemp);
        }
    }

    histo->Draw();
    canvas->Draw();

    std::cout << " Pulse height distribution was plotted." << std::endl;
    return 0;
}

Int_t getPID(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector<Event> &events, const float_t calicoef, std::function<Bool_t(Event)> cond)
{
    std::cout << "Start plotting the pulse integral distribution." << std::endl;

    // if(!setting.PID)
    // {
    //     std::cout << "You choose not to plot the pulse integral distribution." << std::endl;
    //     return 0;
    // }

    if(calicoef == 1)
    {
        histo->GetXaxis()->SetTitle("Voltage (V)");
    }
    else
    {
        histo->GetXaxis()->SetTitle("Light Output (keVee)");
    }
    histo->GetYaxis()->SetTitle("Counts");

    // calculate PI
    // for (int i = 0; i < events.size(); i++)
    // {
    //     events[i].totalIntegral = 0;
    //     for (int j = events[i].heightindex - setting.Pre; j <= events[i].heightindex + setting.Post && j < setting.NSamples; j++)
    //     {
    //         events[i].totalIntegral += events[i].voltage[j];
    //     }
    // }
    // generate PID
    for (int i = 0; i < events.size(); i++)
    {
        if (cond(events[i]))
        {
            histo->Fill(events[i].totalIntegral * calicoef);
        }
    }

    histo->Draw();
    canvas->Draw();

    std::cout << " Pulse integral distribution was plotted." << std::endl;
    return 0;
}

Int_t getPSD(const Parameters &setting, const std::vector<Event> &events, const std::string plotname, std::function<Bool_t(Event)> cond)
{
    std::cout << "Start plotting the PSD." << std::endl;

    std::vector<Float_t> PSDs;
    std::vector<Float_t> totalIntegrals;
    Float_t PSDtemp(0);
    for(int i=0; i<events.size();i++)
    {
        if(cond(events[i]))
        {
            PSDtemp = events[i].tailIntegral / events[i].totalIntegral;
            totalIntegrals.push_back(events[i].totalIntegral);
            PSDs.push_back(PSDtemp);
        }
    }

    srand(time(NULL)); // Seeding the random number generator
    // std::string canvasname;
    // canvasname = "canvas" + std::to_string(rand() % 100);
    TCanvas *canvas = new TCanvas(plotname.c_str(), "Tail vs Total", 200, 10, 700, 500);
    TGraph *graph = new TGraph(PSDs.size(), &(totalIntegrals[0]), &(PSDs[0]));

    graph->SetMarkerStyle(1);
    graph->SetMarkerColor(4);
    graph->SetTitle(plotname.c_str());
    graph->GetXaxis()->SetTitle("Total integral");
    graph->GetXaxis()->SetLimits(0., 10.);
    graph->GetYaxis()->SetRangeUser(0., 1.);
    graph->GetYaxis()->SetTitle("PSD");

    graph->Draw("Ap");
    canvas->Draw();

    return 0;
}

Int_t plotPulse(const Parameters &setting, const std::vector<Event> &events, const std::string plotname, std::function<Bool_t(Event)> cond)
{
    // std::cout << "Start plotting the example pulse !" << std::endl;

    const UInt_t N = setting.NSamples;
    Float_t x_vals[N];
    Float_t gateHeight(0);
    for (int i = 0; i < N; i++)
    {
        x_vals[i] = setting.Delt * i;
    }
    if (events.size() == 0)
    {
        std::cout << "0 good pulses" << std::endl;
        return 0;
    }
    // if (10 < events.size())
    // {
    //     const UInt_t M = 10;
    // }
    // else
    // {
    //     const UInt_t M = events.size();
    // }
    int good_counts = 0;
    for (int i = 0; i < events.size() && good_counts < 10; i++)
    {
        if(cond(events[i]))
        {
            good_counts ++;
        }
    }
    const UInt_t M = (10 < good_counts ? 10 : good_counts);

    std::vector<std::vector<Float_t> > y_vals(M);
    TGraph *graphs[M];
    good_counts = 0;
    for(int i=0; i < events.size() && good_counts < M;i++)
    {
        if(cond(events[i]))
        {
            for (int j = 0; j < N; j++)
            {
                y_vals[good_counts].push_back( events[i].voltage[j] );
            }
            graphs[good_counts] = new TGraph(N, x_vals, &(y_vals[good_counts][0] ));
            // graphs[good_counts]->SetMarkerStyle(20);
            // graphs[i]->SetLineColor(kRed + i* 2);
            if (events[i].height > gateHeight)
            {
                gateHeight = events[i].height;
            }
            good_counts ++;
            // break;
        }
    }
    // Int_t PreTrig = setting.PreTrigger / setting.Delt;
    // Int_t PreGate = setting.PreGate / setting.Delt;
    // PreGate = PreTrig - PreGate;
    // if(PreGate < 0) PreGate = 0;
    // Int_t LongGate = setting.LongGate  / setting.Delt;
    // LongGate += PreGate;
    // // if(LongGate > setting.NSamples) LongGate = setting.NSamples;
    // Int_t ShortGate = setting.ShortGate / setting.Delt;
    // ShortGate += PreGate;
    
    // // Long gate
    // Float_t longgate_vals[N];
    // for (int i = 0; i < N; i++)
    // {
    //     if(i < PreGate || i >= LongGate)
    //     {
    //         longgate_vals[i] = 0.7 * gateHeight;
    //     }
    //     else
    //     {
    //         longgate_vals[i] =  0.9 * gateHeight;
    //     }
    // }
    // graphs[M] = new TGraph(N, x_vals, &(longgate_vals[0] ));
    // // graphs[M]->SetLineColor(kRed);
    // // Short Gate
    // Float_t shortgate_vals[N];
    // for (int i = 0; i < N; i++)
    // {
    //     if(i < PreGate || i >= ShortGate)
    //     {
    //         shortgate_vals[i] = 0.4 * gateHeight;
    //     }
    //     else
    //     {
    //         shortgate_vals[i] = 0.5 * gateHeight;
    //     }
    // }
    // graphs[M+1] = new TGraph(N, x_vals, &(shortgate_vals[0] ));
    // graphs[M+1]->SetLineColor(kGreen);

    // srand(time(NULL)); // Seeding the random number generator
    // std::string canvasname;
    // canvasname = "canvas" + std::to_string(rand() % 100);
    TCanvas *canvas = new TCanvas(plotname.c_str(), "Example pulses", 200, 10, 700, 500);
    // TGraph *graph = new TGraph(N, x_vals, y_vals);

    // // graph->SetMarkerStyle(22);
    // graphs[0]->SetMarkerColor(kRed);
    // graphs[0]->SetTitle(plotname.c_str());
    // graphs[0]->GetXaxis()->SetTitle("Time (ns)");
    // // graph->GetXaxis()->SetLimits(0., 10.);
    // graphs[0]->GetYaxis()->SetRangeUser(0., setting.DynamicRange);
    // graphs[0]->GetYaxis()->SetTitle("Voltage");
    // graphs[0]->Draw("AP");

    // graphs[1]->SetMarkerColor(kRed + 2);
    // graphs[1]->Draw("SAME");
    TMultiGraph *mg = new TMultiGraph();
    for (int i = 0; i < y_vals.size(); i++)
    {
        mg->Add(graphs[i],"lp");
    }
    mg->SetTitle(plotname.c_str());
    mg->GetXaxis()->SetTitle("Time (ns)");
    mg->GetYaxis()->SetTitle("Voltage (V)");
    mg->Draw("A PLC PMC");

    canvas->Draw();

    return 0;
}
