#ifndef COSMETICS_H
#define COSMETICS_H

#include "TColor.h"

#define COLOUR0   TColor::GetColor("#515151") // gray
#define COLOUR1   TColor::GetColor("#f2777a") // red
#define COLOUR2   TColor::GetColor("#e99960") // orange
#define COLOUR3   TColor::GetColor("#ffcc66") // yellow
#define COLOUR4   TColor::GetColor("#99cc99") // green
#define COLOUR5   TColor::GetColor("#6699cc") // blue
#define COLOUR6   TColor::GetColor("#9999cc") // purple
#define COLOUR7   TColor::GetColor("#db2d20") // sat red
#define COLOUR8   TColor::GetColor("#01a252") // sat green
#define COLOUR9   TColor::GetColor("#5C76DA") // sat blue

static const std::vector<int> colours = {
   COLOUR1, COLOUR2, COLOUR3,
   COLOUR4, COLOUR5, COLOUR6,
   COLOUR7, COLOUR8, COLOUR9
};
static const std::size_t ncolours = colours.size();

static const std::vector<int> markers = {
   24, 25, 26,
   30, 32, 46,
   48, 49, 43
};


const char* _t_collision = "OO (5.36 TeV)";

#include "TLatex.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TCanvas.h"

#include "xjjrootuti.h"

void watermark(bool ismc = false, float xshift=0, float yshift=0, float tsize=0.04) {
  auto t_left = ismc ? xjjroot::CMS::simulation : xjjroot::CMS::preliminary;
  xjjroot::drawCMSleft(t_left, xshift, yshift, tsize);
  xjjroot::drawCMSright(_t_collision, 0, 0, tsize);
}

void watermark_inner(bool ismc = false) {
  watermark(ismc, 0.05, -0.1);
}

void watermark_inner_2p(bool ismc = false) {
  watermark(ismc, 0.05, -0.15, 0.065);
}

template<typename T>
void tstyle(T* o, int font, float size) {
   o->SetTextFont(font);
   o->SetTextSize(size);
}

template<typename T>
void tstyle(T* o, int font, float size, int align) {
   o->SetTextFont(font);
   o->SetTextSize(size);
   o->SetTextAlign(align);
}

void hstyle(TH1* h, int style, int colour) {
   h->SetStats(0);
   h->SetMarkerStyle(style);
   h->SetMarkerColor(colour);
   h->SetLineColor(colour);
}

void hstyle(TH1* h, int style, int colour, float size) {
   h->SetStats(0);
   h->SetMarkerSize(size);
   h->SetMarkerStyle(style);
   h->SetMarkerColor(colour);
   h->SetLineColor(colour);
}

// void htitle(TH1* h, const char* title) {
void htitle(TH1* h, std::string title) {
   h->SetStats(0);
   h->SetTitle(title.c_str());
   h->GetXaxis()->CenterTitle();
   h->GetYaxis()->CenterTitle();
}

void hrange(TH1* h, float ymin, float ymax) {
   h->SetStats(0);
   h->SetAxisRange(ymin, ymax, "Y");
}

void hndiv(TH1* h, int xndiv, int yndiv) {
   h->SetStats(0);
   h->SetNdivisions(xndiv, "X");
   h->SetNdivisions(yndiv, "Y");
}

void hlfont(TH1* h, int xfont, int yfont) {
   h->SetStats(0);
   h->SetLabelFont(xfont, "X");
   h->SetLabelFont(yfont, "Y");
}

void hlsize(TH1* h, float xsize, float ysize) {
   h->SetStats(0);
   h->SetLabelSize(xsize, "X");
   h->SetLabelSize(ysize, "Y");
}

void hloffset(TH1* h, float xoffset, float yoffset) {
   h->SetStats(0);
   h->SetLabelOffset(xoffset, "X");
   h->SetLabelOffset(yoffset, "Y");
}

void htfont(TH1* h, int xfont, int yfont) {
   h->SetStats(0);
   h->SetTitleFont(xfont, "X");
   h->SetTitleFont(yfont, "Y");
}

void htsize(TH1* h, float xsize, float ysize) {
   h->SetStats(0);
   h->SetTitleSize(xsize, "X");
   h->SetTitleSize(ysize, "Y");
}

void htoffset(TH1* h, float xoffset, float yoffset) {
   h->SetStats(0);
   h->SetTitleOffset(xoffset, "X");
   h->SetTitleOffset(yoffset, "Y");
}

void htksize(TH1* h, float xsize, float ysize) {
   h->SetStats(0);
   h->SetTickLength(xsize, "X");
   h->SetTickLength(ysize, "Y");
}

void hline(TH1* h, int style, int colour, float width) {
   h->SetStats(0);
   h->SetLineStyle(style);
   h->SetLineColor(colour);
   h->SetLineWidth(width);
}

void rstyle(TH1* h) {
   hlsize(h, 0.1, 0.1);
   htsize(h, 0.1, 0.1);
   h->SetTitleOffset(0.5, "Y");
   h->SetNdivisions(205, "Y");
   h->GetYaxis()->CenterTitle();
}

void rformat(TH1* h, float ymin, float ymax) {
   h->SetStats(0);
   h->SetYTitle("ratio");
   h->SetAxisRange(ymin, ymax, "Y");

   rstyle(h);
}

void gfill(TGraph* g, int colour, int alpha) {
   g->SetFillColorAlpha(colour, alpha);
   g->SetFillStyle(1001);
}

void gstyle(TGraph* g, int style, int colour) {
   g->SetMarkerStyle(style);
   g->SetMarkerColor(colour);
   g->SetLineColor(0);
   g->SetLineWidth(0);
   g->SetFillStyle(1001); 
   g->SetFillColorAlpha(colour, 0.4);
}

void gstyle(TGraph* g, int style, int colour, float size) {
   g->SetMarkerSize(size);
   g->SetMarkerStyle(style);
   g->SetMarkerColor(colour);
   g->SetLineColor(0);
   g->SetLineWidth(0);
   // g->SetLineColor(colour);
   g->SetFillColorAlpha(colour, 0.4);
}

void atstyle(TGaxis* a, int font, float size, float offset) {
   a->SetTitleFont(font);
   a->SetTitleSize(size);
   a->SetTitleOffset(offset);
}

void alstyle(TGaxis* a, int font, float size, float offset) {
   a->SetLabelFont(font);
   a->SetLabelSize(size);
   a->SetLabelOffset(offset);
}

void cmargin(TCanvas* c1, float tm, float bm, float lm, float rm) {
   c1->SetTopMargin(tm);
   c1->SetBottomMargin(bm);
   c1->SetLeftMargin(lm);
   c1->SetRightMargin(rm);
}

void lstyle(TLegend* l, int font, float size) {
   l->SetBorderSize(0);
   l->SetFillStyle(0);

   tstyle(l, font, size);
}

template<typename T>
void lestyle(T* o, int colour, float alpha) {
   o->SetFillColorAlpha(colour, alpha);
   o->SetLineColorAlpha(0, 0);
}

template<typename T>
void lestyle(T* o, int colour, float alpha, int style) {
   o->SetFillColorAlpha(colour, alpha);
   o->SetFillStyle(style);
   o->SetLineColor(colour);
   o->SetLineStyle(2);
   o->SetLineWidth(2);
}

#endif  /* COSMETICS_H */
