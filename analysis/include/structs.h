#ifndef STRUCTS_H
#define STRUCTS_H

#define MAXT   50000
#define MAXH   50000
#define MAXP   50000

struct data_t {
  int run, event, lumi, bx;
  int nv; float vx[8], vy[8], vz[8]; float weight;
  int hlt; int nhfp, nhfn; float hft; int cluscomp;
  int nhfp_low, nhfp_high, nhfn_low, nhfn_high;
  float *eta1, *phi1, *r1;
  float *eta2, *phi2, *r2;
  float *deta, *dphi, *dr2;
  int ntracklet, nhits, nhit1, nhit2;

  data_t()
  {
    eta1 = new float[MAXT];
    phi1 = new float[MAXT];
    r1 = new float[MAXT];
    eta2 = new float[MAXT];
    phi2 = new float[MAXT];
    r2 = new float[MAXT];
    deta = new float[MAXT];
    dphi = new float[MAXT];
    dr2 = new float[MAXT];
  }
};

struct truth_t {
  int process, npart;
  float *pt, *eta, *phi;
  int *chg, *pdg;

  truth_t()
  {
    pt = new float[MAXP];
    eta = new float[MAXP];
    phi = new float[MAXP];
    chg = new int[MAXP];
    pdg = new int[MAXP];
  }
};

void branch_event_data(TTree* t, data_t& data) {

  t->Branch("run", &data.run, "run/I");
  t->Branch("lumi", &data.lumi, "lumi/I");
  t->Branch("event", &data.event, "event/I");
  t->Branch("bx", &data.bx, "bx/I");

  t->Branch("nv", &data.nv, "nv/I");
  t->Branch("vx", data.vx, "vx[nv]/F");
  t->Branch("vy", data.vy, "vy[nv]/F");
  t->Branch("vz", data.vz, "vz[nv]/F");
  t->Branch("weight", &data.weight, "weight/F");

  t->Branch("hlt", &data.hlt, "hlt/I");
  t->Branch("nhfp", &data.nhfp, "nhfp/I");
  t->Branch("nhfn", &data.nhfn, "nhfn/I");
  t->Branch("nhfp_low", &data.nhfp_low, "nhfp_low/I");
  t->Branch("nhfn_low", &data.nhfn_low, "nhfn_low/I");
  t->Branch("nhfp_high", &data.nhfp_high, "nhfp_high/I");
  t->Branch("nhfn_high", &data.nhfn_high, "nhfn_high/I");
  t->Branch("hft", &data.hft, "hft/F");
  t->Branch("cluscomp", &data.cluscomp, "cluscomp/I");

  t->Branch("nhits", &data.nhits, "nhits/I");
  t->Branch("nhit1", &data.nhit1, "nhit1/I");
  t->Branch("nhit2", &data.nhit2, "nhit2/I");
  t->Branch("ntracklet", &data.ntracklet, "ntracklet/I");
  t->Branch("eta1", data.eta1, "eta1[ntracklet]/F");
  t->Branch("phi1", data.phi1, "phi1[ntracklet]/F");
  t->Branch("r1", data.r1, "r1[ntracklet]/F");
  t->Branch("eta2", data.eta2, "eta2[ntracklet]/F");
  t->Branch("phi2", data.phi2, "phi2[ntracklet]/F");
  t->Branch("r2", data.r2, "r2[ntracklet]/F");
  t->Branch("deta", data.deta, "deta[ntracklet]/F");
  t->Branch("dphi", data.dphi, "dphi[ntracklet]/F");
  t->Branch("dr2", data.dr2, "dr2[ntracklet]/F");
}

void branch_event_truth(TTree* t, truth_t& truth) {
  t->Branch("process", &truth.process, "process/I");
  t->Branch("npart", &truth.npart, "npart/I");
  t->Branch("pt", truth.pt, "pt[npart]/F");
  t->Branch("eta", truth.eta, "eta[npart]/F");
  t->Branch("phi", truth.phi, "phi[npart]/F");
  t->Branch("pdg", truth.pdg, "pdg[npart]/I");
  t->Branch("chg", truth.chg, "chg[npart]/I");
}

void set_event_data(TTree* t, data_t& data) {

  t->SetBranchAddress("run", &data.run);
  t->SetBranchAddress("lumi", &data.lumi);
  t->SetBranchAddress("event", &data.event);
  t->SetBranchAddress("bx", &data.bx);

  t->SetBranchAddress("nv", &data.nv);
  t->SetBranchAddress("vx", data.vx);
  t->SetBranchAddress("vy", data.vy);
  t->SetBranchAddress("vz", data.vz);
  t->SetBranchAddress("weight", &data.weight);

  t->SetBranchAddress("hlt", &data.hlt);
  t->SetBranchAddress("nhfp", &data.nhfp);
  t->SetBranchAddress("nhfn", &data.nhfn);
  t->SetBranchAddress("nhfp_low", &data.nhfp_low);
  t->SetBranchAddress("nhfn_low", &data.nhfn_low);
  t->SetBranchAddress("nhfp_high", &data.nhfp_high);
  t->SetBranchAddress("nhfn_high", &data.nhfn_high);
  t->SetBranchAddress("hft", &data.hft);
  t->SetBranchAddress("cluscomp", &data.cluscomp);

  t->SetBranchAddress("nhits", &data.nhits);
  t->SetBranchAddress("nhit1", &data.nhit1);
  t->SetBranchAddress("nhit2", &data.nhit2);
  t->SetBranchAddress("ntracklet", &data.ntracklet);
  t->SetBranchAddress("eta1", data.eta1);
  t->SetBranchAddress("phi1", data.phi1);
  t->SetBranchAddress("r1", data.r1);
  t->SetBranchAddress("eta2", data.eta2);
  t->SetBranchAddress("phi2", data.phi2);
  t->SetBranchAddress("r2", data.r2);
  t->SetBranchAddress("deta", data.deta);
  t->SetBranchAddress("dphi", data.dphi);
  t->SetBranchAddress("dr2", data.dr2);
}

void set_event_truth(TTree* t, truth_t& truth) {
  t->SetBranchAddress("process", &truth.process);
  t->SetBranchAddress("npart", &truth.npart);
  t->SetBranchAddress("pt", truth.pt);
  t->SetBranchAddress("eta", truth.eta);
  t->SetBranchAddress("phi", truth.phi);
  t->SetBranchAddress("pdg", truth.pdg);
  t->SetBranchAddress("chg", truth.chg);
}

struct PixelEvent {
  int run, lumi, event, bx;
  float bsx, bsy, bsz;
  int nv; float vx[8], vy[8], vz[8];
  int hlt; int nhfp, nhfn; float hft; int cluscomp;
  int nhfp_low, nhfp_high, nhfn_low, nhfn_high;

#define DECLARE_LAYER_VARIABLES(q)              \
  int nhits##q;                                 \
  float *eta##q, *phi##q, *r##q, *cs##q;        \

  PIXELS1P(DECLARE_LAYER_VARIABLES)

  int process, npart;
  float *pt, *eta, *phi;
  int *chg, *pdg;

#define DECLARE_LAYER_VARIABLES_ARRAY(q)        \
  eta##q = new float[MAXH];                     \
  phi##q = new float[MAXH];                     \
  r##q = new float[MAXH];                       \
  cs##q = new float[MAXH];                      \

  PixelEvent()
  {
    pt = new float[MAXP];
    eta = new float[MAXP];
    phi = new float[MAXP];
    chg = new int[MAXP];
    pdg = new int[MAXP];

    PIXELS1P(DECLARE_LAYER_VARIABLES_ARRAY)

      }
};

void set_pixel_data(TTree* t, PixelEvent& par) {
  t->SetBranchAddress("run", &par.run);
  t->SetBranchAddress("event", &par.event);
  t->SetBranchAddress("lumi", &par.lumi);
  t->SetBranchAddress("bx", &par.bx);

  t->SetBranchAddress("bsx", &par.bsx);
  t->SetBranchAddress("bsy", &par.bsy);
  t->SetBranchAddress("bsz", &par.bsz);
  t->SetBranchAddress("nv", &par.nv);
  t->SetBranchAddress("vx", par.vx);
  t->SetBranchAddress("vy", par.vy);
  t->SetBranchAddress("vz", par.vz);

  t->SetBranchAddress("hlt", &par.hlt);
  t->SetBranchAddress("nhfp", &par.nhfp);
  t->SetBranchAddress("nhfn", &par.nhfn);
  t->SetBranchAddress("nhfp_low", &par.nhfp_low);
  t->SetBranchAddress("nhfn_low", &par.nhfn_low);
  t->SetBranchAddress("nhfp_high", &par.nhfp_high);
  t->SetBranchAddress("nhfn_high", &par.nhfn_high);
  t->SetBranchAddress("hft", &par.hft);
  t->SetBranchAddress("cluscomp", &par.cluscomp);

#define SET_LAYER_BRANCHES(q)                           \
  t->SetBranchAddress("nhits" #q, &par.nhits##q);       \
  t->SetBranchAddress("eta" #q, par.eta##q);            \
  t->SetBranchAddress("phi" #q, par.phi##q);            \
  t->SetBranchAddress("r" #q, par.r##q);                \
  t->SetBranchAddress("cs" #q, par.cs##q);              \

  PIXELS1P(SET_LAYER_BRANCHES)

    t->SetBranchAddress("process", &par.process);
  t->SetBranchAddress("npart", &par.npart);
  t->SetBranchAddress("pt", par.pt);
  t->SetBranchAddress("eta", par.eta);
  t->SetBranchAddress("phi", par.phi);
  t->SetBranchAddress("chg", par.chg);
  t->SetBranchAddress("pdg", par.pdg);
}


#endif   /* STRUCTS_H */
