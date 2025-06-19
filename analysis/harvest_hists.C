#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TH1.h"
#include "TH2.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLine.h"

#include <vector>
#include <string>
#include <algorithm>

#include "git/config/configurer.h"

#include "include/defines.h"
#include "include/xjjmypdf.h"
#include "include/xjjanauti.h"

#include "include/cosmetics.h"
#include "include/pixgeo.h"

#define OPT(val)  options[opt].val
#define CS(str)   str.data()
#define OS(val)   CS(OPT(val))

#define PIXELS1P(EXPAND)   \
   BPIX1P(EXPAND)          \
   FPIX1P(EXPAND)          \

#define TRKLTS2P(EXPAND)   \
   EXPAND(1, 2)            \
   EXPAND(1, 3)            \
   EXPAND(1, 4)            \
   EXPAND(2, 3)            \
   EXPAND(2, 4)            \
   EXPAND(3, 4)            \
   EXPAND(5, 6)            \
   EXPAND(5, 7)            \
   EXPAND(6, 7)            \

typedef struct varinfo_t {
   std::string                         id;
   std::vector<std::string>            label;
   std::vector<std::string>            var;
   std::vector<std::array<float, 3>>   bins;
   int                                 csize[2];
   int                                 flags;
   std::string                         sel;
   std::string                         gopt;
   Long64_t                            maxnevt;
} varinfo_t;

std::string sel_ = "(1)";
// std::string sel_ = "(nhfp > 1 && nhfn > 1)";

static const std::vector<varinfo_t> options_pixel_1d = {
   {
      "cs", {"cluster size"}, {"cs"},
      {{50, 0, 50}},
      {600, 600}, 0x01, sel_, "", 2000
   }, {
      "nhits", {"number of pixel hits"}, {"nhits"},
      {{100, 0, 4000}},
      {600, 600}, 0x01, sel_, "", TTree::kMaxEntries
   }
};

int compare_pixels(std::vector<varinfo_t> const& options,
      const char* config, const char* label, int opt) {
   configurer* conf = new configurer(config);

   auto files = conf->get<std::vector<std::string>>("files");
   auto legends = conf->get<std::vector<std::string>>("legends");

   std::size_t nfiles = files.size();
   if (!nfiles) { printf("error: no files provided!\n"); exit(1); }

   TCut fsel = OS(sel);

   const char* idstr = OS(id);
   const char* varstr = OS(var[0]);
   
   TH1::SetDefaultSumw2();
   gStyle->SetOptStat(0);

   TFile* f[nfiles]; TTree* t[nfiles];
   for (std::size_t j = 0; j < nfiles; ++j) {
      f[j] = new TFile(files[j].data(), "read");
      t[j] = (TTree*)f[j]->Get("pixel/PixelTree");
   }

#define SETUP_1D_PIXELS(q)                                                    \
   TH1D* h##q[nfiles]; TH1D* hr##q[nfiles] = {0};                             \
   for (std::size_t j = 0; j < nfiles; ++j) {                                 \
      h##q[j] = new TH1D(Form("hp" #q "f%zu%s", j, idstr),                    \
            Form(";%s (layer " #q ");", OS(label[0])),                        \
            (int)OPT(bins[0][0]), OPT(bins[0][1]), OPT(bins[0][2]));          \
   }                                                                          \

   PIXELS1P(SETUP_1D_PIXELS)

#define PROJECT_1D_PIXELS(q)                                                  \
   for (std::size_t j = 0; j < nfiles; ++j) {                                 \
      t[j]->Draw(Form("%s" #q ">>hp" #q "f%zu%s", varstr, j, idstr),          \
                 fsel, "goff", OPT(maxnevt));                                 \
      h##q[j]->Scale(1. / h##q[j]->Integral());                               \
   }

   PIXELS1P(PROJECT_1D_PIXELS)

   int cheight = OPT(flags) & 0x10 ? OPT(csize[1]) * 1.2 : OPT(csize[1]);

   xjjroot::mypdf* pdf_DRAW_1D_PIXELS = new xjjroot::mypdf(Form("figspdf/pixel/pixel-%s-%s.pdf", OS(id), label), "c_DRAW_1D_PIXELS", OPT(csize[0]), cheight);

#define DRAW_1D_PIXELS(q)                                                     \
    pdf_DRAW_1D_PIXELS->prepare();                                            \
   if (OPT(flags) & 0x10) {                                                   \
      TPad* t1 = new TPad("t" #q "1", "", 0, 0.25, 1, 1);                     \
      t1->SetTopMargin(0.11111); t1->SetBottomMargin(0);                      \
      t1->Draw(); t1->SetNumber(1);                                           \
      TPad* t2 = new TPad("t" #q "2", "", 0, 0, 1, 0.25);                     \
      t2->SetTopMargin(0); t2->SetBottomMargin(0.32);                         \
      t2->Draw(); t2->SetNumber(2);                                           \
      pdf_DRAW_1D_PIXELS->getc()->cd(1);                                      \
                                                                              \
      h##q[0]->SetLabelOffset(99, "X");                                       \
      h##q[0]->SetTitleOffset(99, "X");                                       \
   }                                                                          \
                                                                              \
   if (OPT(flags) & 0x1) { gPad->SetLogy(); }                                 \
                                                                              \
   h##q[0]->Draw("axis");                                                     \
   for (std::size_t j = 1; j < nfiles; ++j) {                                 \
      h##q[j]->SetLineColor(colours[j % ncolours]);                           \
      h##q[j]->Draw("hist e same");                                           \
   }                                                                          \
   h##q[0]->SetMarkerStyle(21);                                               \
   h##q[0]->SetMarkerSize(0.6);                                               \
   h##q[0]->SetMarkerColor(1);                                                \
   h##q[0]->SetLineColor(1);                                                  \
   h##q[0]->Draw("p e same");                                                 \
                                                                              \
   TLegend* l##q = new TLegend(0.50, 0.875-0.05*nfiles, 0.80, 0.875);         \
   l##q->AddEntry(h##q[0], CS(legends[0]), "p");                              \
   for (std::size_t j = 1; j < nfiles; ++j)                                   \
      l##q->AddEntry(h##q[j], CS(legends[j]), "l");                           \
   lstyle(l##q, 43, 22); l##q->Draw();                                        \
                                                                              \
   if (OPT(flags) & 0x10) {                                                   \
      pdf_DRAW_1D_PIXELS->getc()->cd(2);                                      \
      for (std::size_t j = 1; j < nfiles; ++j) {                              \
         hr##q[j] = (TH1D*)h##q[j]->Clone(Form("hpr" #q "f%zu%s", j, idstr)); \
         hr##q[j]->Divide(h##q[0]);                                           \
         rformat(hr##q[j], 0.5, 1.5);                                         \
         hr##q[j]->Draw("p e same");                                          \
      }                                                                       \
      pdf_DRAW_1D_PIXELS->getc()->cd(1);                                      \
   }                                                                          \
   watermark();                                                         \
   pdf_DRAW_1D_PIXELS->write(Form("figs/pixel/pixel-%s-l" #q "-%s.png",    \
                                  OS(id), label), "Q");                 \
   // delete c##q;                                                              \

   PIXELS1P(DRAW_1D_PIXELS)
   
   pdf_DRAW_1D_PIXELS->close();

   TFile* fout = new TFile(Form("data/%s.root", label), "update");
#define SAVE_1D_PIXELS(q)                                                     \
   for (std::size_t j = 0; j < nfiles; ++j)                                   \
      h##q[j]->Write("", TObject::kOverwrite);
   PIXELS1P(SAVE_1D_PIXELS)
   fout->Close();

   return 0;
}

static const std::vector<varinfo_t> options_tracklet_1d = {
   {
      "deta", {"#Delta#eta"}, {"deta"},
      {{100, -0.5, 0.5}},
      {600, 600}, 0x11, "abs(deta)<0.5", "", 1000
   }, {
      "dphi", {"#Delta#phi"}, {"dphi"},
      {{100, 0, 0.5}},
      {600, 600}, 0x11, "abs(dphi)<0.5", "", 1000
   }, {
      "dr", {"#Deltar"}, {"sqrt(dr2)"},
      {{100, 0, 0.5}},
      {600, 600}, 0x11, "abs(dr2)<0.25", "", 1000
   }, {
      "vz", {"v_{z}"}, {"vz[1]"},
      {{100, -15, 15}},
      {600, 600}, 0x10, sel_, "", TTree::kMaxEntries
   }, {
      "vz-unw", {"v_{z}"}, {"vz[1]"},
      {{100, -15, 15}},
      {600, 600}, 0x110, sel_, "", TTree::kMaxEntries
   }, {
      "ntracklet", {"number of tracklets"}, {"ntracklet"},
      {{100, 0, 4000}},
      {600, 600}, 0x01, sel_, "", TTree::kMaxEntries
   }, {
     "dr-wide", {"#Deltar"}, {"sqrt(dr2)"},
     {{100, 0, 2.0}},
     {600, 600}, 0x11, "abs(dr2)<4.0", "", 1000
   },
};

int compare_tracklets(std::vector<varinfo_t> const& options,
      const char* config, const char* label, int opt) {
   configurer* conf = new configurer(config);

   auto files = conf->get<std::vector<std::string>>("files");
   auto legends = conf->get<std::vector<std::string>>("legends");

   std::size_t nfiles = files.size();
   if (!nfiles) { printf("error: no files provided!\n"); exit(1); }

   TCut fsel = OS(sel);
   fsel = fsel && Form("abs(vz[1])<15 && %s", sel_.c_str());
   if (!(OPT(flags) & 0x100)) { fsel *= "weight"; }

   const char* idstr = OS(id);
   const char* varstr = OS(var[0]);

   TH1::SetDefaultSumw2();
   gStyle->SetOptStat(0);

   TFile* f[nfiles];
   for (std::size_t j = 0; j < nfiles; ++j)
      f[j] = new TFile(files[j].data(), "read");

#define SETUP_1D_TRACKLETS(q, w)                                              \
   TTree* t##q##w[nfiles];                                                    \
   TH1D* h##q##w[nfiles]; TH1D* hr##q##w[nfiles] = {0};                       \
   for (std::size_t j = 0; j < nfiles; ++j) {                                 \
      t##q##w[j] = (TTree*)f[j]->Get("TrackletTree" #q #w);                   \
      h##q##w[j] = new TH1D(Form("ht" #q #w "f%zu%s", j, idstr),              \
            Form(";%s (layers " #q "+" #w ");", OS(label[0])),                \
            (int)OPT(bins[0][0]), OPT(bins[0][1]), OPT(bins[0][2]));          \
   }                                                                          \

   TRKLTS2P(SETUP_1D_TRACKLETS)

#define PROJECT_1D_TRACKLETS(q, w)                                            \
   for (std::size_t j = 0; j < nfiles; ++j) {                                 \
      t##q##w[j]->Draw(Form("%s>>ht" #q #w "f%zu%s", varstr, j, idstr),       \
                       fsel, "goff", OPT(maxnevt));                           \
      h##q##w[j]->Scale(1. / h##q##w[j]->Integral(), "width");                \
   }                                                                          \

   TRKLTS2P(PROJECT_1D_TRACKLETS)

   int cheight = OPT(flags) & 0x10 ? OPT(csize[1]) * 1.2 : OPT(csize[1]);

   xjjroot::mypdf* pdf_DRAW_1D_TRACKLETS = new xjjroot::mypdf(Form("figspdf/tracklet/tracklet-%s-%s.pdf", OS(id), label), "c_DRAW_1D_TRACKLETS", OPT(csize[0]), cheight);

#define DRAW_1D_TRACKLETS(q, w)                                               \
   pdf_DRAW_1D_TRACKLETS->prepare();                                          \
   if (OPT(flags) & 0x10) {                                                   \
      TPad* t1 = new TPad("t" #q #w "1", "", 0, 0.25, 1, 1);                  \
      t1->SetTopMargin(0.11111); t1->SetBottomMargin(0);                      \
      t1->Draw(); t1->SetNumber(1);                                           \
      TPad* t2 = new TPad("t" #q #w "2", "", 0, 0, 1, 0.25);                  \
      t2->SetTopMargin(0); t2->SetBottomMargin(0.32);                         \
      t2->Draw(); t2->SetNumber(2);                                           \
      pdf_DRAW_1D_TRACKLETS->getc()->cd(1);                                   \
                                                                              \
      h##q##w[0]->SetLabelOffset(99, "X");                                    \
      h##q##w[0]->SetTitleOffset(99, "X");                                    \
   }                                                                          \
                                                                              \
   if (OPT(flags) & 0x1) { gPad->SetLogy(); }                                 \
                                                                              \
   h##q##w[0]->Draw("axis");                                                  \
   for (std::size_t j = 1; j < nfiles; ++j) {                                 \
      h##q##w[j]->SetLineColor(colours[j % ncolours]);                        \
      h##q##w[j]->Draw("hist e same");                                        \
   }                                                                          \
   h##q##w[0]->SetMarkerStyle(21);                                            \
   h##q##w[0]->SetMarkerSize(0.6);                                            \
   h##q##w[0]->SetMarkerColor(1);                                             \
   h##q##w[0]->SetLineColor(1);                                               \
   h##q##w[0]->Draw("p e same");                                              \
                                                                              \
   TLegend* l##q##w = new TLegend(0.50, 0.875-0.05*nfiles, 0.80, 0.875);      \
   l##q##w->AddEntry(h##q##w[0], CS(legends[0]), "p");                        \
   for (std::size_t j = 1; j < nfiles; ++j)                                   \
      l##q##w->AddEntry(h##q##w[j], CS(legends[j]), "l");                     \
   lstyle(l##q##w, 43, 22); l##q##w->Draw();                                  \
                                                                              \
   if (OPT(flags) & 0x10) {                                                   \
      pdf_DRAW_1D_TRACKLETS->getc()->cd(2);                                   \
      for (std::size_t j = 1; j < nfiles; ++j) {                              \
         hr##q##w[j] = (TH1D*)h##q##w[j]->Clone(                              \
               Form("htr" #q "f%zu%s", j, idstr));                            \
         hr##q##w[j]->Divide(h##q##w[0]);                                     \
         hr##q##w[j]->SetAxisRange(0.5, 1.5, "Y");                            \
         hr##q##w[j]->SetLabelSize(0.08, "X");                                \
         hr##q##w[j]->SetTitleSize(0.1, "X");                                 \
         hr##q##w[j]->SetLabelSize(0.08, "Y");                                \
         hr##q##w[j]->SetTitleSize(0.08, "Y");                                \
         hr##q##w[j]->SetTitleOffset(0.5, "Y");                               \
         hr##q##w[j]->SetNdivisions(205, "Y");                                \
         hr##q##w[j]->SetYTitle("ratio");                                     \
         hr##q##w[j]->GetYaxis()->CenterTitle();                              \
         hr##q##w[j]->Draw("p e same");                                       \
      }                                                                       \
                                                                              \
      TLine* line1 = new TLine(OPT(bins[0][1]), 1, OPT(bins[0][2]), 1);       \
      line1->Draw();                                                          \
      pdf_DRAW_1D_TRACKLETS->getc()->cd(1);                             \
   }                                                                          \
   watermark();                                                         \
                                                                              \
   pdf_DRAW_1D_TRACKLETS->write(Form("figs/tracklet/tracklet-%s-t" #q #w "-%s.png", \
                                     OS(id), label), "Q");              \
   // delete c##q##w; \

   TRKLTS2P(DRAW_1D_TRACKLETS)
   pdf_DRAW_1D_TRACKLETS->close();

   TFile* fout = new TFile(Form("data/%s.root", label), "update");
#define SAVE_1D_TRACKLETS(q, w)                                               \
   for (std::size_t j = 0; j < nfiles; ++j)                                   \
      h##q##w[j]->Write("", TObject::kOverwrite);
   TRKLTS2P(SAVE_1D_TRACKLETS)
   fout->Close();

   return 0;
}

static const std::vector<varinfo_t> options_pixel_2d = {
   {
      "eta-phi", {"#eta", "#phi"},
      {"eta@", "phi@"},
      {{1000, -4, 4}, {1000, -4, 4}},
      {600, 600}, 0x03, sel_, "colz", 10000
   }, {
      "eta-r", {"#eta", "r"},
      {"eta@", "r@"},
      {{1000, -4, 4}, {1000, 0, 20}},
      {600, 600}, 0x33, sel_, "colz", 10000
   }, {
      "eta-cs", {"#eta", "cluster size"},
      {"eta@", "cs@"},
      {{200, -4, 4}, {40, 0, 40}},
      {600, 600}, 0x03, sel_, "colz", 10000
   }, {
      "x-y", {"x", "y"},
      {"r@*cos(phi@)", "r@*sin(phi@)"},
      {{1000, -20, 20}, {1000, -20, 20}},
      {600, 600}, 0x11, sel_, "colz", 10000
   }, {
      "z-phi", {"z", "#phi"},
      {"r@/tan(2*atan(exp(-eta@)))", "phi@"},
      {{1000, -30, 30}, {1000, -4, 4}},
      {600, 2400}, 0x01, sel_, "colz", 10000
   }, {
      "z-r", {"z", "r"},
      {"r@/tan(2*atan(exp(-eta@)))", "r@"},
      {{1000, -60, 60}, {1000, 0, 20}},
      {1200, 600}, 0x33, sel_, "colz", 10000
   }, {
      "fpix-x-y-plus", {"x", "y"},
      {"r@*cos(phi@)", "r@*sin(phi@)"},
      {{1000, -20, 20}, {1000, -20, 20}},
      {600, 600}, 0x02, "(eta@>0)", "colz", 10000
   }, {
      "fpix-x-y-minus", {"x", "y"},
      {"r@*cos(phi@)", "r@*sin(phi@)"},
      {{1000, -20, 20}, {1000, -20, 20}},
      {600, 600}, 0x02, "(eta@<0)", "colz", 10000
   }, {
      "fpix-z-phi", {"z", "#phi"},
      {"r@/tan(2*atan(exp(-eta@)))", "phi@"},
      {{1000, -60, 60}, {1000, -4, 4}},
      {1200, 600}, 0x22, sel_, "colz", 10000
   }
};

bool labelid = false;

int map_pixels(std::vector<varinfo_t> const& options,
      const char* input, const char* label, int opt) {
#define SELECTION(q)                                                          \
   std::string sel##q = OPT(sel);                                             \
   std::replace(sel##q.begin(), sel##q.end(), '@', #q[0]);                    \
   TCut fsel##q = sel##q.data();                                              \

   PIXELS1P(SELECTION)

   const char* l0str = OS(label[0]);
   const char* l1str = OS(label[1]);
   const char* idstr = OS(id);

   gStyle->SetOptStat(0);

   pixgeo geo("data/pixelGeo.root");

   TFile* f = new TFile(input, "r");
   TTree* t = (TTree*)f->Get("pixel/PixelTree");

#define SETUP_2D_PIXELS(q)                                                    \
   std::string x##q = OPT(var[0]);                                            \
   std::string y##q = OPT(var[1]);                                            \
   std::replace(x##q.begin(), x##q.end(), '@', #q[0]);                        \
   std::replace(y##q.begin(), y##q.end(), '@', #q[0]);                        \
                                                                              \
   TH2D* h##q = new TH2D(Form("h" #q "%s", idstr),                            \
         Form(";%s (layer " #q ");%s (layer " #q ")", l0str, l1str),          \
         (int)OPT(bins[0][0]), OPT(bins[0][1]), OPT(bins[0][2]),              \
         (int)OPT(bins[1][0]), OPT(bins[1][1]), OPT(bins[1][2]));             \

   PIXELS1P(SETUP_2D_PIXELS)

     xjjroot::mypdf* pdf_DRAW_2D_PIXELS = new xjjroot::mypdf(Form("figspdf/pixel/pixel-%s-%s.pdf", OS(id), label), "c_DRAW_2D_PIXELS",  OPT(csize[0]), OPT(csize[1]));

#define DRAW_2D_PIXELS(q)                                               \
   t->Draw(Form("%s:%s>>h" #q "%s", CS(y##q), CS(x##q), idstr),         \
           fsel##q, "goff", OPT(maxnevt));                              \
                                                                        \
   pdf_DRAW_2D_PIXELS->prepare();                                       \
   h##q->Draw(OS(gopt));                                                \
   if (labelid)                                                         \
     geo.draw##q(OS(id));                                               \
                                                                        \
   watermark();                                                         \
   pdf_DRAW_2D_PIXELS->write(Form("figs/pixel/pixel-%s-l" #q "-%s.png", \
                                  OS(id), label));                      \


   if (OPT(flags) & 0x1) { BPIX1P(DRAW_2D_PIXELS) }
   if (OPT(flags) & 0x2) { FPIX1P(DRAW_2D_PIXELS) }

   TH2D* hall = new TH2D(Form("hall%s", idstr), Form(";%s;%s", l0str, l1str),
         (int)OPT(bins[0][0]), OPT(bins[0][1]), OPT(bins[0][2]),
         (int)OPT(bins[1][0]), OPT(bins[1][1]), OPT(bins[1][2]));

   if (OPT(flags) >> 4) {
#define OVERLAY_2D_PIXELS(q) hall->Add(h##q);
      if (OPT(flags) & 0x10) { BPIX1P(OVERLAY_2D_PIXELS) }
      if (OPT(flags) & 0x20) { FPIX1P(OVERLAY_2D_PIXELS) }

      // TCanvas* call = new TCanvas("call", "", OPT(csize[0]), OPT(csize[1]));
      pdf_DRAW_2D_PIXELS->prepare();
      hall->Draw(OS(gopt));

      watermark();
      pdf_DRAW_2D_PIXELS->write(Form("figs/pixel/pixel-%s-all-%s.png",
                                     OS(id), label));
      // delete call;
   }
   pdf_DRAW_2D_PIXELS->close();

   TFile* fout = new TFile(Form("data/%s.root", label), "update");
#define SAVE_2D_PIXELS(q) h##q->Write("", TObject::kOverwrite);
   PIXELS1P(SAVE_2D_PIXELS)
   hall->Write("", TObject::kOverwrite);
   fout->Close();

   return 0;
}

static const std::vector<varinfo_t> options_compare_pixel_2d = {
  {
    "z-phi", {"z", "#phi"},
    {"r@/tan(2*atan(exp(-eta@)))", "phi@"},
    {{1000, -30, 30}, {1000, -4, 4}},
    {600, 2400}, 0x01, sel_, "colz", 10000
  }, {
    "fpix-x-y-plus", {"x", "y"},
    {"r@*cos(phi@)", "r@*sin(phi@)"},
    {{1000, -20, 20}, {1000, -20, 20}},
    {600, 600}, 0x02, Form("(%s && eta@>0)", sel_.c_str()), "colz", 10000
  }, {
    "fpix-x-y-minus", {"x", "y"},
    {"r@*cos(phi@)", "r@*sin(phi@)"},
    {{1000, -20, 20}, {1000, -20, 20}},
    {600, 600}, 0x02, Form("(%s && eta@<0)", sel_.c_str()), "colz", 10000
  }
};

int compare_map_pixels(std::vector<varinfo_t> const& options,
                       const char* config, const char* label, int opt) {

  const char* idstr = OS(id);

  configurer* conf = new configurer(config);
  auto tags = conf->get<std::vector<std::string>>("tags");
  std::size_t nfiles = tags.size();

  TFile* f[nfiles];
  for (std::size_t j = 0; j < nfiles; ++j)
    f[j] = new TFile(Form("data/%s.root", tags[j].data()), "read");

#define SETUP_2D_PIXELS_COMPARE(q)                                      \
  TH2D* h##q[nfiles]; TH2D* hr##q[nfiles];                              \
  for (std::size_t j = 0; j < nfiles; ++j) {                            \
    h##q[j] = (TH2D*)f[j]->Get(Form("h" #q "%s", idstr));               \
    h##q[j]->SetName(Form("h" #q "%s-f%d", idstr, j));                  \
    h##q[j]->Scale(1. / h##q[j]->Integral());                           \
    hr##q[j] = (TH2D*)h##q[0]->Clone(Form("hr" #q "%s-f%d", idstr, j)); \
    hr##q[j]->Divide(h##q[j]);                                          \
    hr##q[j]->SetMinimum(0);                                            \
    hr##q[j]->SetMaximum(2);                                            \
  }                                                                     \

  PIXELS1P(SETUP_2D_PIXELS_COMPARE)

    xjjroot::mypdf* pdf_DRAW_2D_PIXELS_COMPARE = new xjjroot::mypdf(Form("figspdf/ratio/ratio-%s-%s.pdf", OS(id), label), "c_DRAW_2D_RATIO", OPT(csize[0]), OPT(csize[1]));

#define DRAW_2D_PIXELS_COMPARE(q)                                       \
  for (std::size_t j = 0; j < nfiles; ++j) {                            \
    if (!j) continue;                                                   \
    pdf_DRAW_2D_PIXELS_COMPARE->prepare();                              \
    hr##q[j]->Draw("colz");                                             \
    watermark();                                                        \
    pdf_DRAW_2D_PIXELS_COMPARE->write(Form("figs/pixel/ratio-%s-l" #q "-%s-%s.png", \
                                           OS(id), label, tags[j].data())); \
  }                                                                     \

  if (OPT(flags) & 0x1) { BPIX1P(DRAW_2D_PIXELS_COMPARE) }
  if (OPT(flags) & 0x2) { FPIX1P(DRAW_2D_PIXELS_COMPARE) }

  pdf_DRAW_2D_PIXELS_COMPARE->close();

  return 0;
}

static const std::vector<varinfo_t> options_tracklet_2d = {
   {
      "eta-phi", {"#eta", "#phi"},
      {"eta1", "phi1"},
      {{1000, -4, 4}, {1000, -4, 4}},
      {600, 600}, 0, sel_, "colz", 10000
   }, {
      "eta-vz", {"#eta", "v_{z}"},
      {"eta1", "vz[1]"},
      {{200, -4, 4}, {200, -20, 20}},
      {600, 600}, 0, sel_, "colz", 10000
   }
};

int map_tracklets(std::vector<varinfo_t> const& options,
      const char* input, const char* label, int opt) {
   TCut fsel = OS(sel);
   fsel = fsel && Form("abs(vz[1])<15 && %s", sel_.c_str());
   fsel *= "weight";

   const char* l0str = OS(label[0]);
   const char* l1str = OS(label[1]);
   const char* idstr = OS(id);

   gStyle->SetOptStat(0);

   TFile* f = new TFile(input, "READ");

#define SETUP_2D_TRACKLETS(q, w)                                              \
   TTree* t##q##w = (TTree*)f->Get("TrackletTree" #q #w);                     \
   TH2D* h##q##w = new TH2D(Form("h" #q #w "%s", idstr),                      \
         Form(";%s (layer " #q "+" #w ");%s (layer " #q "+" #w ")",           \
               l0str, l1str),                                                 \
         OPT(bins[0][0]), OPT(bins[0][1]), OPT(bins[0][2]),                   \
         OPT(bins[1][0]), OPT(bins[1][1]), OPT(bins[1][2]));                  \

   TRKLTS2P(SETUP_2D_TRACKLETS)

   xjjroot::mypdf* pdf_DRAW_2D_TRACKLETS = new xjjroot::mypdf(Form("figspdf/tracklet/tracklet-%s-%s.pdf", OS(id), label), "c_DRAW_2D_TRACKLETS", 600, 600);

#define DRAW_2D_TRACKLETS(q, w)                                               \
   t##q##w->Draw(Form("%s:%s>>h" #q #w "%s", OS(var[1]), OS(var[0]), idstr),  \
                 fsel, "goff", OPT(maxnevt));                                 \
                                                                              \
   pdf_DRAW_2D_TRACKLETS->prepare();                                          \
   h##q##w->Draw(OS(gopt));                                                   \
   watermark();                                                         \
                                                                              \
   pdf_DRAW_2D_TRACKLETS->write(Form("figs/tracklet/tracklet-%s-t" #q #w "-%s.png", \
                                     OS(id), label));                   \
   // delete c##q##w;                                                            \

   TRKLTS2P(DRAW_2D_TRACKLETS)

   pdf_DRAW_2D_TRACKLETS->close();

   TFile* fout = new TFile(Form("data/%s.root", label), "update");
#define SAVE_2D_TRACKLETS(q, w) h##q##w->Write("", TObject::kOverwrite);
   TRKLTS2P(SAVE_2D_TRACKLETS)
   fout->Close();

   return 0;
}

static const std::vector<varinfo_t> options_compare_tracklet_2d = {
   {
      "eta-phi", {"#eta", "#phi"},
      {"eta1", "phi1"},
      {{1000, -4, 4}, {1000, -4, 4}},
      {600, 600}, 0, sel_, "colz", 10000
   }
};

int compare_map_tracklets(std::vector<varinfo_t> const& options,
                       const char* config, const char* label, int opt) {

  const char* idstr = OS(id);

  configurer* conf = new configurer(config);
  auto tags = conf->get<std::vector<std::string>>("tags");
  std::size_t nfiles = tags.size();

  TFile* f[nfiles];
  for (std::size_t j = 0; j < nfiles; ++j)
    f[j] = new TFile(Form("data/%s.root", tags[j].data()), "read");

#define SETUP_2D_TRACKLETS_COMPARE(q, w)                                \
  TH2D* h##q##w[nfiles]; TH2D* hr##q##w[nfiles];                        \
  for (std::size_t j = 0; j < nfiles; ++j) {                            \
    h##q##w[j] = (TH2D*)f[j]->Get(Form("h" #q #w "%s", idstr));         \
    h##q##w[j]->SetName(Form("%s-f%d", h##q##w[j]->GetName(), j));      \
    h##q##w[j]->Scale(1. / h##q##w[j]->Integral());                     \
    hr##q##w[j] = (TH2D*)h##q##w[0]->Clone(Form("hr" #q #w "%s-f%d", idstr, j)); \
    hr##q##w[j]->Divide(h##q##w[j]);                                    \
    hr##q##w[j]->SetMinimum(0);                                         \
    hr##q##w[j]->SetMaximum(2);                                         \
  }                                                                     \

  TRKLTS2P(SETUP_2D_TRACKLETS_COMPARE)

    xjjroot::mypdf* pdf_DRAW_2D_TRACKLETS_COMPARE = new xjjroot::mypdf(Form("figspdf/ratio/ratio-%s-%s.pdf", OS(id), label), "c_DRAW_2D_RATIO",  OPT(csize[0]), OPT(csize[1]));

#define DRAW_2D_TRACKLETS_COMPARE(q, w)                                 \
  for (std::size_t j = 0; j < nfiles; ++j) {                            \
    if (!j) continue;                                                   \
    pdf_DRAW_2D_TRACKLETS_COMPARE->prepare();                           \
    hr##q##w[j]->Draw("colz");                                          \
    watermark();                                                        \
                                                                        \
    pdf_DRAW_2D_TRACKLETS_COMPARE->write(Form("figs/tracklet/ratio-%s-t" #q #w "-%s-%s.png", \
                                              OS(id), label, tags[j].data())); \
  }                                                                     \
  
  TRKLTS2P(DRAW_2D_TRACKLETS_COMPARE)

  pdf_DRAW_2D_TRACKLETS_COMPARE->close();

  return 0;
}

int main(int argc, char* argv[]) {
   std::vector<std::vector<varinfo_t> const*> options = {
      &options_pixel_1d, &options_tracklet_1d,
      &options_pixel_2d, &options_tracklet_2d,
      &options_compare_pixel_2d, &options_compare_tracklet_2d
   };

   std::vector<int (*)(std::vector<varinfo_t> const&,
         const char*, const char*, int)> delegates = {
      compare_pixels, compare_tracklets,
      map_pixels, map_tracklets,
      compare_map_pixels, compare_map_tracklets
   };

   std::vector<std::string> usagestrs = {
      "config", "config",
      "pixel", "tracklet",
      "config", "config"
   };

   if (argc > 1) {
      int mode = atoi(argv[1]);

      if (argc == 4) {
         for (std::size_t i = 0; i < options[mode]->size(); ++i)
            (*delegates[mode])(*options[mode], argv[2], argv[3], i);
      } else if (argc > 4) {
         for (int i = 4; i < argc; ++i)
            (*delegates[mode])(*options[mode], argv[2], argv[3], atoi(argv[i]));
      } else {
         printf("usage: ./harvest_hists [%i] [%s] [label] (opt)\n",
               mode, usagestrs[mode].data());
         return 1;
      }
   } else {
      printf("usage: ./harvest_hists [mode]\n");
      return 1;
   }
}
