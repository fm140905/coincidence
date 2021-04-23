#pragma once

#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TGFrame.h>

class MyMainFrame : public TGMainFrame {
private:
    TGMainFrame         *fMain;
    TRootEmbeddedCanvas *fEcanvas;
public:
    MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
    virtual ~MyMainFrame();
    void DoDraw();
    ClassDef(MyMainFrame,0)
};