#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCut.h>

#include <vector>
#include <algorithm>    // std::sort
#include <string>

#include "include/xjjrootuti.h"
#include "include/xjjcuti.h"

int macro(std::string inputname, std::string var="pixel/PixelTree::hft", int division=1)
// int macro(std::string inputname, std::string var="TrackletTree12::hft")
{
  auto inputs = xjjc::str_divide(inputname, ",");
  auto vars = xjjc::str_divide(var, "::");
  auto t = (TTree*)TFile::Open(inputs[0].c_str())->Get(vars[0].c_str());
  auto isMC = t->GetEntries("npart!=0");
  if(isMC) std::cout<<"MC"<<std::endl;
  else std::cout<<"data"<<std::endl;

  t->SetBranchStatus("*", 0);
  float hft; t->SetBranchStatus(vars[1].c_str(), 1); t->SetBranchAddress(vars[1].c_str(), &hft);
  int nhfp; t->SetBranchStatus("nhfp", 1); t->SetBranchAddress("nhfp", &nhfp);
  int nhfn; t->SetBranchStatus("nhfn", 1); t->SetBranchAddress("nhfn", &nhfn);
  int process; t->SetBranchStatus("process", 1); t->SetBranchAddress("process", &process);

  TH1D* hhft = new TH1D("hhft", ";HF Energy;", 1000, 0, 6.e+3);
  
  std::vector<float> vhft, vhibins(1, 0);
  auto nentries = t->GetEntries();
  for(int i=0; i<nentries; i+=division)
    {
      xjjc::progressslide(i, nentries, 100*division);
      t->GetEntry(i);
      if(!isMC && (nhfp <= 2 || nhfn <= 2 )) continue; // data
      if(isMC && (process == 102 || process == 103 || process == 104)) continue; // MC
      vhft.push_back(hft);
      hhft->Fill(hft);
    }
  xjjc::progressbar_summary(nentries);

  int n = vhft.size();
  // std::sort(vhft.begin(), vhft.end(), std::greater<float>());
  std::sort(vhft.begin(), vhft.end());

  for(int i=1; i<200; i++)
    vhibins.push_back(vhft[std::ceil(i/200.*n)-1]);
  vhibins.push_back(2.e+4);

  std::cout<<"const Double_t binTable[nBins+1] = {0";
  for(int i=1; i<=200; i++)
    std::cout<<", "<<vhibins[i];
  std::cout<<"};"<<std::endl;

  std::cout<<"static const float hf[21] = {"<<std::endl<<"0.00, ";
  for(int i=1; i<=20; i++)
    {
      std::cout<<vhibins[i*10];
      if(i<20) std::cout<<", ";
      if(i%5==4) std::cout<<std::endl;
    }
  std::cout<<std::endl<<"};"<<std::endl;

  auto outf = new TFile(Form("rootfiles/%s.root", inputs[1].c_str()), "recreate");
  outf->cd();
  xjjroot::writehist(hhft);
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==3) return macro(argv[1], argv[2]);
  if(argc==2) return macro(argv[1]);
  return 1;
}
