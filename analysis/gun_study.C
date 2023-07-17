#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TGraphAsymmErrors.h>

#include "include/xjjmypdf.h"
#include "include/xjjrootuti.h"
#include "include/defines.h"

#define TRKLTS2P(EXPAND)                        \
  BTRKLT2P(EXPAND)                              \
  FTRKLT2P(EXPAND)                              \

int macro(std::string input_guns) {

  xjjroot::setgstyle(1);
  
  auto inf_guns = TFile::Open(input_guns.c_str());

#define FILL_HIST(q, w)                                                 \
  auto t##q##w = (TTree*)inf_guns->Get("TrackletTree"#q#w);             \
  TH1F* h_dreff_den_##q##w = new TH1F("h_dreff_den_"#q#w, ";p_{T} [GeV/c];", 50, 0, 0.5); \
  h_dreff_den_##q##w->Sumw2();                                          \
  t##q##w->Draw("gpt>>h_dreff_den_"#q#w, "fabs(vz[0]) < 15 && acc > 0"); \
  TH1F* h_dreff_nom_##q##w = new TH1F("h_dreff_nom_"#q#w, ";p_{T} [GeV/c];", 50, 0, 0.5); \
  h_dreff_nom_##q##w->Sumw2();                                          \
  t##q##w->Draw("gpt>>h_dreff_nom_"#q#w, "fabs(vz[0]) < 15 && acc > 0 && dr2 < 0.25"); \
  auto p_dreff_##q##w = new TGraphAsymmErrors(h_dreff_nom_##q##w, h_dreff_den_##q##w); \
  TH2F* h_dr_pt_##q##w = new TH2F("h_dr_pt_"#q#w, ";#Deltar;p_{T} [GeV/c]", 100, 0, 0.5, 100, 0, 0.5); \
  t##q##w->Draw("gpt:TMath::Sqrt(dr2)>>h_dr_pt_"#q#w, "fabs(vz[0]) < 15 && acc > 0"); \

  TRKLTS2P(FILL_HIST)
  
    TH1F* hempty = (TH1F*)h_dreff_den_12->Clone("hempty");
  hempty->GetYaxis()->SetTitle("Relative efficiency");
  hempty->SetMinimum(0); hempty->SetMaximum(1.2);
  xjjroot::sethempty(hempty);
  
  xjjroot::mypdf pdf("figspdf/gmatch/guns.pdf");

#define DRAW_HIST(q, w)                         \
  pdf.prepare();                                \
  h_dr_pt_##q##w->Draw("colz");                 \
  pdf.write();                                  \
  pdf.prepare();                                \
  hempty->Draw("axis");                         \
  p_dreff_##q##w->Draw("ple");                  \
  pdf.write();                                  \

  TRKLTS2P(DRAW_HIST)
  
  pdf.close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 2)
    return macro(argv[1]);

  return 1;
}
