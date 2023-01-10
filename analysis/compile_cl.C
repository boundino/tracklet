#include <TFile.h>
#include <string>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <vector>

#include "include/xjjrootuti.h"
#include "include/xjjanauti.h"
#include "include/sconfig.h"

std::string tcomb(std::string comb)
{
  std::string r(comb);
  r.insert(1, " #otimes ");
  return r;
}

// std::vector<Color_t> cc = {kBlue+2, kAzure, kAzure-2, kGreen+3, kGreen-2, kCyan+2, kRed+3, kRed-3, kRed-6};
int macro(std::string input_corr="362294t-Hydjet_drlt0p5#HYDJET corr,362294t-Epos_drlt0p5#EPOS corr", 
          std::string input_comb="12", 
          std::string input_truth="AMPTnostring_drlt0p5#AMPT (No String)#8,AMPTstring_drlt0p5#AMPT (String Melt)#5,Hydjet_drlt0p5#HYDJET#1,Epos_drlt0p5#EPOS#2", 
          std::string colors="602,860,858,419,414,434,635,629,626",
          std::string text="",
          std::string tag="",
          std::string div="#")
{
  xjjc::sconfig icorr(input_corr, ",", div), icomb(input_comb), itruth(input_truth, ",", div), icolors(colors);
  if(icomb.n() == 1) text = text + ",PIX " + tcomb(icomb.value[0][0]);
  xjjc::sconfig itext(text, ",", div);

  std::vector<Color_t> cc;
  for(auto& icc : icolors.value) cc.push_back(atoi(icc[0].c_str()));
  std::vector<TH1D*> h1WEfinal(icorr.n()*icomb.n(), 0);
  std::vector<TGraphAsymmErrors*> gh1WGhadron(itruth.n(), 0);
  for(int i=0; i<icorr.n(); i++)
    {
      for(int j=0; j<icomb.n(); j++)
        {
          int idx = j + i*icomb.n();
          h1WEfinal[idx] = xjjroot::gethist<TH1D>("output/correction-"+icorr.value[i][0]+"-"+icomb.value[j][0]+".root::h1WEfinal");
          h1WEfinal[idx]->SetName(Form("%s-%s", h1WEfinal[idx]->GetName(), icomb.value[j][0].c_str()));
          xjjroot::setthgrstyle(h1WEfinal[idx], cc[idx], 20, 0.8, cc[idx]);
        }
    }
  for(int i=0; i<itruth.n(); i++)
    {
      auto h = xjjroot::gethist<TH1D>("output/correction-"+itruth.value[i][0]+"-12.root::h1WGhadron");
      gh1WGhadron[i] = xjjana::shifthistcenter(h, "gh1WGhadron-"+itruth.value[i][0], 0, "X0");
      xjjroot::setthgrstyle(gh1WGhadron[i], kBlack, 20, 0.8, kBlack, atoi(itruth.value[i][2].c_str()), 2);
    }

  auto hempty = (TH1D*)h1WEfinal[0]->Clone("hempty");
  xjjroot::sethempty(hempty);
  hempty->SetTitle(";#eta;dN/d#eta");
  hempty->SetMinimum(0);
  hempty->SetMaximum(800);
  
  auto legPIX = new TLegend(0.3, 0.5-0.033*h1WEfinal.size(), 0.3+0.2, 0.5);
  float xleg = 0.55, yleg = 0.5;
  xjjroot::setleg(legPIX, 0.030);
  for(int i=0; i<icorr.n(); i++)
    for(int j=0; j<icomb.n(); j++)
      {
        auto tleg = Form("%s%s%s", icomb.n()==1?"":Form("PIX %s",tcomb(icomb.value[j][0]).c_str()), (icorr.value[i][1]==""||icomb.n()==1?"":", "), icorr.value[i][1].c_str());
        legPIX->AddEntry(h1WEfinal[j+i*icomb.n()], tleg, "p");
        if(std::string(tleg).size() > 15)
          { xleg = 0.31; yleg = 0.48-0.033*h1WEfinal.size(); }
      }
  auto legTRUTH = new TLegend(xleg, yleg-0.033*gh1WGhadron.size(), xleg+0.2, yleg);
  xjjroot::setleg(legTRUTH, 0.030);
  for(int i=0; i<itruth.n(); i++)
    legTRUTH->AddEntry(gh1WGhadron[i],
                       Form("%s", itruth.value[i][1].c_str()),
                       "l");

  xjjroot::setgstyle();
  TCanvas* c = new TCanvas("c", "", 600, 600);
  hempty->Draw("axis");
  for(auto& g : gh1WGhadron)
    g->Draw("c same");
  for(auto& h : h1WEfinal)
    h->Draw("p same");
  legPIX->Draw();
  legTRUTH->Draw();

  for(int i=0; i<itext.n(); i++)
    xjjroot::drawtex(0.25, 0.85-i*0.033, itext.value[i][0].c_str(), 0.030, 13);
  xjjroot::drawCMS("Internal", "PbPb (5.36 TeV)");
  xjjroot::drawcomment(tag);
  xjjroot::saveas(c, "figs/temp/"+tag+".pdf");

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==8) return macro(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
  if(argc==7) return macro(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
  return 1;
}
