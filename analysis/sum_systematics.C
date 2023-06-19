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
  TH1F* hsym = xjjroot::gethist<TH1F>(iavg.value[0][0]+".root::hsym");
  std::vector<float> relerr2(hsym->GetNbinsX(), 0);
  std::vector<TH1F*> hrelerr(iavg.n(), 0);
  for(int j=0; j<iavg.n(); j++) {
    hrelerr[j] = xjjroot::gethist<TH1F>(iavg.value[j][0]+".root::hrelerr");
    hrelerr[j]->SetName(Form("%s-%i", hrelerr[j]->GetName(), j));
    xjjroot::setthgrstyle(hrelerr[j], cc[j], ms[j], 1.0, cc[j]);

    for(int i=0; i<hsym->GetNbinsX(); i++) {
      relerr2[i] += hrelerr[j]->GetBinContent(i+1) * hrelerr[j]->GetBinContent(i+1);
    }
  }
  auto hsyst = (TH1F*)hsym->Clone("hsyst"), hrelerrtotal = (TH1F*)hrelerr[0]->Clone("hrelerrtotal");
  for(int i=0; i<hsym->GetNbinsX(); i++) {
    relerr2[i] = std::sqrt(relerr2[i]);
    hsyst->SetBinError(i+1, relerr2[i]*hsyst->GetBinContent(i+1));
    hrelerrtotal->SetBinContent(i+1, relerr2[i]);
  }
  xjjroot::setthgrstyle(hrelerrtotal, kBlack, 21, 1.0, kBlack);

  auto hempty = makehempty(hsym, ";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}", 1.7);
  hempty->SetAxisRange(-3.2, 3.4, "X");  
  auto hemptyrelerr = makehempty(hrelerrtotal, ";#it{#eta};relative error");
  hemptyrelerr->SetAxisRange(-3.2, 3.4, "X");
  
  float xleg = 0.42, yleg = 0.72;
  auto legPIX = new TLegend(xleg, yleg-0.031*iavg.n(), xleg+0.2, yleg);
  xjjroot::setleg(legPIX, 0.028);
  for(int j=0; j<iavg.n(); j++)
    legPIX->AddEntry(hrelerr[j], iavg.value[j][1].c_str(), "p");

  xjjroot::setgstyle(1);

#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  xjjroot::drawtex(0.88, 0.82, tcent(tag).c_str(), 0.030, 31);          \
  xjjroot::drawCMSleft("Internal", 0.05, -0.1);                         \
  xjjroot::drawCMSright("PbPb (5.36 TeV)");                             \
  
  xjjroot::mypdf pdf("figspdf/comp/syst-"+tag+".pdf", "c", 600, 600);

  // hrelerrtotal
  pdf.prepare();
  hemptyrelerr->Draw("axis");
  for(auto& hh : hrelerr) hh->Draw("phist same");
  hrelerrtotal->Draw("hist same");
  DRAWTEX;
  legPIX->Draw();
  pdf.write("figs/comp/syst-"+tag+"-hrelerrtotal.pdf");
  
  // // havg
  // pdf.prepare();
  // hempty->Draw("axis");
  // for(auto& h : havg)
  //   h->Draw("p same");
  // legPIX->Draw();

  // DRAWTEX;

  // pdf.write("figs/comp/syst-"+tag+".pdf");

  // // hsym
  // pdf.prepare();
  // hempty->Draw("axis");
  // for(auto& h : hsym)
  //   h->Draw("p same");
  // legPIX->Draw();

  // DRAWTEX;
  
  // pdf.write("figs/comp/"+tag+"-hsym.pdf");

  pdf.close();

  // auto outf = xjjroot::newfile("results/comp-"+tag+".root");
  // xjjroot::writehist(herr);
  // xjjroot::writehist(hrelerr);
  // xjjroot::writehist(hhigh);
  // xjjroot::writehist(hlow);
  // for(auto h : hsym) xjjroot::writehist(h);
  // outf->Close();
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==6) return macro(argv[1], argv[2], argv[3], argv[4], argv[5]);
  if(argc==4) return macro(argv[1], argv[2], argv[3]);
  if(argc==3) return macro(argv[1], argv[2]);
  return 1;
}

