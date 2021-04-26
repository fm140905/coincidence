#include "gui.h"


void example() {
   // Popup the GUI...
   new MyMainFrame(gClient->GetRoot(),1000, 1000);
}

int main(int argc, char **argv) {
   TApplication theApp("App",&argc,argv);
   example();
   theApp.Run();
   return 0;
}