#include <TFile.h>
#include "include/xjjanauti.h"
#include "include/xjjmypdf.h"
#include "include/vz.h"

void draw(const std::vector<TH1D*>& ht, std::string outputname) {
  xjjroot::setgstyle(0, 1, 1);
  auto* pdf = new xjjroot::mypdf("figpdf/" + outputname + ".pdf");
  for (int i=0; i<ht.size(); i++) {
    auto h = ht[i];
    auto ismc = !xjjc::str_contains(h->GetName(), "f0");
    xjjroot::sethempty(h);
    h->SetMinimum(0);
    h->SetMaximum(h->GetMaximum()*1.2);
    auto* f = truncated_gaussian("f");
    if (ismc) {
      f->FixParameter(3, 0);
      auto cc = (i?xjjroot::colorlist_dark[i]:kBlack);
      xjjroot::setthgrstyle(h, cc, 25, 0.5,
                            cc, 1, 1);
    }
    xjjroot::setlinestyle(f, h->GetLineColor(), 1, 2);
    pdf->prepare();
    h->Draw("histe");
    auto opt = (i>1?"QN":"N");
    h->Fit(f, opt, "", -15, 15);
    xjjana::drawreldev(h, f);
    f->Draw("same");
    xjjroot::drawCMS((ismc?xjjroot::CMS::simulation:xjjroot::CMS::preliminary),
                     "OO (5.36 TeV)");
    pdf->write();
    delete f;
  }

  pdf->close();
}

int macro(std::string input) {
  auto* inf = TFile::Open(input.c_str());
  auto ht12vzunw = xjjana::get_regexp<TH1D>(inf, "ht12f[0-9]*vz-unw");
  draw(ht12vzunw, "ht12vzunw");
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    return macro(argv[1]);
  }
  return 1;
}
