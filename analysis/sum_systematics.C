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
          std::string text = "Run 362294 corr w/ EPOS")
{
  std::string div = "&";
  xjjroot::silence();
  xjjc::sconfig iavg(input_avg, ",", div), itext(text, ",", div);

  std::vector<Color_t> cc = xjjroot::colorlist_light;
  TH1F* hsym = xjjroot::gethist<TH1F>(iavg.value[0][0]+".root::hsym");
  auto legPIX = new TLegend(0.3, 0.45-0.030*9, 0.3+0.2, 0.45);
  xjjroot::setleg(legPIX, 0.028);
  auto h1WEfinal = combh1WEfinal(iavg.value[0][0]+".root", legPIX);
  auto legTRUTH = new TLegend(0.55, 0.45-0.030*5, 0.55+0.2, 0.45-0.030);
  xjjroot::setleg(legTRUTH, 0.028);
  auto gh1WGhadron = combgh1WGhadron(iavg.value[0][0]+".root", legTRUTH);

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
  // xjjroot::printhistvalue(hsyst);
  xjjroot::setthgrstyle(hrelerrtotal, kBlack, 21, 1.0, kBlack);
  TGraphErrors* gsyst = xjjana::shifthistcenter(hsyst, "gsyst");
  xjjroot::setthgrstyle(gsyst, kGray+3, 21, 0.8, 0, 0, 0, kGray+3, 0.3, 1001, 1, 1);
  xjjroot::setthgrstyle(hsym, kGray+3, 21, 0.8, 0, 0, 0, kGray+3, 0.3, 1001, 1, 1);

  auto legDATA = new TLegend(0.55, 0.45-0.030*1, 0.55+0.2, 0.45);
  xjjroot::setleg(legDATA, 0.028);
  legDATA->AddEntry(gsyst, "averaged & symmetrised", "pf");
  
  auto hempty = makehempty(hsym, ";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}", 1.7);
  hempty->SetAxisRange(-3.2, 3.4, "X");  
  auto hemptyrelerr = makehempty(hrelerrtotal, ";#it{#eta};relative error");
  hemptyrelerr->SetAxisRange(-3.2, 3.4, "X");
  
  float xleg = 0.42, yleg = 0.72;
  auto legERR = new TLegend(xleg, yleg-0.030*iavg.n(), xleg+0.2, yleg);
  xjjroot::setleg(legERR, 0.028);
  for(int j=0; j<iavg.n(); j++)
    legERR->AddEntry(hrelerr[j], iavg.value[j][1].c_str(), "p");
  
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
  legERR->Draw();
  pdf.write("figs/comp/syst-"+tag+"-hrelerrtotal.pdf");
  
  // havg
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& hh : gh1WGhadron)
    hh->Draw("c same");
  gsyst->Draw("2 same");
  for(auto& hh : h1WEfinal)
    hh->Draw("p same");
  hsym->Draw("p same");
  // gsyst->Draw("Y0pX same");
  legPIX->Draw();
  legTRUTH->Draw();
  legDATA->Draw();
  DRAWTEX;
  pdf.write("figs/comp/syst-"+tag+"-hsyst.pdf");

  // havg
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& hh : gh1WGhadron)
    hh->Draw("c same");
  gsyst->Draw("2 same");
  hsym->Draw("p same");
  // gsyst->Draw("Y0pX same");
  legTRUTH->Draw();
  legDATA->Draw();
  DRAWTEX;
  pdf.write("figs/comp/syst-"+tag+"-hclean.pdf");

  pdf.close();

  auto outf = xjjroot::newfile("results/results-"+tag+".root");
  xjjroot::writehist(hsym);
  xjjroot::writehist(hsyst);
  gsyst->Write();
  xjjroot::writehist(hrelerrtotal);
  for(auto h : h1WEfinal) xjjroot::writehist(h);
  for(auto g : gh1WGhadron) g->Write();
  outf->Close();
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==4) return macro(argv[1], argv[2], argv[3]);
  if(argc==3) return macro(argv[1], argv[2]);
  return 1;
}

