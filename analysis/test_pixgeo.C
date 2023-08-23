#include "include/pixgeo.h"
#include <TH2F.h>
#include "include/xjjmypdf.h"

int test() {
  pixgeo geo("data/pixelGeo.root");

  TH2F* hempty_x_y = new TH2F("hempty_x_y", ";x;y", 10, -20, 20, 10, -20, 20);
  xjjroot::setgstyle();
  xjjroot::mypdf pdf("figspdf/geo/pixel.pdf");
  
  pdf.prepare();
  hempty_x_y->Draw("axis");
  geo.draw6("fpix-x-y-plus");
  pdf.write();

  pdf.close();
  
  return 0;
}

int main() {
  return test();
}
