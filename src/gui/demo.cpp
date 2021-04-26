#include "gui.h"


void example() {
   // Popup the GUI...
   new MyMainFrame(gClient->GetRoot(),800, 600);
}

int main(int argc, char **argv) {
   TApplication theApp("App",&argc,argv);
   example();
   theApp.Run();
   return 0;
}