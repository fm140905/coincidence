#if !defined(GETTIMESTAMP_H)
#define GETTIMESTAMP_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include "events.h"
#include "getInputParameters.h"
#include "TFile.h"
#include "TSpline.h"

Int_t getTimeStamp(const Parameters &setting, std::vector< Event > &events);

// Int_t getBipolarPulses(const Parameters &setting, Event  &event, std::vector<std::vector<Float_t>> &bipolarPulses)

// // implement monotone cubic interpolation
// Int_t FritschCarlson(const std::vector<double> &xs, const std::vector<double>& ys, std::vector<std::vector<double> >& coefs);

// Double_t monoCubicInter(double x, const std::vector<double> &xs, const std::vector<double>& ys, const std::vector<std::vector<double> >& coefs);

// sinc interpolation
Int_t sincCoefs(const Int_t N, const Int_t T, const Int_t L, std::vector<double>& coefs);

// Int_t sincInter(const Parameters &setting, const std::vector<double>& coefs, );

#endif // GETTIMESTAMP_H
