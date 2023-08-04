// https://github.com/yenjie/trackletAnalysis/blob/pPb-analysis/analysis/dNdetaAnalysis/pAdNdetaVsCMSE.C

#include "TCanvas.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TMath.h"
#include "TLatex.h"
#include "TLegend.h"

#define _PP_NSD_COLOUR kTeal+9
#define _PP_INEL_COLOUR kOrange-2
#define _PA_COLOUR kAzure+1
#define _PA_INEL_COLOUR kViolet+1
#define _AA_COLOUR kRed-4

// pp, pA, AA points from Constantin Loizides
// INEL pp points from Jan Fiete's compilation

int pAdNdetaVsCMSE() {
    TCanvas* c = new TCanvas("c", "", 600, 600);
    gStyle->SetOptStat(0);
    c->Range(0.6363689, -1.758823, 4.003324, 11.77059);
    c->SetFillColor(10);
    c->SetBorderMode(0);
    c->SetBorderSize(0);
    c->SetLogx();
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.06);
    c->SetBottomMargin(0.12);
    c->SetLeftMargin(0.12);
    c->SetFrameFillColor(0);
    c->SetFrameBorderMode(0);
    c->SetFrameFillColor(0);
    c->SetFrameBorderMode(0);
    c->SetTickx();
    c->SetTicky();

    TH2D* frame = new TH2D("frame", "", 1, 5.0, 20000.0, 240, 0.0, 12.0);
    frame->SetStats(0);
    frame->SetFillColor(10);
    frame->SetFillStyle(0);
    frame->SetMarkerSize(1.2);
    frame->GetXaxis()->SetTitle("#sqrt{#font[52]{s}_{NN}}  (GeV)");
    frame->GetXaxis()->SetNdivisions(506);
    frame->GetXaxis()->CenterTitle();
    frame->GetXaxis()->SetTitleFont(43);
    frame->GetXaxis()->SetTitleSize(20);
    frame->GetXaxis()->SetTitleOffset(1.2);
    frame->GetXaxis()->SetLabelFont(43);
    frame->GetXaxis()->SetLabelSize(15);

    frame->GetYaxis()->SetTitle("(d#font[52]{N}_{ch}/d#eta)/(0.5#LT#font[52]{N}_{part}#GT)");
    frame->GetYaxis()->SetNdivisions(506);
    frame->GetYaxis()->CenterTitle();
    frame->GetYaxis()->SetTitleFont(43);
    frame->GetYaxis()->SetTitleSize(20);
    frame->GetYaxis()->SetTitleOffset(1.2);
    frame->GetYaxis()->SetLabelFont(43);
    frame->GetYaxis()->SetLabelSize(15);

    frame->Draw("A");

    TGaxis* gaX = new TGaxis(5.0, 0.0, 20000.0, 0.0, 5.0, 20000.0, 506, "G");
    gaX->SetTitle("#sqrt{#font[52]{s}_{NN}}  (GeV)");
    gaX->SetNdivisions(506);
    gaX->CenterTitle();
    gaX->SetTitleFont(43);
    gaX->SetTitleSize(20);
    gaX->SetTitleOffset(1.2);
    gaX->SetLabelFont(43);
    gaX->SetLabelSize(15);
    gaX->Draw();

    TGaxis* gaXT = new TGaxis(5.0, 12.0, 20000.0, 12.0, 5.0, 20000.0, 506, "G-U");
    gaXT->Draw();

    TGaxis* gaY = new TGaxis(5.0, 0.0, 5.0, 12.0, 0.0, 6.0, 506);
    gaY->SetTitle("(d#font[52]{N}_{ch}/d#eta)/#LT#font[52]{N}_{part}#GT");
    gaY->SetNdivisions(506);
    gaY->CenterTitle();
    gaY->SetTitleFont(43);
    gaY->SetTitleSize(20);
    gaY->SetTitleOffset(1.15);
    gaY->SetLabelFont(43);
    gaY->SetLabelSize(16);
    gaY->Draw();

    TGaxis* gaYR = new TGaxis(20000.0, 0.0, 20000.0, 12.0, 0.0, 6.0, 506, "+U");
    gaYR->Draw();

    TLatex* lcms = new TLatex();
    lcms->SetTextFont(63);
    lcms->SetTextSize(18);
    lcms->DrawLatexNDC(0.13, 0.95, "CMS");

    TLatex* lprelim = new TLatex();
    lprelim->SetTextFont(53);
    lprelim->SetTextSize(16);
    lprelim->DrawLatexNDC(0.20, 0.95, "Preliminary");

    /******************************************************
    pp NSD data points
    ******************************************************/

    TGraphAsymmErrors* Graph_pp_UA1 = new TGraphAsymmErrors(7);
    Graph_pp_UA1->SetName("Graph_pp_UA1");
    Graph_pp_UA1->SetMarkerStyle(33);
    Graph_pp_UA1->SetMarkerSize(1.2);
    Graph_pp_UA1->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_UA1->SetPoint(0, 200, 2.65);
    Graph_pp_UA1->SetPointError(0, 0, 0, 0.08, 0.08);
    Graph_pp_UA1->SetPoint(1, 260, 2.71);
    Graph_pp_UA1->SetPointError(1, 0, 0, 0.07, 0.07);
    Graph_pp_UA1->SetPoint(2, 380, 2.94);
    Graph_pp_UA1->SetPointError(2, 0, 0, 0.09, 0.09);
    Graph_pp_UA1->SetPoint(3, 500, 3.05);
    Graph_pp_UA1->SetPointError(3, 0, 0, 0.09, 0.09);
    Graph_pp_UA1->SetPoint(4, 620, 3.15);
    Graph_pp_UA1->SetPointError(4, 0, 0, 0.09, 0.09);
    Graph_pp_UA1->SetPoint(5, 790, 3.41);
    Graph_pp_UA1->SetPointError(5, 0, 0, 0.1, 0.1);
    Graph_pp_UA1->SetPoint(6, 900, 3.48);
    Graph_pp_UA1->SetPointError(6, 0, 0, 0.1, 0.1);
    Graph_pp_UA1->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_UA5 = new TGraphAsymmErrors(4);
    Graph_pp_UA5->SetName("Graph_pp_UA5");
    Graph_pp_UA5->SetMarkerStyle(23);
    Graph_pp_UA5->SetMarkerSize(1.0);
    Graph_pp_UA5->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_UA5->SetPoint(0, 53, 2);
    Graph_pp_UA5->SetPointError(0, 0, 0, 0.14, 0.14);
    Graph_pp_UA5->SetPoint(1, 200, 2.51);
    Graph_pp_UA5->SetPointError(1, 0, 0, 0.05, 0.05);
    Graph_pp_UA5->SetPoint(2, 546, 3.1);
    Graph_pp_UA5->SetPointError(2, 0, 0, 0.1, 0.1);
    Graph_pp_UA5->SetPoint(3, 900, 3.43);
    Graph_pp_UA5->SetPointError(3, 0, 0, 0.05, 0.05);
    Graph_pp_UA5->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_CDF = new TGraphAsymmErrors(2);
    Graph_pp_CDF->SetName("Graph_pp_CDF");
    Graph_pp_CDF->SetMarkerStyle(20);
    Graph_pp_CDF->SetMarkerSize(0.9);
    Graph_pp_CDF->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_CDF->SetPoint(0, 630, 3.18);
    Graph_pp_CDF->SetPointError(0, 0, 0, 0.16, 0.16);
    Graph_pp_CDF->SetPoint(1, 1800, 3.95);
    Graph_pp_CDF->SetPointError(1, 0, 0, 0.16, 0.16);
    Graph_pp_CDF->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_CMS = new TGraphAsymmErrors(3);
    Graph_pp_CMS->SetName("Graph_pp_CMS");
    Graph_pp_CMS->SetMarkerStyle(21);
    Graph_pp_CMS->SetMarkerSize(1.0);
    Graph_pp_CMS->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_CMS->SetPoint(0, 900, 3.48);
    Graph_pp_CMS->SetPointError(0, 0, 0, 0.13, 0.13);
    Graph_pp_CMS->SetPoint(1, 2360, 4.47);
    Graph_pp_CMS->SetPointError(1, 0, 0, 0.16, 0.16);
    Graph_pp_CMS->SetPoint(2, 7000, 5.78);
    Graph_pp_CMS->SetPointError(2, 0, 0, 0.23, 0.23);
    Graph_pp_CMS->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_ALICE = new TGraphAsymmErrors(2);
    Graph_pp_ALICE->SetName("Graph_pp_ALICE");
    Graph_pp_ALICE->SetMarkerStyle(22);
    Graph_pp_ALICE->SetMarkerSize(1.0);
    Graph_pp_ALICE->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_ALICE->SetPoint(0, 900, 3.58);
    Graph_pp_ALICE->SetPointError(0, 0, 0, 0.12, 0.12);
    Graph_pp_ALICE->SetPoint(1, 2360, 4.43);
    Graph_pp_ALICE->SetPointError(1, 0, 0, 0.12, 0.17);
    Graph_pp_ALICE->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_STAR = new TGraphAsymmErrors(1);
    Graph_pp_STAR->SetName("Graph_pp_STAR");
    Graph_pp_STAR->SetMarkerStyle(29);
    Graph_pp_STAR->SetMarkerSize(1.2);
    Graph_pp_STAR->SetMarkerColor(_PP_NSD_COLOUR);
    Graph_pp_STAR->SetPoint(0, 200, 2.98);
    Graph_pp_STAR->SetPointError(0, 0, 0, 0.34, 0.34);
    Graph_pp_STAR->Draw("E1P");

    // pp NSD curve from Jan Fiete's compilation
    // TF1 *fit_nsd = new TF1("fit_nsd","[0]*(x*x)^[1]",40,18000);
    // fit_nsd->SetLineWidth(1);
    // fit_nsd->SetLineColor(1);
    // fit_nsd->SetChisquare(11.00471);
    // fit_nsd->SetNDF(16);
    // fit_nsd->SetParameter(0,0.8254567);
    // fit_nsd->SetParError(0,0.0487372);
    // fit_nsd->SetParLimits(0,0,0);
    // fit_nsd->SetParameter(1,0.1059566);
    // fit_nsd->SetParError(1,0.00455987);
    // fit_nsd->SetParLimits(1,0,0);
    // fit_nsd->Draw("same");

    TF1* fit_pp_nsd = new TF1("fit_pp_nsd", "[0]*pow(x*x/abs([1]),[2])", 40, 9000);
    fit_pp_nsd->SetLineWidth(1);
    fit_pp_nsd->SetLineColor(_PP_NSD_COLOUR);
    fit_pp_nsd->SetLineStyle(2);
    fit_pp_nsd->SetParameter(0, 0.784483);
    fit_pp_nsd->SetParError(0, 0);
    fit_pp_nsd->SetParLimits(0, 0, 0);
    fit_pp_nsd->SetParameter(1, 1);
    fit_pp_nsd->SetParError(1, 0);
    fit_pp_nsd->SetParLimits(1, 0, 0);
    fit_pp_nsd->SetParameter(2, 0.11);
    fit_pp_nsd->SetParError(2, 0);
    fit_pp_nsd->SetParLimits(2, 0, 0);
    fit_pp_nsd->Draw("same");

    TLegend* leg_pp_NSD = new TLegend(0.15, 0.64, 0.3, 0.92, NULL, "brNDC");
    leg_pp_NSD->SetBorderSize(0);
    leg_pp_NSD->SetTextFont(43);
    leg_pp_NSD->SetTextSize(15);
    leg_pp_NSD->SetFillColor(0);
    leg_pp_NSD->SetFillStyle(0);
    leg_pp_NSD->SetHeader("pp (p#bar{p}) NSD");
    leg_pp_NSD->AddEntry(Graph_pp_CMS, "CMS", "P");
    leg_pp_NSD->AddEntry(Graph_pp_ALICE, "ALICE", "P");
    leg_pp_NSD->AddEntry(Graph_pp_CDF, "CDF", "P");
    leg_pp_NSD->AddEntry(Graph_pp_UA5, "UA5", "P");
    leg_pp_NSD->AddEntry(Graph_pp_UA1, "UA1", "P");
    leg_pp_NSD->AddEntry(Graph_pp_STAR, "STAR", "P");
    leg_pp_NSD->Draw();

    TLatex* pp_nsd_fit_label = new TLatex(2500, 5.2, "#propto #font[52]{s}_{_{NN}}^{0.11}");
    pp_nsd_fit_label->SetTextFont(43);
    pp_nsd_fit_label->SetTextSize(15);
    pp_nsd_fit_label->Draw();

    /******************************************************
    pp INEL data points
    ******************************************************/

    TGraphAsymmErrors* Graph_pp_INEL_ISR = new TGraphAsymmErrors(5);
    Graph_pp_INEL_ISR->SetName("Graph_pp_INEL_ISR");
    Graph_pp_INEL_ISR->SetMarkerStyle(20);
    Graph_pp_INEL_ISR->SetMarkerSize(0.9);
    Graph_pp_INEL_ISR->SetMarkerColor(_PP_INEL_COLOUR);
    Graph_pp_INEL_ISR->SetPoint(0, 23.6, 1.35);
    Graph_pp_INEL_ISR->SetPointError(0, 0, 0, 0.062, 0.062);
    Graph_pp_INEL_ISR->SetPoint(1, 30.8, 1.55);
    Graph_pp_INEL_ISR->SetPointError(1, 0, 0, 0.062, 0.062);
    Graph_pp_INEL_ISR->SetPoint(2, 45.2, 1.66);
    Graph_pp_INEL_ISR->SetPointError(2, 0, 0, 0.075, 0.075);
    Graph_pp_INEL_ISR->SetPoint(3, 53.8, 1.78);
    Graph_pp_INEL_ISR->SetPointError(3, 0, 0, 0.08, 0.08);
    Graph_pp_INEL_ISR->SetPoint(4, 62.8, 1.87);
    Graph_pp_INEL_ISR->SetPointError(4, 0, 0, 0.1, 0.1);
    Graph_pp_INEL_ISR->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_INEL_UA5 = new TGraphAsymmErrors(4);
    Graph_pp_INEL_UA5->SetName("Graph_pp_INEL_UA5");
    Graph_pp_INEL_UA5->SetMarkerStyle(23);
    Graph_pp_INEL_UA5->SetMarkerSize(1.0);
    Graph_pp_INEL_UA5->SetMarkerColor(_PP_INEL_COLOUR);
    Graph_pp_INEL_UA5->SetPoint(0, 53, 1.776);
    Graph_pp_INEL_UA5->SetPointError(0, 0, 0, 0.113, 0.113);
    Graph_pp_INEL_UA5->SetPoint(1, 200, 2.22);
    Graph_pp_INEL_UA5->SetPointError(1, 0, 0, 0.05, 0.05);
    Graph_pp_INEL_UA5->SetPoint(2, 546, 2.84);
    Graph_pp_INEL_UA5->SetPointError(2, 0, 0, 0.1, 0.1);
    Graph_pp_INEL_UA5->SetPoint(3, 880, 3.09);
    Graph_pp_INEL_UA5->SetPointError(3, 0, 0, 0.05, 0.05);
    Graph_pp_INEL_UA5->Draw("E1P");

    TGraphErrors* Graph_pp_INEL_PHOBOS = new TGraphErrors(1);
    Graph_pp_INEL_PHOBOS->SetName("Graph_pp_INEL_PHOBOS");
    Graph_pp_INEL_PHOBOS->SetMarkerStyle(33);
    Graph_pp_INEL_PHOBOS->SetMarkerSize(1.2);
    Graph_pp_INEL_PHOBOS->SetMarkerColor(_PP_INEL_COLOUR);
    Graph_pp_INEL_PHOBOS->SetPoint(0, 200, 2.315);
    Graph_pp_INEL_PHOBOS->SetPointError(0, 0, 0.117);
    Graph_pp_INEL_PHOBOS->Draw("E1P");

    TGraphAsymmErrors* Graph_pp_INEL_ALICE = new TGraphAsymmErrors(3);
    Graph_pp_INEL_ALICE->SetName("Graph_pp_INEL_ALICE");
    Graph_pp_INEL_ALICE->SetMarkerStyle(22);
    Graph_pp_INEL_ALICE->SetMarkerSize(1.0);
    Graph_pp_INEL_ALICE->SetMarkerColor(_PP_INEL_COLOUR);
    Graph_pp_INEL_ALICE->SetPoint(0, 920, 3.02104);
    Graph_pp_INEL_ALICE->SetPointError(0, 0, 0, 0.054948, 0.075834);
    Graph_pp_INEL_ALICE->SetPoint(1, 2360, 3.76606);
    Graph_pp_INEL_ALICE->SetPointError(1, 0, 0, 0.116661, 0.251962);
    Graph_pp_INEL_ALICE->SetPoint(2, 13000, 5.31);
    Graph_pp_INEL_ALICE->SetPointError(2, 0, 0, 0.18, 0.18);
    Graph_pp_INEL_ALICE->Draw("E1P");

    TGraphErrors* Graph_pp_INEL_CMS = new TGraphErrors(1);
    Graph_pp_INEL_CMS->SetName("Graph_pp_INEL_CMS");
    Graph_pp_INEL_CMS->SetMarkerStyle(21);
    Graph_pp_INEL_CMS->SetMarkerSize(1.0);
    Graph_pp_INEL_CMS->SetMarkerColor(_PP_INEL_COLOUR);
    Graph_pp_INEL_CMS->SetPoint(0, 13000, 5.49);
    Graph_pp_INEL_CMS->SetPointError(0, 0, 0.18);
    Graph_pp_INEL_CMS->Draw("E1P");

    TLegend* leg_pp_INEL = new TLegend(0.15, 0.40, 0.30, 0.64, NULL, "brNDC");
    leg_pp_INEL->SetBorderSize(0);
    leg_pp_INEL->SetTextFont(43);
    leg_pp_INEL->SetTextSize(15);
    leg_pp_INEL->SetFillColor(0);
    leg_pp_INEL->SetFillStyle(0);
    leg_pp_INEL->SetHeader("pp (p#bar{p}) Inel.");
    leg_pp_INEL->AddEntry(Graph_pp_INEL_CMS, "CMS", "P");
    leg_pp_INEL->AddEntry(Graph_pp_INEL_ALICE, "ALICE", "P");
    leg_pp_INEL->AddEntry(Graph_pp_INEL_UA5, "UA5", "P");
    leg_pp_INEL->AddEntry(Graph_pp_INEL_PHOBOS, "PHOBOS", "P");
    leg_pp_INEL->AddEntry(Graph_pp_INEL_ISR, "ISR", "P");
    leg_pp_INEL->Draw();

    // pp INEL fit from Jan Fiete's compilation
    TF1* fit_pp_inel = new TF1("fit_pp_inel", "[0]*(x*x)^[1]", 15, 18000);
    fit_pp_inel->SetLineWidth(1);
    fit_pp_inel->SetLineColor(_PP_INEL_COLOUR);
    fit_pp_inel->SetLineStyle(2);
    fit_pp_inel->SetChisquare(6.24491);
    fit_pp_inel->SetNDF(10);
    fit_pp_inel->SetParameter(0, 0.7502614);
    fit_pp_inel->SetParError(0, 0.03019647);
    fit_pp_inel->SetParLimits(0, 0, 0);
    fit_pp_inel->SetParameter(1, 0.1037778);
    fit_pp_inel->SetParError(1, 0.003331816);
    fit_pp_inel->SetParLimits(1, 0, 0);
    fit_pp_inel->Draw("same");

    TLatex* pp_inel_fit_label = new TLatex(2500, 3.25, "#propto #font[52]{s}_{_{NN}}^{0.10}");
    pp_inel_fit_label->SetTextFont(43);
    pp_inel_fit_label->SetTextSize(15);
    pp_inel_fit_label->Draw();

    /******************************************************
    AA data points
    ******************************************************/

    TGraphErrors* Graph_AA_NA50 = new TGraphErrors(1);
    Graph_AA_NA50->SetName("Graph");
    Graph_AA_NA50->SetMarkerStyle(5);
    Graph_AA_NA50->SetMarkerSize(1.2);
    Graph_AA_NA50->SetMarkerColor(_AA_COLOUR);
    Graph_AA_NA50->SetPoint(0, 17.3, 2.14);
    Graph_AA_NA50->SetPointError(0, 0, 0.17);
    Graph_AA_NA50->Draw("E1P");

    TGraphErrors* Graph_AA_STAR = new TGraphErrors(1);
    Graph_AA_STAR->SetName("Graph_AA_STAR");
    Graph_AA_STAR->SetLineWidth(1);
    Graph_AA_STAR->SetMarkerStyle(29);
    Graph_AA_STAR->SetMarkerSize(1.2);
    Graph_AA_STAR->SetMarkerColor(_AA_COLOUR);
    Graph_AA_STAR->SetPoint(0, 130, 3.296512);
    Graph_AA_STAR->SetPointError(0, 0, 0.2209302);
    Graph_AA_STAR->Draw("E1P");

    TGraphErrors* Graph_AA_PHENIX = new TGraphErrors(2);
    Graph_AA_PHENIX->SetName("Graph");
    Graph_AA_PHENIX->SetMarkerStyle(34);
    Graph_AA_PHENIX->SetMarkerSize(1.1);
    Graph_AA_PHENIX->SetMarkerColor(_AA_COLOUR);
    Graph_AA_PHENIX->SetPoint(0, 130, 3.58);
    Graph_AA_PHENIX->SetPointError(0, 0, 0.26);
    Graph_AA_PHENIX->SetPoint(1, 200, 4.09);
    Graph_AA_PHENIX->SetPointError(1, 0, 0.26);
    Graph_AA_PHENIX->Draw("E1P");

    TGraphErrors* Graph_AA_BRAHMS = new TGraphErrors(2);
    Graph_AA_BRAHMS->SetName("Graph");
    Graph_AA_BRAHMS->SetMarkerColor(_AA_COLOUR);
    Graph_AA_BRAHMS->SetMarkerStyle(23);
    Graph_AA_BRAHMS->SetMarkerSize(1.0);
    Graph_AA_BRAHMS->SetPoint(0, 130, 3.14);
    Graph_AA_BRAHMS->SetPointError(0, 0, 0.205);
    Graph_AA_BRAHMS->SetPoint(1, 200, 3.501);
    Graph_AA_BRAHMS->SetPointError(1, 0, 0.32);
    Graph_AA_BRAHMS->Draw("E1P");

    TGraphErrors* Graph_AA_PHOBOS = new TGraphErrors(5);
    Graph_AA_PHOBOS->SetName("Graph_AA_PHOBOS");
    Graph_AA_PHOBOS->SetMarkerStyle(33);
    Graph_AA_PHOBOS->SetMarkerSize(1.2);
    Graph_AA_PHOBOS->SetMarkerColor(_AA_COLOUR);
    Graph_AA_PHOBOS->SetPoint(0, 19.6, 1.87);
    Graph_AA_PHOBOS->SetPointError(0, 0, 0.15);
    Graph_AA_PHOBOS->SetPoint(1, 56, 2.47);
    Graph_AA_PHOBOS->SetPointError(1, 0, 0.27);
    Graph_AA_PHOBOS->SetPoint(2, 62.4, 2.64);
    Graph_AA_PHOBOS->SetPointError(2, 0, 0.2);
    Graph_AA_PHOBOS->SetPoint(3, 130, 3.35);
    Graph_AA_PHOBOS->SetPointError(3, 0, 0.25);
    Graph_AA_PHOBOS->SetPoint(4, 200, 3.76);
    Graph_AA_PHOBOS->SetPointError(4, 0, 0.33);
    Graph_AA_PHOBOS->Draw("E1P");

    // ALICE A+A (0-5%)
    // centrality   dN/deta
    // 0.0-2.5%     10.2 +/- 0.3
    // 2.5-5.0%     9.9  +/- 0.3
    Double_t v_AA_502_ALICE = (10.2 + 9.9) / 2.0;
    Double_t e_AA_502_ALICE = 0.3 / TMath::Sqrt(2.0);
    TGraphErrors* Graph_AA_ALICE = new TGraphErrors(2);
    Graph_AA_ALICE->SetName("Graph_AA_ALICE");
    Graph_AA_ALICE->SetMarkerStyle(22);
    Graph_AA_ALICE->SetMarkerSize(1.0);
    Graph_AA_ALICE->SetMarkerColor(_AA_COLOUR);
    Graph_AA_ALICE->SetPoint(0, 2760 / 1.1, 8.3);
    Graph_AA_ALICE->SetPointError(0, 0, 0.4);
    Graph_AA_ALICE->SetPoint(1, 5020, v_AA_502_ALICE);
    Graph_AA_ALICE->SetPointError(1, 0, e_AA_502_ALICE);
    Graph_AA_ALICE->Draw("E1P");

    // ATLAS A+A (0-6%)
    // http://inspirehep.net/record/925723
    // centrailty    dN/deta|eta=0 / (0.5 N_part)
    // 0-2%          8.8 +/- 0.4
    // 2-4%          8.4 +/- 0.4
    // 4-6%          8.2 +/- 0.4
    Double_t v_AA_ATLAS = (8.8 + 8.4 + 8.2) / 3.0;
    Double_t e_AA_ATLAS = 0.4 / TMath::Sqrt(3.0);
    TGraphErrors* Graph_AA_ATLAS = new TGraphErrors(1);
    Graph_AA_ATLAS->SetName("Graph_AA_ATLAS");
    Graph_AA_ATLAS->SetMarkerStyle(20);
    Graph_AA_ATLAS->SetMarkerSize(0.9);
    Graph_AA_ATLAS->SetMarkerColor(_AA_COLOUR);
    Graph_AA_ATLAS->SetPoint(0, 2760.0 * 1.1, v_AA_ATLAS);
    Graph_AA_ATLAS->SetPointError(0, 0.0, e_AA_ATLAS);
    Graph_AA_ATLAS->Draw("E1P");

    // CMS A+A (0-5%)
    // http://inspirehep.net/record/919733
    // eta    dN/deta/(0.5 N_part)
    // -0.5-0.0 8.47 +/- 0.29
    //  0.0-0.5 8.45 +/- 0.29
    Double_t v_AA_CMS = (8.47 + 8.45) / 2.0;
    Double_t e_AA_CMS = 0.29 / TMath::Sqrt(2.0);
    TGraphErrors* Graph_AA_CMS = new TGraphErrors(1);
    Graph_AA_CMS->SetName("Graph_AA_CMS");
    Graph_AA_CMS->SetMarkerStyle(21);
    Graph_AA_CMS->SetMarkerSize(1.0);
    Graph_AA_CMS->SetMarkerColor(_AA_COLOUR);
    Graph_AA_CMS->SetPoint(0, 2760.0, v_AA_CMS);
    Graph_AA_CMS->SetPointError(0, 0.0, e_AA_CMS);
    Graph_AA_CMS->Draw("E1P");

    TLegend* leg_AA = new TLegend(0.32, 0.56, 0.47, 0.92, NULL, "brNDC");
    leg_AA->SetBorderSize(0);
    leg_AA->SetTextFont(43);
    leg_AA->SetTextSize(15);
    leg_AA->SetFillColor(0);
    leg_AA->SetFillStyle(0);
    leg_AA->SetHeader("Central AA");
    leg_AA->AddEntry(Graph_AA_CMS, "CMS", "P");
    leg_AA->AddEntry(Graph_AA_ALICE, "ALICE", "P");
    leg_AA->AddEntry(Graph_AA_ATLAS, "ATLAS", "P");
    leg_AA->AddEntry(Graph_AA_BRAHMS, "BRAHMS", "P");
    leg_AA->AddEntry(Graph_AA_PHENIX, "PHENIX", "P");
    leg_AA->AddEntry(Graph_AA_STAR, "STAR", "P");
    leg_AA->AddEntry(Graph_AA_PHOBOS, "PHOBOS", "P");
    leg_AA->AddEntry(Graph_AA_NA50, "NA50", "P");
    leg_AA->Draw();

    // AA fit, log function
    // TF1 *fit_AA_log = new TF1("fit_AA_log","0.78*log(x)-0.4",13,9000);
    // fit_AA_log->SetLineWidth(1);
    // fit_AA_log->SetLineStyle(9);
    // fit_AA_log->Draw("same");

    TF1* fit_AA = new TF1("fit_AA", "[0]*pow(x*x/abs([1]),[2])", 13, 6500);
    fit_AA->SetLineWidth(1);
    fit_AA->SetLineColor(_AA_COLOUR);
    fit_AA->SetLineStyle(2);
    fit_AA->SetParameter(0, 0.7772692);
    fit_AA->SetParError(0, 0);
    fit_AA->SetParLimits(0, 0, 0);
    fit_AA->SetParameter(1, 1);
    fit_AA->SetParError(1, 0);
    fit_AA->SetParLimits(1, 0, 0);
    fit_AA->SetParameter(2, 0.15);
    fit_AA->SetParError(2, 0);
    fit_AA->SetParLimits(2, 0, 0);
    fit_AA->Draw("same");

    TLatex* AA_fit_label = new TLatex(2500, 10.2, "#propto #font[52]{s}_{_{NN}}^{0.15}");
    AA_fit_label->SetTextFont(43);
    AA_fit_label->SetTextSize(15);
    AA_fit_label->Draw();

    /******************************************************
    pA data points
    ******************************************************/

    // PHOBOS dAu point
    // -0.6..0.6 http://arxiv.org/abs/nucl-ex/0311009 (BRAHMS looks very similar: http://arxiv.org/abs/nucl-ex/0401025 )
    Double_t v_dAu_PHOBOS = 9.4;
    Double_t e_dAu_PHOBOS = 0.7;
    // <N_Part> from http://arxiv.org/abs/nucl-ex/0311009 as well
    Double_t v_meanNPart_200 = 8.1;
    Double_t e_meanNPart_200 = 0.7;
    Double_t et1_PHOBOS = e_dAu_PHOBOS / v_meanNPart_200; // term 1 in error propagation
    Double_t et2_PHOBOS = v_dAu_PHOBOS / (v_meanNPart_200 * v_meanNPart_200) * e_meanNPart_200; // term 2 in error propagation
    v_dAu_PHOBOS /= (0.5 * v_meanNPart_200);
    e_dAu_PHOBOS = TMath::Sqrt(et1_PHOBOS * et1_PHOBOS + et2_PHOBOS * et2_PHOBOS);
    TGraphErrors* Graph_dAu_PHOBOS = new TGraphErrors(1);
    Graph_dAu_PHOBOS->SetName("Graph_dAu_PHOBOS");
    Graph_dAu_PHOBOS->SetMarkerStyle(33);
    Graph_dAu_PHOBOS->SetMarkerColor(_PA_COLOUR);
    Graph_dAu_PHOBOS->SetMarkerSize(1.2);
    Graph_dAu_PHOBOS->SetPoint(0, 200.0, v_dAu_PHOBOS);
    Graph_dAu_PHOBOS->SetPointError(0, 0.0, e_dAu_PHOBOS);
    Graph_dAu_PHOBOS->Draw("E1P");
    // for comparison: STAR 10.2+/-0.68, N_part 8.31+/-0.37 from http://prc.aps.org/pdf/PRC/v79/i3/e034909

    // ALICE pA point
    // obtained by integrating -1.0..0.0 over the eta spectrum in pA_dNdeta_vs_eta.C
    Double_t v_pA_ALICE = 2.13643;
    Double_t e_pA_ALICE = 0.17366;

    v_pA_ALICE *= 2.0;
    e_pA_ALICE *= 2.0;

    TGraphErrors* Graph_pA_ALICE = new TGraphErrors(1);
    Graph_pA_ALICE->SetName("Graph_pA_ALICE");
    Graph_pA_ALICE->SetMarkerStyle(22);
    Graph_pA_ALICE->SetMarkerSize(1.0);
    Graph_pA_ALICE->SetMarkerColor(_PA_COLOUR);
    Graph_pA_ALICE->SetPoint(0, 5023.0, v_pA_ALICE);
    Graph_pA_ALICE->SetPointError(0, 0.0, e_pA_ALICE);
    Graph_pA_ALICE->Draw("E1P");

    Double_t v_pA_CMS_5020 = 2.23319 * 2; // 17.3072/7.75
    Double_t v_pA_CMS_8160 = 2.47493 * 2; // 20.0964/8.12

    TGraphErrors* Graph_pA_CMS = new TGraphErrors(2);
    Graph_pA_CMS->SetName("Graph_pA_CMS");
    Graph_pA_CMS->SetMarkerStyle(21);
    Graph_pA_CMS->SetMarkerSize(1.0);
    Graph_pA_CMS->SetMarkerColor(_PA_COLOUR);
    Graph_pA_CMS->SetPoint(0, 5020.0, v_pA_CMS_5020);
    Graph_pA_CMS->SetPointError(0, 0.0, 0.045 * 2);
    Graph_pA_CMS->SetPoint(1, 8160.0, v_pA_CMS_8160);
    Graph_pA_CMS->SetPointError(1, 0.0, 0.068 * 2);
    Graph_pA_CMS->Draw("E1P");

    TLegend* leg_pA = new TLegend(0.49, 0.76, 0.64, 0.92, NULL, "brNDC");
    leg_pA->SetBorderSize(0);
    leg_pA->SetTextFont(43);
    leg_pA->SetTextSize(15);
    leg_pA->SetFillColor(0);
    leg_pA->SetHeader("pA NSD");
    leg_pA->AddEntry(Graph_pA_CMS, "pPb CMS", "P");
    leg_pA->AddEntry(Graph_pA_ALICE, "pPb ALICE", "P");
    leg_pA->AddEntry(Graph_dAu_PHOBOS, "dAu PHOBOS", "P");
    leg_pA->Draw();

    // NA35 p+Au
    // THE EUROPEAN PHYSICAL JOURNAL C Volume 2, Number 4 (1998), 643-659, DOI: 10.1007/s100529800867
    // http://www.springerlink.com/content/nxjfa4v8gg6yhpdx/
    // data from http://hepdata.cedar.ac.uk/view/irn3650057/prev Table 30 ( F 20_21. )
    // y    dN/dy
    // 2.6  1.7959 +/- 0.0580
    // 3    1.6415 +/- 0.0557
    // 3.4  1.4367 +/- 0.0507
    // average over 3 points and divide by <N_part>
    // from http://web-docs.gsi.de/~misko/overlap/over.cgi: <N_part> = 3.3
    // from Constantin's MC Glauber: <N_part> = 4.623
    // from the paper itself: <N_wounded> = 4.7
    Double_t hijcorr = 0.807607;
    Double_t hijrele = 0.1;
    Double_t v_pA_NA35 = hijcorr * 2. * (1.7959 + 1.6415 + 1.4367) / 3.0 / (0.5 * 4.623);
    // 0.807607 factor from HIJING simulation, 2.0 for h-
    Double_t e_pA_NA35 = hijcorr * 2. * TMath::Sqrt(0.0580 * 0.0580 + 0.0557 * 0.0557 + 0.0507 * 0.0507) / 3.0 / (0.5 * 4.623);
    e_pA_NA35 = e_pA_NA35 * (1 + hijrele);
    TGraphErrors* Graph_pA_NA35 = new TGraphErrors(1);
    Graph_pA_NA35->SetName("Graph_pA_NA35");
    Graph_pA_NA35->SetMarkerStyle(20);
    Graph_pA_NA35->SetMarkerSize(0.9);
    Graph_pA_NA35->SetMarkerColor(_PA_INEL_COLOUR);
    Graph_pA_NA35->SetPoint(0, 19.4, v_pA_NA35);
    Graph_pA_NA35->SetPointError(0, 0.0, e_pA_NA35);
    Graph_pA_NA35->Draw("E1P");

    // E178 p+Pb
    // PHYSICAL REVIEW D Volume 22, Number 1 (1980), 13-35
    // J. E. Elias et al.
    // p momentum | y shift | <N_part> |
    // 50 GeV/c   | 2.312   | 4.743    |
    // 100 GeV/c  | 2.658   | 4.784    |
    // 200 GeV/c  | 2.998   | 4.832    |
    Double_t gen_corr = 0.95;
    Double_t gen_relerr = 1.1;
    Double_t nu = 3.8;
    Double_t v_pA_E178[3] = {0};
    Double_t e_pA_E178[3] = {0};
    v_pA_E178[0] = 2 * gen_corr * ((0.16 + 0.22 * nu) / (2.25 - 1.99) + (0.26 + 0.60 * nu - 0.08 * nu * nu) / (2.76 - 2.25)) / 2 / (nu + 1);
    e_pA_E178[0] = TMath::Sqrt(0.09 * 0.09 + 0.11 * 0.11) / 2;
    v_pA_E178[1] = 2 * gen_corr * ((0.41 + 0.49 * nu - 0.02 * nu * nu) / (2.76 - 2.25) + (0.32 + 0.30 * nu - 0.03 * nu * nu) / (3.08 - 2.76)) / 2 / (nu + 1);
    e_pA_E178[1] = TMath::Sqrt(0.06 * 0.06 + 0.06 * 0.06) / 2;
    v_pA_E178[2] = 2 * gen_corr * ((0.18 + 0.43 * nu - 0.04 * nu * nu) / (3.08 - 2.76) + (0.08 + 0.51 * nu - 0.07 * nu * nu) / (3.38 - 3.08)) / 2 / (nu + 1);
    e_pA_E178[2] = TMath::Sqrt(0.06 * 0.06 + 0.06 * 0.06) / 2;
    TGraphErrors* Graph_pA_E178 = new TGraphErrors(3);
    Graph_pA_E178->SetName("Graph_pA_E178");
    Graph_pA_E178->SetMarkerStyle(23);
    Graph_pA_E178->SetMarkerSize(1.0);
    Graph_pA_E178->SetMarkerColor(_PA_INEL_COLOUR);
    Graph_pA_E178->SetPoint(0, 9.69, v_pA_E178[0]);
    Graph_pA_E178->SetPointError(0, 0.0, e_pA_E178[0] * 0.1);
    Graph_pA_E178->SetPoint(1, 13.7, v_pA_E178[1]);
    Graph_pA_E178->SetPointError(1, 0.0, e_pA_E178[1] * 0.06);
    Graph_pA_E178->SetPoint(2, 19.4, v_pA_E178[2]);
    Graph_pA_E178->SetPointError(2, 0.0, e_pA_E178[2] * 0.06);
    Graph_pA_E178->Draw("E1P");

    TLegend* leg_pA_INEL = new TLegend(0.49, 0.64, 0.64, 0.76, NULL, "brNDC");
    leg_pA_INEL->SetBorderSize(0);
    leg_pA_INEL->SetTextFont(43);
    leg_pA_INEL->SetTextSize(15);
    leg_pA_INEL->SetFillColor(0);
    leg_pA_INEL->SetHeader("pA Inel.");
    leg_pA_INEL->AddEntry(Graph_pA_E178, "pPb E178", "P");
    leg_pA_INEL->AddEntry(Graph_pA_NA35, "pAu NA35", "P");
    leg_pA_INEL->Draw();

    // FNAL p+Freon <A>=17
    // midrapidity point at 360GeV 3.33 +/- 0.177
    // read off Fig.5a at midrapidity 3.3
    // sqrt(s)=26GeV rough calc.
    // <N_part> = 1.3 (calculated with misko s_NN inel = 33 mb)
    // <N_part> = 2.75 (calculated by Constantin with MC Glauber for O, A=16)
    // Double_t v_pA_FNAL_360 = 3.33 / (0.5*2.75);
    // Double_t e_pA_FNAL_360 = 0.177 / (0.5*2.75);
    // TGraphErrors * Graph_pA_FNAL_360 = new TGraphErrors(1);
    // Graph_pA_FNAL_360->SetName("Graph_pA_FNAL_360");
    // Graph_pA_FNAL_360->SetMarkerStyle(24);
    // Graph_pA_FNAL_360->SetMarkerSize(1.2);
    // Graph_pA_FNAL_360->SetMarkerColor(_PA_COLOUR);
    // Graph_pA_FNAL_360->SetPoint(0,26.0,v_pA_FNAL_360);
    // Graph_pA_FNAL_360->SetPointError(0,0.0,e_pA_FNAL_360);
    // Graph_pA_FNAL_360->Draw("E1P");

    // TGraphErrors * Graph_pA_ALL = new TGraphErrors(3);
    // Graph_pA_ALL->SetName("Graph_pA_ALL");
    // Graph_pA_ALL->SetPoint(0,19.4,v_pA_NA35);
    // Graph_pA_ALL->SetPointError(0,0.0,e_pA_NA35);
    // Graph_pA_ALL->SetPoint(1,200.0,v_dAu_PHOBOS);
    // Graph_pA_ALL->SetPointError(1,0.0,e_dAu_PHOBOS);
    // Graph_pA_ALL->SetPoint(2,5023.0,v_pA_ALICE);
    // Graph_pA_ALL->SetPointError(2,0.0,e_pA_ALICE);

    // TF1 *fit_pA_log = new TF1("fit_pA_log","[0]*log(x[0])+[1]",13.0,9000.0);
    // Graph_pA_ALL->Fit(fit_pA_log);
    // fit_pA_log->SetLineColor();
    // fit_pA_log->SetLineWidth(1);
    // fit_pA_log->SetLineStyle(9);
    // fit_pA_log->Draw("same");

    // TF1 *fit_pA_pow = new TF1("fit_pA_pow","[0]*pow(x[0],[1])",13.0,9000.0);
    // Graph_pA_ALL->Fit(fit_pA_pow);
    // fit_pA_pow->SetLineColor();
    // fit_pA_pow->SetLineWidth(1);
    // fit_pA_pow->SetLineStyle(9);
    // fit_pA_pow->Draw("same");

    c->Modified();
    c->cd();
    c->SetSelected(c);

    c->SaveAs("figspdf/results/dNdeta_vs_CMSE.pdf");
    c->SaveAs("figs/results/dNdeta_vs_CMSE.png");

    return 0;
}

/*
Temporary collection of potentially interesting papers

p+Au 1GeV Igo et al. http://www.sciencedirect.com/science/article/pii/055032136790168X
p+C->pi 158GeV NA49 http://arxiv.org/pdf/hep-ex/0606028.pdf
NA49 p+p p+A group http://spshadrons.web.cern.ch/spshadrons/index.html

FNAL:
-> Norm? p+Al, p+Au 360GeV EHS http://www.springerlink.com/content/t526r88333217047/

RHIC:
BRAHMS d+Au (wide y range) http://arxiv.org/pdf/nucl-ex/0401025v4.pdf
PHOBOS d+Au scaling http://arxiv.org/pdf/nucl-ex/0409021v1.pdf

Theory:
CGC: http://arxiv.org/pdf/1209.2001v2.pdf
*/

int main() {
    return pAdNdetaVsCMSE();
}
