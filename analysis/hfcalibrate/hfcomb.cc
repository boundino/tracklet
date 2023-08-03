#include <TFile.h>
#include <TH1F.h>
#include <TCut.h>
#include <TRandom.h>
#include <string>
#include "../include/xjjrootuti.h"
#include "../include/sconfig.h"

#include "centvar.h"

int macro(std::string inputname_data,
          std::string inputname_mc,
          std::string outputname,
          float threshold,
          float mcscale=0.86,
          std::string evtsel = "fabs(vz[1])<15 && hlt && cluscomp && nhfp > 1 && nhfn > 1") {

  xjjc::sconfig conf_data(inputname_data), conf_mc(inputname_mc);
  
  auto nt_data = (TTree*)(TFile::Open(conf_data.value[0][0].c_str())->Get(conf_data.value[0][1].c_str()));
  auto nt_mc = (TTree*)(TFile::Open(conf_mc.value[0][0].c_str())->Get(conf_mc.value[0][1].c_str()));
  auto outf = new TFile(Form("%s", outputname.c_str()), "recreate");
  TTree* nt_info = new TTree("info", "");
  nt_info->Branch("threshold", &threshold, "threshold/F");
  nt_info->Branch("mcscale", &mcscale, "mcscale/F");
  nt_info->Fill();
  nt_info->Write();
  TTree* nt_comb = new TTree("PixelTree", "");

  centvar cv_data, cv_mc, cv_comb;
  setbranches(nt_data, cv_data);
  setbranches(nt_mc, cv_mc);
  branches(nt_comb, cv_comb);

  int nentries_data = nt_data->GetEntries(Form("hft>500 && %s", evtsel.c_str()));
  int nentries_mc = nt_mc->GetEntries(Form("hft*%f>500 && %s", mcscale, evtsel.c_str()));
  float scale = 1.*nentries_data/nentries_mc;
  
  int nentries = nt_data->GetEntries();
  for (int i=0; i<nentries; i++) {

    xjjc::progressbar(i, nentries, 1000);
    nt_data->GetEntry(i);

    if(cv_data.nhfp <= 1 || cv_data.nhfn <= 1 ||
       !cv_data.hlt || !cv_data.cluscomp ||
       fabs(cv_data.vz[1]) > 15) continue;

    if (cv_data.hft < threshold) continue;

    clone(cv_comb, cv_data);

    nt_comb->Fill();
  }
  xjjc::progressbar_summary(nentries);

  nentries = nt_mc->GetEntries();
  for (int i=0; i<nentries; i++) {

    xjjc::progressbar(i, nentries, 1000);
    nt_mc->GetEntry(i);

    if (cv_mc.hft*mcscale > threshold) continue;
    if (gRandom->Rndm() > scale) continue;
  
    clone(cv_comb, cv_mc, mcscale);
    // clone(cv_comb, cv_mc, 1.);

    nt_comb->Fill();
  }
  xjjc::progressbar_summary(nentries);
  
  nt_comb->Write();

  
  outf->Close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc==5)
    return macro(argv[1], argv[2], argv[3], atof(argv[4]));
  return 1;
}
