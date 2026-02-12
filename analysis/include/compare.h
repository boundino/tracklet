#ifndef __XJJROOT_COMPARE_H_
#define __XJJROOT_COMPARE_H_

#include <TFile.h>
#include <TColor.h>
#include <string>
#include <vector>

#include <TH1.h>
#include <TH2.h>

#include "xjjcuti.h"
#include "xjjrootuti.h"

namespace xjjroot
{
  enum Opt { obj, type, name };
  std::vector<std::string> opt_default = {"", "TH1F", ""};

  class compare
  {
  public:
    compare(std::string input="inf::hist#,inf::hist#");
    std::vector<TH1*> vh;
    int n() { return vh.size(); }
    void setcolor(std::vector<Color_t> cc = {kBlack, kAzure-3, kRed-3, kGreen+2, kOrange-3, kBlue-5, kCyan-2, kMagenta-5, kYellow+2, kPink+2})
    { for(int i=0; i<vh.size(); i++) { vh[i]->SetLineColor(cc[i%cc.size()]); vh[i]->SetMarkerColor(cc[i%cc.size()]); } }
    void draw(std::vector<const char*> dd = {"pe", "histe", "histe", "histe", "histe", "histe", "histe", "histe", "histe", "histe"});

  private:
    std::vector<std::vector<std::string>> opt;
  };
}

xjjroot::compare::compare(std::string input)
{
  auto inputs = xjjc::str_divide(input, ",");

  for(auto& ii : inputs)
    {
      auto thisopt = opt_default;
      auto pars = xjjc::str_divide(ii, "#");
      for(int i=0; i<pars.size(); i++)
        if(pars[i] != "") 
          thisopt[i] = pars[i];
      opt.push_back(thisopt);

      // pars[0]
      TH1* hh = 0;
      if(thisopt[type]=="TH1D")
        hh = xjjroot::gethist<TH1D>(pars[0]);
      else if(thisopt[type]=="TH1F")
        hh = xjjroot::gethist<TH1F>(pars[0]);
      else if(thisopt[type]=="TH2D")
        hh = xjjroot::gethist<TH2D>(pars[0]);
      else if(thisopt[type]=="TH2F")
        hh = xjjroot::gethist<TH2F>(pars[0]);
      vh.push_back(hh);
      // auto hh = vh[vh.size()-1];
      if(thisopt[name] != "") hh->SetName(thisopt[name].c_str());
      hh->SetMarkerStyle(21);
      hh->SetMarkerSize(0.6);
      hh->SetLineWidth(1);
      setcolor();
    }
}

void xjjroot::compare::draw(std::vector<const char*> dd)
{
  for(int i=0; i<vh.size(); i++)
    {
      if(xjjc::str_contains(dd[i%dd.size()], "hist")) 
        vh[i]->SetMarkerSize(0);
      vh[i]->Draw(Form("%s same", dd[i%dd.size()]));
    }
}

#endif
