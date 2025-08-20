#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

#include <vector>
#include <string>

#include "include/xjjrootuti.h"
#include "include/xjjcuti.h"
#include "include/hf.h"

int make_header(TH1F* hnpart, std::string tag) {

  std::cout<<tag<<" -->"<<std::endl;
  
  std::cout<<"\e[30m"<<std::endl;

  std::cout<<"std::vector<float> "<<tag<<"_oo_5p36_raw = { \n"
    "   0, 0, 0, 0, 0, 0, 0, \n"
    "   0, 0, 0, 0, 0, 0, 0}; \n";
  std::cout<<"std::vector<float> "<<tag<<"_oo_5p36_rawerr = { \n"
    "   0, 0, 0, 0, 0, 0, 0, \n"
    "   0, 0, 0, 0, 0, 0, 0}; \n";
  std::cout<<"std::vector<float> "<<tag<<"_oo_5p36_cent = { \n"
    "   32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5, \n"
    "   67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5}; \n"
    "std::vector<float> "<<tag<<"_oo_5p36_npart = { \n";
  for (int i=6; i<hnpart->GetNbinsX(); i++) {
    std::cout
      <<(i>6?", ":"   ")
      <<hnpart->GetBinContent(i+1);
  }
  std::cout<<"};"<<std::endl;
  std::cout<<"std::vector<float> "<<tag<<"_oo_5p36_nparterr = { \n"
    "   0, 0, 0, 0, 0, 0, 0, \n"
    "   0, 0, 0, 0, 0, 0, 0}; \n"
    "std::vector<float> "<<tag<<"_oo_5p36_ncoll = "<<tag<<"_oo_5p36_npart; \n"
    "std::vector<float> "<<tag<<"_oo_5p36_ncollerr = "<<tag<<"_oo_5p36_nparterr; \n"
    "auto "<<tag<<"_oo_5p36_2a = 32.; \n"
    "auto "<<tag<<"_oo_5p36_size = "<<tag<<"_oo_5p36_raw.size(); \n"
           <<std::endl;

  std::cout<<"\e[0m"<<std::endl;
  std::cout<<"<-- "<<tag<<std::endl;

  return 0;
}

int macro(std::string inputname, unsigned int ctable,
          std::string tag, std::string treevar="pixel/PixelTree")
{
  auto t = (TTree*)TFile::Open(inputname.c_str())->Get(treevar.c_str());
  auto ismc = t->GetEntries("npart!=0");
  std::cout<<"  [ "<<(ismc?"MC":"data")<<" ]"<<std::endl;

  t->SetBranchStatus("*", 0);
  float hi_npart; t->SetBranchStatus("hi_npart", 1); t->SetBranchAddress("hi_npart", &hi_npart);
  float hft; t->SetBranchStatus("hft", 1); t->SetBranchAddress("hft", &hft);
  int process; t->SetBranchStatus("process", 1); t->SetBranchAddress("process", &process);

  auto hfcent = hftables[ctable];
  
  std::vector<TH1F*> hhi_npart(NBIN_CENT, 0);
  for (int i=0; i<NBIN_CENT; i++) {
    hhi_npart[i] = new TH1F(Form("hhi_npart_%d", i), ";N_{part};", 32, 0, 32);
    xjjroot::setthgrstyle(hhi_npart[i], xjjroot::colorlist_middle[i], 21, 1,
                          xjjroot::colorlist_middle[i], 1, 1,
                          xjjroot::colorlist_middle[i], 1, 3005);
  }
  
  auto nentries = t->GetEntries();
  for (int i=0; i<nentries; i++) {
    xjjc::progressslide(i, nentries, 10000);
    t->GetEntry(i);
    if (process == 102 || process == 103 || process == 104) continue; // MC

    auto ibin = hfbin(hft, ctable);
    if (ibin < 0) continue;

    hhi_npart[ibin]->Fill(hi_npart);
  }
  xjjc::progressbar_summary(nentries);

  auto havgnpart = new TH1F("havgnpart", ";Centrality (5\% interval);", NBIN_CENT, 0, NBIN_CENT);
  xjjroot::setthgrstyle(havgnpart, kBlack, 20, 1, kBlack, 1, 1);
  for (int i=0; i<NBIN_CENT; i++) {
    auto avg = hhi_npart[i]->GetMean();
    havgnpart->SetBinContent(i+1, avg);
  }

  make_header(havgnpart, tag);
  
  auto outf = new TFile(Form("../data/npart-%s.root", tag.c_str()), "recreate");
  outf->cd();
  xjjroot::writehist(havgnpart);
  for (auto& h : hhi_npart)
    xjjroot::writehist(h);
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==4) return macro(argv[1], atoi(argv[2]), argv[3]);
  return 1;
}

