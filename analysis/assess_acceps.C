#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TMath.h"
#include "TLegend.h"

#include <string>
#include <vector>
#include <fstream>

#include "include/cosmetics.h"
#include "include/acceptance.h"

#include "include/xjjrootuti.h"
#include "include/xjjanauti.h"
#include "include/xjjmypdf.h"

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

void print_acceps(int type, TH2D* hratio) {
  std::cout<<"static const int a"<<type<<"[neta * nvz] = {"<<std::endl;
  for (int j=hratio->GetYaxis()->GetNbins()-1; j>=0; j--) {
    auto nxbins = hratio->GetXaxis()->GetNbins();
    std::cout<<"   ";
    for (int i=0; i<nxbins; i++) {
      int onoroff = (fabs(hratio->GetBinContent(i+1, j+1)-1)<0.01?1:0);
      std::cout<<" "<<onoroff<<",";
    }
    std::cout<<std::endl;
  }
  std::cout<<"};"<<std::endl;
}

int assess_acceps(bool recreate, int type, float maxdr2,
                  const char* data_list, const char* mc_list,
                  const char* path, const char* label,
                  const char* data_tag = "", const char* mc_tag = "") {

  TTree *tdata = 0, *tmc = 0;
  TFile* fout = 0;
  if (recreate) {
    tdata = (TTree*)TFile::Open(data_list)->Get(Form("TrackletTree%i", type));
    tmc = (TTree*)TFile::Open(mc_list)->Get(Form("TrackletTree%i", type));
    auto outputname = Form("%s/acceptance-%i.root", path, type);
    xjjroot::mkdir(outputname);
    fout = new TFile(outputname, "recreate");
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

    std::cout<<"starting: "<<data_list<<std::endl;
    hdata = new TH2D("hdata", "", nfeta, etamin, etamax, nfvz, vzmin, vzmax);
    tdata->Project("hdata", "vz[1]:eta1", Form("dr2<%f && abs(vz[1])<15", maxdr2), "");
    convert(hdata);
    htitle(hdata, ";#eta;v_{z}");
    hdatacoarse = (TH2D*)hdata->Clone("hdatacoarse");
    hdatacoarse->RebinX(nfeta / neta);
    hdatacoarse->RebinY(nfvz / nvz);

    std::cout<<"starting: "<<mc_list<<std::endl;
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

  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  hratio->SetMinimum(0.5);
  hratio->SetMaximum(1.6);
  
  xjjroot::mypdf pdf(Form("figspdf/geometric/geometric-%s-%i.pdf", label, type), "c", 600, 600);
  pdf.prepare();
  hratio->Draw("colz");
  xjjroot::drawtex(0.5, 0.8, Form("#frac{%s (MC)}{%s (data)}", mc_tag, data_tag), 0.038, 23);
  xjjana::drawhoutline(h2amapxev, kRed);
  watermark();
  pdf.write(Form("figs/corrections/geometric-%s-%i.png", label, type));

  pdf.prepare();
  hdata->Draw("colz");
  xjjroot::drawtex(0.5, 0.8, Form("%s (data)", data_tag), 0.038, 23);
  watermark();
  pdf.write(Form("figs/geometric/geometric-%s-%i-hdata.png", label, type));

  pdf.prepare();
  hdatacoarse->Draw("colz");
  xjjroot::drawtex(0.5, 0.8, Form("%s (data)", data_tag), 0.038, 23);
  watermark();
  pdf.write(Form("figs/geometric/geometric-%s-%i-hdatacoarse.png", label, type));

  pdf.prepare();
  hmc->Draw("colz");
  xjjroot::drawtex(0.5, 0.8, Form("%s (MC)", mc_tag), 0.038, 23);
  watermark(true);
  pdf.write(Form("figs/geometric/geometric-%s-%i-hmc.png", label, type));

  pdf.prepare();
  hmccoarse->Draw("colz");
  xjjroot::drawtex(0.5, 0.8, Form("%s (MC)", mc_tag), 0.038, 23);
  watermark(true);
  pdf.write(Form("figs/geometric/geometric-%s-%i-hmccoarse.png", label, type));

  pdf.close();

  print_acceps(type, hratio);

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 10) {
    return assess_acceps(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
  }
  if (argc == 8) {
    return assess_acceps(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), argv[4], argv[5], argv[6], argv[7]);
  }
  printf("usage: ./assess_acceps [recreate] [type] [dr2] [data] [mc] [path] [label] ([data tag] [mc tag]) \n");
  return 1;
}
