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

auto ms = xjjroot::markerlist_solid;
int macro(std::string input_avg, std::string tag,
          std::string text = "Run 362294 corr w/ EPOS",
          std::string input_truth = "null",
          std::string div = "&")
{
  xjjc::sconfig iavg(input_avg, ",", div), itext(text, ",", div);

  std::vector<Color_t> cc = xjjroot::colorlist_light;
  std::vector<TH1D*> havg(iavg.n(), 0), hsym(iavg.n(), 0);
  for(int j=0; j<iavg.n(); j++)
    {
      havg[j] = xjjroot::gethist<TH1D>("output/avg-"+iavg.value[j][0]+".root::havg");
      havg[j]->SetName(Form("%s-%i", havg[j]->GetName(), j));
      xjjroot::setthgrstyle(havg[j], cc[j], ms[j], 1.0, cc[j]);

      hsym[j] = xjjroot::gethist<TH1D>("output/avg-"+iavg.value[j][0]+".root::hsym");
      hsym[j]->SetName(Form("%s-%i", hsym[j]->GetName(), j));
      xjjroot::setthgrstyle(hsym[j], cc[j], ms[j], 1.0, cc[j]);
    }
  
  TH1F *hhigh = gethhigh(hsym), *hlow = gethlow(hsym);
  // TH1F *hsymhigh = gethsymhigh(hhigh), *hsymlow = gethsymlow(hlow);
  TH1F *herr = getherr(hsym[0], hhigh, hlow),
    *hrelerr = gethrelerr(hsym[0], hhigh, hlow);
  xjjroot::setthgrstyle(hrelerr, kGray+3, -1, -1, kGray+3);

  auto hempty = makehempty(hsym[0], ";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}", 1.7);
  hempty->SetAxisRange(-3.4, 3.4, "X");
  auto hemptyrelerr = makehempty(hrelerr, ";#it{#eta};relative error");
  hemptyrelerr->SetAxisRange(-3.4, 3.4, "X");
  
  float xleg = 0.55, yleg = 0.47;
  auto legPIX = new TLegend(0.3, yleg-0.031*iavg.n(), 0.3+0.2, yleg);
  xjjroot::setleg(legPIX, 0.028);
  for(int j=0; j<iavg.n(); j++)
    legPIX->AddEntry(havg[j], iavg.value[j][1].c_str(), "p");

  xjjroot::setgstyle(1);

#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  xjjroot::drawtex(0.88, 0.82, tcent(tag).c_str(), 0.030, 31);          \
  xjjroot::drawCMSleft(xjjroot::CMS::preliminary, 0.05, -0.1);          \
  xjjroot::drawCMSright("OO (5.36 TeV)");                               \
  
  xjjroot::mypdf pdf("figspdf/comp/"+tag+".pdf", "c", 600, 600);
  // havg
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& h : havg)
    h->Draw("p same");
  legPIX->Draw();

  DRAWTEX;

  pdf.write("figs/comp/"+tag+"-havg.pdf");

  // hsym
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& h : hsym)
    h->Draw("p same");
  legPIX->Draw();

  DRAWTEX;
  
  pdf.write("figs/comp/"+tag+"-hsym.pdf");

  // hsym
  pdf.prepare();
  hemptyrelerr->Draw("axis");
  hrelerr->Draw("pe same");

  DRAWTEX;
  
  pdf.write("figs/comp/"+tag+"-hrelerr.pdf");
  
  pdf.close();

  auto outf = xjjroot::newfile("results/comp-"+tag+".root");
  xjjroot::writehist(herr);
  xjjroot::writehist(hrelerr);
  xjjroot::writehist(hhigh);
  xjjroot::writehist(hlow);
  for(auto h : hsym) xjjroot::writehist(h);
  outf->Close();
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==6) return macro(argv[1], argv[2], argv[3], argv[4], argv[5]);
  if(argc==4) return macro(argv[1], argv[2], argv[3]);
  return 1;
}

