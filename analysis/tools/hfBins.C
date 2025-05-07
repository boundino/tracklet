#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCut.h>

#include <vector>
#include <algorithm>    // std::sort
#include <string>

#include "include/xjjrootuti.h"
#include "include/xjjcuti.h"
#include "include/cfout.h"

#define NCENTBINS 200
const float upper_limit = 2.e+4;

int macro(std::string inputname, std::string outputname, std::string var="pixel/PixelTree::hft", int division=1)
// int macro(std::string inputname, std::string var="TrackletTree12::hft")
{
  auto vars = xjjc::str_divide(var, "::");
  auto t = (TTree*)TFile::Open(inputname.c_str())->Get(vars[0].c_str());
  auto ismc = t->GetEntries("npart!=0");
  std::cout<<"  [ "<<(ismc?"MC":"data")<<" ]"<<std::endl;

  t->SetBranchStatus("*", 0);
  float hft; t->SetBranchStatus(vars[1].c_str(), 1); t->SetBranchAddress(vars[1].c_str(), &hft);
  int nhfp; t->SetBranchStatus("nhfp", 1); t->SetBranchAddress("nhfp", &nhfp);
  int nhfn; t->SetBranchStatus("nhfn", 1); t->SetBranchAddress("nhfn", &nhfn);
  int process; t->SetBranchStatus("process", 1); t->SetBranchAddress("process", &process);

  TH1D* hhft = new TH1D("hhft", ";HF Energy;", 1000, 0, 6.e+3);
  
  std::vector<float> vhft, vhibins(1, 0);
  auto nentries = t->GetEntries();
  for (int i=0; i<nentries; i+=division) {
    xjjc::progressslide(i, nentries, 100*division);
    t->GetEntry(i);
    if (!ismc && (nhfp <= 2 || nhfn <= 2 )) continue; // data
    if (ismc && (process == 102 || process == 103 || process == 104)) continue; // MC
    vhft.push_back(hft);
    hhft->Fill(hft);
  }
  xjjc::progressbar_summary(nentries);

  int n = vhft.size();
  // std::sort(vhft.begin(), vhft.end(), std::greater<float>());
  std::sort(vhft.begin(), vhft.end());

  for (int i=1; i<NCENTBINS; i++)
    vhibins.push_back(vhft[std::ceil(i/(NCENTBINS*1.)*n)-1]);
  vhibins.push_back(upper_limit);

  auto outputh = Form("tools/cent/cent_%s.h", outputname.c_str());
  std::ofstream ofs(outputh);
  xjjc::cfout out(ofs, std::cout);

  std::cout<<"\e[30;3m"<<std::endl;

  out<<"// "<<inputname<<std::endl;
  out<<std::endl;

  out<<"const int nBins=200;"<<std::endl;
  out<<"const double binTable[nBins+1] = { 0., ";
  for (int i=1; i<=NCENTBINS; i++) {
    out<<vhibins[i];
    if (i<NCENTBINS) out<<", ";
    if (i%10==9) out<<std::endl<<"  ";
  }
  out<<"};"<<std::endl;

  out<<std::endl;
  
  out<<"static const float hf[21] = { 0., ";
  for (int i=1; i<=20; i++) {
    out<<vhibins[i*10];
    if (i<20) out<<", ";
    if (i%5==4) out<<std::endl<<"  ";
  }
  out<<std::endl<<"};"<<std::endl;

  out<<std::endl;
  
  out<<"int getHiBinFromhiHF(const double hiHF) { \n\
  int binPos = -1; \n\
  for (int i = 0; i < nBins; ++i) { \n\
    if (hiHF >= binTable[i] && hiHF < binTable[i+1]) { \n\
      binPos = i; \n\
      break; \n\
    } \n\
  } \n\
 \n\
  binPos = nBins - 1 - binPos; \n\
 \n\
  return (int)(200*((double)binPos)/((double)nBins)); \n\
}"<<std::endl;

  std::cout<<"\e[0m"<<std::endl;

  auto outf = new TFile(Form("data/cent-%s.root", outputname.c_str()), "recreate");
  outf->cd();
  xjjroot::writehist(hhft);
  outf->Close();

  std::cout<<outputh<<std::endl;
  
  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==3) return macro(argv[1], argv[2]);
  return 1;
}
