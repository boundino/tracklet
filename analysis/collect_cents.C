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

#define NCENT   20
#define OFFSET  4

// const float npart[NCENT] = {
//   2.65, 2.851, 4.464, 6.209, 8.746,
//   12.35, 16.96, 22.94, 30.09, 38.82,
//   49., 60.89, 74.31, 89.95, 107.9,
//   128.1, 151.4, 177.4, 206.3, 236.1
// };

// 5.5 TeV https://dde.web.cern.ch/glauber_lhc.htm
const float npart[NCENT] = {
  3.1   , 4.5   , 6.4   , 9.1   , 13.1  ,
  18.7  , 26.1  , 35.8  , 47.4  , 61.1  ,
  76.9  , 96.4  , 118.2 , 143.  , 171.7 ,
  204.2 , 241.5 , 284.4 , 333.1 , 384.5
};

const float nparterr[NCENT] = {
  0., 0., 0., 0., 0.,
  0.,  0.,  0.,  0.,  0.,
  0.,  0.,  0.,  0.,  0.,
  0.,   0.,  0.,  0.,  0.
};

// const float nparterr[NCENT] = {
//   0.0329, 0.139, 0.234, 0.455, 0.676,
//   0.89,  1.18,  1.46,  1.71,  1.99,
//   2.22,  2.33,  2.05,  2.22,  2.26,
//   1.78,   1.5,  1.61,  1.67,  1.33
// };

int collect_cents(std::string tag="362294.cgm.epos.m.v1") {
  auto label = tag;

  constexpr int ntotal = NCENT - OFFSET;

  // TFile* fout = new TFile(Form("output/centrality-%s.root", label),
  //                         "recreate");

  TGraphErrors* g = new TGraphErrors(ntotal); g->SetName("g");
  gstyle(g, 21, COLOUR0, 0.8); g->SetFillColorAlpha(COLOUR0, 0.4);
  TGraphErrors* gnorm = (TGraphErrors*)g->Clone("gnorm");
  TGraph* gsnp = new TGraph(2 * ntotal + 2);
  gstyle(gsnp, 21, COLOUR0, 0.8); gsnp->SetFillColorAlpha(COLOUR0, 0.3);

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

    float mnpart = npart[c]; float mnparterr = nparterr[c];

    switch (cindex) {
    case 0:
      gsnp->SetPoint(0, mnpart - mnparterr,
                     (midy - midyerr) / (mnpart - mnparterr));
      break;
    case ntotal - 1:
      gsnp->SetPoint(ntotal + 1, mnpart + mnparterr,
                     (midy + midyerr) / (mnpart + mnparterr));
      break;
    }

    gnorm->SetPoint(cindex, mnpart, midy / mnpart);
    gnorm->SetPointError(cindex, mnparterr, midyerr / mnpart);
    gsnp->SetPoint(cindex + 1, mnpart - mnparterr,
                   (midy + midyerr) / (mnpart - mnparterr));
    gsnp->SetPoint(2 * ntotal - cindex + 1, mnpart + mnparterr,
                   (midy - midyerr) / (mnpart + mnparterr));
  }

  // fout->cd();

  TGraphErrors* gcms_pbpb_2p76 = cms_pbpb_2p76();
  TGraphErrors* galice_pbpb_2p76 = alice_pbpb_2p76();
  TGraphErrors* galice_pbpb_5p02 = alice_pbpb_5p02();
  TGraphErrors* gcms_xexe_5p44 = cms_xexe_5p44();
  TGraphErrors* galice_xexe_5p44 = alice_xexe_5p44();
  TGraphErrors* gphobos_auau_0p2 = phobos_auau_0p2();
  TGraphErrors* gphobos_cucu_0p2 = phobos_cucu_0p2();
  gstyle(gcms_pbpb_2p76, 22, COLOUR5);
  gstyle(galice_pbpb_2p76, 23, COLOUR5);
  gstyle(galice_pbpb_5p02, 20, COLOUR4);
  gstyle(gcms_xexe_5p44, 47, COLOUR2);
  gstyle(galice_xexe_5p44, 34, COLOUR1);
  gstyle(gphobos_auau_0p2, 29, COLOUR6);
  gstyle(gphobos_cucu_0p2, 43, COLOUR9);

  xjjroot::setgstyle(1);
  
  xjjroot::mypdf pdf(Form("figspdf/merged/merged-%s.pdf", label.c_str()), "c", 600, 600);
  pdf.getc()->SetLogy();
  TH1F* gframe = new TH1F("gframe", "", 1, 0, 100);
  // xjjroot::sethempty(gframe);
  gframe->SetLabelOffset(999, "X"); gframe->SetTickLength(0, "X");
  // gframe->GetXaxis()->SetTitleOffset();
  // htitle(gframe, ";Centrality [%];#frac{dN}{d#eta}#lbar_{#eta=0}");
  htitle(gframe, ";Centrality [%];#scale[1.2]{#LT} d#it{N}_{ch}/d#kern[-0.08]{#it{#eta}} #scale[1.2]{#GT} #lower[0.05]{#scale[1.5]{#kern[-0.6]{#cbar}}} #lower[0.6]{#scale[0.6]{#it{#eta}#kern[0.2]{=}#kern[0.2]{0}}}");
  hrange(gframe, 1.5, 4000);

  pdf.prepare();
  gframe->Draw();

  TGaxis* axis = new TGaxis(100, 1.5, 0, 1.5, 0, 100, 510, "-");
  axis->SetLabelOffset(-0.032); axis->SetLabelFont(43);
  axis->SetLabelSize(18); axis->Draw();

  gcms_pbpb_2p76->Draw("3 same"); galice_pbpb_2p76->Draw("3 same");
  gcms_xexe_5p44->Draw("3 same"); galice_xexe_5p44->Draw("3 same");
  galice_pbpb_5p02->Draw("3 same");
  gcms_pbpb_2p76->Draw("pX same"); galice_pbpb_2p76->Draw("pX same");
  gcms_xexe_5p44->Draw("pX same"); galice_xexe_5p44->Draw("pX same");
  galice_pbpb_5p02->Draw("pX same");
  g->Draw("3 same"); g->Draw("pX same");

  xjjroot::drawCMSleft("Internal", 0.05, -0.1);
  TLegend* l2 = new TLegend(0.54, 0.24, 0.9, 0.48);
  TLegendEntry* hcms = l2->AddEntry((TObject*)0, "CMS", "");
  hcms->SetTextFont(63); hcms->SetTextSize(17);
  l2->AddEntry(g, "PbPb 5.36 TeV", "p");
  l2->AddEntry(gcms_xexe_5p44, "XeXe 5.44 TeV", "p");
  l2->AddEntry(gcms_pbpb_2p76, "PbPb 2.76 TeV", "p");
  TLegendEntry* halice = l2->AddEntry((TObject*)0, "ALICE", "");
  halice->SetTextFont(63); halice->SetTextSize(17);
  l2->AddEntry(galice_pbpb_5p02, "PbPb 5.02 TeV", "p");
  lstyle(l2, 43, 15); l2->Draw();

  pdf.write(Form("figs/merged/merged-%s-midy-int1.png", label.c_str()));
  

  // c2->SaveAs(Form("figspdf/merged/merged-%s-midy-int1.pdf", label.c_str()));

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
  // gstyle(gcms_pbpb_2p76_nnpart_x_npart, 24, COLOUR5);
  // gstyle(galice_pbpb_5p02_nnpart_x_npart, 30, COLOUR4);
  // gstyle(gcms_pp_13p0_nnpart_x_npart, 43, COLOUR0);
  // gstyle(gcms_ppb_8p16_nnpart_x_npart, 45, COLOUR6);

  pdf.getc()->SetLogy(0);
  pdf.prepare();

  // TCanvas* c3 = new TCanvas("c3", "", 600, 600);

  TH1F* gnframe = new TH1F("gnframe", "", 1, -20, 420);
  htitle(gnframe, ";N_{part};#frac{dN}{d#eta}#lbar_{#eta=0}/#LTN_{part}#GT");
  hrange(gnframe, 0, 6); gnframe->Draw();

  gcms_pbpb_2p76_nnpart_x_npart->Draw("3 same"); galice_pbpb_2p76_nnpart_x_npart->Draw("3 same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("3 same"); galice_xexe_5p44_nnpart_x_npart->Draw("3 same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("3 same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("3 same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("3 same");
  gnorm->Draw("3 same");  
  gcms_pbpb_2p76_nnpart_x_npart->Draw("pX same"); galice_pbpb_2p76_nnpart_x_npart->Draw("pX same");
  gcms_xexe_5p44_nnpart_x_npart->Draw("pX same"); galice_xexe_5p44_nnpart_x_npart->Draw("pX same");
  galice_pbpb_5p02_nnpart_x_npart->Draw("pX same");
  gphobos_auau_0p2_nnpart_x_npart->Draw("pX same"); gphobos_cucu_0p2_nnpart_x_npart->Draw("pX same");
  gnorm->Draw("pX same");

  // watermark();

  TLegend* l3 = new TLegend(0.6, 0.38, 0.9, 0.58);
  TLegendEntry* h2cms = l3->AddEntry((TObject*)0, "CMS", "");
  h2cms->SetTextFont(63); h2cms->SetTextSize(17);
  l3->AddEntry(gnorm, "XeXe 5.44 TeV", "p");
  l3->AddEntry(gcms_pbpb_2p76_nnpart_x_npart, "PbPb 2.76 TeV", "p");
  TLegendEntry* h2alice = l3->AddEntry((TObject*)0, "ALICE", "");
  h2alice->SetTextFont(63); h2alice->SetTextSize(17);
  l3->AddEntry(galice_pbpb_5p02_nnpart_x_npart, "PbPb 5.02 TeV", "p");
  lstyle(l3, 43, 15); l3->Draw();

  // TLegend* l4 = new TLegend(0.6, 0.18, 0.9, 0.30);
  // TLegendEntry* hphobos = l4->AddEntry((TObject*)0, "PHOBOS", "");
  // hphobos->SetTextFont(63); hphobos->SetTextSize(17);
  // l4->AddEntry(gphobos_auau_0p2_nnpart_x_npart, "AuAu 200 GeV", "p");
  // l4->AddEntry(gphobos_cucu_0p2_nnpart_x_npart, "CuCu 200 GeV", "p");
  // lstyle(l4, 43, 15); l4->Draw();

  // TLegend* l5 = new TLegend(0.35, 0.18, 0.65, 0.30);
  // TLegendEntry* h3cms = l5->AddEntry((TObject*)0, "CMS", "");
  // h3cms->SetTextFont(63); h3cms->SetTextSize(17);
  // l5->AddEntry(gcms_pp_13p0_nnpart_x_npart, "pp 13 TeV", "p");
  // l5->AddEntry(gcms_ppb_8p16_nnpart_x_npart, "pPb 8.16 TeV", "p");
  // lstyle(l5, 43, 15); l5->Draw();

  pdf.write(Form("figs/merged/merged-%s-midynorm-int1.png", label.c_str()));
  
  // c3->SaveAs(Form("figs/merged/merged-%s-midynorm-int1.png", label));

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
