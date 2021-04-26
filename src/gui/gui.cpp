#include "gui.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h){
   // use hierarchical cleaning
   fMain = new TGMainFrame(p,w,h);
   fMain->SetCleanup(kDeepCleanup);

   fMain->Connect("CloseWindow()", "MyMainFrame", this, "CloseWindow()");
   // create layout
   Init();
}

void MyMainFrame::CloseWindow() {
   // Got close message for this MainFrame. Terminates the application.
   gApplication->Terminate();
}

int MyMainFrame::Init(){
   // create a menu bar
   fMenuBar = new TGMenuBar(fMain, 1, 1, kHorizontalFrame);
   // File menus
   fMenuEntries[0] = new TGPopupMenu(gClient->GetRoot());
   fMenuEntries[0]->AddEntry("&Open", M_CONFIG_OPEN);
   fMenuEntries[0]->AddEntry("&Save", M_CONFIG_SAVE);
   fMenuEntries[0]->AddEntry("Save as", M_CONFIG_SAVEAS);
   fMenuBar->AddPopup("File", fMenuEntries[0], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
   fMenuEntries[0]->Connect("Activated(Int_t)", /* signal */
                     "MyMainFrame", this, /* receiver calls, object*/
                     "Response2Menu(Int_t)");
   fMenuEntries[1] = new TGPopupMenu(gClient->GetRoot());
   fMenuEntries[1]->AddEntry("About", M_ABOUT);
   fMenuEntries[1]->Connect("Activated(Int_t)", /* signal */
                     "MyMainFrame", this, /* receiver calls, object*/
                     "Response2Menu(Int_t)");
   fMenuBar->AddPopup("Help", fMenuEntries[1], new TGLayoutHints(kLHintsTop | kLHintsRight));
   fMain->AddFrame(fMenuBar, new TGLayoutHints(kLHintsExpandX, 0,0,2,2) );

   //--------- create the Tab widget
   fTab = new TGTab(fMain, 300, 300);
   //--------- create an empty tab element
   TGCompositeFrame *tf = fTab->AddTab("Load Data");
   tab1 = new LoadDataTab(tf);
   // fCol1 = new TGVerticalFrame(tf, 200, 150);
   // tf->AddFrame(fCol1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
   // fLCol1 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);
   // for (int i = 0; i < col1Labels.size(); i++)
   // {
   //    fFCol1[i] = new TGHorizontalFrame(fCol1, 200, 30);
   //    fCol1->AddFrame(fFCol1[i], fLCol1);
   //    fNCol1[i] = new TGNumberEntry(fFCol1[i], 0, 12, i+10, TGNumberFormat::kNESInteger);
   //    fFCol1[i]->AddFrame(fNCol1[i], fLCol1);
   //    fLabelCol1[i] = new TGLabel(fFCol1[i], col1Labels[i].c_str());
   //    fFCol1[i]->AddFrame(fLabelCol1[i], fLCol1);
   // }
   

   //-------------- create a tab element with an embedded canvas and buttons
   tf = fTab->AddTab("Tab 2");
   TGCompositeFrame* fF3 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   TGTextButton *fStartB = new TGTextButton(fF3, "Start &Filling Hists");
   fStartB->Connect("Clicked()", "MyMainFrame", this, "Start()");
   TGTextButton *fStopB = new TGTextButton(fF3, "&Stop Filling Hists");
   fStopB->Connect("Clicked()", "MyMainFrame", this, "Stop()");
   TGLayoutHints *fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 
                        5, 5, 5, 5);
   fF3->AddFrame(fStartB, fL3);
   fF3->AddFrame(fStopB, fL3);

   TGCompositeFrame *fF5 = new TGCompositeFrame(tf, 60, 60, kHorizontalFrame);
   TGLayoutHints *fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft 
                           | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
   TRootEmbeddedCanvas *fEc1 = new TRootEmbeddedCanvas("ec1", fF5, 100, 100);
   TRootEmbeddedCanvas *fEc2 = new TRootEmbeddedCanvas("ec2", fF5, 100, 100);
   fF5->AddFrame(fEc1, fL4);
   fF5->AddFrame(fEc2, fL4);
   tf->AddFrame(fF3, fL3);
   tf->AddFrame(fF5, fL4);

   // make tab yellow
   Pixel_t yellow;
   gClient->GetColorByName("yellow", yellow);
   TGTabElement *tabel = fTab->GetTabTab("Tab 2");
   tabel->ChangeBackground(yellow);

   // end embeded canvas
   fMain->AddFrame(fTab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX
          | kLHintsExpandY, 2, 2, 5, 1));
   
   // // exit button
   // TGHorizontalFrame *hFrame = new TGHorizontalFrame(fMain, 60, 100);
   // TGTextButton* exit = new TGTextButton(hFrame, "&Exit");
   // exit->Connect("Clicked()", "MyMainFrame", this, "CloseWindow()");
   // hFrame->AddFrame(exit, new TGLayoutHints(kLHintsTop | kLHintsLeft
   //         | kLHintsExpandX, 2, 2, 2, 2));
   // fMain->AddFrame(hFrame, new TGLayoutHints(kLHintsBottom | kLHintsRight,
   //        2, 2, 5, 1));

   fMain->MapSubwindows();
   fMain->Resize(GetDefaultSize());
   fMain->MapWindow();
   return 0;
}

// slots
void MyMainFrame::LoadConfig() {
   // load config
   printf("Load configuration!\n");
}

void MyMainFrame::SaveConfig() {
   // load config
   printf("Save configuration!\n");
}

void MyMainFrame::SaveConfigAs() {
   // load config
   printf("Save configuration as xx!\n");
}

void MyMainFrame::DisplayAbout() {
   // show program info
   printf("Show help information!\n");
}

void MyMainFrame::Response2Menu(Int_t menu_id) {
   switch (menu_id)
   {
   case M_CONFIG_OPEN:
      LoadConfig();
      break;
   case M_CONFIG_SAVE:
      SaveConfig();
      break;
   case M_CONFIG_SAVEAS:
      SaveConfigAs();
      break;
   case M_ABOUT:
      DisplayAbout();
      break;
   default:
      break;
   }
}
void MyMainFrame::DoDraw() {
   // Draws function graphics in randomly chosen interval
   TF1 *f1 = new TF1("f1","sin(x)/x",0,gRandom->Rndm()*10);
   f1->SetLineWidth(3);
   f1->Draw();
   TCanvas *fCanvas = fEcanvas->GetCanvas();
   fCanvas->cd();
   fCanvas->Update();
}

MyMainFrame::~MyMainFrame() {
   // // Clean up used widgets: frames, buttons, layout hints
   // fMain->Cleanup();
   delete fEcanvas;
   delete fMenuEntries[0];
   delete fMenuEntries[1];
   delete fMenuBar;
   delete fMain;
}

void MyMainFrame::Start()
{
   printf("\"Start Filling Hists\" Button Pressed!\n");
}

void MyMainFrame::Stop()
{
   printf("\"Stop Filling Hists\" Button Pressed!\n");
}


LoadDataTab::LoadDataTab(TGCompositeFrame* fTab_) : fTab(fTab_) {
   fVFCol1 = new TGVerticalFrame(fTab, 200, 150);
   fVFLayoutCol1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
   fTab->AddFrame(fVFCol1, fVFLayoutCol1);

   fHFLayoutCol1 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);
   for (int i = 0; i < col1Labels.size(); i++)
   {
      fHFCol1[i] = new TGHorizontalFrame(fVFCol1, 200, 30);
      fVFCol1->AddFrame(fHFCol1[i], fHFLayoutCol1);
      // fNCol1[i] = new TGNumberEntry(fFCol1[i], 0, 12, i+10, TGNumberFormat::kNESInteger);
      // fFCol1[i]->AddFrame(fNCol1[i], fLCol1);
   }
   for (int i = 0; i < col1Labels.size(); i++)
   {
      fLabelCol1[i] = new TGLabel(fHFCol1[i], col1Labels[i].c_str());
      fHFCol1[i]->AddFrame(fLabelCol1[i], fHFLayoutCol1);
   }

   FilePathBuf = new TGTextBuffer(100);
   FilePathBuf->AddText(0, "Data path");
   FilePath = new TGTextEntry(fHFCol1[0], FilePathBuf, 10);
   // FilePath = new TGTextEntry(fHFCol1[0], "Data path", 10);
   FilePath->Connect("TextChanged(char*)", "LoadDataTab", this, "ReadFilePath(char*)");
   fHFCol1[0]->AddFrame(FilePath, fHFLayoutCol1);

   // maxNum_ = 1E6;
   MaxNum = new TGNumberEntryField(fHFCol1[1], 11, 1E6, TGNumberFormat::kNESInteger);
   MaxNum->Connect("TextChanged(char*)", "LoadDataTab", this, "ReadMaxNum(char*)");
   fHFCol1[1]->AddFrame(MaxNum, fHFLayoutCol1);

   MaxTime = new TGNumberEntryField(fHFCol1[2], 12, 1E6, TGNumberFormat::kNESInteger);
   MaxTime->Connect("TextChanged(char*)", "LoadDataTab", this, "ReadMaxTime(char*)");
   fHFCol1[2]->AddFrame(MaxTime, fHFLayoutCol1);

   Polarity = new TGComboBox(fHFCol1[3], 13);
   Polarity->AddEntry("Positive", M_POLARITY_POS); // positve <=> id = 0
   Polarity->AddEntry("Negative", M_POLARITY_NEG); // negative <=> id = 1
   Polarity->Select(M_POLARITY_NEG); // default value
   Polarity->Resize(150, 20);
   Polarity->Connect("Selected(Int_t)", "LoadDataTab", this, "HandleCombo(Int_t)");
   fHFCol1[3]->AddFrame(Polarity, fHFLayoutCol1);

   DynamicRange = new TGComboBox(fHFCol1[4], 14);
   DynamicRange->AddEntry("0.5 Vpp", M_DM_RNG_LOW); // 0.5 Vpp <=> id = 10
   DynamicRange->AddEntry("2.0 Vpp", M_DM_RNG_HIGH); // 2.0 Vpp <=> id = 11
   DynamicRange->Select(M_DM_RNG_HIGH); // default value
   DynamicRange->Resize(150, 20);
   DynamicRange->Connect("Selected(Int_t)", "LoadDataTab", this, "HandleCombo(Int_t)");
   fHFCol1[4]->AddFrame(DynamicRange, fHFLayoutCol1);

   TimeStep = new TGComboBox(fHFCol1[5], 15);
   TimeStep->AddEntry("2 ns", M_TME_LOW); // 2ns <=> id = 20
   TimeStep->AddEntry("4 ns", M_TME_HIGH); // 4ns <=> id = 21
   TimeStep->Select(M_TME_LOW); // default value
   TimeStep->Resize(150, 20);
   TimeStep->Connect("Selected(Int_t)", "LoadDataTab", this, "HandleCombo(Int_t)");
   fHFCol1[5]->AddFrame(TimeStep, fHFLayoutCol1);

}

LoadDataTab::~LoadDataTab()
{
    delete FilePath;
    delete MaxNum;
    delete MaxTime;
    delete Polarity;
    delete DynamicRange;
    delete TimeStep;
    delete fHFLayoutCol1;
    for (int i = 0; i < fLabelCol1.size(); i++)
    {
        delete fLabelCol1[i];
    }
    for (int i = 0; i < fLabelCol1.size(); i++)
    {
        delete fHFCol1[i];
    }
    delete fVFLayoutCol1;
    delete fVFCol1;
}

void LoadDataTab::ReadFilePath(char* txt){
   printf("File path is:%s!\n",txt);
}

void LoadDataTab::ReadMaxNum(char* num){
   printf("Max num of pulses is: %ld!\n",std::stol(num));
}

void LoadDataTab::ReadMaxTime(char* tme){
   printf("Max time is: %ld seconds!\n",std::stol(tme));
}

void LoadDataTab::HandleCombo(Int_t id){
   switch (id)
   {
   case M_POLARITY_POS:
      printf("Positive polarity selected!\n");
      break;
   case M_POLARITY_NEG:
      printf("Negative polarity selected!\n");
      break;
   case M_DM_RNG_LOW:
      printf("0.5 Vpp selected!\n");
      break;
   case M_DM_RNG_HIGH:
      printf("2.0 Vpp selected!\n");
      break;
   case M_TME_LOW:
      printf("2 ns sampling time selected!\n");
      break;
   case M_TME_HIGH:
      printf("4 ns sampling time selected!\n");
      break;
   case M_ADC_12:
      printf("12 bit ADC selected!\n");
      break;
   case M_ADC_14:
      printf("14 bit ADC selected!\n");
      break;
   default:
      break;
   }
}
