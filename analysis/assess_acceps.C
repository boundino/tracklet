#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <string>
#include <vector>
#include <fstream>

#include "include/cosmetics.h"
#include "include/acceptance.h"

#include "include/xjjrootuti.h"
#include "include/xjjanauti.h"

void convert(TH2* h1) {
  TH1D* hvz = (TH1D*)h1->ProjectionY("hvz");

  for (int i=1; i<=h1->GetNbinsX(); ++i) {
    for (int j=1; j<=h1->GetNbinsY(); ++j) {
      // double data_pdf = TMath::Gaus(hvz->GetBinCenter(j), -0.0063239, 4.67374, 1);
      double data_pdf = TMath::Gaus(hvz->GetBinCenter(j), -1.76519e-01, 5.01265, 1);
      if (h1->GetBinContent(i, j)) { h1->SetBinContent(i, j, data_pdf); }
    }
  }

  delete hvz;
}

int assess_acceps(bool recreate, int type, float maxdr2,
                  const char* data_list, const char* mc_list,
                  const char* path, const char* label) {

  TTree *tdata = 0, *tmc = 0;
  TFile* fout = 0;
  if (recreate) {
      tdata = (TTree*)TFile::Open(data_list)->Get(Form("TrackletTree%i", type));
      tmc = (TTree*)TFile::Open(mc_list)->Get(Form("TrackletTree%i", type));
      xjjroot::mkdir(Form("%s/acceptance-%i.root", path, type));
      fout = new TFile(Form("%s/acceptance-%i.root", path, type), "recreate");
    }
  else
    fout = TFile::Open(Form("%s/acceptance-%i.root", path, type));

#define INCLUDE_VZ_RANGE
#define INCLUDE_ETA_RANGE
#include "include/bins.h"

  //
  TH2D *hdata, *hdatacoarse, *hmc, *hmccoarse;
  if (!recreate) {
      hdata = (TH2D*)fout->Get("hdata");
      hdatacoarse = (TH2D*)fout->Get("hdatacoarse");
      hmc = (TH2D*)fout->Get("hmc");
      hmccoarse = (TH2D*)fout->Get("hmccoarse");
    }
  else {
      int nfeta = neta * 100;
      int nfvz = nvz * 100;

      hdata = new TH2D("hdata", "", nfeta, etamin, etamax, nfvz, vzmin, vzmax);
      tdata->Project("hdata", "vz[1]:eta1", Form("dr2<%f && abs(vz[1])<15", maxdr2), "");
      convert(hdata);
      htitle(hdata, ";#eta;v_{z}");
      hdatacoarse = (TH2D*)hdata->Clone("hdatacoarse");
      hdatacoarse->RebinX(nfeta / neta);
      hdatacoarse->RebinY(nfvz / nvz);

      hmc = new TH2D("hmc", "", nfeta, etamin, etamax, nfvz, vzmin, vzmax);
      tmc->Project("hmc", "vz[1]:eta1", Form("dr2<%f && abs(vz[1])<15", maxdr2), "");
      convert(hmc);
      htitle(hmc, ";#eta;v_{z}");
      hmccoarse = (TH2D*)hmc->Clone("hmccoarse");
      hmccoarse->RebinX(nfeta / neta);
      hmccoarse->RebinY(nfvz / nvz);
    }

  TH2D* hratio = (TH2D*)hmccoarse->Clone("hratio");
  hratio->Divide(hdatacoarse);
  hratio->SetStats(0);

  const int* amap = ext_accep_map(type);
  auto h2amapxev = (TH2D*)hratio->Clone("h2amapxev");
  for (int i=1; i<=neta; i++)
    for (int j=1; j<=nvz; j++) {
      h2amapxev->SetBinContent(i, j, amap[(nvz-j)*neta+i-1]);
      h2amapxev->SetBinError(i, j, 0);
    }

  //
  hdata->SetStats(0);
  hdatacoarse->SetStats(0);
  hmc->SetStats(0);
  hmccoarse->SetStats(0);

  if (recreate) {
      fout->cd();
      hdata->Write();
      hdatacoarse->Write();
      hmc->Write();
      hmccoarse->Write();
    }

  xjjroot::mkdir("figs/corrections/x");
  //
  TCanvas* c1 = new TCanvas("c1", "", 1200, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  hdata->Draw("colz");
  c1->cd(2);
  hmc->Draw("colz");
  c1->SaveAs(Form("figs/corrections/geometric-%s-%i-fine.png", label, type));

  TCanvas* c2 = new TCanvas("c2", "", 1200, 600);
  c2->Divide(2, 1);
  c2->cd(1);
  hdatacoarse->Draw("colz");
  c2->cd(2);
  hmccoarse->Draw("colz");
  c2->SaveAs(Form("figs/corrections/geometric-%s-%i-binned.png", label, type));

  TCanvas* c3 = new TCanvas("c3", "", 600, 600);
  // hratio->SetMaximum(5);
  hratio->Draw("colz");
  xjjana::drawhoutline(h2amapxev, kRed);
  c3->SaveAs(Form("figs/corrections/geometric-%s-%i.png", label, type));

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 8) {
    return assess_acceps(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), argv[4], argv[5], argv[6], argv[7]);
  } else {
    printf("usage: ./assess_acceps [type] [dr2] [data] [mc] [path] [label]\n");
    return 1;
  }
}
