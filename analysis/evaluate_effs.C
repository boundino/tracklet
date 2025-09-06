#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <vector>
#include <string>

#include "git/config/configurer.h"

#include "include/cosmetics.h"
#include "include/xjjmypdf.h"

int evaluate_effs(const char* config, const char* label) {
  configurer* conf = new configurer(config);

  auto files = conf->get<std::vector<std::string>>("files");
  auto legends = conf->get<std::vector<std::string>>("legends");

  std::size_t nfiles = files.size();
  if (!nfiles) { printf("error: no files provided!\n"); return 1; }

  auto* foutput = new TFile(Form("output/hh/vertex-%s.root", label), "recreate");

  const int nresbins = 8;
  const double resbins[nresbins + 1] = {
    0, 20, 40, 60, 100, 150, 200, 300, 500};

  TFile* f[nfiles]; TTree* t[nfiles];
  TProfile* heff[nfiles]; TH2D* hvznhit1[nfiles]; TH1D* hres[nfiles]; TH2D* hvzvz[nfiles];
  for (std::size_t i = 0; i < nfiles; ++i) {
    f[i] = new TFile(files[i].data(), "read");
    t[i] = (TTree*)f[i]->Get("TrackletTree12");

    foutput->cd();

    heff[i] = new TProfile(Form("heff_%zu", i), "", 20, 0, 100);
    t[i]->Draw(Form("vz[1]>-98:nhit1>>heff_%zu", i),
               "ntracklet>0 && abs(vz[0])<15", "goff");
    xjjroot::sethempty(heff[i]);
    xjjroot::setthgrstyle(heff[i], colours[i % ncolours], markers[i % ncolours], 1, colours[i % ncolours]);
    // hstyle(heff[i], markers[i % ncolours], colours[i % ncolours]);

    hvznhit1[i] = new TH2D(Form("hvznhit1_%zu", i), "",
                           nresbins, resbins, 100, -0.5, 0.5);
    t[i]->Draw(Form("vz[1]-vz[0]:nhit1>>hvznhit1_%zu", i),
               "ntracklet>0 && abs(vz[0])<15", "goff colz");
    hvznhit1[i]->FitSlicesY();
    hres[i] = (TH1D*)gDirectory->Get(Form("hvznhit1_%zu_2", i));
    xjjroot::sethempty(hres[i]);
    xjjroot::setthgrstyle(hres[i], colours[i % ncolours], markers[i % ncolours], 1, colours[i % ncolours]);
    // hstyle(hres[i], markers[i % ncolours], colours[i % ncolours]);

    hvzvz[i] = new TH2D(Form("hvzvz_%zu", i), "",
                        100, -15, 15, 100, -15, 15);
    t[i]->Draw(Form("vz[1]:vz[2]>>hvzvz_%zu", i),
               "ntracklet>0 && abs(vz[0])<15", "goff");
    xjjroot::sethempty(hvzvz[i]);
  }

  xjjroot::setgstyle(1);
  xjjroot::mypdf pdf(Form("figspdf/vertex/vertex-%s.pdf", label), "c", 600, 600);
   
  pdf.prepare();
  TLegend* l1 = new TLegend(0.5, 0.56, 0.9, 0.72);
  for (std::size_t i = 0; i < nfiles; ++i) {
    hrange(heff[i], 0, 1.2);
    htitle(heff[i], ";Number of pixel hits (layer 1);Efficiency");
    heff[i]->Draw("p e same");
    l1->AddEntry(heff[i], legends[i].data(), "pl");
  }
  lstyle(l1, 43, 16); l1->Draw();
  watermark();
  pdf.write(Form("figs/vertex/vertex-eff-%s.png", label));

  pdf.prepare();
  pdf.getc()->SetLogy();
  TLegend* l2 = new TLegend(0.5, 0.56, 0.9, 0.72);
  for (std::size_t i = 0; i < nfiles; ++i) {
    hrange(hres[i], 0.001, 0.5);
    htitle(hres[i], ";Number of pixel hits (layer 1);Resolution [cm]");
    if(!i) hres[i]->Draw("axis");
    hres[i]->Draw("p e same");
    l2->AddEntry(hres[i], legends[i].data(), "pl");
  }
  lstyle(l2, 43, 16); l2->Draw();
  watermark();
  pdf.write(Form("figs/vertex/vertex-res-%s.png", label));
  pdf.getc()->SetLogy(0);

  pdf.prepare();
  hvzvz[0]->Draw("scat");
  htitle(hvzvz[0], ";v_{z} (RECO) [cm];v_{z} (Tracklet) [cm]");
  watermark();
  pdf.write(Form("figs/vertex/vertex-com-%s.png", label));
  pdf.close();

  foutput->Write("", TObject::kOverwrite);
  foutput->Close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 3) {
    return evaluate_effs(argv[1], argv[2]);
  } else {
    printf("usage: ./evaluate_effs [config] [label]\n");
    return 1;
  }
}
