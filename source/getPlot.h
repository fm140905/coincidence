#if !defined(GETPLOT_H)
#define GETPLOT_H

#include <functional>
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
//#include "TF1.h"
#include "events.h"
#include "getInputParameters.h"

Int_t getPHD(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector< Event > &events, const float_t calicoef, std::function<Bool_t(Event)> cond);

Int_t getPID(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector<Event> &events, const float_t calicoef, std::function<Bool_t(Event)> cond);

Int_t getTOF(TH1F *histo, TCanvas *canvas, const Parameters &setting, const std::vector< std::vector< Event > > &coincidentEvents);

Int_t getPSD(const Parameters &setting, const std::vector<Event> &events, const std::string plotname, std::function<Bool_t(Event)> cond);

Int_t plotPulse(const Parameters &setting, const std::vector<Event> &events, const std::string plotname, std::function<Bool_t(Event)> cond);
//Int_t savePHD(TH1F *histo, TCanvas *canvas, const Parameters& setting, const std::string format);

#endif // GETPLOT_H