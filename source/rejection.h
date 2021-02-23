#if !defined(REJECTION_H)
#define REJECTION_H

#include "TF1.h"
#include "events.h"
#include "getInputParameters.h"

Int_t rejection(const Parameters &setting, std::vector< Event >& events );

Int_t energyCut(const Parameters &setting, std::vector< Event >& events, const int &channelI);

Int_t PSDCut(const Parameters &setting, const UInt_t index, std::vector< Event >& events );
// Int_t filterBad(const Parameters &setting, std::vector<Event> &events);

#endif // REJECTION_H
