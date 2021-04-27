#pragma once

#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TGFrame.h>
#include <TGTab.h>
#include <TGMenu.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGFileDialog.h>

#include "getInput.h"

class LoadDataTab : public TGCompositeFrame
{
private:
    // Combo entry id
    constexpr static Int_t M_POLARITY_POS=0;
    constexpr static Int_t M_POLARITY_NEG=1;
    constexpr static Int_t M_DM_RNG_LOW=10;
    constexpr static Int_t M_DM_RNG_HIGH=11;
    constexpr static Int_t M_TME_LOW=20;
    constexpr static Int_t M_TME_HIGH=21;
    constexpr static Int_t M_ADC_12 = 30;
    constexpr static Int_t M_ADC_14 = 31;
    
    // tab 1;
    TGVerticalFrame*   fVFCol1; // column 1 frame
    TGLayoutHints*     fVFLayoutCol1; // column 1 layout
    std::array<std::string, 6>        col1Labels={"Bin File", "Max Number", "Max Time", "Polarity", "Dynamic Range", "Time Step"};
    std::array<TGHorizontalFrame*, 6> fHFCol1; // column 1, frames
    TGLayoutHints*     fHFLayoutCol1;
    std::array<TGLabel*, 6>           fLabelCol1;
    // std::array<TGNumberEntry*, 6>     fNECol1; // column 1, number entries
    TGTextEntry *FilePath;
    TGTextBuffer* FilePathBuf;
    TGNumberEntryField *MaxNum;
    // ULong64_t maxNum_; // max num of pulses
    TGNumberEntryField *MaxTime;
    // ULong64_t maxTime_; // seconds
    TGComboBox *Polarity;
    TGComboBox *DynamicRange;
    TGComboBox *TimeStep;

    // std::array<std::string, 6>       col2Labels={"Resolution", "Baseline", "PreTrig", "PreGate", "ShortGate", "LongGate"};
    // TGVerticalFrame*   fVFCol2; // column 2 frame
    // std::array<TGHorizontalFrame*, 6> fHFCol2; // column 2, frames
    // std::array<TGNumberEntry*, 6>     fNECol2; // column 2, number entries
public:
    InputParameters* config;
    TGCompositeFrame *fTab;
    LoadDataTab(TGCompositeFrame* fTab_);
    virtual ~LoadDataTab();
    // slots
    void ReadFilePath(char* txt);
    void ReadMaxNum(char* num);
    void ReadMaxTime(char* tme);
    void HandleCombo(Int_t id);
    void UpdateContent();

};
class MyMainFrame : public TGMainFrame {
private:
    constexpr static Int_t          M_EXIT=100; // must be non-negative
    constexpr static Int_t          M_CONFIG_OPEN=0;
    constexpr static Int_t          M_CONFIG_SAVE=1;
    constexpr static Int_t          M_CONFIG_SAVEAS=2;
    constexpr static Int_t          M_ABOUT=3;
    TGFileInfo fin;
    TGFileInfo fOut;

    TGMainFrame         *fMain;
    TGMenuBar           *fMenuBar; // menu bar
    TGPopupMenu         *fMenuEntries[2]; // menu items
    // tab
    TGTab *fTab;
    // tab 1, load data
    LoadDataTab* tab1;

    TRootEmbeddedCanvas *fEcanvas;
public:
    // configurations
    InputParameters* config;

    MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
    virtual ~MyMainFrame();
    // slots
    // menus
    void Response2Menu(Int_t menu_id); // respond based on menu id
    void LoadConfig(); // load config, id = 0
    void SaveConfig(); // save config, id = 1
    void SaveConfigAs(); // save config as, id = 2
    void DisplayAbout() ;// display program info, id = 3
    // update entire window
    void DoDraw();
    void Start();
    void Stop();
    int Init();
    void CloseWindow();

    ClassDef(MyMainFrame,0)
};
