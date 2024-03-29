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

#define NCENT   20
#define OFFSET  4

// 5.5 TeV https://dde.web.cern.ch/glauber_lhc.htm
const float npart[NCENT] = {
  2.179   , 3.327   , 5.529   , 8.524   , 12.92  ,
  18.74  , 26.42  , 35.99  , 47.47  , 61.37  ,
  77.73  , 96.65  , 118.4 , 143.6  , 172.0 ,
  204.4 , 241.0 , 283.6 , 331.5 , 382.3
};

const float nparterr[NCENT] = {
  0.290 , 0.273   , 0.119  , 0.219  , 0.37  ,
  0.58 ,  0.79  ,  0.94 ,  1.17 ,  1.21 ,
  1.32 ,  1.30  ,  1.4 ,  1.5 ,  1.5 ,
  1.4 ,   1.3 ,  1.3 ,  1.2 ,  1.5
};

void printnpart() {
  for (int i=NCENT-1; i>=0; i--) {
    std::cout<<(NCENT-i-1)*5<<"--"<<(NCENT-i)*5<<" & $"<<Form("%.1f", npart[i])<<" \\pm "<<Form("%.1f", nparterr[i])<<"$ \\\\"<<std::endl;
  }
}

std::string cmsprel = "CMS";
void drawdNdeta(xjjroot::mypdf& pdf, std::string tag);
int collect_cents(std::string tag="362294.cgm.epos.m.v2") {
  auto label = tag;

  constexpr int ntotal = NCENT - OFFSET;

  // TFile* fout = new TFile(Form("output/centrality-%s.root", label),
  //                         "recreate");

  TGraphErrors* g = new TGraphErrors(ntotal); g->SetName("g");
  gstyle(g, 21, COLOUR0, 0.8); g->SetFillColorAlpha(COLOUR0, 0.5);
  TGraphErrors* g2a = (TGraphErrors*)g->Clone("g2a");
  gstyle(g2a, 21, COLOUR0, 0.8); g2a->SetFillColorAlpha(COLOUR0, 0.3);
  TGraphErrors* gnorm = (TGraphErrors*)g->Clone("gnorm");
  gstyle(gnorm, 21, COLOUR0, 0.8); gnorm->SetFillColorAlpha(COLOUR0, 0.2);
  TGraphErrors* gnorm2a = (TGraphErrors*)g->Clone("gnorm2a");
  gstyle(gnorm2a, 21, COLOUR0, 0.8); gnorm2a->SetFillColorAlpha(COLOUR0, 0.2);
  TGraphErrors* gnorm2a2a = (TGraphErrors*)g->Clone("gnorm2a2a");
  gstyle(gnorm2a2a, 21, COLOUR0, 0.8); gnorm2a2a->SetFillColorAlpha(COLOUR0, 0.5);
  TGraph* gsnp = new TGraph(2 * ntotal + 2);
  gstyle(gsnp, 21, COLOUR0, 0.8); gsnp->SetFillColorAlpha(COLOUR0, 0.2);
  TGraph* gsnp2a = new TGraph(2 * ntotal + 2);
  gstyle(gsnp2a, 21, COLOUR0, 0.8); gsnp2a->SetFillColorAlpha(COLOUR0, 0.2);

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

    g2a->SetPoint(cindex, 5 * c + 2.5, midy / 416.);
    g2a->SetPointError(cindex, 0, midyerr / 416.);

    float mnpart = npart[c]; float mnparterr = nparterr[c];

    switch (cindex) {
    case 0:
      gsnp->SetPoint(0, mnpart - mnparterr,
                     (midy - midyerr) / (mnpart - mnparterr));
      gsnp2a->SetPoint(0, (mnpart - mnparterr) / 416.,
                     (midy - midyerr) / (mnpart - mnparterr));
      break;
    case ntotal - 1:
      gsnp->SetPoint(ntotal + 1, mnpart + mnparterr,
                     (midy + midyerr) / (mnpart + mnparterr));
      gsnp2a->SetPoint(ntotal + 1, (mnpart + mnparterr) / 416.,
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
    gnorm2a->SetPoint(cindex, mnpart / 416., midy / mnpart);
    gnorm2a->SetPointError(cindex, mnparterr / 416., midyerr / mnpart);
    gsnp2a->SetPoint(cindex + 1, (mnpart - mnparterr) / 416.,
                   (midy + midyerr) / (mnpart - mnparterr));
    gsnp2a->SetPoint(2 * ntotal - cindex + 1, (mnpart + mnparterr) / 416.,
                   (midy - midyerr) / (mnpart + mnparterr));
    gnorm2a2a->SetPoint(cindex, mnpart / 416., midy / 416.);
    gnorm2a2a->SetPointError(cindex, mnparterr / 416., midyerr / 416.);
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
  
  xjjroot::mypdf pdf(Form("figspdf/results/merged-%s.pdf", label.c_str()), "c", 600, 600);

  pdf.getc()->SetLogy();
  TH1F* gframe = new TH1F("gframe", "", 1, 0, 100);
  xjjroot::sethempty(gframe, 0, 0);
  gframe->SetLabelOffset(999, "X"); gframe->SetTickLength(0, "X");
  htitle(gframe, ";Centrality [%];#scale[1.2]{#LT} d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}} #scale[1.2]{#GT} #lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}} #lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#kern[0.2]{<}#kern[0.2]{0.5}}}");
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
  
  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  TLegend* l2 = new TLegend(0.54, 0.53-heightperline*8, 0.9, 0.53);
  TLegendEntry* hcms = l2->AddEntry((TObject*)0, "CMS", "");
  hcms->SetTextFont(63); hcms->SetTextSize(18);
  l2->AddEntry(g, "PbPb 5.36 TeV", "p");
  l2->AddEntry(gcms_pbpb_2p76, "PbPb 2.76 TeV", "p");
  l2->AddEntry(gcms_xexe_5p44, "XeXe 5.44 TeV", "p");
  TLegendEntry* halice = l2->AddEntry((TObject*)0, "ALICE", "");
  halice->SetTextFont(63); halice->SetTextSize(18);
  l2->AddEntry(galice_pbpb_5p02, "PbPb 5.02 TeV", "p");
  l2->AddEntry(galice_pbpb_2p76, "PbPb 2.76 TeV", "p");
  l2->AddEntry(galice_xexe_5p44, "XeXe 5.44 TeV", "p");
  lstyle(l2, 43, 18); l2->Draw();

  pdf.write(Form("figs/results/merged-%s-midy-int1.pdf", label.c_str()), "Q");
  
  // c2->SaveAs(Form("figspdf/results/merged-%s-midy-int1.pdf", label.c_str()));

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
  gstyle(gphobos_auau_0p2_n2a, 29, COLOUR6);
  gstyle(gphobos_cucu_0p2_n2a, 43, COLOUR9);

  pdf.prepare();
  TH1F* g2aframe = new TH1F("g2aframe", "", 1, 0, 100);
  xjjroot::sethempty(g2aframe, 0, 0);
  g2aframe->SetLabelOffset(999, "X"); g2aframe->SetTickLength(0, "X");
  htitle(g2aframe, ";Centrality [%];#lower[-0.05]{(}1/#kern[0.05]{2#it{A}}#lower[-0.05]{)}#kern[-0.1]{ }#scale[1.2]{#LT} d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}} #scale[1.2]{#GT} #lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}} #lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#kern[0.2]{<}#kern[0.2]{0.5}}}");
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

  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  TLegend* l7 = new TLegend(0.23, 0.77-heightperline*4, 0.53, 0.77);
  TLegendEntry* h3cms = l7->AddEntry((TObject*)0, "CMS", "");
  h3cms->SetTextFont(63); h3cms->SetTextSize(18);
  l7->AddEntry(gnorm, "PbPb 5.36 TeV", "p");
  l7->AddEntry(gcms_pbpb_2p76_n2a, "PbPb 2.76 TeV", "p");
  l7->AddEntry(gcms_xexe_5p44_n2a, "XeXe 5.44 TeV", "p");
  lstyle(l7, 43, 18); l7->Draw();
  
  TLegend* l8 = new TLegend(0.23, 0.57-heightperline*4, 0.53, 0.57);
  TLegendEntry* h3alice = l8->AddEntry((TObject*)0, "ALICE", "");
  h3alice->SetTextFont(63); h3alice->SetTextSize(18);
  l8->AddEntry(galice_pbpb_5p02_n2a, "PbPb 5.02 TeV", "p");
  l8->AddEntry(galice_pbpb_2p76_n2a, "PbPb 2.76 TeV", "p");
  l8->AddEntry(galice_xexe_5p44_n2a, "XeXe 5.44 TeV", "p");
  lstyle(l8, 43, 18); l8->Draw();

  TLegend* l9 = new TLegend(0.53, 0.77-heightperline*4, 0.73, 0.77-heightperline);
  TLegendEntry* h3phobos = l9->AddEntry((TObject*)0, "PHOBOS", "");
  h3phobos->SetTextFont(63); h3phobos->SetTextSize(18);
  l9->AddEntry(gphobos_auau_0p2_n2a, "AuAu 200 GeV", "p");
  l9->AddEntry(gphobos_cucu_0p2_n2a, "CuCu 200 GeV", "p");
  lstyle(l9, 43, 18); l9->Draw();

  pdf.write(Form("figs/results/merged-%s-midy2a-int1.pdf", label.c_str()), "Q");
  
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
  gstyle(gphobos_auau_0p2_nnpart_x_npart, 29, COLOUR6);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart, 43, COLOUR9);
  // gstyle(gcms_pp_13p0_nnpart_x_npart, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart, 45, COLOUR6);

  pdf.prepare();
  // TCanvas* c3 = new TCanvas("c3", "", 600, 600);
  TH1F* gnframe = new TH1F("gnframe", "", 1, -20, 420);
  xjjroot::sethempty(gnframe, 0, 0);
  // htitle(gnframe, ";N_{part};#frac{dN}{d#eta}#lbar_{#eta=0}/#LTN_{part}#GT");
  htitle(gnframe, ";#scale[1.2]{#LT}#it{N}#lower[0.4]{#scale[0.7]{#kern[-0.05]{part}}}#scale[1.2]{#GT};#lower[-0.05]{(}1/#kern[0.1]{#scale[1.2]{#LT}}#lower[0.1]{#it{N}}#lower[0.5]{#scale[0.6]{#kern[-0.08]{part}}}#scale[1.2]{#GT}#lower[-0.05]{)}#kern[-0.1]{ }#scale[1.2]{#LT}d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}#scale[1.2]{#GT}#lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}}#lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#kern[0.2]{<}#kern[0.2]{0.5}}}");
  gnframe->SetTitleOffset(1.3, "Y");
  hrange(gnframe, 0, 6); gnframe->Draw();

  gcms_pbpb_2p76_nnpart_x_npart->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("3 same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("3 same");
  gsnp->Draw("f");  
  gnorm->Draw("3 same");  
  gcms_pbpb_2p76_nnpart_x_npart->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("pX same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("pX same");
  gnorm->Draw("pX same");

  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  TLegend* l3 = new TLegend(0.6, 0.57-heightperline*4, 0.9, 0.57);
  TLegendEntry* h2cms = l3->AddEntry((TObject*)0, "CMS", "");
  h2cms->SetTextFont(63); h2cms->SetTextSize(18);
  l3->AddEntry(gnorm, "PbPb 5.36 TeV", "p");
  l3->AddEntry(gcms_pbpb_2p76_nnpart_x_npart, "PbPb 2.76 TeV", "p");
  l3->AddEntry(gcms_xexe_5p44_nnpart_x_npart, "XeXe 5.44 TeV", "p");
  lstyle(l3, 43, 18); l3->Draw();
  
  TLegend* l6 = new TLegend(0.6, 0.34-heightperline*4, 0.9, 0.34);
  TLegendEntry* h2alice = l6->AddEntry((TObject*)0, "ALICE", "");
  h2alice->SetTextFont(63); h2alice->SetTextSize(18);
  l6->AddEntry(galice_pbpb_5p02_nnpart_x_npart, "PbPb 5.02 TeV", "p");
  l6->AddEntry(galice_pbpb_2p76_nnpart_x_npart, "PbPb 2.76 TeV", "p");
  l6->AddEntry(galice_xexe_5p44_nnpart_x_npart, "XeXe 5.44 TeV", "p");
  lstyle(l6, 43, 18); l6->Draw();

  TLegend* l4 = new TLegend(0.23, 0.34-heightperline*4, 0.53, 0.34-heightperline);
  TLegendEntry* hphobos = l4->AddEntry((TObject*)0, "PHOBOS", "");
  hphobos->SetTextFont(63); hphobos->SetTextSize(18);
  l4->AddEntry(gphobos_auau_0p2_nnpart_x_npart, "AuAu 200 GeV", "p");
  l4->AddEntry(gphobos_cucu_0p2_nnpart_x_npart, "CuCu 200 GeV", "p");
  lstyle(l4, 43, 18); l4->Draw();

  TLegend* l5 = new TLegend(0.23, 0.78-heightperline*2, 0.48, 0.78);
  l5->AddEntry(gsnp, "(expt. + #LTN_{part}#GT) unc.", "f");
  l5->AddEntry(gnorm2a2a, "expt. unc.", "f");
  lstyle(l5, 43, 18); l5->Draw();
  
  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(18);
  // l5->AddEntry(gcms_pp_13p0_nnpart_x_npart, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_nnpart_x_npart, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 18); l5->Draw();

  pdf.write(Form("figs/results/merged-%s-midynorm-int1.pdf", label.c_str()), "Q");
  
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
  gstyle(gphobos_auau_0p2_nnpart_x_npart2a, 29, COLOUR6);
  gstyle(gphobos_cucu_0p2_nnpart_x_npart2a, 43, COLOUR9);
  // gstyle(gcms_pp_13p0_nnpart_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  TH1F* gn2aframe = new TH1F("gn2aframe", "", 1, -0.02, 1);
  xjjroot::sethempty(gn2aframe, 0, 0);
  htitle(gn2aframe, ";#scale[1.2]{#LT}#it{N}#lower[0.4]{#scale[0.7]{#kern[-0.05]{part}}}#scale[1.2]{#GT}#kern[0.1]{#lower[0.15]{#scale[1.25]{/}}}2#it{A};#lower[-0.05]{(}1/#kern[0.1]{#scale[1.2]{#LT}}#lower[0.1]{#it{N}}#lower[0.5]{#scale[0.6]{#kern[-0.08]{part}}}#scale[1.2]{#GT}#lower[-0.05]{)}#kern[-0.1]{ }#scale[1.2]{#LT}d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}#scale[1.2]{#GT}#lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}}#lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#kern[0.2]{<}#kern[0.2]{0.5}}}");
  gn2aframe->SetTitleOffset(1.3, "Y");
  hrange(gn2aframe, 0, 6); gn2aframe->Draw();

  gcms_pbpb_2p76_nnpart_x_npart2a->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("3 same");
  gphobos_auau_0p2_nnpart_x_npart2a->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("3 same");
  gsnp2a->Draw("f");  
  gnorm2a->Draw("3 same");  
  gcms_pbpb_2p76_nnpart_x_npart2a->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart2a->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart2a->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart2a->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart2a->Draw("pX same");
  gphobos_auau_0p2_nnpart_x_npart2a->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart2a->Draw("pX same");
  gnorm2a->Draw("pX same");

  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  TLegend* l10 = new TLegend(0.6, 0.57-heightperline*4, 0.9, 0.57);
  TLegendEntry* h4cms = l10->AddEntry((TObject*)0, "CMS", "");
  h4cms->SetTextFont(63); h4cms->SetTextSize(18);
  l10->AddEntry(gnorm2a, "PbPb 5.36 TeV", "p");
  l10->AddEntry(gcms_pbpb_2p76_nnpart_x_npart2a, "PbPb 2.76 TeV", "p");
  l10->AddEntry(gcms_xexe_5p44_nnpart_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l10, 43, 18); l10->Draw();
  
  TLegend* l11 = new TLegend(0.6, 0.34-heightperline*4, 0.9, 0.34);
  TLegendEntry* h4alice = l11->AddEntry((TObject*)0, "ALICE", "");
  h4alice->SetTextFont(63); h4alice->SetTextSize(18);
  l11->AddEntry(galice_pbpb_5p02_nnpart_x_npart2a, "PbPb 5.02 TeV", "p");
  l11->AddEntry(galice_pbpb_2p76_nnpart_x_npart2a, "PbPb 2.76 TeV", "p");
  l11->AddEntry(galice_xexe_5p44_nnpart_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l11, 43, 18); l11->Draw();

  TLegend* l12 = new TLegend(0.23, 0.34-heightperline*4, 0.53, 0.34-heightperline);
  TLegendEntry* h4phobos = l12->AddEntry((TObject*)0, "PHOBOS", "");
  h4phobos->SetTextFont(63); h4phobos->SetTextSize(18);
  l12->AddEntry(gphobos_auau_0p2_nnpart_x_npart2a, "AuAu 200 GeV", "p");
  l12->AddEntry(gphobos_cucu_0p2_nnpart_x_npart2a, "CuCu 200 GeV", "p");
  lstyle(l12, 43, 18); l12->Draw();

  l5->Draw();
  
  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(18);
  // l5->AddEntry(gcms_pp_13p0_nnpart_x_npart2a, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_nnpart_x_npart2a, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 18); l5->Draw();

  pdf.write(Form("figs/results/merged-%s-midynorm2a-int1.pdf", label.c_str()), "Q");
  
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
  gstyle(gphobos_auau_0p2_n2a_x_npart2a, 29, COLOUR6);
  gstyle(gphobos_cucu_0p2_n2a_x_npart2a, 43, COLOUR9);
  // gstyle(gcms_pp_13p0_n2a_x_npart2a, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_n2a_x_npart2a, 45, COLOUR6);

  pdf.prepare();
  TH1F* gn2a2aframe = new TH1F("gn2a2aframe", "", 1, -0.02, 1);
  xjjroot::sethempty(gn2a2aframe, 0, 0);
  htitle(gn2a2aframe, ";#scale[1.2]{#LT}#it{N}#lower[0.4]{#scale[0.7]{#kern[-0.05]{part}}}#scale[1.2]{#GT}#kern[0.1]{#lower[0.15]{#scale[1.25]{/}}}2#it{A};#lower[-0.05]{(}1/#kern[0.05]{2#it{A}}#lower[-0.05]{)}#kern[-0.1]{ }#scale[1.2]{#LT}d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}}#scale[1.2]{#GT}#lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}}#lower[0.6]{#scale[0.6]{#kern[0.15]{#cbar}#it{#eta}#kern[-0.4]{#cbar}#kern[0.2]{<}#kern[0.2]{0.5}}}");
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

  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); // Preliminary
  TLegend* l13 = new TLegend(0.22, 0.77-heightperline*4, 0.52, 0.77);
  TLegendEntry* h5cms = l13->AddEntry((TObject*)0, "CMS", "");
  h5cms->SetTextFont(63); h5cms->SetTextSize(18);
  l13->AddEntry(gnorm2a2a, "PbPb 5.36 TeV", "p");
  l13->AddEntry(gcms_pbpb_2p76_n2a_x_npart2a, "PbPb 2.76 TeV", "p");
  l13->AddEntry(gcms_xexe_5p44_n2a_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l13, 43, 18); l13->Draw();
  
  TLegend* l14 = new TLegend(0.22, 0.57-heightperline*4, 0.52, 0.57);
  TLegendEntry* h5alice = l14->AddEntry((TObject*)0, "ALICE", "");
  h5alice->SetTextFont(63); h5alice->SetTextSize(18);
  l14->AddEntry(galice_pbpb_5p02_n2a_x_npart2a, "PbPb 5.02 TeV", "p");
  l14->AddEntry(galice_pbpb_2p76_n2a_x_npart2a, "PbPb 2.76 TeV", "p");
  l14->AddEntry(galice_xexe_5p44_n2a_x_npart2a, "XeXe 5.44 TeV", "p");
  lstyle(l14, 43, 18); l14->Draw();

  TLegend* l15 = new TLegend(0.53, 0.77-heightperline*4, 0.73, 0.77-heightperline);
  TLegendEntry* h5phobos = l15->AddEntry((TObject*)0, "PHOBOS", "");
  h5phobos->SetTextFont(63); h5phobos->SetTextSize(18);
  l15->AddEntry(gphobos_auau_0p2_n2a_x_npart2a, "AuAu 200 GeV", "p");
  l15->AddEntry(gphobos_cucu_0p2_n2a_x_npart2a, "CuCu 200 GeV", "p");
  lstyle(l15, 43, 18); l15->Draw();

  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(18);
  // l5->AddEntry(gcms_pp_13p0_n2a_x_npart2a, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_n2a_x_npart2a, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 18); l5->Draw();

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
  TH1D* hsym;
  TGraphErrors* gsyst;
  std::vector<TGraphErrors*> gh1WGhadron;
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
  leg->AddEntry(gsyst, "data", "pf");
  gh1WGhadron = combgh1WGhadron(filename, leg);
  for(auto gr : gh1WGhadron) gr->SetLineWidth(2);
}

void spectrum::style(int color, Style_t ms) {
  hsym->SetMarkerStyle(ms);
  hsym->SetMarkerColor(color);
  gsyst->SetMarkerStyle(ms);
  gsyst->SetMarkerColor(color);
  gsyst->SetFillColor(color);
  gsyst->SetFillColorAlpha(color, 0.4);
  gsyst->SetLineStyle(0);
  for(auto gr : gh1WGhadron) gr->SetLineColor(color);
}

void drawdNdeta(xjjroot::mypdf& pdf, std::string tag) {
  spectrum sp_4_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 4, 20), "0 - 80\%", 0.55, 0.45),
    sp_19_20(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 19, 20), "0 - 5\%", 0.23, 0.62),
    sp_9_10(Form("results/results-%s.s.%i.%i.root", tag.c_str(), 9, 10), "50 - 55\%", 0.59, 0.62);
  sp_4_20.style(COLOUR0, 21);
  sp_19_20.style(COLOUR1, 21);
  sp_9_10.style(COLOUR5, 21);

  auto hempty = makehempty(sp_4_20.hsym, ";#it{#eta};#scale[1.2]{#LT} d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}} #scale[1.2]{#GT}", 1.7);
  hempty->SetAxisRange(-3.2, 3.4, "X");
  auto hempty2 = makehempty(sp_19_20.hsym, ";#it{#eta};#scale[1.2]{#LT} d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}} #scale[1.2]{#GT}", 3, 0.04);
  hempty2->SetAxisRange(-3.2, 3.4, "X");
  
  xjjroot::setgstyle(1);
  // xjjc::sconfig itext("362294 corr. w. E#scale[0.8]{POS} #scale[0.9]{LHC}", ",", "&");
  xjjc::sconfig itext("", ",", "&");
#define DRAWTEX                                                         \
  for(int i=0; i<itext.n(); i++)                                        \
    { xjjroot::drawtex(0.24, 0.79-i*0.033, itext.value[i][0].c_str(), 0.030, 13); } \
  xjjroot::drawCMSleft(cmsprel.c_str(), 0.05, -0.1); /*Preliminary*/    \
  xjjroot::drawCMSright("PbPb #sqrt{s_{NN}} = 5.36 TeV");               \
  // xjjroot::drawtex(0.88, 0.82, tcent(tag).c_str(), 0.030, 31);          \
  
  pdf.prepare();
  hempty->Draw("axis");
  for(auto& hh : sp_4_20.gh1WGhadron)
    hh->Draw("c same");
  sp_4_20.gsyst->Draw("2 same");
  sp_4_20.hsym->Draw("p same");
  sp_4_20.leg->Draw();
  DRAWTEX;
  pdf.write(Form("figs/results/merged-%s-fulleta-1.pdf", tag.c_str()), "Q");

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
  pdf.write(Form("figs/results/merged-%s-fulleta-2.pdf", tag.c_str()), "Q");


  printnpart();
}

