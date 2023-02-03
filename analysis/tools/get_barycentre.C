#include <TH2D.h>
#include <TH3D.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TF1.h>

// #include "include/compare.h"
#include "include/xjjcuti.h"
#include "include/xjjrootuti.h"
#include "include/xjjmypdf.h"
#include "include/sconfig.h"

// #include "include/defines.h"

#define BPIX1P(EXPAND)                          \
  EXPAND(1)                                     \
  //   EXPAND(2)                                \
  //   EXPAND(3)                                \
  //   EXPAND(4)                                \

#define MAXEVT 50
#define NBINS 1000

const Color_t cc[] = {kBlack, kRed, kBlue};
float dxshift[] = {0, -0.0723367}, dyshift[] = {0, 0.167845}, dzshift[] = {0, 0.36}; // BS_MC = BS_data + dshift

std::vector<std::string> legtags;
std::string comment;

enum opt { file, tagname };

int maphits(std::vector<TTree*>& t, std::string var, std::vector<std::array<float, 4>> bins, xjjroot::mypdf* pdf);
int mapvertex(std::vector<TTree*>& t, std::vector<std::array<float, 2>> bins, xjjroot::mypdf* pdf);
void drawbs(std::vector<std::array<float, 3>> bsxyz, std::string type);
void drawtags();

int macro(std::string input="/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root#Data,/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221207_MB_Hydjet_Run3_subehera_Th4.root#MC",
          std::string tag="362294t-Hydjet")
{
  xjjc::sconfig conf(input);
  std::vector<TTree*> t(conf.n());
  legtags.clear();
  comment = tag;
 
  for(int i=0; i<conf.n(); i++)
    {
      auto inf = new TFile(conf.value[i][file].c_str(), "read");
      t[i] = (TTree*)inf->Get("pixel/PixelTree");
      legtags.push_back(conf.value[i][tagname]);
    }

  xjjroot::setgstyle(1);
  xjjroot::mypdf* pdf = new xjjroot::mypdf(Form("figs/pixel/pixel-barycentre-%s.pdf", tag.c_str()), "c", 600, 600);

  maphits(t, "r@*sin(phi@):r@*cos(phi@),x,y,x,y", {{-5, 5, -5, 5}, {-10, 10, -10, 10}, {-15, 15, -15, 15}, {-20, 20, -20, 20}},  pdf);
  maphits(t, "r@*sin(phi@)-bsy:r@*cos(phi@)-bsx,xbsx,ybsy,x - x_{BS},y - y_{BS}", {{-5, 5, -5, 5}, {-10, 10, -10, 10}, {-15, 15, -15, 15}, {-20, 20, -20, 20}},  pdf);
  maphits(t, "r@:r@/tan(2*atan(exp(-eta@))),z,r,z,r", {{-8, 8, 0, 5}, {-30, 30, 5, 10}, {-30, 30, 10, 15}, {-30, 30, 15, 20}}, pdf);
  maphits(t, "sqrt((r@*sin(phi@)-bsy)*(r@*sin(phi@)-bsy)+(r@*cos(phi@)-bsx)*(r@*cos(phi@)-bsx)):r@/tan(2*atan(exp(-eta@)))-bsz,zbsz,rbsr,z - z_{BS},|#vec{r}-#vec{r}_{BS}|", {{-30, 30, 0, 5}, {-30, 30, 5, 10}, {-30, 30, 10, 15}, {-30, 30, 15, 20}}, pdf);

  mapvertex(t, {{0.14, 0.20}, {-0.20, -0.14}, {-15, 15}}, pdf);

  pdf->close();

  return 0;
}

#define VTX1D(EXPAND)                           \
  EXPAND(z)                                     \
  EXPAND(x)                                     \
  EXPAND(y)                                     \

#define GET_BS(q)                                       \
  std::vector<float> bs##q(n);                          \
  for(int i=0; i<n; i++)                                \
    {                                                   \
      t[i]->SetBranchAddress("bs" #q, &(bs##q[i]));     \
      t[i]->GetEntry(1);                                \
      t[i]->GetBranch("bs" #q)->ResetAddress();         \
    }                                                   \

int maphits(std::vector<TTree*>& t, std::string var, std::vector<std::array<float, 4>> bins, xjjroot::mypdf* pdf)
{
  int n = t.size();
  auto vars = xjjc::str_divide(var, ",");

#define SETUP_XY_PIXELS(q)                                              \
  std::vector<TH2D*> hxy##q(n);                                         \
    for(int i=0; i<n; i++)                                              \
      {                                                                 \
        hxy##q[i] = new TH2D(Form("h%s%s" #q "f%d", vars[1].c_str(), vars[2].c_str(), i), \
                             Form(";%s (layer " #q ");%s (layer " #q ")", vars[3].c_str(), vars[4].c_str()), \
                             NBINS, bins[atoi(#q)-1][0], bins[atoi(#q)-1][1], \
                             NBINS, bins[atoi(#q)-1][2], bins[atoi(#q)-1][3]); \
          xjjroot::sethempty(hxy##q[i]);                                \
          hxy##q[i]->SetMarkerColor(cc[i]);                             \
      }                                                                 \

  BPIX1P(SETUP_XY_PIXELS)

    VTX1D(GET_BS)

#define DRAW_XY_PIXELS(q)                                               \
    std::string var##q = vars[0];                                       \
      std::replace(var##q.begin(), var##q.end(), '@', #q[0]);           \
      for(int i=0; i<n; i++)                                            \
        {                                                               \
          t[i]->Draw(Form("%s>>h%s%s" #q "f%d", var##q.c_str(), vars[1].c_str(), vars[2].c_str(), i), \
                     "(nhfp > 1 && nhfn > 1)", "goff", MAXEVT);         \
        }                                                               \
      pdf->prepare();                                                   \
      for(int i=0; i<n; i++)                                            \
        { hxy##q[i]->Draw(i?"same":""); }                               \
      drawbs({{bsx[0],bsy[0],bsz[0]},{bsx[1],bsy[1],bsz[1]}}, Form("%s-%s", vars[1].c_str(), vars[2].c_str())); \
      drawtags();                                                       \
      pdf->write();                                                     \
  
    BPIX1P(DRAW_XY_PIXELS)

    return 0;
}

void drawtags()
{
  float y = 0.84, dy = 0.042;
  for(int i=0; i<legtags.size(); i++)
    xjjroot::drawtex(0.90, y-dy*i, legtags[i].c_str(), 0.038, 33, 62, cc[i]);
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright("PbPb (5.36 TeV)");
  xjjroot::drawtex(0.25, y, "BPIX", 0.038, 13);
  xjjroot::drawcomment(comment);
}

void drawbs(std::vector<std::array<float, 3>> bsxyz, std::string type)
{
  std::cout<<type<<std::endl;
  float bsxdata = bsxyz[0][0], bsydata = bsxyz[0][1], bszdata = bsxyz[0][2],
    bsxmc = bsxyz[1][0], bsymc = bsxyz[1][1], bszmc = bsxyz[1][2],
    bsxshiftto = bsxdata + dxshift[1], bsyshiftto = bsydata + dyshift[1], bszshiftto = bszdata + dzshift[1];

  float xdata = -99, ydata = -99, xmc = -99, ymc = -99, xshiftto = -99, yshiftto = -99; 
  bool draw = false;
  if(type=="x-y")
    {
      xdata = bsxdata; ydata = bsydata;
      xmc = bsxmc; ymc = bsymc;
      xshiftto = bsxshiftto; yshiftto = bsyshiftto;
      draw = true;
    }
  if(type=="z-r")
    {
      xdata = bszdata; ydata = std::sqrt(bsxdata*bsxdata+bsydata*bsydata);
      xmc = bszmc; ymc = std::sqrt(bsxmc*bsxmc+bsymc*bsymc);
      xshiftto = bszshiftto; yshiftto = std::sqrt(bsxshiftto*bsxshiftto+bsyshiftto*bsyshiftto);
      draw = true;
    }

  if(draw)
    {
      auto leg = new TLegend(0.22, 0.20, 0.32, 0.32);
      leg->AddEntry(xjjroot::drawpoint(xdata, ydata, cc[0], 20, 1), "BS_{data}", "p");
      leg->AddEntry(xjjroot::drawpoint(xshiftto, yshiftto, cc[2], 53, 1.2), "BS_{MC} (correct)", "p");
      leg->AddEntry(xjjroot::drawpoint(xmc, ymc, cc[1], 20, 1), "BS_{MC} (sample)", "p");
      xjjroot::setlegndraw(leg, 0.03);
      // xjjroot::drawline(xdata, ydata, xshiftto, yshiftto, cc[2], 1, 2);
    }
}

int mapvertex(std::vector<TTree*>& t, std::vector<std::array<float, 2>> bins, xjjroot::mypdf* pdf)
{
  int n = t.size();
  std::vector<TH3D*> hv(n);
  for(int i=0; i<n; i++)
    {
      hv[i] = new TH3D(Form("hvxyz%d", i), ";v_{x} (Shifted);v_{y} (Shifted);v_{z} (Shifted)", 
                       NBINS, bins[0][0], bins[0][1],
                       NBINS, bins[1][0], bins[1][1],
                       100, bins[2][0], bins[2][1]);
      xjjroot::sethempty(hv[i]);

      t[i]->Draw(Form("vz[1]-%f:vy[1]-%f:vx[1]-%f>>hvxyz%d", dzshift[i], dyshift[i], dxshift[i], i), 
                 "(nhfp > 1 && nhfn > 1 && fabs(vz[1])<15)", "goff");
    }

  VTX1D(GET_BS)

#define DRAW_1D_VTX(q)                                                  \
    std::vector<TH1D*> hv##q(n);                                        \
      std::vector<TF1*> fs##q(n);                                       \
        for(int i=0; i<n; i++)                                          \
          {                                                             \
            hv##q[i] = (TH1D*)hv[i]->Project3D(#q);                     \
              hv##q[i]->Scale(1./hv##q[i]->Integral(), "width");        \
                hv##q[i]->SetTitle("");                                 \
                  xjjroot::sethempty(hv##q[i]);                         \
                  hv##q[i]->SetLineColor(cc[i]);                        \
                    fs##q[i] = new TF1(Form("f" #q "%d",i), "gaus", hv##q[i]->GetXaxis()->GetXmin(), hv##q[i]->GetXaxis()->GetXmax()); \
                      fs##q[i]->SetParameters(1, bs##q[i], 3);          \
                        hv##q[i]->Fit(Form("f" #q "%d",i), "N");        \
                          fs##q[i]->SetLineColor(hv##q[i]->GetLineColor()); \
          }                                                             \
        pdf->prepare();                                                 \
        for(int i=0; i<n; i++)                                          \
          {                                                             \
            hv##q[i]->SetMaximum(hv##q[n-1]->GetMaximum()*1.4);         \
              hv##q[i]->Draw(i?"histe same":"histe");                   \
                fs##q[i]->Draw("same");                                 \
          }                                                             \
        drawtags();                                                     \
        pdf->write();                                                   \
  
    VTX1D(DRAW_1D_VTX)
 
#define DRAW_2D_VTX(q)                                          \
    std::vector<TH2D*> hv##q(n);                                \
      for(int i=0; i<n; i++)                                    \
        {                                                       \
          hv##q[i] = (TH2D*)hv[i]->Project3D(#q);               \
            hv##q[i]->Scale(1./hv##q[i]->Integral(), "width");  \
              hv##q[i]->SetTitle("");                           \
                xjjroot::sethempty(hv##q[i]);                   \
                hv##q[i]->SetMarkerColor(cc[i]);                \
        }                                                       \
      pdf->prepare();                                           \
      for(int i=0; i<n; i++)                                    \
        { hv##q[i]->Draw(i?"same":""); }                        \
      drawtags();                                               \
      pdf->write();                                             \
  
    DRAW_2D_VTX(yx)

    return 0;  
}

// for(int i=0; i<comp.n(); i++)
//   {
//     fs[i] = new TF1(Form("f%d",i), "gaus", -15, 15);
//     fs[i]->SetParameters(1, 0, 5);
//     comp.vh[i]->Fit(Form("f%d",i), "N");
//     fs[i]->SetLineColor(comp.vh[i]->GetLineColor());
//   }
// for(auto ff : fs) ff->Draw("same");

int main()
{
  return macro();
}
