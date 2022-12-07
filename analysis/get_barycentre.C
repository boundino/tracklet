#include <TH1D.h>
#include <TCanvas.h>

#include "include/compare.h"
#include "include/xjjrootuti.h"
#include "include/xjjanauti.h"

int macro(std::string input="data/362294t.root::ht12f0vz-unw#TH1D,data/362294t.root::ht12f1vz-unw#TH1D",
          std::string tag="362294t")
{
  xjjroot::compare comp(input);
  auto ymax = xjjana::sethsmax(comp.vh);
  auto hempty = (TH1D*)comp.vh[0]->Clone("hempty");
  xjjroot::sethempty(hempty);
  hempty->SetLabelOffset();
  hempty->SetMinimum(0);
  hempty->SetMaximum(ymax*1.2);

  //
  std::vector<TF1*> fs(comp.n());
  for(int i=0; i<comp.n(); i++)
    {
      fs[i] = new TF1(Form("f%d",i), "gaus", -15, 15);
      fs[i]->SetParameters(1, 0, 5);
      comp.vh[i]->Fit(Form("f%d",i), "N");
      fs[i]->SetLineColor(comp.vh[i]->GetLineColor());
    }

  xjjroot::setgstyle(1);
  TCanvas* c = new TCanvas("c", "", 600, 600);
  hempty->Draw("AXIS");
  comp.draw();
  for(auto ff : fs) ff->Draw("same");
  xjjroot::saveas(c, "figs/tracklet/tracklet-vzshift-"+tag+".pdf");

  return 0;
}

int main()
{
  return macro();
}
