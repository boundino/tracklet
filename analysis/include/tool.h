#ifndef __INCLUDE_TOOL_H_
#define __INCLUDE_TOOL_H_

#include "xjjcuti.h"
#include "xjjanauti.h"
#include "sconfig.h"
#include "cosmetics.h"

std::string _t_slash = "#kern[0.1]{#lower[0.15]{#scale[1.25]{/}}}";
std::string _t_dNdeta = "#scale[1.2]{#LT}d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}#scale[1.2]{#GT}";
std::string _t_eta0p5 = "#lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}}#lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#scale[0.5]{ }#kern[0.2]{<}#kern[0.2]{0.5}}}";
std::string _t_2a = "#lower[-0.05]{(}1/#kern[0.05]{2#it{A}}#lower[-0.05]{)}";
std::string _t_1npart = "#lower[-0.05]{(}1/#kern[0.1]{#scale[1.2]{#LT}}#lower[0.1]{#it{N}}#lower[0.5]{#scale[0.6]{#kern[-0.08]{part}}}#scale[1.2]{#GT}#lower[-0.05]{)}";
std::string _t_npart = "#scale[1.2]{#LT}#it{N}#lower[0.4]{#scale[0.7]{#kern[-0.05]{part}}}#scale[1.2]{#GT}";
std::string _t_hep_dNdeta = "\\mathrm{d}N_{\\mathrm{ch}}/\\mathrm{d}\\eta";

std::string _t_dNdetatitle = ";#it{#eta};d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}";

// std::string _t_truth = "hijing&H#scale[0.9]{IJING}&2,amptnm&A#scale[0.9]{MPT}&6,amptnm2&A#scale[0.9]{MPT}#scale[0.5]{ }#scale[0.9]{(varied params)}&4,hydjet&H#scale[0.8]{YDJET}&9";
std::string _t_truth = "hijing&H#scale[0.9]{IJING}&2,amptnm&A#scale[0.9]{MPT}&6,amptsm&A#scale[0.9]{MPT}#scale[0.5]{ }#scale[0.9]{(string melting)}&4,hydjet&H#scale[0.9]{YDJET}&9,angantyr&A#scale[0.9]{NGANTYR}&8,epos&E#scale[0.9]{POS}#scale[0.5]{ }#scale[0.8]{LHC}&10";

static const int goodrange[] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
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
  r.insert(1, "#scale[0.5]{ }#otimes ");
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

TH1D* gethsym(TH1* havg, const char* name="hsym") {
  TH1D* hsym = (TH1D*)havg->Clone(name);
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

TH1D* gethsymhigh(TH1* havg, const char* name="hsymhigh") {
  TH1D* hsymhigh = (TH1D*)havg->Clone(name);
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

TH1D* gethsymlow(TH1* havg, const char* name="hsymlow") {
  TH1D* hsymlow = (TH1D*)havg->Clone(name);
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

TH1D* gethhigh(std::vector<TH1D*> h1WEfinal)
{
  TH1D* hhigh = (TH1D*)h1WEfinal[0]->Clone("hhigh");
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

TH1D* gethlow(std::vector<TH1D*> h1WEfinal)
{
  TH1D* hlow = (TH1D*)h1WEfinal[0]->Clone("hlow");
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

TH1D* getherr(TH1* havg, TH1D* hhigh, TH1D* hlow) {
  TH1D* herr = (TH1D*)havg->Clone("herr");
  herr->SetTitle(_t_dNdetatitle.c_str());
  for(int i=1; i<=havg->GetNbinsX(); i++) {
    float err = std::max(fabs(hhigh->GetBinContent(i)-havg->GetBinContent(i)),
                         fabs(havg->GetBinContent(i)-hlow->GetBinContent(i)));
    herr->SetBinError(i, err);
  }
  return herr;
}

TH1D* gethrelerr(TH1* havg, TH1D* hhigh, TH1D* hlow) {
  TH1D* hrelerr = (TH1D*)havg->Clone("hrelerr");
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
T* makehempty(T* horg, std::string title=_t_dNdetatitle.c_str(), float ymax=1.5, float ymin=0, int hmaxdiv = 0) {
  auto hempty = (T*)horg->Clone(Form("hempty-%s", xjjc::currenttime().c_str()));
  xjjroot::sethempty(hempty);
  if(title != "")
    hempty->SetTitle(title.c_str());
  hempty->SetMinimum(xjjana::gethnonzerominimum(horg)*ymin);
  float maxy = xjjana::gethmaximum(horg)*ymax;
  if (hmaxdiv > 0) {
    maxy = std::ceil(maxy/hmaxdiv) * hmaxdiv;
  }
  hempty->SetMaximum(maxy);
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
                                           TLegend* &legTRUTH,
                                           std::string input_truth="") {
  xjjc::sconfig itruth(input_truth, ",", "&", "v");
  if(!legTRUTH) {
    legTRUTH = new TLegend(0.55, 0.42-0.031*itruth.n(), 0.55+0.2, 0.42);
    xjjroot::setleg(legTRUTH, 0.028);
  }
  std::vector<TGraphErrors*> gh1WGhadron;
  gStyle->SetLineStyleString(9,"40 20");
  for(int i=0; i<itruth.n(); i++) {
    std::cout<<filename<<std::endl;
    auto gr = xjjroot::gethist<TGraphErrors>(filename + "::gh1WGhadron-"+itruth.value[i][0]);
    if(!gr) continue;
    if (itruth.value[i].size() > 2)
      gr->SetLineStyle(atoi(itruth.value[i][2].c_str()));
    gh1WGhadron.push_back(gr);
    legTRUTH->AddEntry(gr, Form("%s", itruth.value[i][1].c_str()), "l");
  }
  return gh1WGhadron;
}

class combgh1WGhadron_multfiles {
public:
  unsigned int n() { return n_; }
  float leg_up() { return yleg_; }
  float leg_down() { return yleg_-n_*1.1*tleg_; }
  float leg_left() { return xleg_; }
  float leg_right() { return xleg_+0.2; }
  std::vector<std::pair<std::string, TGraphErrors*>> gh1() { return gh1_; }
  std::vector<std::pair<std::string, TH1D*>> h1() { return h1_; }
  bool valid() { return gh1_.size()>0; }
  combgh1WGhadron_multfiles(std::string input_truth,
                            std::string div = "&") :
    leg_(0), xleg_(0.55), yleg_(0.47), tleg_(0.028) {
    if(input_truth != "null") {
      xjjc::sconfig itruth(input_truth, ",", div, "v");
      n_ = 0;
      for(int i=0; i<itruth.n(); i++) {
        auto key = itruth.value[i][0];
        auto h = xjjroot::gethist<TH1D>("output/correction-"+key+"-12.root::h1WGhadron");
        if (!h) continue;
        h1_.push_back({key, h});
        auto gh = xjjana::shifthistcenter(h, "gh1WGhadron-"+xjjc::str_erasestar(key, ".*"), 0x10);
        xjjroot::setthgrstyle(gh, kBlack, 20, 0.8, kBlack, atoi(itruth.value[i][2].c_str()), 1);
        gh1_.push_back({key, gh});
        latex_[key] = itruth.value[i][1];

        n_++;
      }
      remakeleg();
    }
  }

  void remakeleg(float xleg = -1, float yleg = -1, float tleg = 0.028) {
    if (xleg >= 0) xleg_ = xleg;
    if (yleg >= 0) yleg_ = yleg;
    if (tleg >= 0) tleg_ = tleg;
    if (leg_) delete leg_;
    leg_ = new TLegend(xleg_, yleg_-tleg_*1.1*n_, xleg_+0.2, yleg_);
    xjjroot::setleg(leg_, tleg_);
    for (auto& g : gh1_) {
      leg_->AddEntry(g.second,
                     Form("%s", latex_[g.first].c_str()),
                     "l");
    }
  }
  
  void draw() {
    if (valid()) {
      for(auto& g : gh1_)
        g.second->Draw("cX same");
      leg_->Draw();
    }
  }

  bool goodkey(std::string tag) {
    for (auto& g : gh1_) {
      if (xjjc::str_contains(g.first, tag) ||
          xjjc::str_contains(tag, g.first)) return true;
    }
    std::cout<<"\e[31merror\e[0m ("<<__PRETTY_FUNCTION__<<"): bad tag "<<tag<<std::endl;
    return false;
  }
  
  TH1D* geth(std::string tag) {
    TH1D* h = 0;
    if (goodkey(tag)) {
      for (auto& hh : h1_) {
        if (xjjc::str_contains(hh.first, tag) ||
            xjjc::str_contains(tag, hh.first))
          h = hh.second;
      }
    }
    return h;
  }

  TH1D* gethmax() {
    float ymax = 0;
    TH1D* hmax = 0;
    for (auto& hh : h1_) {
      if (hh.second->GetMaximum() > ymax) { 
        ymax = hh.second->GetMaximum();
        hmax = hh.second;
      }
    }
    return hmax;
  }

  template<class T>
  T* closeratio(T* h1WEfinal, std::string tag) {
    auto hratio = (T*)h1WEfinal->Clone(Form("%s_ratio", h1WEfinal->GetName()));
    auto htruth = geth(tag);
    if (htruth) {
      hratio->Divide(htruth);
    } else {
      delete hratio;
      hratio = 0;
    }
    return hratio;
  }
  
private:
  std::vector<std::pair<std::string, TGraphErrors*>> gh1_; 
  std::vector<std::pair<std::string, TH1D*>> h1_;
  std::map<std::string, std::string> latex_;
  float xleg_, yleg_, tleg_;
  TLegend* leg_;
  unsigned int n_;
};

// std::vector<TGraphErrors*> combgh1WGhadron_multfiles::combgh1WGhadron_multfiles(std::string input_truth,
//                                                                                 TLegend* &legTRUTH,
//                                                                                 std::string div = "&") {
//   std::vector<TGraphErrors*> gh1WGhadron;
//   if(input_truth != "null") {
//     xjjc::sconfig itruth(input_truth, ",", div, "v");
//     if(!legTRUTH) {
//       legTRUTH = new TLegend(0.55, 0.47-0.031*itruth.n(), 0.55+0.2, 0.47);
//       xjjroot::setleg(legTRUTH, 0.028);
//     }
//     for(int i=0; i<itruth.n(); i++) {
//       auto h = xjjroot::gethist<TH1D>("output/correction-"+itruth.value[i][0]+"-12.root::h1WGhadron");
//       if (!h) continue;
//       auto gh = xjjana::shifthistcenter(h, "gh1WGhadron-"+xjjc::str_erasestar(itruth.value[i][0], ".*"), 0x10);
//       xjjroot::setthgrstyle(gh, kBlack, 20, 0.8, kBlack, atoi(itruth.value[i][2].c_str()), 1);
//       legTRUTH->AddEntry(gh,
//                          Form("%s", itruth.value[i][1].c_str()),
//                          "l");
//       gh1WGhadron.push_back(gh);
//     }
//   }
//   return gh1WGhadron;
// }


#endif
