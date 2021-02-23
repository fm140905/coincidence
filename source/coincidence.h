#if !defined(COINCIDENCE_H)
#define COINCIDENCE_H

#include "TF1.h"
#include <fstream>
#include "getInputParameters.h"

Int_t find_coincidence(const Parameters& setting, const std::vector< std::vector<ULong64_t> > &timestamps, std::vector< std::vector< UInt_t > > &pairs);

#endif // COINCIDENCE_H
