#include "gui.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h)
   : TGMainFrame(p,w,h) {

   // // Creates widgets of the example
   // fEcanvas = new TRootEmbeddedCanvas ("Ecanvas",this,200,200);
   // AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX |
   //                               kLHintsExpandY, 10,10,10,1));
   // TGHorizontalFrame *hframe=new TGHorizontalFrame(this, 200,40);
   // TGTextButton *draw = new TGTextButton(hframe,"&Draw");
   // draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   // hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,
   //                                        5,5,3,4));
   // TGTextButton *exit = new TGTextButton(hframe,"&Exit ",
   //                            "gApplication->Terminate()");
   // hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,
   //                                        5,5,3,4));
   // AddFrame(hframe,new TGLayoutHints(kLHintsCenterX,2,2,2,2));


   // // Sets window name and shows the main frame
   // SetWindowName("Simple Example");
   // MapSubwindows();
   // Resize(GetDefaultSize());
   // MapWindow();

   TGHorizontalFrame *hFrame = new TGHorizontalFrame(this, 60, 20, 
                               kFixedWidth);
   TGTextButton* exit = new TGTextButton(hFrame, "&Exit", 
                        "gApplication->Terminate(0)");
   hFrame->AddFrame(exit, new TGLayoutHints(kLHintsTop | kLHintsLeft
           | kLHintsExpandX, 2, 2, 2, 2));
   hFrame->Resize(150, exit->GetDefaultHeight());
   AddFrame(hFrame, new TGLayoutHints(kLHintsBottom | kLHintsRight,
          2, 2, 5, 1));

   //--------- create the Tab widget
   TGTab *fTab = new TGTab(this, 300, 300);
   TGLayoutHints *fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 
                        5, 5, 5, 5);

   //--------- create an empty tab element
   TGCompositeFrame *tf = fTab->AddTab("Tab 1");

   //-------------- create a tab element with an embedded canvas and buttons
   tf = fTab->AddTab("Tab 2");
   TGCompositeFrame* fF3 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   TGTextButton *fStartB = new TGTextButton(fF3, "Start &Filling Hists", 
                           "Start()");
   TGTextButton *fStopB = new TGTextButton(fF3, "&Stop Filling Hists",
                            "Stop()");
   fF3->AddFrame(fStartB, fL3);
   fF3->AddFrame(fStopB, fL3);

   TGCompositeFrame *fF5 = new TGCompositeFrame(tf, 60, 60, kHorizontalFrame);
   TGLayoutHints *fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft 
                           | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
   TRootEmbeddedCanvas *fEc1 = new TRootEmbeddedCanvas("ec1", fF5, 100, 100);
   TRootEmbeddedCanvas *fEc2 = new TRootEmbeddedCanvas("ec2", fF5, 100, 100);
   fF5->AddFrame(fEc1, fL4);
   fF5->AddFrame(fEc2, fL4);

   // make tab yellow
   Pixel_t yellow;
   gClient->GetColorByName("yellow", yellow);
   TGTabElement *tabel = fTab->GetTabTab("Tab 2");
   tabel->ChangeBackground(yellow);

   // end embeded canvas
   AddFrame(fTab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX
          | kLHintsExpandY, 2, 2, 5, 1));
   MapSubwindows();
   Resize();
   MapWindow();
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
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}