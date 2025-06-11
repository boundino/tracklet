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
#define OFFSET  4

// https://twiki.cern.ch/twiki/bin/view/CMS/Glauber5360GeVPbPb
const float npart[NCENT] = {
                            3.105   , 4.471   , 6.289   , 9.047   , 13.10  ,
                            18.76  , 26.32  , 35.68  , 47.16  , 61.08  ,
                            77.36  , 96.32  , 118.2 , 143.2  , 171.7 ,
                            204.1 , 241.0 , 283.3 , 331.3 , 382.3
};

const float nparterr[NCENT] = {
                               0.414 , 0.367   , 0.136  , 0.236  , 0.37  ,
                               0.59 ,  0.79  ,  0.94 ,  1.17 ,  1.21 ,
                               1.34 ,  1.32  ,  1.4 ,  1.5 ,  1.5 ,
                               1.5 ,   1.4 ,  1.4 ,  1.3 ,  1.6
};

void printnpart() {
  for (int i=NCENT-1; i>=0; i--) {
    std::cout<<(NCENT-i-1)*5<<"--"<<(NCENT-i)*5<<" & $"<<Form("%.1f", npart[i])<<" \\pm "<<Form("%.1f", nparterr[i])<<"$ \\\\"<<std::endl;
  }
}

// std::string cmsprel = "#scale[1.1]{#bf{CMS}} #scale[0.9]{#it{Preliminary}}";
// std::string cmsprel = "#scale[1.1]{#bf{CMS}}";
  
void drawdNdeta(xjjroot::mypdf& pdf, std::string tag);
int collect_cents(std::string tag="362294.cgm.epos.m.v2") {
  auto label = tag;
  bool ismc = xjjc::str_contains(tag, "CLOSE");

  constexpr int ntotal = NCENT - OFFSET;

  // TFile* fout = new TFile(Form("output/centrality-%s.root", label),
  //                         "recreate");

  TGraphErrors* g = new TGraphErrors(ntotal); g->SetName("g");
  gstyle(g, 21, COLOUR0, 1.4); g->SetFillColorAlpha(COLOUR0, 0.5);
  TGraphErrors* g2a = (TGraphErrors*)g->Clone("g2a");
  gstyle(g2a, 21, COLOUR0, 1.4); g2a->SetFillColorAlpha(COLOUR0, 0.3);
  TGraphErrors* gnorm = (TGraphErrors*)g->Clone("gnorm");
  gstyle(gnorm, 21, COLOUR0, 1.4); gnorm->SetFillColorAlpha(COLOUR0, 0.2);
  TGraphErrors* gnorm2a = (TGraphErrors*)g->Clone("gnorm2a");
  gstyle(gnorm2a, 21, COLOUR0, 1.4); gnorm2a->SetFillColorAlpha(COLOUR0, 0.2);
  TGraphErrors* gnorm2a2a = (TGraphErrors*)g->Clone("gnorm2a2a");
  gstyle(gnorm2a2a, 21, COLOUR0, 1.4); gnorm2a2a->SetFillColorAlpha(COLOUR0, 0.4);
  gnorm2a2a->SetLineColor(COLOUR0);
  gnorm2a2a->SetLineWidth(1);
  gnorm2a2a->SetLineStyle(2);
  TGraph* gsnp = new TGraph(2 * ntotal + 2);
  gstyle(gsnp, 21, COLOUR0, 1.4); gsnp->SetFillColorAlpha(COLOUR0, 0.2);
  TGraph* gsnp2a = new TGraph(2 * ntotal + 2);
  gstyle(gsnp2a, 21, COLOUR0, 1.4); gsnp2a->SetFillColorAlpha(COLOUR0, 0.2);

  for (int c = OFFSET; c < NCENT; c+=1) {
    TFile* f = new TFile(Form("results/results-%s.s.%i.%i.root",
                              label.c_str(), c, c + 1));
    TH1F* h = (TH1F*)f->Get("hsyst");
    int nbins = h->GetNbinsX();
    float midy = h->GetBinContent((nbins + 1) / 2);
    // float midyerr = (hsymhigh->GetBinContent((nbins + 1) / 2) - hsymlow->GetBinContent((nbins + 1) / 2)) / 2.;
    float midyerr = h->GetBinError((nbins + 1) / 2);

    int cindex = c - OFFSET;

    g->SetPoint(cindex, 5 * c + 2.5, midy);
    g->SetPointError(cindex, 0, midyerr);

    g2a->SetPoint(cindex, 5 * c + 2.5, midy / 32.);
    g2a->SetPointError(cindex, 0, midyerr / 32.);

    float mnpart = npart[c]; float mnparterr = nparterr[c];

    switch (cindex) {
    case 0:
      gsnp->SetPoint(0, mnpart - mnparterr,
                     (midy - midyerr) / (mnpart - mnparterr));
      gsnp2a->SetPoint(0, (mnpart - mnparterr) / 32.,
                       (midy - midyerr) / (mnpart - mnparterr));
      break;
    case ntotal - 1:
      gsnp->SetPoint(ntotal + 1, mnpart + mnparterr,
                     (midy + midyerr) / (mnpart + mnparterr));
      gsnp2a->SetPoint(ntotal + 1, (mnpart + mnparterr) / 32.,
                       (midy + midyerr) / (mnpart + mnparterr));
      break;
    }

    std::cout<<cindex<<" "<<midy<<" +- "<<midyerr<<std::endl;
    gnorm->SetPoint(cindex, mnpart, midy / mnpart);
    gnorm->SetPointError(cindex, mnparterr, midyerr / mnpart);
    gsnp->SetPoint(cindex + 1, mnpart - mnparterr,
                   (midy + midyerr) / (mnpart - mnparterr));
    gsnp->SetPoint(2 * ntotal - cindex + 1, mnpart + mnparterr,
                   (midy - midyerr) / (mnpart + mnparterr));
    gnorm2a->SetPoint(cindex, mnpart / 32., midy / mnpart);
    gnorm2a->SetPointError(cindex, mnparterr / 32., midyerr / mnpart);
    gsnp2a->SetPoint(cindex + 1, (mnpart - mnparterr) / 32.,
                     (midy + midyerr) / (mnpart - mnparterr));
    gsnp2a->SetPoint(2 * ntotal - cindex + 1, (mnpart + mnparterr) / 32.,
                     (midy - midyerr) / (mnpart + mnparterr));
    gnorm2a2a->SetPoint(cindex, mnpart / 32., midy / 32.);
    gnorm2a2a->SetPointError(cindex, mnparterr / 32., midyerr / 32.);
  }

  // fout->cd();

  TGraphErrors* gcms_pbpb_2p76 = cms_pbpb_2p76();
  TGraphErrors* galice_pbpb_2p76 = alice_pbpb_2p76();
  TGraphErrors* galice_pbpb_5p02 = alice_pbpb_5p02();
  TGraphErrors* gcms_xexe_5p44 = cms_xexe_5p44();
  TGraphErrors* galice_xexe_5p44 = alice_xexe_5p44();
  // TGraphErrors* gphobos_auau_0p2 = phobos_auau_0p2();
  // TGraphErrors* gphobos_cucu_0p2 = phobos_cucu_0p2();
  gstyle(gcms_pbpb_2p76, 22, COLOUR5);
  gstyle(galice_pbpb_2p76, 23, COLOUR5);
  gstyle(galice_pbpb_5p02, 20, COLOUR4);
  gstyle(gcms_xexe_5p44, 47, COLOUR2);
  gstyle(galice_xexe_5p44, 34, COLOUR1);
  // gstyle(gphobos_auau_0p2, 29, COLOUR6);
  // gstyle(gphobos_cucu_0p2, 43, COLOUR9);

  xjjroot::setgstyle();
  
  xjjroot::mypdf pdf(Form("figspdf/results/merged-%s.pdf", label.c_str()), "c", 800, 800);

  pdf.getc()->SetLogy();
  TH1F* gframe = new TH1F("gframe", "", 1, 0, 100);
  xjjroot::sethempty(gframe, 0, 0);
  gframe->SetLabelOffset(999, "X"); gframe->SetTickLength(0, "X");
  htitle(gframe, ";Centrality (%);" + _t_dNdeta + " " + _t_eta0p5);
  hrange(gframe, 1.5, 4000);

  pdf.prepare();
  gframe->Draw();

  TGaxis* axis = new TGaxis(100, 1.5, 0, 1.5, 0, 100, 510, "-");
  axis->SetLabelOffset(-0.032); axis->SetLabelFont(42);
  axis->SetLabelSize(0.045); axis->Draw();

  gcms_pbpb_2p76->Draw("3 same"); galice_pbpb_2p76->Draw("3 same");
  gcms_xexe_5p44->Draw("3 same"); galice_xexe_5p44->Draw("3 same");
  galice_pbpb_5p02->Draw("3 same");
  gcms_pbpb_2p76->Draw("pX same"); galice_pbpb_2p76->Draw("pX same");
  gcms_xexe_5p44->Draw("pX same"); galice_xexe_5p44->Draw("pX same");
  galice_pbpb_5p02->Draw("pX same");
  g->Draw("3 same"); g->Draw("pX same");

  float heightperline = 0.0375;

  watermark_inner(ismc);  
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");

  TLegend* l2 = new TLegend(0.55, 0.55-heightperline*5, 0.85, 0.55);
  TLegendEntry* h23cms = l2->AddEntry((TObject*)0, "CMS", "");
  h23cms->SetTextFont(63); h23cms->SetTextSize(20);
  l2->AddEntry(gnorm, "OO 5.36 TeV", "p");
  l2->AddEntry((TObject*)0, "(A#scale[0.9]{MPT})", NULL);
  l2->AddEntry(gcms_pbpb_2p76, "PbPb 2.76 TeV", "p");
  l2->AddEntry(gcms_xexe_5p44, "XeXe 5.44 TeV", "p");
  lstyle(l2, 43, 20); l2->Draw();
  
  TLegend* l22 = new TLegend(0.55, 0.35-heightperline*4, 0.85, 0.35);
  TLegendEntry* h23alice = l22->AddEntry((TObject*)0, "ALICE", "");
  h23alice->SetTextFont(63); h23alice->SetTextSize(20);
  l22->AddEntry(galice_pbpb_5p02, "PbPb 5.02 TeV", "p");
  l22->AddEntry(galice_pbpb_2p76, "PbPb 2.76 TeV", "p");
  l22->AddEntry(galice_xexe_5p44, "XeXe 5.44 TeV", "p");
  lstyle(l22, 43, 20); l22->Draw();

  // Figure 2a
  pdf.write(Form("figs/results/merged-%s-midy-int1.pdf", label.c_str()), "Q");
  // c2->SaveAs(Form("figspdf/results/merged-%s-midy-int1.pdf", label.c_str()));

  TH1F* h_g = new TH1F("h_g", ";;", 20, 0, 100);
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

  pdf.getc()->SetLogy(0);

  TGraphErrors* gcms_pbpb_2p76_n2a = cms_pbpb_2p76_n2a();
  TGraphErrors* galice_pbpb_2p76_n2a = alice_pbpb_2p76_n2a();
  TGraphErrors* galice_pbpb_5p02_n2a = alice_pbpb_5p02_n2a();
  TGraphErrors* gcms_xexe_5p44_n2a = cms_xexe_5p44_n2a();
  TGraphErrors* galice_xexe_5p44_n2a = alice_xexe_5p44_n2a();
  TGraphErrors* gphobos_auau_0p2_n2a = phobos_auau_0p2_n2a();
  TGraphErrors* gphobos_cucu_0p2_n2a = phobos_cucu_0p2_n2a();
  gstyle(gcms_pbpb_2p76_n2a, 22, COLOUR5);
  gstyle(galice_pbpb_2p76_n2a, 23, COLOUR5);
  gstyle(galice_pbpb_5p02_n2a, 20, COLOUR4);
  gstyle(gcms_xexe_5p44_n2a, 47, COLOUR2);
  gstyle(galice_xexe_5p44_n2a, 34, COLOUR1);
  gstyle(gphobos_auau_0p2_n2a, 29, COLOUR6, 1.7);
  gstyle(gphobos_cucu_0p2_n2a, 43, COLOUR9, 1.7);

  pdf.prepare();
  TH1F* g2aframe = new TH1F("g2aframe", "", 1, 0, 100);
  xjjroot::sethempty(g2aframe, 0, 0);
  g2aframe->SetLabelOffset(999, "X"); g2aframe->SetTickLength(0, "X");
  htitle(g2aframe, ";Centrality (%);" + _t_2a + "#kern[-0.1]{ }" + _t_dNdeta + " " + _t_eta0p5);
  g2aframe->SetTitleOffset(1.3, "Y");
  hrange(g2aframe, 1.e-5, 5.5); g2aframe->Draw();
  TGaxis* axis2 = new TGaxis(100, 0, 0, 0, 0, 100, 510, "-");
  axis2->SetLabelOffset(-0.032); axis2->SetLabelFont(42);
  axis2->SetLabelSize(0.045); axis2->Draw();
 
  gcms_pbpb_2p76_n2a->Draw("3 same"); galice_pbpb_2p76_n2a->Draw("3 same");
  gcms_xexe_5p44_n2a->Draw("3 same"); galice_xexe_5p44_n2a->Draw("3 same");
  galice_pbpb_5p02_n2a->Draw("3 same");
  gphobos_auau_0p2_n2a->Draw("3 same"); gphobos_cucu_0p2_n2a->Draw("3 same");
  g2a->Draw("3 same");  
  gcms_pbpb_2p76_n2a->Draw("pX same"); galice_pbpb_2p76_n2a->Draw("pX same");
  gcms_xexe_5p44_n2a->Draw("pX same"); galice_xexe_5p44_n2a->Draw("pX same");
  galice_pbpb_5p02_n2a->Draw("pX same");
  gphobos_auau_0p2_n2a->Draw("pX same"); gphobos_cucu_0p2_n2a->Draw("pX same");
  g2a->Draw("pX same");

  watermark_inner(ismc);
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");
  TLegend* l7 = new TLegend(0.23, 0.77-heightperline*5, 0.53, 0.77);
  TLegendEntry* h3cms = l7->AddEntry((TObject*)0, "CMS", "");
  h3cms->SetTextFont(63); h3cms->SetTextSize(20);
  l7->AddEntry(gnorm, "OO 5.36 TeV", "p");
  l7->AddEntry((TObject*)0, "(A#scale[0.9]{MPT})", NULL);
  l7->AddEntry(gcms_pbpb_2p76_n2a, "PbPb 2.76 TeV", "p");
  l7->AddEntry(gcms_xexe_5p44_n2a, "XeXe 5.44 TeV", "p");
  lstyle(l7, 43, 20); l7->Draw();
  
  TLegend* l8 = new TLegend(0.23, 0.57-heightperline*4, 0.53, 0.57);
  TLegendEntry* h3alice = l8->AddEntry((TObject*)0, "ALICE", "");
  h3alice->SetTextFont(63); h3alice->SetTextSize(20);
  l8->AddEntry(galice_pbpb_5p02_n2a, "PbPb 5.02 TeV", "p");
  l8->AddEntry(galice_pbpb_2p76_n2a, "PbPb 2.76 TeV", "p");
  l8->AddEntry(galice_xexe_5p44_n2a, "XeXe 5.44 TeV", "p");
  lstyle(l8, 43, 20); l8->Draw();

  TLegend* l9 = new TLegend(0.53, 0.77-heightperline*3, 0.73, 0.77);
  TLegendEntry* h3phobos = l9->AddEntry((TObject*)0, "PHOBOS", "");
  h3phobos->SetTextFont(63); h3phobos->SetTextSize(20);
  l9->AddEntry(gphobos_auau_0p2_n2a, "AuAu 200 GeV", "p");
  l9->AddEntry(gphobos_cucu_0p2_n2a, "CuCu 200 GeV", "p");
  lstyle(l9, 43, 20); l9->Draw();

  // Figure 2b
  pdf.write(Form("figs/results/merged-%s-midy2a-int1.pdf", label.c_str()), "Q");
  TH1F* h_g2a = new TH1F("h_g2a", ";;", 20, 0, 100);
  for (int i=0; i<g2a->GetN(); i++) {
    auto x = 100-g2a->GetPointX(i), y = g2a->GetPointY(i), ey = g2a->GetErrorY(i);
    auto ibin = h_g2a->FindBin(x);
    h_g2a->SetBinContent(ibin, y);
    h_g2a->SetBinError(ibin, ey);
  }
  std::ofstream outf_Table_2b("hepdatas/Table_2b.yaml");
  xjjroot::hepdata hep_Table_2b(h_g2a, 0,
                                {"CENTRALITY", "'$\\%$'", ""},
                                {"'$"+_t_hep_dNdeta+"/2A$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_2b.print(outf_Table_2b, 0, 3, 0, 80);

  
  //  
  TGraphErrors* gcms_pbpb_2p76_nnpart_x_npart = cms_pbpb_2p76_nnpart_x_npart();
  TGraphErrors* galice_pbpb_2p76_nnpart_x_npart = alice_pbpb_2p76_nnpart_x_npart();
  TGraphErrors* galice_pbpb_5p02_nnpart_x_npart = alice_pbpb_5p02_nnpart_x_npart();
  TGraphErrors* gcms_xexe_5p44_nnpart_x_npart = cms_xexe_5p44_nnpart_x_npart();
  TGraphErrors* galice_xexe_5p44_nnpart_x_npart = alice_xexe_5p44_nnpart_x_npart();
  TGraphErrors* gphobos_auau_0p2_nnpart_x_npart = phobos_auau_0p2_nnpart_x_npart();
  TGraphErrors* gphobos_cucu_0p2_nnpart_x_npart = phobos_cucu_0p2_nnpart_x_npart();
  gstyle(gcms_pbpb_2p76_nnpart_x_npart, 22, COLOUR5);
  gstyle(galice_pbpb_2p76_nnpart_x_npart, 23, COLOUR5);
  gstyle(galice_pbpb_5p02_nnpart_x_npart, 20, COLOUR4);
  gstyle(gcms_xexe_5p44_nnpart_x_npart, 47, COLOUR2);
  gstyle(galice_xexe_5p44_nnpart_x_npart, 34, COLOUR1);
  gstyle(gphobos_auau_0p2_nnpart_x_npart, 29, COLOUR6, 1.7);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_nnpart_x_npart, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart, 45, COLOUR6);

  pdf.prepare();
  // TCanvas* c3 = new TCanvas("c3", "", 600, 600);
  TH1F* gnframe = new TH1F("gnframe", "", 1, -20, 420);
  xjjroot::sethempty(gnframe, 0, 0);
  // htitle(gnframe, ";N_{part};#frac{dN}{d#eta}#lbar_{#eta=0}/#LTN_{part}#GT");
  htitle(gnframe, ";" + _t_npart + ";" + _t_1npart + "#kern[-0.1]{ }" + _t_dNdeta + " " + _t_eta0p5);
  gnframe->SetTitleOffset(1.3, "Y");
  hrange(gnframe, 0, 6); gnframe->Draw();

  gcms_pbpb_2p76_nnpart_x_npart->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("3 same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("3 same");
  gsnp->Draw("f");
  gnorm->Draw("3 same");
  xjjana::drawgroutline(gnorm, COLOUR0, 2, 1);
  gcms_pbpb_2p76_nnpart_x_npart->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("pX same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("pX same");
  gnorm->Draw("pX same");

  watermark_inner(ismc);
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");
  TLegend* l3 = new TLegend(0.6, 0.59-heightperline*5, 0.9, 0.59);
  TLegendEntry* h2cms = l3->AddEntry((TObject*)0, "CMS", "");
  h2cms->SetTextFont(63); h2cms->SetTextSize(20);
  l3->AddEntry(gnorm, "OO 5.36 TeV", "p");
  l3->AddEntry((TObject*)0, "(A#scale[0.9]{MPT})", NULL);
  l3->AddEntry(gcms_pbpb_2p76_nnpart_x_npart, "PbPb 2.76 TeV", "p");
  l3->AddEntry(gcms_xexe_5p44_nnpart_x_npart, "XeXe 5.44 TeV", "p");
  lstyle(l3, 43, 20); l3->Draw();
  // xjjroot::drawtex(0.61, 0.58-heightperline*2, "(A#scale[0.9]{MPT})", 0.035, 31);
  
  TLegend* l6 = new TLegend(0.6, 0.34-heightperline*4, 0.9, 0.34);
  TLegendEntry* h2alice = l6->AddEntry((TObject*)0, "ALICE", "");
  h2alice->SetTextFont(63); h2alice->SetTextSize(20);
  l6->AddEntry(galice_pbpb_5p02_nnpart_x_npart, "PbPb 5.02 TeV", "p");
  l6->AddEntry(galice_pbpb_2p76_nnpart_x_npart, "PbPb 2.76 TeV", "p");
  l6->AddEntry(galice_xexe_5p44_nnpart_x_npart, "XeXe 5.44 TeV", "p");
  lstyle(l6, 43, 20); l6->Draw();

  TLegend* l4 = new TLegend(0.23, 0.34-heightperline*4, 0.53, 0.34-heightperline);
  TLegendEntry* hphobos = l4->AddEntry((TObject*)0, "PHOBOS", "");
  hphobos->SetTextFont(63); hphobos->SetTextSize(20);
  l4->AddEntry(gphobos_auau_0p2_nnpart_x_npart, "AuAu 200 GeV", "p");
  l4->AddEntry(gphobos_cucu_0p2_nnpart_x_npart, "CuCu 200 GeV", "p");
  lstyle(l4, 43, 20); l4->Draw();

  TLegend* l5 = new TLegend(0.23, 0.78-heightperline*2, 0.48, 0.78);
  l5->AddEntry(gsnp, "(Expt. + #LTN_{part}#GT) unc.", "f");
  l5->AddEntry(gnorm2a2a, "Expt. unc.", "f");
  lstyle(l5, 43, 20); l5->Draw();
  
  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(20);
  // l5->AddEntry(gcms_pp_13p0_nnpart_x_npart, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_nnpart_x_npart, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 20); l5->Draw();

  // Figure 3a
  pdf.write(Form("figs/results/merged-%s-midynorm-int1.pdf", label.c_str()), "Q");
  std::ofstream outf_Table_3a("hepdatas/Table_3a.yaml");
  xjjroot::hepdata hep_Table_3a(gnorm, 0,
                                {"'$N_{part}$'", "", ""},
                                {"'$"+_t_hep_dNdeta+"/N_{part}$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_3a.print(outf_Table_3a, 1, 2);

  
  pdf.prepare();
  gnframe->Draw("axis");
  gsnp->Draw("f");
  gnorm->Draw("3 same");
  xjjana::drawgroutline(gnorm, COLOUR0, 2, 1);
  gnorm->Draw("pX same");

  watermark_inner(ismc);
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");
  l3->Draw();
  l5->Draw();
  pdf.write(Form("figs/results/merged-%s-midynorm-int1-conf1.pdf", label.c_str()), "Q");
  
  //  
  TGraphErrors* gcms_pbpb_2p76_nnpart_x_npart2a = cms_pbpb_2p76_nnpart_x_npart2a();
  TGraphErrors* galice_pbpb_2p76_nnpart_x_npart2a = alice_pbpb_2p76_nnpart_x_npart2a();
  TGraphErrors* galice_pbpb_5p02_nnpart_x_npart2a = alice_pbpb_5p02_nnpart_x_npart2a();
  TGraphErrors* gcms_xexe_5p44_nnpart_x_npart2a = cms_xexe_5p44_nnpart_x_npart2a();
  TGraphErrors* galice_xexe_5p44_nnpart_x_npart2a = alice_xexe_5p44_nnpart_x_npart2a();
  TGraphErrors* gphobos_auau_0p2_nnpart_x_npart2a = phobos_auau_0p2_nnpart_x_npart2a();
  TGraphErrors* gphobos_cucu_0p2_nnpart_x_npart2a = phobos_cucu_0p2_nnpart_x_npart2a();
  gstyle(gcms_pbpb_2p76_nnpart_x_npart2a, 22, COLOUR5);
  gstyle(galice_pbpb_2p76_nnpart_x_npart2a, 23, COLOUR5);
  gstyle(galice_pbpb_5p02_nnpart_x_npart2a, 20, COLOUR4);
  gstyle(gcms_xexe_5p44_nnpart_x_npart2a, 47, COLOUR2);
  gstyle(galice_xexe_5p44_nnpart_x_npart2a, 34, COLOUR1);
  gstyle(gphobos_auau_0p2_nnpart_x_npart2a, 29, COLOUR6, 1.7);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart2a, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_nnpart_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  TH1F* gn2aframe = new TH1F("gn2aframe", "", 1, -0.02, 1);
  xjjroot::sethempty(gn2aframe, 0, 0);
  htitle(gn2aframe, ";" + _t_npart + "" + _t_slash + "2#it{A};" + _t_1npart + "#kern[-0.1]{ }" + _t_dNdeta + " " + _t_eta0p5);
  gn2aframe->SetTitleOffset(1.3, "Y");
  hrange(gn2aframe, 0, 6); gn2aframe->Draw();

  gcms_pbpb_2p76_nnpart_x_npart2a->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("3 same");
  gphobos_auau_0p2_nnpart_x_npart2a->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("3 same");
  gsnp2a->Draw("f");  
  gnorm2a->Draw("3 same");  
  xjjana::drawgroutline(gnorm2a, COLOUR0, 2, 1);
  gcms_pbpb_2p76_nnpart_x_npart2a->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("pX same");
  gphobos_auau_0p2_nnpart_x_npart2a->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("pX same");
  gnorm2a->Draw("pX same");

  watermark_inner(ismc);
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");
  TLegend* l10 = new TLegend(0.6, 0.59-heightperline*5, 0.9, 0.59);
  TLegendEntry* h4cms = l10->AddEntry((TObject*)0, "CMS", "");
  h4cms->SetTextFont(63); h4cms->SetTextSize(20);
  l10->AddEntry(gnorm2a, "OO 5.36 TeV", "p");
  l10->AddEntry((TObject*)0, "(A#scale[0.9]{MPT})", NULL);
  l10->AddEntry(gcms_pbpb_2p76_nnpart_x_npart2a, "PbPb 2.76 TeV", "p");
  l10->AddEntry(gcms_xexe_5p44_nnpart_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l10, 43, 20); l10->Draw();
  
  TLegend* l11 = new TLegend(0.6, 0.34-heightperline*4, 0.9, 0.34);
  TLegendEntry* h4alice = l11->AddEntry((TObject*)0, "ALICE", "");
  h4alice->SetTextFont(63); h4alice->SetTextSize(20);
  l11->AddEntry(galice_pbpb_5p02_nnpart_x_npart2a, "PbPb 5.02 TeV", "p");
  l11->AddEntry(galice_pbpb_2p76_nnpart_x_npart2a, "PbPb 2.76 TeV", "p");
  l11->AddEntry(galice_xexe_5p44_nnpart_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l11, 43, 20); l11->Draw();

  TLegend* l12 = new TLegend(0.23, 0.34-heightperline*4, 0.53, 0.34-heightperline);
  TLegendEntry* h4phobos = l12->AddEntry((TObject*)0, "PHOBOS", "");
  h4phobos->SetTextFont(63); h4phobos->SetTextSize(20);
  l12->AddEntry(gphobos_auau_0p2_nnpart_x_npart2a, "AuAu 200 GeV", "p");
  l12->AddEntry(gphobos_cucu_0p2_nnpart_x_npart2a, "CuCu 200 GeV", "p");
  lstyle(l12, 43, 20); l12->Draw();

  l5->Draw();
  
  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(18);
  // l5->AddEntry(gcms_pp_13p0_nnpart_x_npart2a, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_nnpart_x_npart2a, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 20); l5->Draw();

  // Figure 3b
  pdf.write(Form("figs/results/merged-%s-midynorm2a-int1.pdf", label.c_str()), "Q");
  std::ofstream outf_Table_3b("hepdatas/Table_3b.yaml");
  xjjroot::hepdata hep_Table_3b(gnorm2a, 0,
                                {"'$N_{part}/2A$'", "", ""},
                                {"'$"+_t_hep_dNdeta+"/N_{part}$'", "", ""},
                                { {"RE", "", "PB PB --> CHARGED X"},
                                  {"SQRT(S)", "GEV", "5360"},
                                  {"'$|\\eta|$'", "", "< 0.5"} }
                                );
  hep_Table_3b.print(outf_Table_3b, 2, 2);

  
  //  
  TGraphErrors* gcms_pbpb_2p76_n2a_x_npart2a = cms_pbpb_2p76_n2a_x_npart2a();
  TGraphErrors* galice_pbpb_2p76_n2a_x_npart2a = alice_pbpb_2p76_n2a_x_npart2a();
  TGraphErrors* galice_pbpb_5p02_n2a_x_npart2a = alice_pbpb_5p02_n2a_x_npart2a();
  TGraphErrors* gcms_xexe_5p44_n2a_x_npart2a = cms_xexe_5p44_n2a_x_npart2a();
  TGraphErrors* galice_xexe_5p44_n2a_x_npart2a = alice_xexe_5p44_n2a_x_npart2a();
  TGraphErrors* gphobos_auau_0p2_n2a_x_npart2a = phobos_auau_0p2_n2a_x_npart2a();
  TGraphErrors* gphobos_cucu_0p2_n2a_x_npart2a = phobos_cucu_0p2_n2a_x_npart2a();
  gstyle(gcms_pbpb_2p76_n2a_x_npart2a, 22, COLOUR5);
  gstyle(galice_pbpb_2p76_n2a_x_npart2a, 23, COLOUR5);
  gstyle(galice_pbpb_5p02_n2a_x_npart2a, 20, COLOUR4);
  gstyle(gcms_xexe_5p44_n2a_x_npart2a, 47, COLOUR2);
  gstyle(galice_xexe_5p44_n2a_x_npart2a, 34, COLOUR1);
  gstyle(gphobos_auau_0p2_n2a_x_npart2a, 29, COLOUR6, 1.7);
  gstyle(gphobos_cucu_0p2_n2a_x_npart2a, 43, COLOUR9, 1.7);
  // gstyle(gcms_pp_13p0_n2a_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_n2a_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  TH1F* gn2a2aframe = new TH1F("gn2a2aframe", "", 1, -0.02, 1);
  xjjroot::sethempty(gn2a2aframe, 0, 0);
  htitle(gn2a2aframe, ";" + _t_npart + "" + _t_slash + "2#it{A};" + _t_2a + "#kern[-0.1]{ }" + _t_dNdeta + " " + _t_eta0p5);
  gn2a2aframe->SetTitleOffset(1.3, "Y");
  hrange(gn2a2aframe, 0, 6); gn2a2aframe->Draw();

  gcms_pbpb_2p76_n2a_x_npart2a->Draw("3 same"); galice_pbpb_2p76_n2a_x_npart2a->Draw("3 same");
  gcms_xexe_5p44_n2a_x_npart2a->Draw("3 same"); galice_xexe_5p44_n2a_x_npart2a->Draw("3 same");
  galice_pbpb_5p02_n2a_x_npart2a->Draw("3 same");
  gphobos_auau_0p2_n2a_x_npart2a->Draw("3 same"); gphobos_cucu_0p2_n2a_x_npart2a->Draw("3 same");
  gnorm2a2a->Draw("3 same");  
  gcms_pbpb_2p76_n2a_x_npart2a->Draw("pX same"); galice_pbpb_2p76_n2a_x_npart2a->Draw("pX same");
  gcms_xexe_5p44_n2a_x_npart2a->Draw("pX same"); galice_xexe_5p44_n2a_x_npart2a->Draw("pX same");
  galice_pbpb_5p02_n2a_x_npart2a->Draw("pX same");
  gphobos_auau_0p2_n2a_x_npart2a->Draw("pX same"); gphobos_cucu_0p2_n2a_x_npart2a->Draw("pX same");
  gnorm2a2a->Draw("pX same");

  watermark_inner(ismc);
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  // xjjroot::drawCMSright("OO (5.36 TeV)");
  TLegend* l13 = new TLegend(0.22, 0.77-heightperline*5, 0.52, 0.77);
  TLegendEntry* h5cms = l13->AddEntry((TObject*)0, "CMS", "");
  h5cms->SetTextFont(63); h5cms->SetTextSize(20);
  l13->AddEntry(gnorm2a2a, "OO 5.36 TeV", "p");
  l13->AddEntry((TObject*)0, "(A#scale[0.9]{MPT})", NULL);
  l13->AddEntry(gcms_pbpb_2p76_n2a_x_npart2a, "PbPb 2.76 TeV", "p");
  l13->AddEntry(gcms_xexe_5p44_n2a_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l13, 43, 20); l13->Draw();
  
  TLegend* l14 = new TLegend(0.22, 0.57-heightperline*4, 0.52, 0.57);
  TLegendEntry* h5alice = l14->AddEntry((TObject*)0, "ALICE", "");
  h5alice->SetTextFont(63); h5alice->SetTextSize(20);
  l14->AddEntry(galice_pbpb_5p02_n2a_x_npart2a, "PbPb 5.02 TeV", "p");
  l14->AddEntry(galice_pbpb_2p76_n2a_x_npart2a, "PbPb 2.76 TeV", "p");
  l14->AddEntry(galice_xexe_5p44_n2a_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l14, 43, 20); l14->Draw();

  TLegend* l15 = new TLegend(0.53, 0.77-heightperline*4, 0.73, 0.77-heightperline);
  TLegendEntry* h5phobos = l15->AddEntry((TObject*)0, "PHOBOS", "");
  h5phobos->SetTextFont(63); h5phobos->SetTextSize(20);
  l15->AddEntry(gphobos_auau_0p2_n2a_x_npart2a, "AuAu 200 GeV", "p");
  l15->AddEntry(gphobos_cucu_0p2_n2a_x_npart2a, "CuCu 200 GeV", "p");
  lstyle(l15, 43, 20); l15->Draw();

  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(20);
  // l5->AddEntry(gcms_pp_13p0_n2a_x_npart2a, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_n2a_x_npart2a, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 20); l5->Draw();

  pdf.write(Form("figs/results/merged-%s-midynorm2a2a-int1.pdf", label.c_str()), "Q");
  
  // g->Write("", TObject::kOverwrite);
  // gcms_pbpb_2p76->Write("", TObject::kOverwrite);
  // galice_pbpb_5p02->Write("", TObject::kOverwrite);
  // gphobos_cucu_0p2->Write("", TObject::kOverwrite);
  // gphobos_auau_0p2->Write("", TObject::kOverwrite);

  // gnorm->Write("", TObject::kOverwrite);
  // gsnp->Write("", TObject::kOverwrite);
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

int main(int argc, char* argv[]) {
  if (argc == 2)
    return collect_cents(argv[1]);
  if (argc==1) 
    return collect_cents();

  printf("usage: ./collect_cents [label]\n");
  return 1;
}

class spectrum {
public:
  spectrum(std::string filename, std::string title, float xleg, float yleg);
  void style(int color, Style_t ms);
  void setgcolor(std::vector<int> color);
  TH1D* hsym;
  TGraphErrors* gsyst;
  std::vector<TGraphErrors*> gh1WGhadron;
  TGraphErrors* gsyst_ratio;
  std::vector<TGraphErrors*> gratio;
  TLegend* leg;
  std::string title;
};

spectrum::spectrum(std::string filename, std::string title, float xleg, float yleg) {
  TFile* f = new TFile(filename.c_str());
  hsym = xjjroot::gethist<TH1D>(filename + "::hsym");
  gsyst = xjjroot::gethist<TGraphErrors>(filename + "::gsyst");
  leg = new TLegend(xleg, yleg-0.04*6, xleg+0.2, yleg);
  xjjroot::setleg(leg, 0.038);
  leg->AddEntry((TObject*)0, Form("#bf{%s}", title.c_str()), NULL);
  leg->AddEntry(gsyst, "Data", "pf");
  gh1WGhadron = combgh1WGhadron(filename, leg, "hijing&H#scale[0.9]{IJING},amptnm&A#scale[0.9]{MPT},amptnm2&A#scale[0.9]{MPT} #scale[0.9]{(varied params)},hydjet&H#scale[0.8]{YDJET}");

  gsyst_ratio = (TGraphErrors*)gsyst->Clone("gsyst_ratio");
  for (int i=0; i<gsyst->GetN(); i++) {
    float bincontent = gsyst->GetPointY(i);
    if (bincontent == 0) { bincontent = 1.; }
    gsyst_ratio->SetPointY(i, gsyst->GetPointY(i) / bincontent);
    gsyst_ratio->SetPointError(i, gsyst->GetErrorX(i), gsyst->GetErrorY(i) / bincontent);
  }

  int nmidy = gsyst->GetN() / 2;
  for (auto gr : gh1WGhadron) {
    gr->SetLineWidth(2);
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
  spectrum sp_0_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 0, 20), "Cent. 0 - 100\%", 0.55, 0.45),
    sp_19_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 19, 20), "Cent. 0 - 5\%", 0.23, 0.62),
    sp_9_10(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 9, 10), "Cent. 50 - 55\%", 0.59, 0.62);
  sp_0_20.style(COLOUR0, 21);
  sp_19_20.style(COLOUR1, 21);
  sp_9_10.style(COLOUR5, 21);

  auto hempty = makehempty(sp_0_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 1.7);
  hempty->SetAxisRange(-3.4, 3.5, "X");
  auto hemptyp1 = makehempty(sp_0_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 1.7);
  hemptyp1->SetAxisRange(-3.4, 3.5, "X");
  hemptyp1->SetMinimum(0.1);
  auto hempty_ratio = (TH1F*)hemptyp1->Clone("hempty_ratio");
  hempty_ratio->GetYaxis()->SetTitle("MC / data");
  hempty_ratio->SetMinimum(0.7); hempty_ratio->SetMaximum(1.3);
  // hempty_ratio->SetAxisRange(-3.4, 3.5, "X");
  auto hempty2 = makehempty(sp_19_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 3, 0.1);
  hempty2->SetAxisRange(-3.4, 3.5, "X");
  auto hempty2p1 = makehempty(sp_19_20.hsym, ";#it{#eta};" + _t_dNdeta + "", 3, 0.1);
  hempty2p1->SetAxisRange(-3.4, 3.5, "X");
  
  
  xjjroot::setgstyle(1);
  // xjjc::sconfig itext("362294 corr. w. E#scale[0.8]{POS} #scale[0.9]{LHC}", ",", "&");
  xjjc::sconfig itext("", ",", "&");
#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  watermark_inner(ismc);                                                \
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); /*Preliminary*/    \
  // xjjroot::drawCMSright("OO (5.36 TeV)");                               \
  // xjjroot::drawtex(0.88, 0.82, tcent(tag).c_str(), 0.030, 31);       \
  
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& hh : sp_0_20.gh1WGhadron)
    hh->Draw("c same");
  sp_0_20.gsyst->Draw("2 same");
  sp_0_20.hsym->Draw("p same");
  sp_0_20.leg->Draw();
  DRAWTEX;
  pdf.write(Form("figs/results/merged-%s-fulleta-1noratio.pdf", tag.c_str()), "Q");

  sp_0_20.leg->SetY1NDC(0.05);
  sp_0_20.leg->SetY2NDC(0.42);
  sp_0_20.leg->SetTextSize(0.038 * 1.6);

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
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  p2->cd();
  drawNormtext();
  sp_0_20.gsyst_ratio->Draw("p2 same");
  for(auto& gg : sp_0_20.gratio) {
    gg->Draw("c same");
  }
  pdf.write(Form("figs/results/merged-%s-fulleta-1.pdf", tag.c_str()), "Q");
  pdf.getc()->SaveAs(Form("figs/results/merged-%s-fulleta-1.C", tag.c_str()));

  // for(auto& gg : sp_4_20.gh1WGhadron)
  //   std::cout<<gg->GetName()<<std::endl;
  // for(auto& gg : sp_4_20.gratio)
  //   std::cout<<gg->GetName()<<std::endl;

  // int cc = TColor::GetColor("#4356b2");
  int cc = TColor::GetColor("#2240A3");

  // sp_4_20.setgcolor(std::vector<int>({0, 0, 0, 0}));
  // pdf.prepare();
  // TPad *p1c01, *p1c02;
  // xjjroot::twopads(pdf.getc(), p1c01, p1c02, hemptyp1, hempty_ratio);
  // p1c01->cd();
  // for(auto& hh : sp_4_20.gh1WGhadron)
  //   if (hh->GetLineColor())
  //     hh->Draw("c same");
  // sp_4_20.gsyst->Draw("2 same");
  // sp_4_20.hsym->Draw("p same");
  // sp_4_20.leg->Draw();
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  // p1c02->cd();
  // drawNormtext();
  // sp_4_20.gsyst_ratio->Draw("p2 same");
  // for(auto& gg : sp_4_20.gratio) {
  //   if (gg->GetLineColor())
  //     gg->Draw("c same");
  // }
  // pdf.write(Form("figs/results/merged-%s-fulleta-1-conf0.pdf", tag.c_str()), "Q");

  // sp_4_20.setgcolor(std::vector<int>({cc, cc, 0, 0}));
  // pdf.prepare();
  // TPad *p1c11, *p1c12;
  // xjjroot::twopads(pdf.getc(), p1c11, p1c12, hemptyp1, hempty_ratio);
  // p1c11->cd();
  // for(auto& hh : sp_4_20.gh1WGhadron)
  //   if (hh->GetLineColor())
  //     hh->Draw("c same");
  // sp_4_20.gsyst->Draw("2 same");
  // sp_4_20.hsym->Draw("p same");
  // sp_4_20.leg->Draw();
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  // p1c12->cd();
  // drawNormtext();
  // sp_4_20.gsyst_ratio->Draw("p2 same");
  // for(auto& gg : sp_4_20.gratio) {
  //   if (gg->GetLineColor())
  //     gg->Draw("c same");
  // }
  // pdf.write(Form("figs/results/merged-%s-fulleta-1-conf1.pdf", tag.c_str()), "Q");

  // sp_4_20.setgcolor(std::vector<int>({0-COLOUR0, 0-COLOUR0, cc, 0}));
  // pdf.prepare();
  // TPad *p1c21, *p1c22;
  // xjjroot::twopads(pdf.getc(), p1c21, p1c22, hemptyp1, hempty_ratio);
  // p1c21->cd();
  // for(auto& hh : sp_4_20.gh1WGhadron)
  //   if (hh->GetLineColor())
  //     hh->Draw("c same");
  // sp_4_20.gsyst->Draw("2 same");
  // sp_4_20.hsym->Draw("p same");
  // sp_4_20.leg->Draw();
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  // p1c22->cd();
  // drawNormtext();
  // sp_4_20.gsyst_ratio->Draw("p2 same");
  // for(auto& gg : sp_4_20.gratio) {
  //   if (gg->GetLineColor())
  //     gg->Draw("c same");
  // }
  // pdf.write(Form("figs/results/merged-%s-fulleta-1-conf2.pdf", tag.c_str()), "Q");

  // sp_4_20.setgcolor(std::vector<int>({0-COLOUR0, 0-COLOUR0, 0-COLOUR0, cc}));
  // pdf.prepare();
  // TPad *p1c31, *p1c32;
  // xjjroot::twopads(pdf.getc(), p1c31, p1c32, hemptyp1, hempty_ratio);
  // p1c31->cd();
  // for(auto& hh : sp_4_20.gh1WGhadron)
  //   if (hh->GetLineColor())
  //     hh->Draw("c same");
  // sp_4_20.gsyst->Draw("2 same");
  // sp_4_20.hsym->Draw("p same");
  // sp_4_20.leg->Draw();
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  // p1c32->cd();
  // drawNormtext();
  // sp_4_20.gsyst_ratio->Draw("p2 same");
  // for(auto& gg : sp_4_20.gratio) {
  //   if (gg->GetLineColor())
  //     gg->Draw("c same");
  // }
  // pdf.write(Form("figs/results/merged-%s-fulleta-1-conf3.pdf", tag.c_str()), "Q");

  pdf.getc()->SetLogy();
  pdf.prepare();
  hempty2->Draw("axis");
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
  DRAWTEX;
  pdf.write(Form("figs/results/merged-%s-fulleta-2noratio.pdf", tag.c_str()), "Q");

  sp_9_10.leg->SetY1NDC(0.26);
  sp_19_20.leg->SetY1NDC(0.26);
  sp_9_10.leg->SetY2NDC(0.60);
  sp_19_20.leg->SetY2NDC(0.60);
  // sp_9_10.leg->SetY2NDC(0.42);
  sp_9_10.leg->SetTextSize(0.038 * 1.5);
  sp_19_20.leg->SetTextSize(0.038 * 1.5);
  
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
  // xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.15, 0.065); /*Preliminary*/
  // xjjroot::drawCMSright("OO (5.36 TeV)", 0, 0, 0.055);
  p22->cd();
  sp_19_20.gsyst_ratio->Draw("p2 same");
  sp_9_10.gsyst_ratio->Draw("p2 same");
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
}

