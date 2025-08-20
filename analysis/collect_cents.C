#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TGaxis.h"

#include "include/cosmetics.h"
#include "include/measurements.h"
#include "include/xjjcuti.h"
#include "include/xjjmypdf.h"
#include "include/tool.h"
#include "include/hepdata.h"

#define NCENT   20
#define OFFSET  6 // up to 70%

#include "include/mc.h"

#define COLOR_PbPb_2 COLOUR5
#define COLOR_PbPb_5 COLOUR4
#define COLOR_PbPb_53_1 COLOUR10
#define COLOR_PbPb_53_2 TColor::GetColor("#A7C5A5") 
#define COLOR_XeXe_5_1 COLOUR2 // cms
#define COLOR_XeXe_5_2 COLOUR1 // alice
#define COLOR_AuAu_2 COLOUR6
#define COLOR_CuCu_2 COLOUR9

const float _2a = 32;
// https://indico.cern.ch/event/1578825/contributions/6652811/attachments/3118529/5529753/o_cent_15aug25.pdf#page=15
const float npart[NCENT] = {
  //95-100,  90-95,   85-90,   80-85,   75-80
  0,         0,       0,       0,       4.537,
  //70-75,   65-70,   60-65,   55-60,   50-55,
  5.142,     5.858,   6.685,   7.671,   8.813,
  //45-50,   40-45,   35-40,   30-35,   25-30,
  10.096,    11.553,  13.152,  14.890,  16.743,
  //20-25,   15-20,   10-15,   5-10,    0-5
  18.660,    20.582,  22.447,  24.251,  26.419
};

const float nparterr[NCENT] = {
  //95-100,  90-95,  85-90,  80-85,  75-80
  0,         0,      0,      0,      0.240,
  //70-75,   65-70,  60-65,  55-60,  50-55,
  0.270,     0.303,  0.312,  0.320,  0.326,
  //45-50,   40-45,  35-40,  30-35,  25-30,
  0.353,     0.379,  0.379,  0.360,  0.345,
  //20-25,   15-20,  10-15,  5-10,   0-5
  0.349,     0.356,  0.359,  0.369,  0.397
};

const float ncoll[NCENT] = {
  //95-100,  90-95,   85-90,   80-85,   75-80
  0,         0,       0,       0,       3.246,
  //70-75,   65-70,   60-65,   55-60,   50-55,
  3.847,     4.586,   5.479,   6.597,   7.964,
  //45-50,   40-45,   35-40,   30-35,   25-30,
  9.596,    11.574,  13.912,  16.675,  19.911,
  //20-25,   15-20,   10-15,   5-10,    0-5
  23.619,    27.735,  32.142,  36.865,  43.695
};

const float ncollerr[NCENT] = {
  //95-100,  90-95,  85-90,  80-85,  75-80
  0,         0,      0,      0,      0.239,
  //70-75,   65-70,  60-65,  55-60,  50-55,
  0.280,     0.327,  0.353,  0.384,  0.418,
  //45-50,   40-45,  35-40,  30-35,  25-30,
  0.482,     0.554,  0.602,  0.627,  0.660,
  //20-25,   15-20,  10-15,  5-10,   0-5
  0.727,     0.801,  0.860,  0.939,  1.096
};

void printnpart();
template <class T> void cleangrzero(T* gr);
void drawdNdeta(xjjroot::mypdf& pdf, std::string tag);
void moveleg_and_draw(TLegend* leg, float x1=-1, float y2=-1);

int collect_cents(std::string tag="362294.cgm.epos.m.v2") {
  auto label = tag;
  bool ismc = xjjc::str_contains(tag, "CLOSE");

  constexpr int ntotal = NCENT - OFFSET;

  // TFile* fout = new TFile(Form("output/centrality-%s.root", label),
  //                         "recreate");

  init_mc_cent();
  
  auto* g = new TGraphErrors(ntotal); g->SetName("g");
  xjjana::grzero(g); 
  gstyle(g, 21, COLOUR0, 1.4); g->SetFillColorAlpha(COLOUR0, 0.4);
  auto* g_n2a = (TGraphErrors*)g->Clone("g_n2a"); 
  gstyle(g_n2a, 21, COLOUR0, 1.4); g_n2a->SetFillColorAlpha(COLOUR0, 0.3);
  auto* g_nnpart_x_npart = (TGraphErrors*)g->Clone("g_nnpart_x_npart"); 
  gstyle(g_nnpart_x_npart, 21, COLOUR0, 1.4); g_nnpart_x_npart->SetFillColorAlpha(COLOUR0, 0.3);
  auto* g_nnpart_x_npart_err = new TGraph(2 * ntotal + 2);
  xjjana::grzero(g_nnpart_x_npart_err);
  gstyle(g_nnpart_x_npart_err, 21, COLOUR0, 1.4); g_nnpart_x_npart_err->SetFillColorAlpha(COLOUR0, 0.2);
  auto* g_nnpart_x_npart2a = (TGraphErrors*)g->Clone("g_nnpart_x_npart2a"); 
  gstyle(g_nnpart_x_npart2a, 21, COLOUR0, 1.4); g_nnpart_x_npart2a->SetFillColorAlpha(COLOUR0, 0.3);
  auto* g_nnpart_x_npart2a_err = new TGraph(2 * ntotal + 2);
  xjjana::grzero(g_nnpart_x_npart2a_err);
  gstyle(g_nnpart_x_npart2a_err, 21, COLOUR0, 1.4); g_nnpart_x_npart2a_err->SetFillColorAlpha(COLOUR0, 0.2);
  auto* g_nnpart_x_ncollnpart = (TGraphErrors*)g->Clone("g_nnpart_x_ncollnpart"); 
  gstyle(g_nnpart_x_ncollnpart, 21, COLOUR0, 1.4); g_nnpart_x_ncollnpart->SetFillColorAlpha(COLOUR0, 0.3);
  auto* g_nnpart_x_ncollnpart_err = new TGraph(2 * ntotal + 2);
  xjjana::grzero(g_nnpart_x_ncollnpart_err);
  gstyle(g_nnpart_x_ncollnpart_err, 21, COLOUR0, 1.4); g_nnpart_x_ncollnpart_err->SetFillColorAlpha(COLOUR0, 0.2);
  auto* g_nncoll = (TGraphErrors*)g->Clone("g_nncoll"); 
  gstyle(g_nncoll, 21, COLOUR0, 1.4); g_nncoll->SetFillColorAlpha(COLOUR0, 0.3);
  auto* g_nncoll_err = new TGraph(2 * ntotal + 2);
  xjjana::grzero(g_nncoll_err);
  gstyle(g_nncoll_err, 21, COLOUR0, 1.4); g_nncoll_err->SetFillColorAlpha(COLOUR0, 0.2);

  auto* g_n2a_x_npart2a = (TGraphErrors*)g->Clone("g_n2a_x_npart2a"); 
  gstyle(g_n2a_x_npart2a, 21, COLOUR0, 1.4); g_n2a_x_npart2a->SetFillColorAlpha(COLOUR0, 0.4);
  g_n2a_x_npart2a->SetLineColor(COLOUR0);
  g_n2a_x_npart2a->SetLineWidth(1);
  g_n2a_x_npart2a->SetLineStyle(2);

  for (int c = OFFSET; c < NCENT; c+=1) {
    auto* f = new TFile(Form("results/results-%s.s.%i.%i.root",
                              label.c_str(), c, c + 1));
    auto* h = (TH1F*)f->Get("hsyst");
    int nbins = h->GetNbinsX();
    float midy = h->GetBinContent((nbins + 1) / 2);
    // float midyerr = (hsymhigh->GetBinContent((nbins + 1) / 2) - hsymlow->GetBinContent((nbins + 1) / 2)) / 2.;
    float midyerr = h->GetBinError((nbins + 1) / 2);

    int cindex = c - OFFSET;
    float xcent = 5 * c + 2.5;
    g->SetPoint(cindex, 5 * c + 2.5, midy);
    g->SetPointError(cindex, 0, midyerr);

    g_n2a->SetPoint(cindex, 5 * c + 2.5, midy / _2a);
    g_n2a->SetPointError(cindex, 0, midyerr / _2a);

    for (auto& im : _m_mc)
      fill_mc_cent(im, f, cindex);
    
    float mnpart = npart[c]; float mnparterr = nparterr[c];
    float mncoll = ncoll[c]; float mncollerr = ncollerr[c];

    if (mnpart == 0 || mncoll == 0) continue;

    float mncollnpart = mncoll / mnpart;
    float mncollnparterr = ((mncoll + mncollerr) / (mnpart - mnparterr) - (mncoll - mncollerr) / (mnpart + mnparterr)) / 2.;
    
    switch (cindex) {
    case 0:
      g_nnpart_x_npart_err->SetPoint(0, mnpart - mnparterr,
                     (midy - midyerr) / (mnpart - mnparterr));
      g_nnpart_x_npart2a_err->SetPoint(0, (mnpart - mnparterr) / _2a,
                       (midy - midyerr) / (mnpart - mnparterr));
      g_nnpart_x_ncollnpart_err->SetPoint(0, mncollnpart - mncollnparterr,
                       (midy - midyerr) / (mnpart + mnparterr));
      break;
    case ntotal - 1:
      g_nnpart_x_npart_err->SetPoint(ntotal + 1, mnpart + mnparterr,
                     (midy + midyerr) / (mnpart + mnparterr));
      g_nnpart_x_npart2a_err->SetPoint(ntotal + 1, (mnpart + mnparterr) / _2a,
                       (midy + midyerr) / (mnpart + mnparterr));
      g_nnpart_x_ncollnpart_err->SetPoint(ntotal + 1, mncollnpart + mncollnparterr,
                       (midy + midyerr) / (mnpart + mnparterr));
      break;
    }

    g_nnpart_x_npart->SetPoint(cindex, mnpart, midy / mnpart);
    g_nnpart_x_npart->SetPointError(cindex, mnparterr, midyerr / mnpart);
    g_nnpart_x_npart_err->SetPoint(cindex + 1, mnpart - mnparterr,
                   (midy + midyerr) / (mnpart - mnparterr));
    g_nnpart_x_npart_err->SetPoint(2 * ntotal - cindex + 1, mnpart + mnparterr,
                   (midy - midyerr) / (mnpart + mnparterr));

    g_nnpart_x_npart2a->SetPoint(cindex, mnpart / _2a, midy / mnpart);
    g_nnpart_x_npart2a->SetPointError(cindex, mnparterr / _2a, midyerr / mnpart);
    g_nnpart_x_npart2a_err->SetPoint(cindex + 1, (mnpart - mnparterr) / _2a,
                     (midy + midyerr) / (mnpart - mnparterr));
    g_nnpart_x_npart2a_err->SetPoint(2 * ntotal - cindex + 1, (mnpart + mnparterr) / _2a,
                     (midy - midyerr) / (mnpart + mnparterr));
    
    g_nnpart_x_ncollnpart->SetPoint(cindex, mncollnpart, midy / mnpart);
    g_nnpart_x_ncollnpart->SetPointError(cindex, mncollnparterr, midyerr / mnpart);
    g_nnpart_x_ncollnpart_err->SetPoint(cindex + 1, mncollnpart - mncollnparterr,
                     (midy + midyerr) / (mnpart - mnparterr));
    g_nnpart_x_ncollnpart_err->SetPoint(2 * ntotal - cindex + 1, mncollnpart + mncollnparterr,
                     (midy - midyerr) / (mnpart + mnparterr));

    g_n2a_x_npart2a->SetPoint(cindex, mnpart / _2a, midy / _2a);
    g_n2a_x_npart2a->SetPointError(cindex, mnparterr / _2a, midyerr / _2a);
  }

  cleangrzero(g_nnpart_x_npart);
  cleangrzero(g_nnpart_x_npart_err);
  cleangrzero(g_nnpart_x_npart2a);
  cleangrzero(g_nnpart_x_npart2a_err);
  cleangrzero(g_nnpart_x_ncollnpart);
  cleangrzero(g_nnpart_x_ncollnpart_err);
  cleangrzero(g_n2a_x_npart2a);
  
  xjjroot::printgrvalue(g);
  
  // fout->cd();

  auto* gcms_pbpb_2p76 = cms_pbpb_2p76();
  auto* gcms_pbpb_5p36 = cms_pbpb_5p36();
  auto* galice_pbpb_2p76 = alice_pbpb_2p76();
  auto* galice_pbpb_5p02 = alice_pbpb_5p02();
  auto* galice_pbpb_5p36 = alice_pbpb_5p36();
  auto* gcms_xexe_5p44 = cms_xexe_5p44();
  auto* galice_xexe_5p44 = alice_xexe_5p44();
  auto* gphobos_auau_0p2 = phobos_auau_0p2();
  auto* gphobos_cucu_0p2 = phobos_cucu_0p2();
  gstyle(gcms_pbpb_2p76, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2, 29, COLOR_AuAu_2);
  gstyle(gphobos_cucu_0p2, 43, COLOUR9);

  MAKE_SET_MC();
  
  float hline = 0.038;

  auto* lcms = new TLegend(0., 1.-hline*5, 0.25, 1.);
  xjjroot::setleg(lcms, 0.034);
  lcms->AddEntry((TObject*)0, "#bf{CMS}", NULL);
  lcms->AddEntry(g_nnpart_x_npart, "OO 5.36 TeV", "p");
  lcms->AddEntry((TObject*)0, "(This analysis)", NULL);
  lcms->AddEntry(gcms_pbpb_5p36, "PbPb 5.36 TeV", "p");
  // lcms->AddEntry(gcms_pbpb_2p76, "PbPb 2.76 TeV", "p");
  lcms->AddEntry(gcms_xexe_5p44, "XeXe 5.44 TeV", "p");
  lcms->Draw(); // somehow this is necessary
  
  auto* lalice = new TLegend(0., 1.-hline*4, 0.25, 1.);
  xjjroot::setleg(lalice, 0.034);
  lalice->AddEntry((TObject*)0, "#bf{ALICE}", NULL);
  lalice->AddEntry(galice_pbpb_5p02, "PbPb 5.02 TeV", "p");
  lalice->AddEntry(galice_pbpb_5p36, "PbPb 5.36 TeV", "p");
  // lalice->AddEntry(galice_pbpb_2p76, "PbPb 2.76 TeV", "p");
  lalice->AddEntry(galice_xexe_5p44, "XeXe 5.44 TeV", "p");
  lalice->Draw();

  auto* lphobos = new TLegend(0., 0., 0.25, hline*3);
  xjjroot::setleg(lphobos, 0.034);
  lphobos->AddEntry((TObject*)0, "#bf{PHOBOS}", "");
  lphobos->AddEntry(gphobos_auau_0p2, "AuAu 200 GeV", "p");
  lphobos->AddEntry(gphobos_cucu_0p2, "CuCu 200 GeV", "p");
  lphobos->Draw();

  auto* lnpart = new TLegend(0., 0., 0.25, hline*2);
  xjjroot::setleg(lnpart, 0.034);
  lnpart->AddEntry(g_nnpart_x_npart_err, "(Expt. + #LTN_{part}#GT) unc.", "f");
  lnpart->AddEntry(g_n2a_x_npart2a, "Expt. unc.", "f");
  lnpart->Draw();

  auto* lmc = new TLegend(0., 0., 0.6, hline*4.1);
  xjjroot::setleg(lmc, 0.037);
  lmc->SetNColumns(2);
  lmc->AddEntry((TObject*)0, "", NULL);
  lmc->AddEntry(g_nnpart_x_npart, "Data", "p");
  lmc->AddEntry(ghijing_oo_5p36, get_mc_tex("hijing").c_str(), "l");
  lmc->AddEntry(gangantyr_oo_5p36, get_mc_tex("angantyr").c_str(), "l");
  lmc->AddEntry(gamptnm_oo_5p36, get_mc_tex("amptnm").c_str(), "l");
  lmc->AddEntry(gamptsm_oo_5p36, get_mc_tex("amptsm").c_str(), "l");
  lmc->AddEntry(ghydjet_oo_5p36, get_mc_tex("hydjet").c_str(), "l");
  lmc->AddEntry(gepos_oo_5p36, get_mc_tex("epos").c_str(), "l");
  lmc->Draw();
  
  xjjroot::setgstyle(1);
  
  xjjroot::mypdf pdf(Form("figspdf/results/merged-%s.pdf", label.c_str()), "c", 800, 800);

  pdf.getc()->SetLogy();
  auto* hframe = new TH1F("hframe", "", 1, 0, 100);
  xjjroot::sethempty(hframe, 0, 0);
  hframe->SetLabelOffset(999, "X"); hframe->SetTickLength(0, "X");
  htitle(hframe, ";Centrality (%);" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hrange(hframe, 0.8, 3000);
  auto* axis = new TGaxis(100, hframe->GetYaxis()->GetXmin(), 0, hframe->GetYaxis()->GetXmin(), 0, 100, 510, "-");
  axis->SetLabelOffset(-0.032); axis->SetLabelFont(42);
  axis->SetLabelSize(0.045); 

  //
  pdf.prepare();
  hframe->Draw();
  axis->Draw();
  // gcms_pbpb_2p76->Draw("3 same"); galice_pbpb_2p76->Draw("3 same");
  gcms_pbpb_5p36->Draw("3 same"); galice_pbpb_5p36->Draw("3 same");
  gcms_xexe_5p44->Draw("3 same"); galice_xexe_5p44->Draw("3 same");
  galice_pbpb_5p02->Draw("3 same");
  // gcms_pbpb_2p76->Draw("pX same"); galice_pbpb_2p76->Draw("pX same");
  gcms_pbpb_5p36->Draw("pX same"); galice_pbpb_5p36->Draw("pX same");
  gcms_xexe_5p44->Draw("pX same"); galice_xexe_5p44->Draw("pX same");
  galice_pbpb_5p02->Draw("pX same");
  g->Draw("3 same"); g->Draw("pX same");

  watermark_inner(ismc);  

  // moveleg_and_draw(lcms, 0.58, 0.42);
  // moveleg_and_draw(lalice, 0.22, 0.76);
  moveleg_and_draw(lcms, 0.55, 0.4);
  moveleg_and_draw(lalice, 0.25, lcms->GetY2NDC()-hline);
  
  // Figure 2a
  pdf.write(Form("figs/results/merged-%s-midy-int1.pdf", label.c_str()), "Q");

  auto* h_g = new TH1F("h_g", ";;", 20, 0, 100);
  for (int i=0; i<g->GetN(); i++) {
    auto x = 100-g->GetPointX(i), y = g->GetPointY(i), ey = g->GetErrorY(i);
    auto ibin = h_g->FindBin(x);
    h_g->SetBinContent(ibin, y);
    h_g->SetBinError(ibin, ey);
  }
  std::ofstream outf_Table_2a("hepdatas/Table_2a.yaml");
  xjjroot::hepdata hep_Table_2a(h_g, 0,
                                {"CENTRALITY", "'$\\%$'", ""},
                                {"'$"+_t_hep_dNdeta+"$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_2a.print(outf_Table_2a, 0, 1, 0, 80);

  //
  pdf.prepare();
  hrange(hframe, 2., 300);
  hframe->Draw();
  axis->Draw();
  DRAW_MC();
  g->Draw("3 same"); g->Draw("pX same");
  watermark_inner(ismc);  
  moveleg_and_draw(lmc, 0.28, 0.35);
  pdf.write("", "Q");
 
  pdf.getc()->SetLogy(0);

  auto* gcms_pbpb_2p76_n2a = cms_pbpb_2p76_n2a();
  auto* gcms_pbpb_5p36_n2a = cms_pbpb_5p36_n2a();
  auto* galice_pbpb_2p76_n2a = alice_pbpb_2p76_n2a();
  auto* galice_pbpb_5p02_n2a = alice_pbpb_5p02_n2a();
  auto* galice_pbpb_5p36_n2a = alice_pbpb_5p36_n2a();
  auto* gcms_xexe_5p44_n2a = cms_xexe_5p44_n2a();
  auto* galice_xexe_5p44_n2a = alice_xexe_5p44_n2a();
  auto* gphobos_auau_0p2_n2a = phobos_auau_0p2_n2a();
  auto* gphobos_cucu_0p2_n2a = phobos_cucu_0p2_n2a();
  gstyle(gcms_pbpb_2p76_n2a, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36_n2a, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76_n2a, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02_n2a, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36_n2a, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44_n2a, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44_n2a, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2_n2a, 29, COLOR_AuAu_2, 1.7);
  gstyle(gphobos_cucu_0p2_n2a, 43, COLOUR9, 1.7);

  pdf.prepare();
  auto* hframe_n2a = new TH1F("hframe_n2a", "", 1, 0, 100);
  xjjroot::sethempty(hframe_n2a, 0, 0);
  hframe_n2a->SetLabelOffset(999, "X"); hframe_n2a->SetTickLength(0, "X");
  htitle(hframe_n2a, ";Centrality (%);" + _t_2a + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_n2a->SetTitleOffset(1.3, "Y");
  hrange(hframe_n2a, 1.e-5, 5.5); hframe_n2a->Draw();
  auto* axis2 = new TGaxis(100, 0, 0, 0, 0, 100, 510, "-");
  axis2->SetLabelOffset(-0.032); axis2->SetLabelFont(42);
  axis2->SetLabelSize(0.045); axis2->Draw();
 
  // gcms_pbpb_2p76_n2a->Draw("3 same"); galice_pbpb_2p76_n2a->Draw("3 same");
  gcms_pbpb_5p36_n2a->Draw("3 same"); galice_pbpb_5p36_n2a->Draw("3 same");
  gcms_xexe_5p44_n2a->Draw("3 same"); galice_xexe_5p44_n2a->Draw("3 same");
  galice_pbpb_5p02_n2a->Draw("3 same");
  // gphobos_auau_0p2_n2a->Draw("3 same"); gphobos_cucu_0p2_n2a->Draw("3 same");
  g_n2a->Draw("3 same");  
  // gcms_pbpb_2p76_n2a->Draw("pX same"); galice_pbpb_2p76_n2a->Draw("pX same");
  gcms_pbpb_5p36_n2a->Draw("pX same"); galice_pbpb_5p36_n2a->Draw("pX same");
  gcms_xexe_5p44_n2a->Draw("pX same"); galice_xexe_5p44_n2a->Draw("pX same");
  galice_pbpb_5p02_n2a->Draw("pX same");
  // gphobos_auau_0p2_n2a->Draw("pX same"); gphobos_cucu_0p2_n2a->Draw("pX same");
  g_n2a->Draw("pX same");
  xjjroot::printgrvalue(g_n2a);

  watermark_inner(ismc);

  moveleg_and_draw(lcms, 0.22, 0.75);
  moveleg_and_draw(lalice, 0.22, lcms->GetY1NDC() - 0.5*hline);
  
  // Figure 2b
  pdf.write(Form("figs/results/merged-%s-midy2a-int1.pdf", label.c_str()), "Q");
  auto* h_g_n2a = new TH1F("h_g_n2a", ";;", 20, 0, 100);
  for (int i=0; i<g_n2a->GetN(); i++) {
    auto x = 100-g_n2a->GetPointX(i), y = g_n2a->GetPointY(i), ey = g_n2a->GetErrorY(i);
    auto ibin = h_g_n2a->FindBin(x);
    h_g_n2a->SetBinContent(ibin, y);
    h_g_n2a->SetBinError(ibin, ey);
  }
  std::ofstream outf_Table_2b("hepdatas/Table_2b.yaml");
  xjjroot::hepdata hep_Table_2b(h_g_n2a, 0,
                                {"CENTRALITY", "'$\\%$'", ""},
                                {"'$"+_t_hep_dNdeta+"/2A$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_2b.print(outf_Table_2b, 0, 3, 0, 80);

  pdf.prepare();

  hframe_n2a->Draw();
  axis2->Draw();
  MAKE_SET_MC(_n2a);
  DRAW_MC(_n2a);
  g_n2a->Draw("3 same"); g_n2a->Draw("pX same");
  watermark_inner(ismc);

  xjjroot::rewidthleg(lmc, 1);
  xjjroot::autolegndraw(lmc, 0.25, 0.80);

  pdf.write("", "Q");
  
  //  
  auto* gcms_pbpb_2p76_nnpart_x_npart = cms_pbpb_2p76_nnpart_x_npart();
  auto* gcms_pbpb_5p36_nnpart_x_npart = cms_pbpb_5p36_nnpart_x_npart();
  auto* galice_pbpb_2p76_nnpart_x_npart = alice_pbpb_2p76_nnpart_x_npart();
  auto* galice_pbpb_5p02_nnpart_x_npart = alice_pbpb_5p02_nnpart_x_npart();
  auto* galice_pbpb_5p36_nnpart_x_npart = alice_pbpb_5p36_nnpart_x_npart();
  auto* gcms_xexe_5p44_nnpart_x_npart = cms_xexe_5p44_nnpart_x_npart();
  auto* galice_xexe_5p44_nnpart_x_npart = alice_xexe_5p44_nnpart_x_npart();
  auto* gphobos_auau_0p2_nnpart_x_npart = phobos_auau_0p2_nnpart_x_npart();
  auto* gphobos_cucu_0p2_nnpart_x_npart = phobos_cucu_0p2_nnpart_x_npart();
  gstyle(gcms_pbpb_2p76_nnpart_x_npart, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36_nnpart_x_npart, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76_nnpart_x_npart, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02_nnpart_x_npart, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36_nnpart_x_npart, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44_nnpart_x_npart, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44_nnpart_x_npart, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2_nnpart_x_npart, 29, COLOR_AuAu_2, 1.7);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_nnpart_x_npart, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart, 45, COLOUR6);

  pdf.prepare();
  auto* hframe_nnpart_x_npart = new TH1F("hframe_nnpart_x_npart", "", 1, -20, 420);
  xjjroot::sethempty(hframe_nnpart_x_npart, 0, 0);
  htitle(hframe_nnpart_x_npart, ";" + _t_npart + ";" + _t_1npart + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_nnpart_x_npart->SetTitleOffset(1.3, "Y");
  hrange(hframe_nnpart_x_npart, 0, 6); hframe_nnpart_x_npart->Draw();

  // gcms_pbpb_2p76_nnpart_x_npart->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart->Draw("3 same");
  gcms_pbpb_5p36_nnpart_x_npart->Draw("3 same"); galice_pbpb_5p36_nnpart_x_npart->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("3 same");
  // gphobos_auau_0p2_nnpart_x_npart->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("3 same");
  g_nnpart_x_npart_err->Draw("f");
  g_nnpart_x_npart->Draw("3 same");
  xjjana::drawgroutline(g_nnpart_x_npart, COLOUR0, 2, 1);
  // gcms_pbpb_2p76_nnpart_x_npart->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart->Draw("pX same");
  gcms_pbpb_5p36_nnpart_x_npart->Draw("pX same"); galice_pbpb_5p36_nnpart_x_npart->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("pX same");
  // gphobos_auau_0p2_nnpart_x_npart->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("pX same");
  g_nnpart_x_npart->Draw("pX same");
  xjjroot::printgrvalue(g_nnpart_x_npart);

  watermark_inner(ismc);

  moveleg_and_draw(lnpart, 0.55, 0.55);
  moveleg_and_draw(lcms, 0.55, 0.4);
  moveleg_and_draw(lalice, 0.25, lcms->GetY2NDC()-hline);
  
  // Figure 3a
  pdf.write(Form("figs/results/merged-%s-midynorm-int1.pdf", label.c_str()), "Q");
  std::ofstream outf_Table_3a("hepdatas/Table_3a.yaml");
  xjjroot::hepdata hep_Table_3a(g_nnpart_x_npart, 0,
                                {"'$N_{part}$'", "", ""},
                                {"'$"+_t_hep_dNdeta+"/N_{part}$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_3a.print(outf_Table_3a, 1, 2);
 
  auto* hframe_nnpart_x_npart_zoom = new TH1F("hframe_nnpart_x_npart_zoom", "", 1, 1, 32);
  xjjroot::sethempty(hframe_nnpart_x_npart_zoom, 0, 0);
  htitle(hframe_nnpart_x_npart_zoom, ";" + _t_npart + ";" + _t_1npart + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_nnpart_x_npart_zoom->SetTitleOffset(1.3, "Y");
  hrange(hframe_nnpart_x_npart_zoom, 0, 8); 

  pdf.prepare();
  hframe_nnpart_x_npart_zoom->Draw("axis");
  g_nnpart_x_npart_err->Draw("f same");
  g_nnpart_x_npart->Draw("3 same");
  xjjana::drawgroutline(g_nnpart_x_npart, COLOUR0, 2, 1);
  MAKE_SET_MC(_nnpart_x_npart);
  DRAW_MC(_nnpart_x_npart);
  g_nnpart_x_npart->Draw("pX same");
  watermark_inner(ismc);
  
  moveleg_and_draw(lnpart, 0.23, 0.78);
  xjjroot::rewidthleg(lmc, 2);
  xjjroot::autolegndraw(lmc, 0.28, 0.35);

  pdf.write("", "Q");
  
  //  
  auto* gcms_pbpb_2p76_nnpart_x_npart2a = cms_pbpb_2p76_nnpart_x_npart2a();
  auto* gcms_pbpb_5p36_nnpart_x_npart2a = cms_pbpb_5p36_nnpart_x_npart2a();
  auto* galice_pbpb_2p76_nnpart_x_npart2a = alice_pbpb_2p76_nnpart_x_npart2a();
  auto* galice_pbpb_5p02_nnpart_x_npart2a = alice_pbpb_5p02_nnpart_x_npart2a();
  auto* galice_pbpb_5p36_nnpart_x_npart2a = alice_pbpb_5p36_nnpart_x_npart2a();
  auto* gcms_xexe_5p44_nnpart_x_npart2a = cms_xexe_5p44_nnpart_x_npart2a();
  auto* galice_xexe_5p44_nnpart_x_npart2a = alice_xexe_5p44_nnpart_x_npart2a();
  auto* gphobos_auau_0p2_nnpart_x_npart2a = phobos_auau_0p2_nnpart_x_npart2a();
  auto* gphobos_cucu_0p2_nnpart_x_npart2a = phobos_cucu_0p2_nnpart_x_npart2a();
  gstyle(gcms_pbpb_2p76_nnpart_x_npart2a, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36_nnpart_x_npart2a, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76_nnpart_x_npart2a, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02_nnpart_x_npart2a, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36_nnpart_x_npart2a, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44_nnpart_x_npart2a, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44_nnpart_x_npart2a, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2_nnpart_x_npart2a, 29, COLOR_AuAu_2, 1.7);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart2a, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_nnpart_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  auto* hframe_nnpart_x_npart2a = new TH1F("hframe_nnpart_x_npart2a", "", 1, -0.02, 1);
  xjjroot::sethempty(hframe_nnpart_x_npart2a, 0, 0);
  htitle(hframe_nnpart_x_npart2a, ";" + _t_npart + "" + _t_slash + "2#it{A};" + _t_1npart + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_nnpart_x_npart2a->SetTitleOffset(1.3, "Y");
  hrange(hframe_nnpart_x_npart2a, 0, 6); hframe_nnpart_x_npart2a->Draw();

  // gcms_pbpb_2p76_nnpart_x_npart2a->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("3 same");
  gcms_pbpb_5p36_nnpart_x_npart2a->Draw("3 same"); galice_pbpb_5p36_nnpart_x_npart2a->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("3 same");
  // gphobos_auau_0p2_nnpart_x_npart2a->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("3 same");
  g_nnpart_x_npart2a_err->Draw("f");  
  g_nnpart_x_npart2a->Draw("3 same");  
  xjjana::drawgroutline(g_nnpart_x_npart2a, COLOUR0, 2, 1);
  // gcms_pbpb_2p76_nnpart_x_npart2a->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("pX same");
  gcms_pbpb_5p36_nnpart_x_npart2a->Draw("pX same"); galice_pbpb_5p36_nnpart_x_npart2a->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("pX same");
  // gphobos_auau_0p2_nnpart_x_npart2a->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("pX same");
  g_nnpart_x_npart2a->Draw("pX same");
  xjjroot::printgrvalue(g_nnpart_x_npart2a);
  
  watermark_inner(ismc);
  
  moveleg_and_draw(lnpart, 0.23, 0.78);
  moveleg_and_draw(lcms, 0.55, 0.4);
  moveleg_and_draw(lalice, 0.25, lcms->GetY2NDC()-hline);
  
  pdf.write(Form("figs/results/merged-%s-midynorm2a-int1.pdf", label.c_str()), "Q");

  // Figure 3b
  std::ofstream outf_Table_3b("hepdatas/Table_3b.yaml");
  xjjroot::hepdata hep_Table_3b(g_nnpart_x_npart2a, 0,
                                {"'$N_{part}/2A$'", "", ""},
                                {"'$"+_t_hep_dNdeta+"/N_{part}$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_3b.print(outf_Table_3b, 2, 2);

  //
  pdf.prepare();
  hframe_nnpart_x_npart2a->Draw("axis");
  g_nnpart_x_npart2a_err->Draw("f same");
  g_nnpart_x_npart2a->Draw("3 same");
  xjjana::drawgroutline(g_nnpart_x_npart2a, COLOUR0, 2, 1);
  MAKE_SET_MC(_nnpart_x_npart2a);
  DRAW_MC(_nnpart_x_npart2a);
  g_nnpart_x_npart2a->Draw("pX same");
  watermark_inner(ismc);
  
  moveleg_and_draw(lnpart, 0.23, 0.78);
  xjjroot::autolegndraw(lmc, 0.28, 0.35);

  pdf.write("", "Q");
  
  auto* gcms_pbpb_2p76_nnpart_x_ncollnpart = cms_pbpb_2p76_nnpart_x_ncollnpart();
  auto* gcms_pbpb_5p36_nnpart_x_ncollnpart = cms_pbpb_5p36_nnpart_x_ncollnpart();
  auto* galice_pbpb_2p76_nnpart_x_ncollnpart = alice_pbpb_2p76_nnpart_x_ncollnpart();
  auto* galice_pbpb_5p02_nnpart_x_ncollnpart = alice_pbpb_5p02_nnpart_x_ncollnpart();
  auto* galice_pbpb_5p36_nnpart_x_ncollnpart = alice_pbpb_5p36_nnpart_x_ncollnpart();
  auto* gcms_xexe_5p44_nnpart_x_ncollnpart = cms_xexe_5p44_nnpart_x_ncollnpart();
  auto* galice_xexe_5p44_nnpart_x_ncollnpart = alice_xexe_5p44_nnpart_x_ncollnpart();
  auto* gphobos_auau_0p2_nnpart_x_ncollnpart = phobos_auau_0p2_nnpart_x_ncollnpart();
  auto* gphobos_cucu_0p2_nnpart_x_ncollnpart = phobos_cucu_0p2_nnpart_x_ncollnpart();
  gstyle(gcms_pbpb_2p76_nnpart_x_ncollnpart, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36_nnpart_x_ncollnpart, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76_nnpart_x_ncollnpart, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02_nnpart_x_ncollnpart, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36_nnpart_x_ncollnpart, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44_nnpart_x_ncollnpart, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44_nnpart_x_ncollnpart, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2_nnpart_x_ncollnpart, 29, COLOR_AuAu_2, 1.7);
  gstyle(gphobos_cucu_0p2_nnpart_x_ncollnpart, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_nnpart_x_ncollnpart, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_ncollnpart, 45, COLOUR6);

  //
  pdf.prepare();
  auto* hframe_nnpart_x_ncollnpart = new TH1F("hframe_nnpart_x_ncollnpart", "", 1, 0.5, 5);
  xjjroot::sethempty(hframe_nnpart_x_ncollnpart, 0, 0);
  htitle(hframe_nnpart_x_ncollnpart, ";" + _t_ncoll + _t_slash + _t_npart + ";" + _t_1npart + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_nnpart_x_ncollnpart->SetTitleOffset(1.3, "Y");
  hrange(hframe_nnpart_x_ncollnpart, 0, 6); hframe_nnpart_x_ncollnpart->Draw();

  // gcms_pbpb_2p76_nnpart_x_ncollnpart->Draw("3 same"); galice_pbpb_2p76_nnpart_x_ncollnpart->Draw("3 same");
  gcms_pbpb_5p36_nnpart_x_ncollnpart->Draw("3 same"); // galice_pbpb_5p36_nnpart_x_ncollnpart->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_ncollnpart->Draw("3 same"); // galice_xexe_5p44_nnpart_x_ncollnpart->Draw("3 same");
  // galice_pbpb_5p02_nnpart_x_ncollnpart->Draw("3 same");
  // gphobos_auau_0p2_nnpart_x_ncollnpart->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_ncollnpart->Draw("3 same");
  g_nnpart_x_ncollnpart_err->Draw("f");  
  g_nnpart_x_ncollnpart->Draw("3 same");  
  xjjana::drawgroutline(g_nnpart_x_ncollnpart, COLOUR0, 2, 1);
  // gcms_pbpb_2p76_nnpart_x_ncollnpart->Draw("pX same"); galice_pbpb_2p76_nnpart_x_ncollnpart->Draw("pX same");
  gcms_pbpb_5p36_nnpart_x_ncollnpart->Draw("pX same"); // galice_pbpb_5p36_nnpart_x_ncollnpart->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_ncollnpart->Draw("pX same"); // galice_xexe_5p44_nnpart_x_ncollnpart->Draw("pX same");
  // galice_pbpb_5p02_nnpart_x_ncollnpart->Draw("pX same");
  // gphobos_auau_0p2_nnpart_x_ncollnpart->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_ncollnpart->Draw("pX same");
  g_nnpart_x_ncollnpart->Draw("pX same");
  xjjroot::printgrvalue(g_nnpart_x_ncollnpart);
  
  watermark_inner(ismc);
  
  moveleg_and_draw(lnpart, 0.55, 0.55);
  moveleg_and_draw(lcms, 0.55, 0.4);
  // moveleg_and_draw(lalice, 0.25, lcms->GetY2NDC()-hline);

  // pdf.write(Form("figs/results/merged-%s-midynorm2a-int1.pdf", label.c_str()), "Q");
  pdf.write("", "Q");
  
  //  
  auto* gcms_pbpb_2p76_n2a_x_npart2a = cms_pbpb_2p76_n2a_x_npart2a();
  auto* gcms_pbpb_5p36_n2a_x_npart2a = cms_pbpb_5p36_n2a_x_npart2a();
  auto* galice_pbpb_2p76_n2a_x_npart2a = alice_pbpb_2p76_n2a_x_npart2a();
  auto* galice_pbpb_5p02_n2a_x_npart2a = alice_pbpb_5p02_n2a_x_npart2a();
  auto* galice_pbpb_5p36_n2a_x_npart2a = alice_pbpb_5p36_n2a_x_npart2a();
  auto* gcms_xexe_5p44_n2a_x_npart2a = cms_xexe_5p44_n2a_x_npart2a();
  auto* galice_xexe_5p44_n2a_x_npart2a = alice_xexe_5p44_n2a_x_npart2a();
  auto* gphobos_auau_0p2_n2a_x_npart2a = phobos_auau_0p2_n2a_x_npart2a();
  auto* gphobos_cucu_0p2_n2a_x_npart2a = phobos_cucu_0p2_n2a_x_npart2a();
  gstyle(gcms_pbpb_2p76_n2a_x_npart2a, 22, COLOR_PbPb_2, 1.2);
  gstyle(gcms_pbpb_5p36_n2a_x_npart2a, 22, COLOR_PbPb_53_1, 1.2);
  gstyle(galice_pbpb_2p76_n2a_x_npart2a, 23, COLOR_PbPb_2, 1.2);
  gstyle(galice_pbpb_5p02_n2a_x_npart2a, 20, COLOR_PbPb_5, 1.2);
  gstyle(galice_pbpb_5p36_n2a_x_npart2a, 23, COLOR_PbPb_53_2, 1.2);
  gstyle(gcms_xexe_5p44_n2a_x_npart2a, 47, COLOR_XeXe_5_1, 1.2);
  gstyle(galice_xexe_5p44_n2a_x_npart2a, 34, COLOR_XeXe_5_2, 1.2);
  gstyle(gphobos_auau_0p2_n2a_x_npart2a, 29, COLOR_AuAu_2, 1.7);
  gstyle(gphobos_cucu_0p2_n2a_x_npart2a, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_n2a_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_n2a_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  auto* hframe_n2a_x_npart2a = new TH1F("hframe_n2a_x_npart2a", "", 1, -0.02, 1);
  xjjroot::sethempty(hframe_n2a_x_npart2a, 0, 0);
  htitle(hframe_n2a_x_npart2a, ";" + _t_npart + "" + _t_slash + "2#it{A};" + _t_2a + "#scale[0.5]{ }" + _t_dNdeta + "#scale[0.5]{ }" + _t_eta0p5);
  hframe_n2a_x_npart2a->SetTitleOffset(1.3, "Y");
  hrange(hframe_n2a_x_npart2a, 0, 6); hframe_n2a_x_npart2a->Draw();

  // gcms_pbpb_2p76_n2a_x_npart2a->Draw("3 same"); galice_pbpb_2p76_n2a_x_npart2a->Draw("3 same");
  gcms_pbpb_5p36_n2a_x_npart2a->Draw("3 same"); galice_pbpb_5p36_n2a_x_npart2a->Draw("3 same");
  gcms_xexe_5p44_n2a_x_npart2a->Draw("3 same"); galice_xexe_5p44_n2a_x_npart2a->Draw("3 same");
  galice_pbpb_5p02_n2a_x_npart2a->Draw("3 same");
  // gphobos_auau_0p2_n2a_x_npart2a->Draw("3 same"); gphobos_cucu_0p2_n2a_x_npart2a->Draw("3 same");
  g_n2a_x_npart2a->Draw("3 same");  
  // gcms_pbpb_2p76_n2a_x_npart2a->Draw("pX same"); galice_pbpb_2p76_n2a_x_npart2a->Draw("pX same");
  gcms_pbpb_5p36_n2a_x_npart2a->Draw("pX same"); galice_pbpb_5p36_n2a_x_npart2a->Draw("pX same");
  gcms_xexe_5p44_n2a_x_npart2a->Draw("pX same"); galice_xexe_5p44_n2a_x_npart2a->Draw("pX same");
  galice_pbpb_5p02_n2a_x_npart2a->Draw("pX same");
  // gphobos_auau_0p2_n2a_x_npart2a->Draw("pX same"); gphobos_cucu_0p2_n2a_x_npart2a->Draw("pX same");
  g_n2a_x_npart2a->Draw("pX same");
  xjjroot::printgrvalue(g_n2a_x_npart2a);

  watermark_inner(ismc);

  moveleg_and_draw(lcms, 0.22, 0.75);
  moveleg_and_draw(lalice, 0.22, lcms->GetY1NDC() - 0.5*hline);
  
  pdf.write(Form("figs/results/merged-%s-midynorm2a2a-int1.pdf", label.c_str()), "Q");
  
  //
  pdf.prepare();
  hframe_n2a_x_npart2a->Draw("axis");
  g_n2a_x_npart2a->Draw("3 same");
  MAKE_SET_MC(_n2a_x_npart2a);
  DRAW_MC(_n2a_x_npart2a);
  g_n2a_x_npart2a->Draw("pX same");
  watermark_inner(ismc);
  
  xjjroot::rewidthleg(lmc, 1);
  xjjroot::autolegndraw(lmc, 0.25, 0.80);

  pdf.write("", "Q");

  // g->Write("", TObject::kOverwrite);
  // gcms_pbpb_2p76->Write("", TObject::kOverwrite);
  // galice_pbpb_5p02->Write("", TObject::kOverwrite);
  // gphobos_cucu_0p2->Write("", TObject::kOverwrite);
  // gphobos_auau_0p2->Write("", TObject::kOverwrite);

  // g_nnpart_x_npart->Write("", TObject::kOverwrite);
  // g_nnpart_x_npart_err->Write("", TObject::kOverwrite);
  // gcms_pbpb_2p76_nnpart_x_npart->Write("", TObject::kOverwrite);
  // galice_pbpb_5p02_nnpart_x_npart->Write("", TObject::kOverwrite);
  // gphobos_auau_0p2_nnpart_x_npart->Write("", TObject::kOverwrite);
  // gphobos_cucu_0p2_nnpart_x_npart->Write("", TObject::kOverwrite);
  // gcms_pp_13p0_nnpart_x_npart->Write("", TObject::kOverwrite);
  // gcms_ppb_8p16_nnpart_x_npart->Write("", TObject::kOverwrite);

  // fout->Write("", TObject::kOverwrite);
  // fout->Close();

  drawdNdeta(pdf, label);
  
  pdf.close();
  
  return 0;
}

class spectrum {
public:
  // 0x[ncol]
  spectrum(std::string filename, std::string title, float xleg, float yleg, float wcol=0.2, uint8_t option=0x01);
  void style(int color, Style_t ms);
  void setgcolor(std::vector<int> color);
  void draw_leg() { resize_leg(); leg->Draw(); }
  void set_tleg(float tleg) { resize_leg(-1, -1, tleg); }
  void move_leg(float xleg=-1, float yleg=-1) { resize_leg(xleg, yleg); }
  TH1D* hsym;
  TGraphErrors* gsyst;
  std::vector<TGraphErrors*> gh1WGhadron;
  TGraphErrors* gsyst_ratio;
  std::vector<TGraphErrors*> gratio;
  TLegend* leg;
  std::string title;

private:
  void resize_leg(float xleg=-1, float yleg=-1, float tleg=-1);
  float xleg_, yleg_, tleg_, fhleg_, wcol_;
  int ncol_;
};

void spectrum::resize_leg(float xleg, float yleg, float tleg) {
  if (tleg > 0) tleg_ = tleg;
  if (xleg > 0) xleg_ = xleg;
  if (yleg > 0) yleg_ = yleg;
  leg->SetTextSize(tleg_);  
  auto ng = gh1WGhadron.size();
  float hleg = tleg_*fhleg_*std::min((int)std::ceil((ng+2)/ncol_), 6);
  leg->SetY2NDC(yleg_);
  leg->SetY1NDC(yleg_-hleg);
  leg->SetX1NDC(xleg_);
  leg->SetX2NDC(xleg_ + wcol_*ncol_);
}

spectrum::spectrum(std::string filename, std::string title, float xleg, float yleg, float wcol, uint8_t option) :
  xleg_(xleg), yleg_(yleg), wcol_(wcol), tleg_(0.038), fhleg_(1.05) {
  auto* f = new TFile(filename.c_str());
  hsym = xjjroot::gethist<TH1D>(filename + "::hsym");
  gsyst = xjjroot::gethist<TGraphErrors>(filename + "::gsyst");
  ncol_ = (option & 0xF);
  leg = new TLegend(xleg_, yleg_-0.24, xleg_+wcol_*ncol_, yleg_);
  xjjroot::setleg(leg, tleg_);
  if (ncol_ == 1) {
    leg->AddEntry((TObject*)0, Form("#bf{%s}", title.c_str()), NULL);
    leg->AddEntry(gsyst, "Data", "pf");
  } else {
    leg->AddEntry(gsyst, "Data", "pf");
    leg->AddEntry((TObject*)0, Form("#bf{%s}", title.c_str()), NULL);
  }
  gh1WGhadron = combgh1WGhadron(filename, leg, _t_truth);
  leg->SetNColumns(ncol_);
  leg->Draw(); // make SetNDC work
  resize_leg();

  gsyst_ratio = (TGraphErrors*)gsyst->Clone("gsyst_ratio");
  for (int i=0; i<gsyst->GetN(); i++) {
    float bincontent = gsyst->GetPointY(i);
    if (bincontent == 0) { bincontent = 1.; }
    gsyst_ratio->SetPointY(i, gsyst->GetPointY(i) / bincontent);
    gsyst_ratio->SetPointError(i, gsyst->GetErrorX(i), gsyst->GetErrorY(i) / bincontent);
  }

  int nmidy = gsyst->GetN() / 2;
  for (auto gr : gh1WGhadron) {
    gr->SetLineWidth(3);
    auto gra = (TGraphErrors*)gr->Clone(Form("%s_ratio", gr->GetName()));
    float norm = gr->GetPointY(nmidy) / gsyst->GetPointY(nmidy);
    for (int i=0; i<gsyst->GetN(); i++) {
      if (gsyst->GetPointY(i) != 0) {
        gra->SetPointY(i, gr->GetPointY(i) / gsyst->GetPointY(i) / norm);
        gra->SetPointError(i, gr->GetErrorX(i), gr->GetErrorY(i) / gsyst->GetPointY(i) / norm);
      } else {
        gra->SetPointY(i, 0);
      }
    }
    xjjana::rmgrbins(gra);
    gratio.push_back(gra);
  }
  
}

void spectrum::style(int color, Style_t ms) {
  hsym->SetMarkerStyle(ms);
  hsym->SetMarkerColor(color);
  gsyst->SetMarkerStyle(ms);
  gsyst->SetMarkerColor(color);
  gsyst->SetFillColor(color);
  gsyst->SetFillColorAlpha(color, 0.4);
  gsyst->SetLineStyle(0);
  gsyst_ratio->SetMarkerStyle(ms);
  gsyst_ratio->SetMarkerColor(color);
  gsyst_ratio->SetFillColor(color);
  gsyst_ratio->SetFillColorAlpha(color, 0.4);
  gsyst_ratio->SetLineStyle(0);
  for(auto gr : gh1WGhadron) gr->SetLineColor(color);
  for(auto gr : gratio) gr->SetLineColor(color);
}

void spectrum::setgcolor(std::vector<int> color) {
  for (int i=0; i<color.size(); i++) {
    if (color[i] >= 0) { 
      gh1WGhadron[i]->SetLineColor(color[i]);
      gratio[i]->SetLineColor(color[i]);
    } else {
      gh1WGhadron[i]->SetLineColor(abs(color[i]));
      gratio[i]->SetLineColor(abs(color[i]));
      gh1WGhadron[i]->SetLineColorAlpha(abs(color[i]), 0.3);
      gratio[i]->SetLineColorAlpha(abs(color[i]), 0.3);
    }
  }
}

void drawNormtext() {
  xjjroot::drawtex(0.23, 0.85, "Normalized to unity at #eta = 0", 0.09);  
}

void drawdNdeta(xjjroot::mypdf& pdf, std::string tag) {
  bool ismc = xjjc::str_contains(tag, "CLOSE");
  spectrum sp_0_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 0, 20), "Cent. 0 - 100\%", 0.3, 0.4, 0.3, 0x02),
    sp_19_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 19, 20), "Cent. 0 - 5\%", 0.23, 0.62),
    sp_9_10(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 9, 10), "Cent. 50 - 55\%", 0.59, 0.62);
  sp_0_20.style(COLOUR0, 21);
  sp_19_20.style(COLOUR1, 21);
  sp_9_10.style(COLOUR5, 21);

  sp_0_20.setgcolor({COLOR_PbPb_53_1, COLOUR2, COLOUR5, COLOUR6, COLOUR3, COLOUR1});
  
  auto hempty = makehempty(sp_0_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 1.7);
  hempty->SetAxisRange(-3.0, 2.9, "X");
  auto hemptyp1 = makehempty(sp_0_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 1.7);
  hemptyp1->SetAxisRange(-3.0, 2.9, "X");
  hemptyp1->SetMinimum(0.1);
  auto hempty_ratio = (TH1F*)hemptyp1->Clone("hempty_ratio");
  hempty_ratio->GetYaxis()->SetTitle("MC / data");
  hempty_ratio->SetMinimum(0.7); hempty_ratio->SetMaximum(1.3);
  // hempty_ratio->SetAxisRange(-3.0, 2.9, "X");
  auto hempty2 = makehempty(sp_19_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 3, 0.1);
  hempty2->SetAxisRange(-3.0, 2.9, "X");
  auto hempty2p1 = makehempty(sp_19_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 3, 0.1);
  hempty2p1->SetAxisRange(-3.0, 2.9, "X");
  
  xjjroot::setgstyle(1);
  xjjc::sconfig itext("", ",", "&");
#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  watermark_inner(ismc);                                                \
  
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& hh : sp_0_20.gh1WGhadron)
    hh->Draw("c same");
  sp_0_20.gsyst->Draw("2 same");
  sp_0_20.hsym->Draw("p same");
  sp_0_20.draw_leg();
  DRAWTEX;
  pdf.write(Form("figs/results/merged-%s-fulleta-1noratio.pdf", tag.c_str()), "Q");

  sp_0_20.set_tleg(0.038*1.6);
  sp_0_20.move_leg(-1, 0.3);

  // Paper figure
  pdf.prepare();
  TPad *p1, *p2;
  xjjroot::twopads(pdf.getc(), p1, p2, hemptyp1, hempty_ratio);
  p1->cd();
  for(auto& hh : sp_0_20.gh1WGhadron)
    hh->Draw("c same");
  sp_0_20.gsyst->Draw("2 same");
  sp_0_20.hsym->Draw("p same");
  sp_0_20.leg->Draw();
  watermark_inner_2p(ismc);
  p2->cd();
  drawNormtext();
  sp_0_20.gsyst_ratio->Draw("2 same");
  for(auto& gg : sp_0_20.gratio) {
    gg->Draw("c same");
  }
  pdf.write(Form("figs/results/merged-%s-fulleta-1.pdf", tag.c_str()), "Q");
  pdf.getc()->SaveAs(Form("figs/results/merged-%s-fulleta-1.C", tag.c_str()));

  pdf.getc()->SetLogy();
  pdf.prepare();
  hempty2->Draw("axis");
  for(auto& hh : sp_19_20.gh1WGhadron)
    hh->Draw("c same");
  sp_19_20.gsyst->Draw("2 same");
  sp_19_20.hsym->Draw("p same");
  sp_19_20.draw_leg();
  for(auto& hh : sp_9_10.gh1WGhadron)
    hh->Draw("c same");
  sp_9_10.gsyst->Draw("2 same");
  sp_9_10.hsym->Draw("p same");
  sp_9_10.draw_leg();
  DRAWTEX;
  pdf.write(Form("figs/results/merged-%s-fulleta-2noratio.pdf", tag.c_str()), "Q");

  sp_9_10.set_tleg(0.038 * 1.5);
  sp_19_20.set_tleg(0.038 * 1.5);
  sp_9_10.move_leg(-1, 0.6);
  sp_19_20.move_leg(-1, 0.6);
  
  pdf.prepare();
  TPad *p21, *p22;
  xjjroot::twopads(pdf.getc(), p21, p22, hempty2p1, hempty_ratio);
  p21->SetLogy();
  p21->cd();
  hempty2p1->Draw("axis");
  for(auto& hh : sp_19_20.gh1WGhadron)
    hh->Draw("c same");
  sp_19_20.gsyst->Draw("2 same");
  sp_19_20.hsym->Draw("p same");
  sp_19_20.leg->Draw();
  for(auto& hh : sp_9_10.gh1WGhadron)
    hh->Draw("c same");
  sp_9_10.gsyst->Draw("2 same");
  sp_9_10.hsym->Draw("p same");
  sp_9_10.leg->Draw();
  watermark_inner_2p(ismc);
  p22->cd();
  sp_19_20.gsyst_ratio->Draw("2 same");
  sp_9_10.gsyst_ratio->Draw("2 same");
  for(auto& gg : sp_19_20.gratio) {
    gg->Draw("c same");
  }
  for(auto& gg : sp_9_10.gratio) {
    gg->Draw("c same");
  }
  drawNormtext();
  
  pdf.write(Form("figs/results/merged-%s-fulleta-2.pdf", tag.c_str()), "Q");
  // pdf.write(Form("figs/results/merged-%s-fulleta-2.C", tag.c_str()), "Q");

  std::ofstream outf_Table_1("hepdatas/Table_1.yaml");
  xjjroot::hepdata hep_0_20(sp_0_20.hsym, sp_0_20.gsyst,
                            {"'$\\eta$'", "", ""},
                            {"'$"+_t_hep_dNdeta+"$'", "", ""},
                            { {"RE", "", "PB PB --> CHARGED X"},
                              {"SQRT(S)", "GEV", "5360"},
                              {"CENTRALITY", "'$\\%$'", "0 - 80"} }
                            );
  hep_0_20.print(outf_Table_1, 1, 1, -3.4, 3.4);
  xjjroot::hepdata hep_19_20(sp_19_20.hsym, sp_19_20.gsyst,
                             {"'$\\eta$'", "", ""},
                             {"'$"+_t_hep_dNdeta+"$'", "", ""},
                             { {"RE", "", "PB PB --> CHARGED X"},
                               {"SQRT(S)", "GEV", "5360"},
                               {"CENTRALITY", "'$\\%$'", "0 - 5"} }
                             );
  hep_19_20.print(outf_Table_1, 1, 1, -3.4, 3.4, true);
  xjjroot::hepdata hep_9_10(sp_9_10.hsym, sp_9_10.gsyst,
                            {"'$\\eta$'", "", ""},
                            {"'$"+_t_hep_dNdeta+"$'", "", ""},
                            { {"RE", "", "PB PB --> CHARGED X"},
                              {"SQRT(S)", "GEV", "5360"},
                              {"CENTRALITY", "'$\\%$'", "50 - 55"} }
                            );
  hep_9_10.print(outf_Table_1, 1, 1, -3.4, 3.4, true);

  printnpart();

  // for (auto& im : _m_mc)
  //   print_mc_cent(im);
}

void printnpart() {
  for (int i=NCENT-1; i>=0; i--) {
    std::cout<<(NCENT-i-1)*5<<"--"<<(NCENT-i)*5<<" & $"<<Form("%.3f", npart[i])<<" \\pm "<<Form("%.3f", nparterr[i])<<"$ \\\\"<<std::endl;
  }
}

template <class T>
void cleangrzero(T* gr) {
  for (int i=0; i<gr->GetN();) {
    if (gr->GetPointY(i) < 1.e-8) {
      gr->RemovePoint(i);
      continue;
    }
    i++;
  }
}

void moveleg_and_draw(TLegend* leg, float x1/*=-1*/, float y2/*=-1*/) {
  if (x1 < 0) x1 = leg->GetX1NDC();
  if (y2 < 0) y2 = leg->GetY2NDC();
  auto delta_x = leg->GetX2NDC() - leg->GetX1NDC();
  float x2 = x1 + delta_x;
  auto delta_y = leg->GetY2NDC() - leg->GetY1NDC(); 
  float y1 = y2 - delta_y;
  leg->SetX1NDC(x1);
  leg->SetY1NDC(y1);
  leg->SetX2NDC(x2);
  leg->SetY2NDC(y2);
  leg->Draw();
}

int main(int argc, char* argv[]) {
  if (argc == 2)
    return collect_cents(argv[1]);
  if (argc==1) 
    return collect_cents();

  printf("usage: ./collect_cents [label]\n");
  return 1;
}

