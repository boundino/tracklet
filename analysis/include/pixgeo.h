#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <TTree.h>
#include <TFile.h>

#include "xjjrootuti.h"
#include "xjjcuti.h"

#define vmod std::vector<mod> 

class pixgeo {
public:
  pixgeo(std::string inputname);
  void draw1(std::string var) { draw(var, 1); }
  void draw2(std::string var) { draw(var, 2); }
  void draw3(std::string var) { draw(var, 3); }
  void draw4(std::string var) { draw(var, 4); }
  void draw5(std::string var) { draw(var, 5); }
  void draw6(std::string var) { draw(var, 6); }
  void draw7(std::string var) { draw(var, 7); }
private:
  TTree* t;
  void draw(std::string var="fpix-x-y-minus", int layer=1);
  struct mod {
    int rawid;
    std::map<std::string, double> pos;
  };
  std::vector<std::string> m_var = {"x", "y", "z", "phi"};
  std::vector<vmod> m_bpix = std::vector<vmod>(4),
    m_fpix_plus = std::vector<vmod>(3),
    m_fpix_minus = std::vector<vmod>(3);
  void parsevar(std::string var, std::string &x, std::string &y, std::string &opt);
  template< typename T >
  std::string int_to_hex( T i )
  {
    std::stringstream stream;
    stream << "0x" 
           << std::setfill ('0') << std::setw(sizeof(T)*2) 
           << std::hex << i;
    return stream.str();
  }
};

pixgeo::pixgeo(std::string inputname) {
  t = (TTree*)(TFile::Open(inputname.c_str())->Get("alignTree"));
  double m_x; t->SetBranchAddress("x", &m_x); 
  double m_y; t->SetBranchAddress("y", &m_y); 
  double m_z; t->SetBranchAddress("z", &m_z); 
  double m_phi; t->SetBranchAddress("phi", &m_phi); 
  int m_rawid; t->SetBranchAddress("rawid", &m_rawid); 
  int m_subdetid; t->SetBranchAddress("subdetid", &m_subdetid); 
  int m_layer; t->SetBranchAddress("layer", &m_layer);
  // std::cout<<t->GetEntries()<<std::endl;
  for (int i=0; i<t->GetEntries(); i++) {
    t->GetEntry(i);
    mod module;
    module.rawid = m_rawid;
    module.pos["x"] = m_x;
    module.pos["y"] = m_y;
    module.pos["z"] = m_z;
    module.pos["phi"] = m_phi;
    if (m_layer <= 4) {
      m_bpix[m_layer-1].push_back(module);
    } else {
      if (m_z > 0)
        m_fpix_plus[m_layer-5].push_back(module);
      else
        m_fpix_minus[m_layer-5].push_back(module);
    }
  }
  // for (auto p : m_bpix) {
  //   std::cout<<"m_bpix "<<p.size()<<std::endl;
  // }
  // for (auto p : m_fpix_plus) {
  //   std::cout<<"m_fpix_plus "<<p.size()<<std::endl;
  // }
  // for (auto p : m_fpix_minus) {
  //   std::cout<<"m_fpix_minus "<<p.size()<<std::endl;
  // }
}

void pixgeo::parsevar(std::string var, std::string &x, std::string &y, std::string &opt) {
  var = xjjc::str_replaceall(var, "fpix-", "");
  auto vars = xjjc::str_divide(var, "-");
  x = vars[0];
  y = vars[1];
  if (vars.size() > 2) {
    opt = vars[2];
  }
}

void pixgeo::draw(std::string var, int layer) {
  std::string x, y, opt;
  parsevar(var, x, y, opt);
  std::cout<<__FUNCTION__<<": "<<x<<" "<<y<<" "<<opt<<std::endl;
  if (std::find(m_var.begin(), m_var.end(), x) == m_var.end() ||
      std::find(m_var.begin(), m_var.end(), y) == m_var.end())
    return;
  std::vector<mod> modules;
  if (layer < 5) modules = m_bpix[layer-1];
  else {
    if (xjjc::str_contains(opt, "plus")) modules = m_fpix_plus[layer-5];
    else modules = m_fpix_minus[layer-5];
  }
  for (auto mm : modules) {
    double shifty = 0.;
    // xjjroot::drawtexnum(mm.pos[x], mm.pos[y] + shifty, Form("%d", mm.rawid),
    xjjroot::drawtexnum(mm.pos[x], mm.pos[y] + shifty, int_to_hex(mm.rawid).c_str(),
                        0.015, 22, 62, kBlack, 0);
  }
}
