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
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }
};
std::map<std::string, int> idx = {{"12", 0}, {"13", 1}, {"14", 2},
                                  {"23", 3}, {"24", 4}, {"34", 5},
                                  {"56", 6}, {"57", 7}, {"67", 8}};
int macro(std::string input_corr,
          std::string text = "Run 362294 corr w/ EPOS",
          std::string input_comb = "12,13,14,23,24,34,56,57,67",
          std::string input_truth = "null",
          std::string div = "&")
{
  // xjjroot::silence();
  
  auto tag = input_corr;
  xjjc::sconfig icomb(input_comb), itext(text, ",", div);
  auto tsample = xjjc::str_eraseall(xjjc::str_erasestar(tag, ".*"), "CLOSE");
  auto gtruth = new combgh1WGhadron_multfiles(input_truth, div);
  auto isclose = xjjc::str_contains(tag, "CLOSE") && gtruth->goodkey(tsample);

  auto cc = colours;
  auto ms = xjjroot::markerlist_open;

  std::vector<TH1D*> h1WEfinal(icomb.n(), 0), h1WEratio(icomb.n(), 0);
  for(int j=0; j<icomb.n(); j++)
    {
      h1WEfinal[j] = xjjroot::gethist<TH1D>("output/correction-"+input_corr+"-"+icomb.value[j][0]+".root::h1WEfinal");
      h1WEfinal[j]->SetName(Form("%s-%s", h1WEfinal[j]->GetName(), icomb.value[j][0].c_str()));
      setzero(h1WEfinal[j], icomb.value[j][0]);
      goodresult(h1WEfinal[j]);
      xjjroot::setthgrstyle(h1WEfinal[j], cc[j], ms[j], 0.8, cc[j]);

      if (isclose) {
        h1WEratio[j] = gtruth->closeratio(h1WEfinal[j], tsample);
      }
    }

  auto havg = (TH1D*)h1WEfinal[0]->Clone("havg");
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
                        -1, -1, -1, 0.5, 0.5);
  
  TH1D* havgratio = 0;
  if (isclose) {
    havgratio = gtruth->closeratio(havg, tsample);
  }

  auto hsym = gethsym(havg);
  xjjroot::setthgrstyle(hsym, kBlack, 21, 0.8, kBlack);
  auto hhigh = gethhigh(h1WEfinal),
    hlow = gethlow(h1WEfinal);
  auto hsymhigh = gethsymhigh(hhigh),
    hsymlow = gethsymlow(hlow);
  auto herr = getherr(hsym, hsymhigh, hsymlow),
    hrelerr = gethrelerr(hsym, hsymhigh, hsymlow);

  auto hempty = makehempty(gtruth->gethmax(), _t_dNdetatitle.c_str(), 1.3, 0, 10);
  hempty->SetAxisRange(-3.2, 3.2, "X");
  if (isclose) {
    hempty->SetMinimum(0.01);
  }
  auto hemptyerr = makehempty(hrelerr, ";#it{#eta};Relative error", 1.3);
  hemptyerr->SetAxisRange(-3.2, 3.2, "X");
  auto hemptyratio = (TH1F*)hempty->Clone("hemptyratio");
  hemptyratio->SetTitle(";#it{#eta};Ratio");
  hemptyratio->SetAxisRange(-3.2, 3.2, "X");
  hemptyratio->SetAxisRange(0.9, 1.099, "Y");
  
  float xleg = 0.55, yleg0 = isclose?0.4:0.47, yleg=yleg0, txtleg = 0.028*(isclose?1.25:1.);
  auto legPIX = new TLegend(0.3, yleg-txtleg*1.1*h1WEfinal.size(), 0.3+0.2, yleg);
  xjjroot::setleg(legPIX, txtleg);
  gtruth->remakeleg(-1, yleg, txtleg);
  for(int j=0; j<icomb.n(); j++) {
    auto tleg = tcomb(icomb.value[j][0]);
    legPIX->AddEntry(h1WEfinal[j], tleg.c_str(), "p");
    if(std::string(tleg).size() > 20)
      { xleg = 0.31; yleg = 0.47-0.034*h1WEfinal.size(); }
  }
  float ylegavg = isclose?gtruth->leg_down():yleg;
  auto legAVG = new TLegend(gtruth->leg_left(), ylegavg-txtleg*1.1, gtruth->leg_right(), ylegavg);
  xjjroot::setleg(legAVG, txtleg);
  legAVG->AddEntry(havg, (isclose?"Corrected reco":"Averaged"), "p");
  auto legOUT = new TLegend(0.5, 0.47-0.031, 0.5+0.2, 0.47);
  xjjroot::setleg(legOUT, 0.028);
  legOUT->AddEntry(hsymhigh, "Combination deviation", "l");
  xjjroot::setgstyle(1);
  

#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.23, 0.79-i*0.034, itext.value[i][0].c_str(), 0.038, 13); } \
  xjjroot::drawtex(0.89, 0.81, tcent(tag).c_str(), 0.038, 31);          \
  watermark_inner(isclose);                                             \

  TPad *p1 = 0, *p2 = 0;
  xjjroot::mypdf pdf("figspdf/avg/"+tag+".pdf", "c", 600, 600);
  // havg only
  if (isclose) {
    pdf.prepare();
    xjjroot::twopads(pdf.getc(), p1, p2, hempty, hemptyratio, 0.7);
    p1->cd();
    gtruth->draw();
    havg->Draw("p same");
    legAVG->Draw();
    DRAWTEX;
    p2->cd();
    xjjroot::drawline(-3.2, 1, 3.2, 1, kBlack, 6, 1);
    havgratio->Draw("p same");
    pdf.getc()->cd();
    pdf.write("figs/avg/"+tag+"-havg.pdf");  
  }
  
  // h1WEfinal
  pdf.prepare();
  if (isclose) {
    xjjroot::twopads(pdf.getc(), p1, p2, hempty, hemptyratio, 0.7);
    p1->cd();
  } else {
    hempty->Draw("axis");
  }
  gtruth->draw();
  for(auto& h : h1WEfinal)
    h->Draw("p same");
  legPIX->Draw();
  DRAWTEX;
  if (isclose) {
    p2->cd();
    xjjroot::drawline(-3.2, 1, 3.2, 1, kBlack, 6, 1);
    for(auto& h : h1WEratio) {
      h->Draw("p same");
    }
    pdf.getc()->cd();
  }
  pdf.write("figs/avg/"+tag+"-h1WEfinal.pdf");

  // havg
  pdf.prepare();
  if (isclose) {
    xjjroot::twopads(pdf.getc(), p1, p2, hempty, hemptyratio, 0.7);
    p1->cd();
  } else {
    hempty->Draw("axis");
  }
  gtruth->draw();
  for(auto& h : h1WEfinal)
    h->Draw("p same");
  havg->Draw("p same");
  legPIX->Draw();
  legAVG->Draw();
  DRAWTEX;
  if (isclose) {
    p2->cd();
    xjjroot::drawline(-3.2, 1, 3.2, 1, kBlack, 6, 1);
    for(auto& h : h1WEratio) {
      h->Draw("p same");
    }
    havgratio->Draw("p same");
    pdf.getc()->cd();
  }
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
  xjjroot::drawtex(0.23, 0.79-itext.n()*0.034, "Deviation from average", 0.038, 13);
  DRAWTEX;  
  pdf.write("figs/avg/"+tag+"-hrelerr.pdf");

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
  for(auto& g : gtruth->gh1())
    g.second->Write();
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==6) return macro(argv[1], argv[2], argv[3], argv[4], argv[5]);
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
