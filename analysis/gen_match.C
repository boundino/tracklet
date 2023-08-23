#include <TFile.h>
#include <TTree.h>

#include "include/defines.h"
#include "include/utili.h"
#include "include/xjjcuti.h"

#include "include/acceptance.h"

#define PIXELS1P(EXPAND)                        \
  BPIX1P(EXPAND)                                \
  FPIX1P(EXPAND)                                \

#define TRKLTS2P(EXPAND)                        \
  BTRKLT2P(EXPAND)                              \
  FTRKLT2P(EXPAND)                              \

#include "include/structs.h"

const float maxDeltaR = 2.;
int gen_match(std::string inputname, std::string outputname) {
  auto inf = TFile::Open(inputname.c_str());

  auto ttruth = (TTree*)inf->Get("truth");
  truth_t truth; set_event_truth(ttruth, truth);

#define READ_TRACKLET(q, w)                                     \
  auto t##q##w = (TTree*)inf->Get("TrackletTree"#q#w);          \
  data_t data##q##w; set_event_data(t##q##w, data##q##w);       \

  TRKLTS2P(READ_TRACKLET)

    auto outf = new TFile(outputname.c_str(), "recreate");

#define CREATE_NEW_TREE(q, w)                                           \
  auto newt##q##w = new TTree("TrackletTree"#q#w, "genmatch");          \
  branch_event_data(newt##q##w, data##q##w);                            \
  float *pt##q##w = new float[MAXT]; newt##q##w->Branch("gpt", pt##q##w, "gpt[ntracklet]/F"); \
  float *eta##q##w = new float[MAXT]; newt##q##w->Branch("geta", eta##q##w, "geta[ntracklet]/F"); \
  float *phi##q##w = new float[MAXT]; newt##q##w->Branch("gphi", phi##q##w, "gphi[ntracklet]/F"); \
  int *acc##q##w = new int[MAXT]; newt##q##w->Branch("acc", acc##q##w, "acc[ntracklet]/I"); \
  branch_event_truth(newt##q##w, truth);                                \

  TRKLTS2P(CREATE_NEW_TREE)

    
    inf->cd();
  auto nentries = t12->GetEntries();
  for (int i=0; i<nentries; i++) {
    xjjc::progressbar(i, nentries, 1000);
    ttruth->GetEntry(i);

#define INCLUDE_VZ_BINS
#define INCLUDE_ETA_BINS
#include "include/bins.h"


#define FILL_TREE(q, w)                                                 \
    t##q##w->GetEntry(i);                                               \
    for (int j=0; j<data##q##w.ntracklet; j++) {                        \
      pt##q##w[j] = -1;                                                 \
      eta##q##w[j] = -99;                                               \
      phi##q##w[j] = -99;                                               \
      acc##q##w[j] = 0;                                                 \
      auto ivz = xjjc::findibin(&vzb, data##q##w.vz[1]);                \
      auto ieta = xjjc::findibin(&etab, data##q##w.eta1[j]);            \
      if (ivz >=0 && ieta >= 0) {                                       \
        acc##q##w[j] = a##q##w[(nvz-ivz-1)*neta+ieta]; \
      }                                                                 \
    }                                                                   \
    for (int k=0; k<truth.npart; k++) {                                 \
      float drnow = maxDeltaR;                                          \
      int jnow = -1;                                                    \
      for (int j=0; j<data##q##w.ntracklet; j++) {                      \
        float drmatch1 = xjjana::cal_dr(data##q##w.phi1[j], data##q##w.eta1[j], truth.phi[k], truth.eta[k]); \
        float drmatch2 = xjjana::cal_dr(data##q##w.phi2[j], data##q##w.eta2[j], truth.phi[k], truth.eta[k]); \
        if (drmatch1 < maxDeltaR && drmatch2 < maxDeltaR && drmatch1 < drnow) {                 \
          drnow = drmatch1;                                             \
          jnow = j;                                                     \
        }                                                               \
      }                                                                 \
      if (jnow >= 0) {                                                  \
        pt##q##w[jnow] = truth.pt[k];                                   \
        eta##q##w[jnow] = truth.eta[k];                                 \
        phi##q##w[jnow] = truth.phi[k];                                 \
      }                                                                 \
    }                                                                   \
    newt##q##w->Fill();                                                 \

    TRKLTS2P(FILL_TREE)    
      }
  xjjc::progressbar_summary(nentries);

  outf->Write();

  return 0;
  
}

int main(int argc, char* argv[]) {
  if (argc == 3)
    return gen_match(argv[1], argv[2]);

  return 1;
}
