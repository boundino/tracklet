#include <TFile.h>
#include <string>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <map>
#include <vector>

#include "include/xjjrootuti.h"

int macro(std::string input_truth, std::string input_corr)
{
  std::vector<int> types = {12, 13, 14, 23, 24, 34, 56, 57, 67};
  std::map<int, TH1D*> h1WEfinal;
  int count = 0;
  for(auto t : types)
    {
      h1WEfinal[t] = (TH1D*)TFile::Open((input_corr+std::string(Form("-%d", t))+".root").c_str())->Get("h1WEfinal");
      h1WEfinal[t]->SetName(Form("%s-%d", h1WEfinal[t]->GetName(), t));
      if(t < 50)
        xjjroot::setthgrstyle(h1WEfinal[t], xjjroot::colorlist_middle[count], 20, 1);
      else
        xjjroot::setthgrstyle(h1WEfinal[t], xjjroot::colorlist_middle[count], 53, 1);
      count++;
    }
  TH1D* h1WGhadron = (TH1D*)TFile::Open(input_truth.c_str())->Get("h1WGhadron");
  xjjroot::sethempty(h1WGhadron);
  h1WGhadron->SetTitle(";#eta;dN/d#eta");
  h1WGhadron->SetMinimum(0);
  h1WGhadron->SetMaximum(700);
  h1WGhadron->SetLineWidth(3);
  
  auto leg = new TLegend(0.5, 0.2, 0.7, 0.55);
  xjjroot::setleg(leg, 0.035);
  leg->AddEntry(h1WGhadron, "HYDJET", "l");
  leg->AddEntry((TObject*)0, "362294", NULL);
  for(auto& t : types)
    leg->AddEntry(h1WEfinal[t], Form("%d", t), "p");

  xjjroot::setgstyle();
  TCanvas* c = new TCanvas("c", "", 600, 600);
  h1WGhadron->Draw("hist");
  for(auto& t : types)
    h1WEfinal[t]->Draw("p same");
  leg->Draw();
  c->SaveAs("figs/temp/r362294.pdf");

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==3) return macro(argv[1], argv[2]);
  return 1;
}
