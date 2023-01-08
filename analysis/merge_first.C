#include <TFile.h>
#include <string>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <map>
#include <vector>

#include "include/xjjrootuti.h"
#include "include/xjjanauti.h"

int macro(std::string input_truth, std::string input_corr, std::string tag)
{
  std::vector<std::string> types = {"1@2", "1@3", "1@4", "2@3", "2@4", "3@4", "5@6", "5@7", "6@7"};
  std::vector<Color_t> cc = {kBlue+2, kAzure, kAzure-2, kGreen+3, kGreen-2, kCyan+2, kRed+3, kRed-3, kRed-6};
  std::map<std::string, TH1D*> h1WEfinal;
  int count = 0;
  for(auto t : types)
    {
      h1WEfinal[t] = xjjroot::gethist<TH1D>(input_corr+"-"+xjjc::str_replaceall(t, "@", "")+".root::h1WEfinal");
      h1WEfinal[t]->SetName(Form("%s-%d", h1WEfinal[t]->GetName(), t));
      if(count < 6)
        xjjroot::setthgrstyle(h1WEfinal[t], cc[count], 20, 1, cc[count]);
      else
        xjjroot::setthgrstyle(h1WEfinal[t], cc[count], 53, 1, cc[count]);
      count++;
    }
  TH1D* h1WGhadron = (TH1D*)TFile::Open(input_truth.c_str())->Get("h1WGhadron");
  xjjroot::sethempty(h1WGhadron);
  h1WGhadron->SetTitle(";#eta;dN/d#eta");
  h1WGhadron->SetMinimum(0);
  h1WGhadron->SetMaximum(800);
  auto gh1WGhadron = xjjana::shifthistcenter(h1WGhadron, "gh1WGhadron", 0, "X0");
  gh1WGhadron->SetLineWidth(2);
  
  auto legBPIX = new TLegend(0.3, 0.5-0.038*6, 0.5, 0.5);
  xjjroot::setleg(legBPIX, 0.035);
  for(int i=0; i<6; i++)
    legBPIX->AddEntry(h1WEfinal[types[i]], Form("BPIX %s", xjjc::str_replaceall(types[i], "@", " #otimes ").c_str()), "p");

  auto legFPIX = new TLegend(0.55, 0.5-0.038*3, 0.75, 0.5);
  xjjroot::setleg(legFPIX, 0.035);
  for(int i=6; i<9; i++)
    legFPIX->AddEntry(h1WEfinal[types[i]], Form("FPIX %s", xjjc::str_replaceall(types[i], "@", " #otimes ").c_str()), "p");

  auto legHYDJET = new TLegend(0.55, 0.5-0.038*5, 0.75, 0.5-0.038*4);
  xjjroot::setleg(legHYDJET, 0.035);
  legHYDJET->AddEntry(gh1WGhadron, "HYDJET", "l");

  xjjroot::setgstyle();
  TCanvas* c = new TCanvas("c", "", 600, 600);
  h1WGhadron->Draw("axis");
  gh1WGhadron->Draw("c same");
  for(auto& t : types)
    h1WEfinal[t]->Draw("p same");
  legBPIX->Draw();
  legFPIX->Draw();
  legHYDJET->Draw();
  xjjroot::drawCMS("Internal", "PbPb (5.36 TeV)");
  xjjroot::drawcomment(tag);
  xjjroot::saveas(c, "figs/temp/"+tag+".pdf");

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==4) return macro(argv[1], argv[2], argv[3]);
  return 1;
}
