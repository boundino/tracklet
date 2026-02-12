/*
  qualifiers:
  - {name: RE, value: PB PB --> D0 --> K + PI}
  - {name: SQRT(S), units: GEV, value: 5020}
  - {name: $|$RAPIDITY(D)$|$, value: $<$ 1}
  - {name: CENTRALITY, units: $\%$, value: 0.0 - 100.0}
  - {name: GLOBAL UNCERTAINTY (TAA and LUMI), units: $\%$, value: -4.6 +4.1}
*/

#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include "TH1F.h"
#include <TGraphErrors.h>

namespace xjjroot {
  struct hep_nuv {
    std::string name;
    std::string units;
    std::string value;
  };
  hep_nuv make_hep_nuv(std::vector<std::string> v) {
    hep_nuv h = {v[0], v[1], v[2]};
    return h;
  }
  std::string print_hep_nuv(hep_nuv v, bool print = false) {
    std::string r = "{name: " + v.name;
    if (v.units != "")
      r += (", units: " + v.units);
    if (v.value != "")
      r += (", value: " + v.value);
    r += "}";
    if (print)
      std::cout<<r<<std::endl;
    return r;
  }

  //
  class hepdata {
  public:
    hepdata(TH1* _hstat, TGraphErrors* _hsyst,
            std::vector<std::string> _x_header, std::vector<std::string> _y_header,
            std::vector<std::vector<std::string>> _qualifiers) :
      hstat(_hstat), gstat(0), hsyst(_hsyst),
      x_header(make_hep_nuv(_x_header)), y_header(make_hep_nuv(_y_header)) {
      for(auto& q : _qualifiers) {
        qualifiers.push_back(make_hep_nuv(q));
      }
    } 

    hepdata(TGraphErrors* _gstat, TGraphErrors* _hsyst,
            std::vector<std::string> _x_header, std::vector<std::string> _y_header,
            std::vector<std::vector<std::string>> _qualifiers) :
      hstat(0), gstat(_gstat), hsyst(_hsyst),
      x_header(make_hep_nuv(_x_header)), y_header(make_hep_nuv(_y_header)) {
      for(auto& q : _qualifiers) {
        qualifiers.push_back(make_hep_nuv(q));
      }
    } 

    // void set_qualifiers(std::vector<hep_nuv> _qualifiers) { qualifiers = _qualifiers; }
    void print_qualifiers() {
      for (auto& q : qualifiers)
        print_hep_nuv(q, true);
    }
    void print(std::ostream& out, int fx = 0, int fy = 0, float range_l = 99, float range_h = -99, bool hide_x = false);
    
  private:
    TH1* hstat;
    TGraphErrors* gstat;
    TGraphErrors* hsyst;
    hep_nuv x_header, y_header;
    std::vector<hep_nuv> qualifiers;
    int findhsystbin(double xval);
  };
}

void xjjroot::hepdata::print(std::ostream &out,
                             int fx, int fy,
                             float range_l, float range_h, bool hide_x) {
  int nx = hstat?hstat->GetNbinsX():gstat->GetN();

  if (! hide_x) {
    out << "independent_variables:" << std::endl;
    out << "- header: " << print_hep_nuv(x_header) << std::endl;
    out << "  values:" << std::endl;

    for (int i=0; i<nx; i++) {
      auto xval = hstat?hstat->GetBinCenter(i+1):gstat->GetPointX(i);
      if ((range_l < range_h) && (xval < range_l || xval > range_h)) continue;
      if (hstat) {
        out << "  - {low: " << Form("%.*f", fx, hstat->GetBinCenter(i+1) - hstat->GetBinWidth(i+1)/2.)
            << ", high: " << Form("%.*f", fx, hstat->GetBinCenter(i+1) + hstat->GetBinWidth(i+1)/2.)
            << "}" << std::endl;
      } else if (gstat) {
        out << "  - {value: " << Form("%.*f", fx, xval) << "}" << std::endl;
      }
    }
    out << "dependent_variables:" << std::endl;
  }
  
  out << "- header: " << print_hep_nuv(y_header) << std::endl;
  out << "  qualifiers:" << std::endl;
  for (auto& q : qualifiers) {
    out << "  - " << print_hep_nuv(q) << std::endl;
  }
  out << "  values:" << std::endl;
  for (int i=0; i<nx; i++) {
    auto xval = hstat?hstat->GetBinCenter(i+1):gstat->GetPointX(i),
      yval = hstat?hstat->GetBinContent(i+1):gstat->GetPointY(i),
      ystat = hstat?hstat->GetBinError(i+1):gstat->GetErrorY(i);
    
    if ((range_l < range_h) && (xval < range_l || xval > range_h)) continue;
    
    out << "  - value: " << Form("%.*f", fy, yval) << std::endl;
    out << "    errors:" << std::endl;
    
    if (hsyst) {
      out << "    - {symerror: " << Form("%.*f", fy, ystat) << ", label: stat}" << std::endl;
      auto j = findhsystbin(xval);
      out << "    - {symerror: " << Form("%.*f", fy, hsyst->GetErrorY(j)) << ", label: sys}" << std::endl;
    } else {
      out << "    - {symerror: " << Form("%.*f", fy, ystat) << ", label: total}" << std::endl;
    }
  }
}

int xjjroot::hepdata::findhsystbin(double xval) {
  std::vector<double> vx(hsyst->GetX(), hsyst->GetX() + hsyst->GetN());
  auto j = xjjc::find_ibin(vx, xval);
  return j;
}
