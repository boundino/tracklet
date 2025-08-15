#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TMath.h"
#include "TH3.h"

#define PIXELS1P(EXPAND)   \
   BPIX1P(EXPAND)          \
   FPIX1P(EXPAND)          \

#define COUNT(q)  + 1
#define NPIXEL1P  (0 PIXELS1P(COUNT))

#define TRKLTS2P(EXPAND)   \
   BTRKLT2P(EXPAND)        \
   FTRKLT2P(EXPAND)        \

#include "include/defines.h"
#include "include/structs.h"
#include "include/rechit.h"
#include "include/tracklet.h"
#include "include/hf.h"

#define NSAMPLES  1
static const float vzpar[NSAMPLES][2] = { // for reweight
   {0.175361, 5.29697},    /* private Jul31 */
};

#define BKG_ARG(q)   , float add_bkg_l##q = 0
#define BKG_ARGV(q)  , atof(argv[11 + q])

int transmute_trees(const char* input,
                    const char* output,
                    uint64_t start = 0,
                    int end = -1,
                    int sample = -1,
                    bool reweight = 0,
                    float pileup = 0.0f,
                    bool random = 0,
                    float split = 0,
                    float drop = 0,
                    bool smear = 0
                    PIXELS1P(BKG_ARG))
{
   printf("................................................................\n");

   auto* finput = TFile::Open(input);
   auto* t = (TTree*)finput->Get("pixel/PixelTree");

   float vz_shift = 0;
   float vx = 0, vy = 0, vz, recovx = -99, recovy = -99;
   
   if (t->GetEntries("run < 10") != 0) {
      printf("$ Monte Carlo analysis\n");

      /* Realistic2024PbPbCollision */
      // https://github.com/cms-sw/cmssw/blob/master/IOMC/EventVertexGenerators/python/VtxSmearedParameters_cfi.py#L1088
      // vx = 0.0302546; // BS x0
      // vy = -0.0170382; // BS y0
      // /* pixel barycentre */
      // vz_shift = -0.355; // barycentre, no flip 

      /* Nominal2025OOCollision */
      // https://github.com/cms-sw/cmssw/blob/master/IOMC/EventVertexGenerators/python/VtxSmearedParameters_cfi.py#L1088
      // vx = 0.0184194; // BS x0
      // vy = -0.0141852; // BS y0
      // vz_shift = -0.367294; // barycentre, 391856 (May 9)
      /* Realistic2025OORealistic not official */
      vx = 0.0158483; // BS x0
      vy = -0.00652439; // BS y0
      vz_shift = -0.365973; // barycentre, 394153
      
   } else {
      printf("$ data analysis\n");

      /* Data */
      /* 150X_dataRun3_Prompt_v3 */
      vx = 0.0859173;
      vy = -0.183422;

      pileup = 0.0f;
      sample = -1;
      reweight = 0;
   }

   if (pileup) {
      printf("$ mixing events\n  > pileup: %f\n", pileup);
   }

   if (reweight && (sample < 0 || sample > NSAMPLES - 1)) {
      printf("! invalid sample [%i] for vertex reweighting!\n", sample);
      return 1;
   }

   if (random) {
      printf("$ random vertex\n");
      pileup = 0.0f;
      reweight = 0;
   } else if (reweight) {
      printf("$ reweighting vertex\n  > sample: %i\n", sample);
   } else {
      printf("$ tracklet vertex\n");
   }

   PixelEvent par;
   set_pixel_data(t, par);

   TFile* foutput = new TFile(output, "recreate");

#define DECLARE_TREES(q, w)                                                   \
   TTree* trklttree##q##w = new TTree("TrackletTree" #q #w, "tracklets");     \
   data_t trkltdata##q##w;                                                    \
   branch_event_data(trklttree##q##w, trkltdata##q##w);                       \

   TRKLTS2P(DECLARE_TREES);

   TTree* ttruth = new TTree("truth", "particles");
   truth_t truth;
   branch_event_truth(ttruth, truth);

   // gRandom->SetSeed(144);
   gRandom->SetSeed(std::hash<std::string>{}(input)); // cannot use fixed seed for condor jobs ...
   printf(" # init random: %f\n", gRandom->Rndm());

#define PROJECT_BACKGROUND(q)                                                 \
   TH3F* hl##q = 0;                                                           \
   if (add_bkg_l##q) {                                                        \
      printf(" # projecting hit distribution of layer " #q "\n");             \
      hl##q = new TH3F("hl" #q, "", 80, 2, 18, 200, -4, 4, 160, -3.2, 3.2);   \
      t->Project("hl" #q, "phi" #q ":eta" #q ":r" #q, "", "", 8192, start);   \
   }                                                                          \

   PIXELS1P(PROJECT_BACKGROUND);

   uint64_t nentries = t->GetEntries();
   if (end > nentries || end < 0) end = nentries;
   printf(" # number of events: %lu\n", nentries);
   printf("................................................................\n");

   std::vector<uint64_t> events;
   for (uint64_t i=start; i<end; i=i+events.size()) {
      events.clear();

      uint32_t npileup = 1;
      if (pileup != 0) {
         do {
            npileup = gRandom->Poisson(pileup);
         } while (!npileup);
      }

      for (uint32_t e=0; e<npileup && i+e<end; ++e)
         events.push_back(i+e);

      bool hltor = 0;
      float hftsum = 0;

#define DECLARE_HITS(q) std::vector<rechit> layer##q;
      PIXELS1P(DECLARE_HITS);

      for (const auto& event : events) {
         t->GetEntry(event);
         if (event % 10 == 0)
            printf("   run: %i, entry: %lu\n", par.run, event);

#ifdef  CENTRALITY
         if (hfbin(par.hft) != CENTRALITY)
            continue;
#endif  /* CENTRALITY */

#define ADD_BACKGROUND(q)                                                     \
         int bkghits##q = 0;                                                  \
         if (add_bkg_l##q)                                                    \
            for (int h=0; h<par.nhits##q; ++h)                                \
               if (gRandom->Rndm() < add_bkg_l##q)                            \
                  ++bkghits##q;                                               \
                                                                              \
         if (bkghits##q != 0) {                                               \
            for (int j=par.nhits##q; j<par.nhits##q + bkghits##q; ++j) {      \
               double eta, phi, r;                                            \
               hl##q->GetRandom3(r, eta, phi);                                \
               par.eta##q[j] = eta;                                           \
               par.phi##q[j] = phi;                                           \
               par.r##q[j] = r;                                               \
            }                                                                 \
            par.nhits##q += bkghits##q;                                       \
         }                                                                    \

         PIXELS1P(ADD_BACKGROUND);

#define POPULATE_HITS(q) populate(layer##q, par, q, split, drop, smear);
         PIXELS1P(POPULATE_HITS);

         hltor |= par.hlt;
         hftsum += par.hft;
      }

#ifdef  CENTRALITY
      if (hftsum == 0) { continue; }
#endif  /* CENTRALITY */

      if (layer1.size() > MAXH)
         continue;

#define SAVE_VERTICES(q, w)                                                   \
      trkltdata##q##w.nv = par.nv + 1;                                        \
      trkltdata##q##w.vx[0] = par.vx[0];                                      \
      trkltdata##q##w.vy[0] = par.vy[0];                                      \
      trkltdata##q##w.vz[0] = par.vz[0];                                      \

      TRKLTS2P(SAVE_VERTICES);

      if (random) {
         vz = gRandom->Rndm() * 30 - 15 - vz_shift;
      } else {
         std::vector<rechit> layer1v(layer1);
         std::vector<rechit> layer2v(layer2);

         project(layer1v, vx, vy, 0);
         project(layer2v, vx, vy, 0);

         vz = reco_vertex(layer1v, layer2v, 0.09, 0.12);
      }

#define SET_VERTEX(q, w)                                                \
      trkltdata##q##w.vz[1] = vz;                                       \
      trkltdata##q##w.vx[1] = recovx;                                   \
      trkltdata##q##w.vy[1] = recovy;                                   \
      for(int j=1; j<par.nv; j++)                                       \
        {                                                               \
          trkltdata##q##w.vx[j+1] = par.vx[j];                          \
          trkltdata##q##w.vy[j+1] = par.vy[j];                          \
          trkltdata##q##w.vz[j+1] = par.vz[j];                          \
        }                                                               \
      
      TRKLTS2P(SET_VERTEX);

      if (!reweight && sample != -1) {
#define CLEAR_HITS(q) std::vector<rechit>().swap(layer##q);
         PIXELS1P(CLEAR_HITS);
      }

      float event_weight = 1.;
      if (reweight) {
         float event_vz = (vz < -98 ? par.vz[0] : vz) + vz_shift;
         if(event_vz < -20 || event_vz > 20)
           event_weight = 0;
         else
           {
             /* run 362294 + bad private MC */
             double data_pdf = TMath::Gaus(event_vz, 0.465915, 5.44665, 1);
             double mc_pdf = TMath::Gaus(event_vz, vzpar[sample][0], vzpar[sample][1], 1);
             
             event_weight = event_weight * data_pdf / mc_pdf;
           }
      }

      auto sorteta = [](const rechit& a, const rechit& b) -> bool {
         return a.eta < b.eta;
      };

#define PROJECT_HITS(q)                                                       \
      project(layer##q, vx, vy, vz);                                          \
      std::sort(layer##q.begin(), layer##q.end(), sorteta);                   \

      PIXELS1P(PROJECT_HITS);

#define RECONSTRUCT_TRACKLETS(q, w)                                           \
      std::vector<Tracklet> tracklets##q##w;                                  \
      tracklets##q##w.reserve(layer##q.size());                               \
      reco_tracklets(tracklets##q##w, layer##q, layer##w);                    \

      TRKLTS2P(RECONSTRUCT_TRACKLETS);

#define FILL_TREE(q, w)                                                       \
      trkltdata##q##w.run        = par.run;                                   \
      trkltdata##q##w.lumi       = par.lumi;                                  \
      trkltdata##q##w.event      = par.event;                                 \
      trkltdata##q##w.bx         = par.bx;                                    \
      trkltdata##q##w.hlt        = hltor;                                     \
      trkltdata##q##w.cluscomp   = par.cluscomp;                              \
      trkltdata##q##w.nhfp       = par.nhfp;                                  \
      trkltdata##q##w.nhfn       = par.nhfn;                                  \
      trkltdata##q##w.nhfp_low   = par.nhfp_low;                              \
      trkltdata##q##w.nhfn_low   = par.nhfn_low;                              \
      trkltdata##q##w.nhfp_high  = par.nhfp_high;                             \
      trkltdata##q##w.nhfn_high  = par.nhfn_high;                             \
      trkltdata##q##w.hft        = hftsum;                                    \
      trkltdata##q##w.weight     = event_weight;                              \
                                                                              \
      for (int j=0; j<trkltdata##q##w.nv; j++)                                \
         trkltdata##q##w.vz[j] += vz_shift;                                   \
                                                                              \
      trkltdata##q##w.nhits      = layer1.size() + layer2.size();             \
      trkltdata##q##w.nhit1      = layer##q.size();                           \
      trkltdata##q##w.nhit2      = layer##w.size();                           \
                                                                              \
      trkltdata##q##w.ntracklet  = tracklets##q##w.size();                    \
      for (int j=0; j<trkltdata##q##w.ntracklet; j++) {                       \
         trkltdata##q##w.eta1[j] = tracklets##q##w[j].eta1;                   \
         trkltdata##q##w.eta2[j] = tracklets##q##w[j].eta2;                   \
         trkltdata##q##w.phi1[j] = tracklets##q##w[j].phi1;                   \
         trkltdata##q##w.phi2[j] = tracklets##q##w[j].phi2;                   \
         trkltdata##q##w.r1[j]   = tracklets##q##w[j].r1;                     \
         trkltdata##q##w.r2[j]   = tracklets##q##w[j].r2;                     \
         trkltdata##q##w.deta[j] = tracklets##q##w[j].deta;                   \
         trkltdata##q##w.dphi[j] = tracklets##q##w[j].dphi;                   \
         trkltdata##q##w.dr2[j]  = tracklets##q##w[j].dr2;                    \
      }                                                                       \
                                                                              \
      trklttree##q##w->Fill();                                                \

      TRKLTS2P(FILL_TREE);

      truth.process = par.process;
      truth.npart = 0;

      if (random) { continue; }

      for (int j=0; j<par.npart; j++) {
         if (fabs(par.eta[j]) > 4 || par.chg[j] == 0 ||
               abs(par.pdg[j]) == 11 || abs(par.pdg[j]) == 13)
            continue;
         truth.pt[truth.npart]  = par.pt[j];
         truth.eta[truth.npart] = par.eta[j];
         truth.phi[truth.npart] = par.phi[j];
         truth.chg[truth.npart] = par.chg[j];
         truth.pdg[truth.npart] = par.pdg[j];
         truth.npart++;
      }

      ttruth->Fill();
   }

#define WRITE_TREE(q, w) trklttree##q##w->Write("", TObject::kOverwrite);
   TRKLTS2P(WRITE_TREE);
   ttruth->Write("", TObject::kOverwrite);

   foutput->Close();
   finput->Close();

   return 0;
}

int main(int argc, char* argv[]) {
   if (argc == 3) {
      return transmute_trees(argv[1], argv[2]);
   } else if (argc == 5) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
   } else if (argc == 7) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]));
   } else if (argc == 8) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]));
   } else if (argc == 9) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atoi(argv[8]));
   } else if (argc == 10) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atoi(argv[8]),
            atof(argv[9]));
   } else if (argc == 11) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atoi(argv[8]),
            atof(argv[9]), atof(argv[10]));
   } else if (argc == 12) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atoi(argv[8]),
            atof(argv[9]), atof(argv[10]), atoi(argv[11]));
   } else if (argc == 12 + NPIXEL1P) {
      return transmute_trees(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]),
            atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atoi(argv[8]),
            atof(argv[9]), atof(argv[10]), atoi(argv[11]) PIXELS1P(BKG_ARGV));
   } else {
      printf("usage: ./transmute_trees [in out] [start end]\n"
             "  [sample reweight] [pileup] [random]\n"
             "  [split] [drop] [smear] [hits * %i]\n", NPIXEL1P);
      return 1;
   }
}
