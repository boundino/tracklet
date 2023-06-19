#include <TFile.h>
#include <string>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <vector>

#include "include/xjjrootuti.h"
#include "include/xjjmypdf.h"
#include "include/xjjanauti.h"
#include "include/sconfig.h"

#include "include/tool.h"

void setzero(TH1D* h, std::string comb);

#include "include/defines.h"

#define TRKLTS2P(EXPAND)                        \
  BTRKLT2P(EXPAND)                              \
  FTRKLT2P(EXPAND)                              \

#define TRKLTS3P(p, EXPAND)                     \
  BTRKLT3P(p, EXPAND)                           \
  FTRKLT3P(p, EXPAND)                           \

#define COUNT(q, w)  + 1
#define NTRKLT2P  (0 TRKLTS2P(COUNT))

#define INCLUDE_ETA_RANGE
#include "include/bins.h"

static const int good[NTRKLT2P][neta] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0 }
};
std::map<std::string, int> idx = {{"12", 0}, {"13", 1}, {"14", 2},
                                  {"23", 3}, {"24", 4}, {"34", 5},
                                  {"56", 6}, {"57", 7}, {"67", 8}};
auto ms = xjjroot::markerlist_open;
int macro(std::string input_corr,
          std::string text = "Run 362294 corr w/ EPOS",
          std::string input_comb = "12,13,14,23,24,34,56,57,67",
          std::string input_truth = "null",
          std::string colors = "default",
          std::string div = "&")
{
  if(colors == "default") colors = "602,860,858,419,414,434,635,629,626";
  std::string tag = input_corr;
  xjjc::sconfig icomb(input_comb), icolors(colors);
  if(icomb.n() == 1) text = text + ", " + tcomb(icomb.value[0][0]);
  xjjc::sconfig itext(text, ",", div);

  std::vector<Color_t> cc;
  for(auto& icc : icolors.value) cc.push_back(atoi(icc[0].c_str()));
  std::vector<TH1D*> h1WEfinal(icomb.n(), 0);
  for(int j=0; j<icomb.n(); j++)
    {
      h1WEfinal[j] = xjjroot::gethist<TH1D>("output/correction-"+input_corr+"-"+icomb.value[j][0]+".root::h1WEfinal");
      h1WEfinal[j]->SetName(Form("%s-%s", h1WEfinal[j]->GetName(), icomb.value[j][0].c_str()));
      setzero(h1WEfinal[j], icomb.value[j][0]);
      goodresult(h1WEfinal[j]);
      xjjroot::setthgrstyle(h1WEfinal[j], cc[j], ms[j], 0.8, cc[j]);
    }

  std::vector<TGraphAsymmErrors*> gh1WGhadron;
  TLegend* legTRUTH = 0;
  if(input_truth != "null") {
    xjjc::sconfig itruth(input_truth, ",", div);
    gh1WGhadron.resize(itruth.n());
    for(int i=0; i<itruth.n(); i++) {
      auto h = xjjroot::gethist<TH1D>("output/correction-"+itruth.value[i][0]+"-12.root::h1WGhadron");
      gh1WGhadron[i] = xjjana::shifthistcenter(h, "gh1WGhadron-"+itruth.value[i][0], 0, "X0");
      xjjroot::setthgrstyle(gh1WGhadron[i], kBlack, 20, 0.8, kBlack, atoi(itruth.value[i][2].c_str()), 1);
    }
    legTRUTH = new TLegend(0.55, 0.47-0.031*gh1WGhadron.size(), 0.55+0.2, 0.47);
    xjjroot::setleg(legTRUTH, 0.028);
    for(int i=0; i<itruth.n(); i++)
      legTRUTH->AddEntry(gh1WGhadron[i],
                         Form("%s", itruth.value[i][1].c_str()),
                         "l");
  }
  
  TH1F* havg = (TH1F*)h1WEfinal[0]->Clone("havg");
  for(int i=1; i<=havg->GetNbinsX(); i++)
    {
      float avg = 0; float avg_err = 0; int nsum = 0;
      for(int j=0; j<icomb.n(); j++)
        {
          
          if(h1WEfinal[j]->GetBinContent(i) != 0)
            {
              avg += h1WEfinal[j]->GetBinContent(i);
              avg_err += h1WEfinal[j]->GetBinError(i) * h1WEfinal[j]->GetBinError(i);
              nsum++;
            }                                                                    
        }
      if (nsum) { avg /= nsum; avg_err = sqrt(avg_err) / nsum; }
      havg->SetBinContent(i, avg);
      havg->SetBinError(i, avg_err);
    }
  xjjroot::setthgrstyle(havg, kBlack, 21, 0.8, kBlack, 1, 1,
                        -1, -1, -1, 0.7, 0.7);

  TH1F* hsym = gethsym(havg);
  xjjroot::setthgrstyle(hsym, kBlack, 21, 0.8, kBlack);
  TH1F *hhigh = gethhigh(h1WEfinal),
    *hlow = gethlow(h1WEfinal);
  TH1F *hsymhigh = gethsymhigh(hhigh),
    *hsymlow = gethsymlow(hlow);
  TH1F *herr = getherr(hsym, hsymhigh, hsymlow),
    *hrelerr = gethrelerr(hsym, hsymhigh, hsymlow);

  auto hempty = makehempty(havg, ";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}", 1.7);
  hempty->SetAxisRange(-3.2, 3.2, "X");
  auto hemptyerr = makehempty(hrelerr, ";#it{#eta};relative error");
  hemptyerr->SetAxisRange(-3.2, 3.2, "X");
  
  float xleg = 0.55, yleg = 0.47;
  auto legPIX = new TLegend(0.3, yleg-0.031*h1WEfinal.size(), 0.3+0.2, yleg);
  xjjroot::setleg(legPIX, 0.028);
  for(int j=0; j<icomb.n(); j++)
    {
      auto tleg = Form("%s", tcomb(icomb.value[j][0]).c_str());
      legPIX->AddEntry(h1WEfinal[j], tleg, "p");
      if(std::string(tleg).size() > 20)
        { xleg = 0.31; yleg = 0.47-0.033*h1WEfinal.size(); }
    }
  auto legOUT = new TLegend(0.5, 0.47-0.031, 0.5+0.2, 0.47);
  xjjroot::setleg(legOUT, 0.028);
  legOUT->AddEntry(hsymhigh, "Combination deviation", "l");
  xjjroot::setgstyle(1);

#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  xjjroot::drawtex(0.88, 0.82, tcent(tag).c_str(), 0.030, 31);          \
  xjjroot::drawCMSleft("Internal", 0.05, -0.1);                         \
  xjjroot::drawCMSright("PbPb (5.36 TeV)");                             \
  
  xjjroot::mypdf pdf("figspdf/avg/"+tag+".pdf", "c", 600, 600);
  // havg
  pdf.prepare();
  hempty->Draw("axis");
  if(input_truth != "null") {
    for(auto& g : gh1WGhadron)
      g->Draw("c same");
    legTRUTH->Draw();
  }
  for(auto& h : h1WEfinal)
    h->Draw("p same");
  havg->Draw("p same");
  legPIX->Draw();
  DRAWTEX;
  pdf.write("figs/avg/"+tag+".pdf");

  // hsym
  pdf.prepare();
  hempty->Draw("axis");
  hsymlow->Draw("hist same");
  hsymhigh->Draw("hist same");
  hsym->Draw("p same");
  legOUT->Draw();
  DRAWTEX;
  pdf.write("figs/avg/"+tag+"-hsym.pdf");

  // hrelerr
  pdf.prepare();
  hemptyerr->Draw("axis");
  hrelerr->Draw("pe same");
  xjjroot::drawtex(0.88, 0.82-0.033, "deviation from average", 0.030, 31);
  DRAWTEX;  
  pdf.write("figs/avg/"+tag+"-hrelerr.png");

  pdf.close();

  auto outf = new TFile(Form("output/avg-%s.root", tag.c_str()), "recreate");
  xjjroot::writehist(havg);
  xjjroot::writehist(hhigh);
  xjjroot::writehist(hlow);
  xjjroot::writehist(hsym);
  xjjroot::writehist(hsymhigh);
  xjjroot::writehist(hsymlow);
  xjjroot::writehist(herr);
  xjjroot::writehist(hrelerr);
  for(auto& h : h1WEfinal)
    xjjroot::writehist(h);
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==7) return macro(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
  if(argc==5) return macro(argv[1], argv[2], argv[3], argv[4]);
  if(argc==4) return macro(argv[1], argv[2], argv[3]);
  if(argc==3) return macro(argv[1], argv[2]);
  if(argc==2) return macro(argv[1]);
  return 1;
}

void setzero(TH1D* h, std::string comb)
{
  if(idx.find(comb) != idx.end()) {
    for(int i=0; i<h->GetXaxis()->GetNbins(); i++) {
      if(good[idx[comb]][i] == 0) {
        h->SetBinContent(i+1, 0);
        h->SetBinError(i+1, 0);
      }
    }
  }
}
