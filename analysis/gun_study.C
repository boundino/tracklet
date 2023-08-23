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
  TH1F* h_dreff1_nom_##q##w = new TH1F("h_dreff1_nom_"#q#w, ";p_{T} [GeV/c];", 50, 0, 0.5); \
  h_dreff1_nom_##q##w->Sumw2();                                         \
  t##q##w->Draw("gpt>>h_dreff1_nom_"#q#w, "fabs(vz[0]) < 15 && acc > 0 && dr2 < 0.16"); \
  auto p_dreff1_##q##w = new TGraphAsymmErrors(h_dreff1_nom_##q##w, h_dreff_den_##q##w); \
  TH1F* h_dreff2_nom_##q##w = new TH1F("h_dreff2_nom_"#q#w, ";p_{T} [GeV/c];", 50, 0, 0.5); \
  h_dreff2_nom_##q##w->Sumw2();                                         \
  t##q##w->Draw("gpt>>h_dreff2_nom_"#q#w, "fabs(vz[0]) < 15 && acc > 0 && dr2 < 0.36"); \
  auto p_dreff2_##q##w = new TGraphAsymmErrors(h_dreff2_nom_##q##w, h_dreff_den_##q##w); \
  TH2F* h_dr_pt_##q##w = new TH2F("h_dr_pt_"#q#w, ";#Deltar;p_{T} [GeV/c]", 100, 0, 0.5, 100, 0, 0.5); \
  t##q##w->Draw("gpt:TMath::Sqrt(dr2)>>h_dr_pt_"#q#w, "fabs(vz[0]) < 15 && acc > 0"); \

  TRKLTS2P(FILL_HIST)
  
    TH1F* hempty = (TH1F*)h_dreff_den_12->Clone("hempty");
  hempty->GetYaxis()->SetTitle("Relative efficiency");
  hempty->SetMinimum(0); hempty->SetMaximum(1.2);
  xjjroot::sethempty(hempty);

#define STYLE_HIST(q, w)                                                \
  xjjroot::setthgrstyle(p_dreff_##q##w, xjjroot::colorlist_light[0], 21, 0.5, xjjroot::colorlist_light[0], 1, 1); \
  xjjroot::setthgrstyle(p_dreff1_##q##w, xjjroot::colorlist_light[1], 21, 0.5, xjjroot::colorlist_light[1], 1, 1); \
  xjjroot::setthgrstyle(p_dreff2_##q##w, xjjroot::colorlist_light[2], 21, 0.5, xjjroot::colorlist_light[2], 1, 1); \

  TRKLTS2P(STYLE_HIST)

  auto leg = new TLegend(0.60, 0.60-3*0.04, 0.85, 0.60);
  xjjroot::setleg(leg, 0.035);
  leg->AddEntry(p_dreff_12, "#Deltar < 0.5 (nominal)", "pl");
  leg->AddEntry(p_dreff1_12, "#Deltar < 0.4", "pl");
  leg->AddEntry(p_dreff2_12, "#Deltar < 0.6", "pl");
    
    xjjroot::mypdf pdf("figspdf/gmatch/guns.pdf");

#define DRAW_HIST(q, w)                                         \
  pdf.prepare();                                                \
  h_dr_pt_##q##w->Draw("colz");                                 \
  pdf.write("figs/gmatch/gun_map_"#q#w".png");                  \
  pdf.prepare();                                                \
  hempty->Draw("axis");                                         \
  p_dreff_##q##w->Draw("ple");                                  \
  p_dreff1_##q##w->Draw("ple");                                 \
  p_dreff2_##q##w->Draw("ple");                                 \
  leg->Draw(); \
  xjjroot::drawtex(0.28, 0.83, "combination "#q#w, 0.035);      \
  pdf.write("figs/gmatch/guns_eff_"#q#w".png");                 \

  TRKLTS2P(DRAW_HIST)
  
    pdf.close();

  auto outf = new TFile("data/guns.root", "recreate");

#define WRITE_HIST(q, w)                        \
  h_dr_pt_##q##w->Write();                      \
  p_dreff_##q##w->Write();                      \

  TRKLTS2P(WRITE_HIST)

    outf->Close();
  return 0;
}

int macro2(std::string input_guns, std::string input_mc) {
  auto h_dr_pt_12 = (TH2F*)(TFile::Open(input_guns.c_str())->Get("h_dr_pt_12"));
  int ynbins = h_dr_pt_12->GetYaxis()->GetNbins(),
    xnbins = h_dr_pt_12->GetXaxis()->GetNbins();
  std::vector<TH1F*> h_dr(ynbins);
  std::vector<float> vpt(ynbins+1);
  vpt[0] = h_dr_pt_12->GetYaxis()->GetXmin();
  for(int i=0; i<ynbins; i++) {
    h_dr[i] = new TH1F(Form("h_dr%d", i), ";dr;", h_dr_pt_12->GetXaxis()->GetNbins(), h_dr_pt_12->GetXaxis()->GetXmin(), h_dr_pt_12->GetXaxis()->GetXmax());
    for (int j=0; j<xnbins; j++) {
      h_dr[i]->SetBinContent(j+1, h_dr_pt_12->GetBinContent(j+1, i+1));
    }
    vpt[i+1] = h_dr_pt_12->GetYaxis()->GetBinCenter(i+1) + h_dr_pt_12->GetYaxis()->GetBinWidth(i+1)/2.;
  }

  TH1F* hdr_reco = new TH1F("hdr_reco", ";#Deltar;", 50, 0, 0.5);
  TH1F* hdr_gen = new TH1F("hdr_gen", ";#Deltar;", 50, 0, 0.5);
  xjjroot::sethempty(hdr_reco);
  xjjroot::sethempty(hdr_gen);

  xjjroot::setthgrstyle(hdr_reco, xjjroot::colorlist_middle[0], 20, 1, xjjroot::colorlist_middle[0], 1, 1);
  xjjroot::setthgrstyle(hdr_gen, xjjroot::colorlist_middle[1], 20, 1, xjjroot::colorlist_middle[1], 1, 1);
  
  auto t12 = (TTree*)(TFile::Open(input_mc.c_str())->Get("TrackletTree12"));
  int ntracklet; t12->SetBranchAddress("ntracklet", &ntracklet);
  float dr2[50000]; t12->SetBranchAddress("dr2", dr2);
  auto ttruth = (TTree*)(TFile::Open(input_mc.c_str())->Get("truth"));
  int npart; ttruth->SetBranchAddress("npart", &npart);
  int process; ttruth->SetBranchAddress("process", &process);
  float pt[50000]; ttruth->SetBranchAddress("pt", pt);
  int chg[50000]; ttruth->SetBranchAddress("chg", chg);
  for (int i=0; i<t12->GetEntries(); i++) {
    t12->GetEntry(i);
    ttruth->GetEntry(i);
    xjjc::progressslide(i, t12->GetEntries(), 1000);
    for (int j=0; j<ntracklet; j++) {
      hdr_reco->Fill(std::sqrt(dr2[j]));

      if (chg[j] == 0) continue;
      if (pt[j] >= 0.5) continue;
      int k=0;
      for (;k<ynbins;k++) {
        if (pt[j] < vpt[k+1])
          break;
      }
      if (h_dr[k]->Integral() == 0) continue;
      hdr_gen->Fill(h_dr[k]->GetRandom());
    }
  }
  xjjc::progressbar_summary(t12->GetEntries());

  hdr_reco->Scale(1./hdr_reco->GetBinContent(hdr_reco->FindBin(0.1)));
  hdr_gen->Scale(1./hdr_gen->GetBinContent(hdr_gen->FindBin(0.1)));
  auto hdr_ratio = (TH1F*)hdr_gen->Clone("hdr_ratio");
  hdr_ratio->Divide(hdr_reco);
  hdr_ratio->GetYaxis()->SetTitle("Ratio");
  hdr_ratio->SetMinimum(0);
  hdr_ratio->SetMaximum(2);
  
  auto leg = new TLegend(0.70, 0.85-2*0.04, 0.85, 0.85);
  xjjroot::setleg(leg, 0.035);
  leg->AddEntry(hdr_gen, "Gen", "pl");
  leg->AddEntry(hdr_reco, "Reco", "pl");
  
  xjjroot::setgstyle();
  xjjroot::mypdf pdf("figspdf/gmatch/dr.pdf");

  pdf.prepare();
  hdr_gen->Draw("pe");
  hdr_reco->Draw("pe same");
  leg->Draw();
  xjjroot::drawCMSleft();
  pdf.write("figs/gmatch/dr-epos.png");

  pdf.prepare();
  hdr_ratio->Draw("pe");
  leg->Draw();
  xjjroot::drawCMSleft();
  pdf.write("figs/gmatch/dr-epos-ratio.png");

  
  pdf.close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 2)
    return macro(argv[1]);
  if (argc == 3)
    return macro2(argv[1], argv[2]);

  return 1;
}
