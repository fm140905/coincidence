/*
 * @Description: Cs spectra calibration
 * @Author: Ming Fang
 * @Date: 2021-12-29 10:35:01
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-01-24 14:11:04
 */
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include <utility>

#include "TCanvas.h"
#include "TError.h"
#include "TStyle.h"
#include "TApplication.h"
#include "TGraphSmooth.h"

#include "../include/getInput.h"
#include "../include/loadEvents.h"
#include "../include/loadChannel.h"


/**
 * @brief Find the Compton edge of the given PHD/PID.
 * 
 * @param histo Cs spectrum, PHD or PID
 * @return std::pair<double, double> Comtpon edge, {x, y}
 */
std::pair<double, double> find_compton_edge(const TH1D& histo);

int drawSpectrumWithEdge(const TH1D& histo, const std::pair<double, double> comptonedge);

int compareSpectra(const std::shared_ptr<Channel> channel, const std::pair<double, double> comptonedge_ph, const std::pair<double, double> comptonedge_pi);

int main(int argc, char** argv)
{
    // create output directory
    struct stat st = {0};
    // create directories to save the output files and pics
    std::string outdir = "CsCalibration_output";
    if (stat(outdir.c_str(), &st) == -1) {
        mkdir(outdir.c_str(), 0700);
    }
    // supress root mesg
    gErrorIgnoreLevel = kWarning;
    // set stat box positions
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    //Tapplication to display results
    TApplication* myapp = new TApplication("CsCalibration", 0, 0);

    auto startTime = std::chrono::high_resolution_clock::now();
    const std::string settingFilePath(argv[1]);
    const InputParameters settings(settingFilePath);
    // std::vector<TCanvas*> canvas;

    // process each channel
    // for (std::size_t i = 0; i < 1; i++)
    for (std::size_t i = 0; i < settings.channelSettings.size(); i++)
    {
        const ChannelSettings& chseti = settings.channelSettings[i];
        if (!chseti.processOutput)
        {
            continue;
        }
        
        std::shared_ptr<Channel> channelI(new Channel(chseti));
        std::pair<double, double> comptonedge_ph;
        std::pair<double, double> comptonedge_pi;
        channelI->loadEvents();
        std::string outname;
        // save pulse heights and pulse integrals
        outname = outdir + "/PH_channel " + std::to_string(chseti.channelNumber);
        channelI->savePH(outname);
        outname = outdir + "/PI_channel " + std::to_string(chseti.channelNumber);
        channelI->savePI(outname);

        std::string plotname;
        plotname = "Channel " + std::to_string(chseti.channelNumber);
        TCanvas canvasCHI(plotname.c_str(), plotname.c_str(), 200, 10, 800, 800);
        canvasCHI.Divide(2, 2);
        
        if (chseti.plotPHD)
        {
            canvasCHI.cd(1);
            gPad->Update();
            plotname = "PHD_channel " + std::to_string(chseti.channelNumber);
            channelI->getPHD(plotname);
            // draw spectrum and edge
            comptonedge_ph = find_compton_edge(channelI->PHD);
            std::cout << "Compton edge of PHD is " << comptonedge_ph.first << " V, calibration coefficient for PHD is: " << 478.0 / comptonedge_ph.first << " keVee/V" << std::endl;
            drawSpectrumWithEdge(channelI->PHD, comptonedge_ph);
        }

        if (chseti.plotPID)
        {
            canvasCHI.cd(2);
            gPad->Update();
            plotname = "PID_channel " + std::to_string(chseti.channelNumber);
            channelI->getPID(plotname);
            comptonedge_pi = find_compton_edge(channelI->PID);
            std::cout << "Compton edge of PID is " << comptonedge_pi.first << " V*ns/dt, calibration coefficient for PID is: " << 478.0 / comptonedge_pi.first << " keVee/(V*ns/dt)" << std::endl;
            drawSpectrumWithEdge(channelI->PID, comptonedge_pi);
        }

        if (chseti.plotPID && chseti.plotPHD)
        {
            canvasCHI.cd(3);
            gPad->Update();
            plotname = "Calibrated_channel " + std::to_string(chseti.channelNumber);
            compareSpectra(channelI, comptonedge_ph, comptonedge_pi);
        }

        if (chseti.plotGoodPulses)
        {
            canvasCHI.cd(4);
            gPad->Update();
            plotname = "Good pulses_channel " + std::to_string(chseti.channelNumber);
            channelI->getGoodPulseGraph(plotname);
            channelI->goodPulseGraph.DrawClone("A PLC PMC");
        }

        gPad->Update();
        canvasCHI.DrawClone();
    }
   
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
    // display results
    std::cout << "Press ctrl+c to exit.." << std::endl;
    myapp->Run();
    return 0;
}

std::pair<double, double> find_compton_edge(const TH1D& histo)
{
    // convert histogram to graph
    const int n = histo.GetNbinsX();
    double xs[n];
    double ys[n];
    for (int i = 1; i <= n; i++)
    {
        xs[i-1] = histo.GetBinCenter(i);
        ys[i-1] = histo.GetBinContent(i);
    }

    TGraph graph(n, xs, ys);

    // smooth
    TGraphSmooth *gs = new TGraphSmooth("normal");
    TGraph* smoothGraph = gs->SmoothLowess(&graph, "", 0.1, 1);

    // find range of Compton edge
    // 1st derivative pattern: -, -, -, +
    double derivative[n-1];
    for (int i = 0; i < n-1; i++)
    {
        derivative[i] = smoothGraph->Eval(xs[i+1]) - smoothGraph->Eval(xs[i]);
    }
    int i_left = 0;
    for (int i = 0; i < n - 4; i++)
    {
        if(derivative[i] < 0 && derivative[i+1] < 0 &&
           derivative[i+2] < 0 && derivative[i+3] > 0)
        {
            i_left = i + 3;
            break;
        }
    }
    int i_right = n - 1;

    // find max within [x_left, x_right]
    int i_peak;
    double y_peak(0);
    for (int i = i_left; i <= i_right; i++)
    {
        if (ys[i] > y_peak)
        {
            i_peak = i;
            y_peak = ys[i];
        }
    }

    // find compton edge, which corresponds to 80% of y_peak
    double x_edge(0);
    double y_edge = 0.8 * y_peak;
    for (int i = i_peak; i < i_right; i++)
    {
        if (ys[i] > y_edge && ys[i+1] <= y_edge)
        {
            x_edge = ( (ys[i] - y_edge) * xs[i + 1] + (y_edge - ys[i+1]) * xs[i] ) / (ys[i] - ys[i+1]);
            break;
        }
    }

    // std::cout << i_left << '\t' << i_right << std::endl;
    // std::cout << i_peak << '\t' << y_peak << std::endl;
    // std::cout << x_edge << '\t' << y_edge << std::endl;

    // // plot
    // TCanvas graphCanvas("graphCanvas","Cs Calibration",200,10,600,200);
    // graphCanvas.Divide(3,1);
    // TGraph derivativeGraph(n-1, xs, derivative);
    // double xlims[] = {xs[i_left], xs[i_right]};
    // double ylims[] = {smoothGraph->Eval(xlims[0]), smoothGraph->Eval(xlims[1])};
    // TGraph lims(2, xlims, ylims);
    // lims.SetMarkerColor(kBlue);
    // lims.SetMarkerStyle(21);
    // lims.SetMarkerSize(0.5);

    // double xedge[] = {xs[i_peak], x_edge};
    // double yedge[] = {y_peak, y_edge};
    // TGraph edges(2, xedge, yedge);
    // edges.SetMarkerColor(kBlue);
    // edges.SetMarkerStyle(21);
    // edges.SetMarkerSize(0.5);

    // graphCanvas.cd(1);
    // graph.SetTitle("Raw histogram");
    // graph.Draw("AL");
    // edges.Draw("P");
    // graphCanvas.cd(2);
    // smoothGraph->SetTitle("Smoothed histogram");
    // smoothGraph->Draw("ALX");
    // lims.Draw("P");
    // graphCanvas.cd(3);
    // derivativeGraph.SetTitle("1st-order derivative");
    // derivativeGraph.Draw("AL*");

    // gPad->Update();
    // graphCanvas.DrawClone();
    
    delete gs;

    return {x_edge, y_edge};
}

int drawSpectrumWithEdge(const TH1D& histo, const std::pair<double, double> comptonedge)
{
    // canvas.cd(padi);
    // convert histogram to graph
    const int n = histo.GetNbinsX();
    double xs[n];
    double ys[n];
    for (int i = 1; i <= n; i++)
    {
        xs[i-1] = histo.GetBinCenter(i);
        ys[i-1] = histo.GetBinContent(i);
    }

    TGraph graph(n, xs, ys);
    graph.SetTitle(histo.GetTitle());
    graph.GetXaxis()->SetTitle(histo.GetXaxis()->GetTitle());
    graph.GetYaxis()->SetTitle(histo.GetYaxis()->GetTitle());

    double xedge[] = {comptonedge.first};
    double yedge[] = {comptonedge.second};
    TGraph edgeGraph(1, xedge, yedge);
    edgeGraph.SetMarkerColor(kBlue);
    edgeGraph.SetMarkerStyle(21);
    edgeGraph.SetMarkerSize(0.5);

    graph.DrawClone("AL");
    edgeGraph.DrawClone("P");
    return 0;
}

int compareSpectra(const std::shared_ptr<Channel> channel, const std::pair<double, double> comptonedge_ph, const std::pair<double, double> comptonedge_pi)
{
    double cali_coef_ph = 478.0 / comptonedge_ph.first;
    double cali_coef_pi = 478.0 / comptonedge_pi.first;
    double minLO = 0;
    double maxLO = 800;
    int numLOBins = 200;

    TH1D calibratedPHD("Calibrated PHD", "Calibrated PHD", numLOBins, minLO, maxLO);
    TH1D calibratedPID("Calibrated PID", "Calibrated PID", numLOBins, minLO, maxLO);
    const std::vector<Event>& events = channel->events;
    for (int i = 0; i < events.size(); i++)
    {
        calibratedPHD.Fill(events[i].height * cali_coef_ph);
        calibratedPID.Fill(events[i].totalIntegral * cali_coef_pi);
    }
    TGraph graph_ph(&calibratedPHD);
    graph_ph.SetLineColor(kBlue);
    TGraph graph_pi(&calibratedPID);
    graph_pi.SetLineColor(kRed);

    TMultiGraph mg;
    mg.Add(&graph_ph);
    mg.Add(&graph_pi);
    mg.SetTitle("Calibrated Spectra");
    mg.GetXaxis()->SetTitle("Light output (keVee)");
    mg.GetYaxis()->SetTitle("Counts");
    mg.DrawClone("AC");
    gPad->BuildLegend(0.6, 0.7, 0.9, 0.9);

    return 0;
    
}