#include <TFile.h>
#include <TH1F.h>
#include <TCut.h>
#include <string>
#include "../include/xjjrootuti.h"
#include "../include/xjjmypdf.h"
#include "../include/sconfig.h"
#include "centvar.h"

std::vector<float> hibincali(TTree* t);
void printout(std::vector<float> &vhibins);
int hfbin(float hft, std::vector<float> &vhibins);
void fillhibin(TTree* t, std::vector<float> &vhibins, TH1F* hall, TH1F* hsel);

int macro(std::string inputname_data,
          std::string inputname_mc,
          std::string inputname_comb,
          std::string outputname,
          std::string tag,
          std::string evtsel = "fabs(vz[1])<15&&hlt&&cluscomp&&nhfp>1&&nhfn>1") {

  xjjc::sconfig conf_data(inputname_data), conf_mc(inputname_mc), conf_comb(inputname_comb);
  
  auto nt_data = (TTree*)(TFile::Open(conf_data.value[0][0].c_str())->Get(conf_data.value[0][1].c_str()));
  auto nt_mc = (TTree*)(TFile::Open(conf_mc.value[0][0].c_str())->Get(conf_mc.value[0][1].c_str()));
  auto nt_comb = (TTree*)(TFile::Open(conf_comb.value[0][0].c_str())->Get(conf_comb.value[0][1].c_str()));

  auto nt_info = (TTree*)(TFile::Open(conf_comb.value[0][0].c_str())->Get("info"));
  float threshold; nt_info->SetBranchAddress("threshold", &threshold);
  float mcscale; nt_info->SetBranchAddress("mcscale", &mcscale);
  nt_info->GetEntry(0);
  std::cout<<"threshold: "<<threshold<<" | mcscale: "<<mcscale<<std::endl;

  TH1F* hhft_data = new TH1F("hhft_data", ";HF E_{T} (GeV);", 72, 0, 6000);
  TH1F* hhft_mc = new TH1F("hhft_mc", ";HF E_{T} (GeV);", 72, 0, 6000);
  TH1F* hhft_comb = new TH1F("hhft_comb", ";HF E_{T} (GeV);", 72, 0, 6000);

  nt_data->Draw("hft>>hhft_data", TCut(evtsel.c_str()), "goff");
  nt_mc->Draw(Form("hft*%f>>hhft_mc", mcscale), TCut(evtsel.c_str()), "goff");
  nt_comb->Draw("hft>>hhft_comb", TCut(evtsel.c_str()), "goff");
  hhft_data->Sumw2();
  hhft_mc->Sumw2();
  hhft_comb->Sumw2();

  hhft_mc->Scale(hhft_data->GetEntries()/hhft_mc->GetEntries());

  auto hhft_ratio_data = (TH1F*)hhft_data->Clone("hhft_ratio_data");
  hhft_ratio_data->Divide(hhft_mc);
  auto hhft_ratio_comb = (TH1F*)hhft_comb->Clone("hhft_ratio_comb");
  hhft_ratio_comb->Divide(hhft_mc);

  auto hfs = hibincali(nt_comb);
  printout(hfs);
  TH1F* hhfbin_data_all = new TH1F("hhfbin_data_all", ";hiBin;", 200, 0, 200);
  TH1F* hhfbin_comb_all = new TH1F("hhfbin_comb_all", ";hiBin;", 200, 0, 200);
  TH1F* hhfbin_data_sel = new TH1F("hhfbin_data_sel", ";hiBin;", 200, 0, 200);
  TH1F* hhfbin_comb_sel = new TH1F("hhfbin_comb_sel", ";hiBin;", 200, 0, 200);
  fillhibin(nt_data, hfs, hhfbin_data_all, hhfbin_data_sel);
  fillhibin(nt_comb, hfs, hhfbin_comb_all, hhfbin_comb_sel);
  
  xjjroot::setthgrstyle(hhft_data, kOrange+1, 20, 1.2, kOrange+1, 1, 2);
  xjjroot::setthgrstyle(hhft_mc, kMagenta+2, 20, 0.2, kMagenta+2, 1, 2);
  xjjroot::setthgrstyle(hhft_comb, kCyan-2, 20, 1.2, kCyan-2, 1, 2);
  xjjroot::setthgrstyle(hhft_ratio_data, kOrange+1, 20, 1.2, kOrange+1, 1, 2);
  xjjroot::setthgrstyle(hhft_ratio_comb, kCyan-2, 20, 1.2, kCyan-2, 1, 2);
  xjjroot::setthgrstyle(hhfbin_data_all, kAzure-5, 20, 1.2, kAzure-5, 1, 2);
  xjjroot::setthgrstyle(hhfbin_data_sel, kOrange+1, 20, 1.2, kOrange+1, 1, 2);
  xjjroot::setthgrstyle(hhfbin_comb_all, kCyan-2, 20, 1.2, kCyan-2, 1, 2);
  xjjroot::setthgrstyle(hhfbin_comb_sel, kMagenta+2, 20, 1.2, kMagenta+2, 1, 2);

  hhft_mc->SetMaximum(std::max(hhft_mc->GetMaximum(), hhft_data->GetMaximum())*5);
  hhft_mc->SetMinimum(hhft_mc->GetMaximum()*1.e-6);
  hhft_ratio_data->SetMaximum(1.3);
  hhft_ratio_data->SetMinimum(0.7);
hhfbin_comb_all->SetMaximum(hhfbin_comb_all->GetMaximum()*1.5);

  auto leg = new TLegend(0.40, 0.45-0.04*3, 0.60, 0.45);
  xjjroot::setleg(leg, 0.035);
  leg->AddEntry(hhft_mc, "Hydjet #times 0.86", "l");
  leg->AddEntry(hhft_data, "Data", "p");
  leg->AddEntry(hhft_comb, "Combined", "p");
  
  auto leghibin = new TLegend(0.40, 0.45-0.04*3, 0.60, 0.45);
  xjjroot::setleg(leghibin, 0.035);
  leghibin->AddEntry(hhfbin_data_sel, "Data w. evtsel", "p");
  leghibin->AddEntry(hhfbin_comb_all, "Combined", "p");
  leghibin->AddEntry(hhfbin_comb_sel, "Combined w. evtsel", "p");
  
  xjjroot::setgstyle();
  xjjroot::sethempty(hhft_mc);
  xjjroot::sethempty(hhft_ratio_data);
  xjjroot::sethempty(hhfbin_data_all);
  xjjroot::sethempty(hhfbin_comb_all);

  auto pdf = new xjjroot::mypdf(Form("../figspdf/hfcali/%s.pdf", outputname.c_str()));
  pdf->getc()->SetLogy();
  pdf->prepare();
  hhft_mc->Draw("hist");
  hhft_data->Draw("pe same");
  hhft_comb->Draw("pe same");
  leg->Draw();
  xjjroot::drawtex(0.23, 0.85, evtsel.c_str(), 0.03, 12, 62);
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright("PbPb #sqrt{s_{NN}} = 5.36 TeV");
  pdf->write();

  pdf->prepare();
  pdf->getc()->SetLogy(0);
  hhft_ratio_data->Draw("pe");
  xjjroot::drawline(0, 1, 6000, 1, hhft_mc->GetLineColor(), 2, 2);
  hhft_ratio_comb->Draw("pe same");
  pdf->write();

  pdf->prepare();
  // hhfbin_data_all->Draw("pe");
  hhfbin_comb_all->Draw("pe");
  hhfbin_comb_sel->Draw("pe same");
  hhfbin_data_sel->Draw("pe same");
leghibin->Draw();
  xjjroot::drawtex(0.23, 0.85, Form("thres = %.0f", threshold), 0.03, 12, 62);
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright("PbPb #sqrt{s_{NN}} = 5.36 TeV");
  pdf->write();

  pdf->close();
  
  auto outf = xjjroot::newfile(Form("output/%s.root", outputname.c_str()));
  xjjroot::writehist(hhft_data);
  xjjroot::writehist(hhft_mc);
  xjjroot::writehist(hhft_comb);
  xjjroot::writehist(hhfbin_data_all);
  xjjroot::writehist(hhfbin_comb_all);
  xjjroot::writehist(hhfbin_data_sel);
  xjjroot::writehist(hhfbin_comb_sel);
  outf->Close();

  return 0;
}

Color_t cc[] = {kRed, kGreen-5, kBlue-6};
Style_t ss[] = {24, 33, 22};
int combine(std::string inputs="100,50,300") {
  xjjc::sconfig conf(inputs);

  auto leg = new TLegend(0.30, 0.45-0.04*3, 0.50, 0.45);
  xjjroot::setleg(leg, 0.035);
  std::vector<TH1F*> hhibin(conf.n());
  for (int i=0; i<conf.n(); i++) {
    float thresh = atof(conf.value[i][0].c_str());
    
    hhibin[i] = (TH1F*)(TFile::Open(Form("output/hf2Th4_362294_HYDJET_thre%.0f.root", thresh))->Get("hhfbin_data_sel"));
    hhibin[i]->SetName(Form("hhfbin_data_sel_%.0f", thresh));
    xjjroot::setthgrstyle(hhibin[i], cc[i], ss[i], 1.2, cc[i], 1, 2);
    leg->AddEntry(hhibin[i], Form("thresh = %.0f, %s", thresh, (thresh==100?"nominal":"syst")));
    xjjroot::sethempty(hhibin[i]);
    hhibin[i]->SetMaximum(hhibin[i]->GetMaximum()*1.4);
  }
  
  xjjroot::setgstyle();
  auto pdf = new xjjroot::mypdf("../figspdf/hfcali/hf2Th4_362294_HYDJET_combthre.pdf");
  pdf->prepare();
  hhibin[0]->Draw("p");
  for (int i=conf.n()-1; i>=0; i--) {
    hhibin[i]->Draw("p same");
  }
  leg->Draw();
  xjjroot::drawtex(0.23, 0.85, "Data w. evtsel", 0.03, 12, 62);
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright("PbPb #sqrt{s_{NN}} = 5.36 TeV");
  pdf->write();
  pdf->close();
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc==6)
    return macro(argv[1], argv[2], argv[3], argv[4], argv[5]);
  if (argc==1)
    return combine();
  return 1;
}


std::vector<float> hibincali(TTree* t) {
  // t->SetBranchStatus("*", 0);
  float hft; t->SetBranchStatus("hft", 1); t->SetBranchAddress("hft", &hft);
  std::vector<float> vhft, vhibins(1, 0);
  auto nentries = t->GetEntries();
  for(int i=0; i<nentries; i++)
    {
      xjjc::progressslide(i, nentries, 10000);
      t->GetEntry(i);
      vhft.push_back(hft);
    }
  xjjc::progressbar_summary(nentries);

  int n = vhft.size();
  // std::sort(vhft.begin(), vhft.end(), std::greater<float>());
  std::sort(vhft.begin(), vhft.end());

  for(int i=1; i<200; i++)
    vhibins.push_back(vhft[std::ceil(i/200.*n)-1]);
  vhibins.push_back(2.e+4);
  std::cout<<vhibins.size()<<std::endl;

  return vhibins;
}

void printout(std::vector<float> &vhibins) {
  std::cout<<"const Double_t binTable[nBins+1] = {0";
  for(int i=1; i<=200; i++)
    std::cout<<", "<<vhibins[i];
  std::cout<<"};"<<std::endl;

  std::cout<<"static const float hf[21] = {"<<std::endl<<"  0.00, ";
  for(int i=1; i<=20; i++)
    {
      std::cout<<vhibins[i*10];
      if(i<20) std::cout<<", ";
      if(i%5==4) std::cout<<std::endl<<"  ";
    }
  std::cout<<std::endl<<"};"<<std::endl;
}

int hfbin(float hft, std::vector<float> &vhibins) {
  for (int i=0; i<vhibins.size()-1; ++i) {
    if (hft < vhibins[i+1])
      return 199-i;
  }
  return -1;
}

void fillhibin(TTree* t, std::vector<float> &vhibins, TH1F* hall, TH1F* hsel) {
  centvar cv;
  setbranches(t, cv);
  auto nentries = t->GetEntries();
  for (int i=0; i<nentries; i++) {
    xjjc::progressslide(i, nentries, 10000);
    t->GetEntry(i);
    auto hibin = hfbin(cv.hft, vhibins);
    if (hibin < 0) continue;
    hall->Fill(hibin);
    if(cv.nhfp <= 1 || cv.nhfn <= 1 ||
       !cv.hlt || !cv.cluscomp ||
       fabs(cv.vz[1]) > 15) continue;
    hsel->Fill(hibin);
  }
hall->SetMinimum(0);
hsel->SetMinimum(0);
}
