
struct centvar {
  int nhfp;
  int nhfn;
  int hlt;
  int cluscomp;
  float hft;
  int nv;
  float vz[8];
};

void branches(TTree* t, centvar& cv) {
  t->Branch("nhfp", &cv.nhfp, "nhfp/I");
  t->Branch("nhfn", &cv.nhfn, "nhfn/I");
  t->Branch("hft", &cv.hft, "hft/F");
  t->Branch("hlt", &cv.hlt, "hlt/I");
  t->Branch("cluscomp", &cv.cluscomp, "cluscomp/I");
  t->Branch("nv", &cv.nv, "nv/I");
  t->Branch("vz", cv.vz, "vz[nv]/F");
}

void setbranches(TTree* t, centvar& cv) {
  t->SetBranchAddress("nhfp", &cv.nhfp);
  t->SetBranchAddress("nhfn", &cv.nhfn);
  t->SetBranchAddress("hft", &cv.hft);
  t->SetBranchAddress("hlt", &cv.hlt);
  t->SetBranchAddress("cluscomp", &cv.cluscomp);
  t->SetBranchAddress("nv", &cv.nv);
  t->SetBranchAddress("vz", cv.vz);
}

void clone(centvar& cv1, centvar& cv2, float hfscale=1) {
  cv1.nhfp = cv2.nhfp;
  cv1.nhfn = cv2.nhfn;
  cv1.hft = cv2.hft * hfscale;
  cv1.hlt = cv2.hlt;
  cv1.cluscomp = cv2.cluscomp;
  cv1.nv = cv2.nv;
  for (int i=0; i<cv2.nv; i++) {
    cv1.vz[i] = cv2.vz[i];
  }
}
