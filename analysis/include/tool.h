#ifndef __INCLUDE_TOOL_H_
#define __INCLUDE_TOOL_H_

#include "xjjcuti.h"
#include "xjjanauti.h"
#include "sconfig.h"
#include "cosmetics.h"

static const int goodrange[] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
void goodresult(TH1* h, const int* goodbin = goodrange) {
  for(int i=0; i<h->GetXaxis()->GetNbins(); i++) {
    if(goodbin[i] == 0) {
      h->SetBinContent(i+1, 0);
      h->SetBinError(i+1, 0);
    }
  }
}

std::string tcomb(std::string comb) {
  std::string r(comb);
  r.insert(1, " #otimes ");
  r = Form("%s#scale[0.8]{PIX} %s", (atoi(comb.c_str())>50?"F":"B"), r.c_str());
  return r;
}

std::string tcent(std::string tag) {
  
  if(!xjjc::str_contains(tag, ".s.")) return "";
  tag = xjjc::str_erasestar(tag, "*.s.");
  auto v = xjjc::str_divide(tag, ".");
  if(v.size() < 2) return "";
  float cmax = 5*(20-atof(v[0].c_str())),
    cmin = 5*(20-atof(v[1].c_str()));
  return Form("Cent. %.0f-%.0f%s", cmin, cmax, "%");
}

TH1F* gethsym(TH1* havg, const char* name="hsym") {
  TH1F* hsym = (TH1F*)havg->Clone(name);
  int nbins = hsym->GetNbinsX();
  for (int i=1; i<=nbins; i++) {
    if (havg->GetBinContent(i) != 0 && havg->GetBinContent(nbins - i + 1) != 0) {
      hsym->SetBinContent(i, (havg->GetBinContent(i) + havg->GetBinContent(nbins - i + 1)) / 2);
      hsym->SetBinError(i, (havg->GetBinError(i) + havg->GetBinError(nbins - i + 1)) / 2);
    }
    // else {
    //   hsym->SetBinContent(i, havg->GetBinContent(i) + havg->GetBinContent(nbins - i + 1));
    //   hsym->SetBinError(i, havg->GetBinError(i) + havg->GetBinError(nbins - i + 1));
    // }
  }
  return hsym;
}

TH1F* gethsymhigh(TH1* havg, const char* name="hsymhigh") {
  TH1F* hsymhigh = (TH1F*)havg->Clone(name);
  int nbins = hsymhigh->GetNbinsX();
  for (int i=1; i<=nbins; i++) {
    if (havg->GetBinContent(i) != 0 && havg->GetBinContent(nbins - i + 1) != 0) {
      int bin = havg->GetBinContent(i)>havg->GetBinContent(nbins - i + 1)?i:(nbins - i + 1);
      hsymhigh->SetBinContent(i, havg->GetBinContent(bin));
      hsymhigh->SetBinError(i, havg->GetBinError(bin));
    }
    // else {
    //   hsymhigh->SetBinContent(i, havg->GetBinContent(i) + havg->GetBinContent(nbins - i + 1));
    //   hsymhigh->SetBinError(i, havg->GetBinError(i) + havg->GetBinError(nbins - i + 1));
    // }
  }
  return hsymhigh;
}

TH1F* gethsymlow(TH1* havg, const char* name="hsymlow") {
  TH1F* hsymlow = (TH1F*)havg->Clone(name);
  int nbins = hsymlow->GetNbinsX();
  for (int i=1; i<=nbins; i++) {
    if (havg->GetBinContent(i) != 0 && havg->GetBinContent(nbins - i + 1) != 0) {
      int bin = havg->GetBinContent(i)<havg->GetBinContent(nbins - i + 1)?i:(nbins - i + 1);
      hsymlow->SetBinContent(i, havg->GetBinContent(bin));
      hsymlow->SetBinError(i, havg->GetBinError(bin));
    }
  }
  return hsymlow;
}

TH1F* gethhigh(std::vector<TH1D*> h1WEfinal)
{
  TH1F* hhigh = (TH1F*)h1WEfinal[0]->Clone("hhigh");
  for(int i=1; i<=hhigh->GetNbinsX(); i++) {
    float vh = 0, vh_err = 0;
    for(int j=0; j<h1WEfinal.size(); j++) {
      if(h1WEfinal[j]->GetBinContent(i) != 0 &&
         h1WEfinal[j]->GetBinContent(i) > vh) {
        vh = h1WEfinal[j]->GetBinContent(i);
        vh_err = h1WEfinal[j]->GetBinError(i);
      }
    }
    hhigh->SetBinContent(i, vh);
    hhigh->SetBinError(i, vh_err);
  }
  xjjroot::setthgrstyle(hhigh, kRed, 21, 0.2, kRed, 1, 1);

  return hhigh;
}

TH1F* gethlow(std::vector<TH1D*> h1WEfinal)
{
  TH1F* hlow = (TH1F*)h1WEfinal[0]->Clone("hlow");
  for(int i=1; i<=hlow->GetNbinsX(); i++) {
    float vl = 1.e+10, vl_err = 0;
    for(int j=0; j<h1WEfinal.size(); j++) {
      if(h1WEfinal[j]->GetBinContent(i) != 0) {
        if(h1WEfinal[j]->GetBinContent(i) < vl) {
          vl = h1WEfinal[j]->GetBinContent(i);
          vl_err = h1WEfinal[j]->GetBinError(i);
        }
      }
    }
    if(vl > 1.e9) vl = 0;
    hlow->SetBinContent(i, vl);
    hlow->SetBinError(i, vl_err);
  }
  xjjroot::setthgrstyle(hlow, kRed, 21, 0.2, kRed, 1, 1);

  return hlow;
}

TH1F* getherr(TH1* havg, TH1F* hhigh, TH1F* hlow) {
  TH1F* herr = (TH1F*)havg->Clone("herr");
  herr->SetTitle(";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}");
  for(int i=1; i<=havg->GetNbinsX(); i++) {
    float err = std::max(fabs(hhigh->GetBinContent(i)-havg->GetBinContent(i)),
                         fabs(havg->GetBinContent(i)-hlow->GetBinContent(i)));
    herr->SetBinError(i, err);
  }
  return herr;
}

TH1F* gethrelerr(TH1* havg, TH1F* hhigh, TH1F* hlow) {
  TH1F* hrelerr = (TH1F*)havg->Clone("hrelerr");
  hrelerr->SetTitle(";#it{#eta};relative uncertainty");
  for(int i=1; i<=havg->GetNbinsX(); i++) {
    float err = std::max(fabs(hhigh->GetBinContent(i)-havg->GetBinContent(i)),
                         fabs(havg->GetBinContent(i)-hlow->GetBinContent(i)));
    hrelerr->SetBinContent(i, err);    
  }
  hrelerr->Divide(havg);
  return hrelerr;
}

template<class T>
T* makehempty(T* horg, std::string title=";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}", float ymax=1.5, float ymin=0) {
  auto hempty = (T*)horg->Clone(Form("hempty-%s", xjjc::currenttime().c_str()));
  xjjroot::sethempty(hempty);
  if(title != "")
    hempty->SetTitle(title.c_str());
  hempty->SetMinimum(xjjana::gethnonzerominimum(horg)*ymin);
  hempty->SetMaximum(xjjana::gethmaximum(horg)*ymax);
  // hempty->SetAxisRange(-3.2, 3.2, "X");
  return hempty;
}

std::vector<TH1D*> combh1WEfinal(std::string filename,
                                 TLegend* legPIX = 0,
                                 std::string input_comb="12,13,14,23,24,34,56,57,67") {
  if(!legPIX) {
    legPIX = new TLegend(0.3, 0.47-0.031*9, 0.3+0.2, 0.47);
    xjjroot::setleg(legPIX, 0.028);
  }
  xjjc::sconfig icomb(input_comb);
  std::vector<TH1D*> h1WEfinal(icomb.n(), 0);
  for(int j=0; j<icomb.n(); j++)
    {
      h1WEfinal[j] = xjjroot::gethist<TH1D>(filename + "::h1WEfinal-"+icomb.value[j][0]);
      xjjroot::setthgrstyle(h1WEfinal[j], colours[j], xjjroot::markerlist_open[j], 0.8, colours[j]);

      legPIX->AddEntry(h1WEfinal[j], Form("%s", tcomb(icomb.value[j][0]).c_str()), "p");
    }
  return h1WEfinal;
}

std::vector<TGraphErrors*> combgh1WGhadron(std::string filename,
                                           TLegend* legTRUTH = 0,
                                           // std::string input_truth="amptnm&A#scale[0.9]{MPT} #scale[0.9]{(no melting)},amptsm&A#scale[0.9]{MPT} #scale[0.9]{(string melting)},hydjet&H#scale[0.8]{YDJET},epos&E#scale[0.8]{POS} #scale[0.9]{LHC}") {
                                           std::string input_truth="amptnm&A#scale[0.9]{MPT} #scale[0.9]{(no melting)},hijing&H#scale[0.9]{IJING}") {
                                           // std::string input_truth="amptnm&A#scale[0.9]{MPT} #scale[0.9]{(no melting)}") {
  xjjc::sconfig itruth(input_truth, ",", "&", "v");
  if(!legTRUTH) {
    legTRUTH = new TLegend(0.55, 0.47-0.031*itruth.n(), 0.55+0.2, 0.47);
    xjjroot::setleg(legTRUTH, 0.028);
  }
  std::vector<TGraphErrors*> gh1WGhadron;
  gStyle->SetLineStyleString(9,"40 20");
  for(int i=0; i<itruth.n(); i++) {
    std::cout<<filename<<std::endl;
    auto gr = xjjroot::gethist<TGraphErrors>(filename + "::gh1WGhadron-"+itruth.value[i][0]);
    if(!gr) continue;
    if(xjjc::str_contains(itruth.value[i][0], "amptsm"))
      gr->SetLineStyle(9);
    gh1WGhadron.push_back(gr);
    legTRUTH->AddEntry(gr, Form("%s", itruth.value[i][1].c_str()), "l");
  }
  return gh1WGhadron;
}

#endif
