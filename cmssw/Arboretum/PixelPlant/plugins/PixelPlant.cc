// -*- C++ -*-
//
// Package:    Arboretum/PixelPlant
// Class:      PixelPlant
//
/**\class PixelPlant PixelPlant.cc Arboretum/PixelPlant/plugins/PixelPlant.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Ran Bi
//         Created:  Mon, 18 Dec 2017 22:06:42 GMT
//
//

// system include files
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/EmptyGroupDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HeavyIonEvent/interface/ClusterCompatibility.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ROOT files
#include "TTree.h"

#define PIXELS1P(EXPAND)                        \
  EXPAND(1)                                     \
  EXPAND(2)                                     \
  EXPAND(3)                                     \
  EXPAND(4)                                     \
  EXPAND(5)                                     \
  EXPAND(6)                                     \
  EXPAND(7)                                     \

#define MAXPART   500000
#define MAXHITS   500000
#define MAXVTX    100

struct PixelEvent {
  int run;
  int lumi;
  int event;
  int bx;

  float bsx;
  float bsy;
  float bsz;

  // vertex information
  int nv;
  float vx[MAXVTX];
  float vy[MAXVTX];
  float vz[MAXVTX];
  float vxerr[MAXVTX];
  float vyerr[MAXVTX];
  float vzerr[MAXVTX];

  // hlt decision
  int hlt;

  // HIClusterCompatibilityFilter decision
  int cluscomp;

  // hf information
  int nhfp, nhfp_low, nhfp_high;
  int nhfn, nhfn_low, nhfn_high;
  float hft;
  float hftp;
  float hftm;
  // hf adc
  int maxhfadcp;
  int maxhfadcn;
  
#define STRUCT(q)                               \
  int nhits##q;                                 \
  float eta##q[MAXHITS];                        \
  float phi##q[MAXHITS];                        \
  float r##q[MAXHITS];                          \
  float cs##q[MAXHITS];                         \
  float ch##q[MAXHITS];                         \

  // pixel hits
  PIXELS1P(STRUCT);

  // gen particles
  int process;
  float hi_npart;
  float hi_b;
  int npart;
  float pt[MAXPART];
  float eta[MAXPART];
  float phi[MAXPART];
  int pdg[MAXPART];
  int chg[MAXPART];
};

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PixelPlant : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit PixelPlant(const edm::ParameterSet&);
  ~PixelPlant();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  // ------------ member data ------------
  void fill_hlt(const edm::Event&);
  void fill_beamspot(const edm::Event&);
  void fill_vertices(const edm::Event&);
  void fill_hf(const edm::Event&);
  void fill_hfadc(const edm::Event&);
  void fill_pixels(const edm::Event&);
  void fill_particles(const edm::Event&);
  void fill_cluscomp(const edm::Event&);
  double determineQuality(const reco::ClusterCompatibility& cc, double minZ, double maxZ);

  bool fillhlt_;
  bool fillgen_;
  bool fillhf_;
  bool fillhfadc_;
  bool fillcluscomp_;

  edm::EDGetTokenT<edm::TriggerResults> hlt_;
  std::vector<std::string> paths_;

  edm::EDGetTokenT<reco::BeamSpot> beamspot_;
  edm::EDGetTokenT<edm::SimVertexContainer> genvertex_;
  std::vector<edm::EDGetTokenT<reco::VertexCollection>> vertices_;
  edm::EDGetTokenT<CaloTowerCollection> towers_;
  edm::EDGetTokenT<pat::PackedCandidateCollection> pfcands_;
  edm::EDGetTokenT<QIE10DigiCollection> hfqie_;
  edm::EDGetTokenT<SiPixelRecHitCollection> pixels_;
  edm::EDGetTokenT<edm::HepMCProduct> generator_;
  edm::EDGetTokenT<edm::GenHIEvent> genhi_;
  edm::EDGetTokenT<reco::ClusterCompatibility> cluscomp_;
  

  edm::ESHandle<ParticleDataTable> pdt_;
  edm::ESGetToken<ParticleDataTable, edm::DefaultRecord> pdt_token_;
  edm::ESHandle<TrackerGeometry> geo_;
  edm::ESGetToken<TrackerGeometry, TrackerDigiGeometryRecord> geo_token_;
  edm::ESHandle<TrackerTopology> topo_;
  edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> topo_token_;
  edm::ESHandle<HcalDbService> hcal_;
  edm::ESGetToken<HcalDbService, HcalDbRecord> hcal_token_;


  edm::Service<edm::service::TriggerNamesService> nameserv_;

  PixelEvent pix_;

  edm::Service<TFileService> fs_;
  TTree* tpix_;

  std::vector<uint32_t> deads = {
    // 0x12315010, 0x12315018, 0x12311018, 0x12310008, 0x1230a010, 0x12308014, 0x12302018, 0x1232100c, 0x1231c008,  // l3
  };
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

PixelPlant::PixelPlant(const edm::ParameterSet& iConfig) {
  // now do what ever initialization is needed
  usesResource("TFileService");

  pdt_token_ = esConsumes();
  geo_token_ = esConsumes();
  topo_token_ = esConsumes();
  hcal_token_ = esConsumes();

  beamspot_ = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamspot_tag"));
  for (const auto& tag : iConfig.getParameter<std::vector<edm::InputTag>>("vertex_tags"))
    vertices_.push_back(consumes<reco::VertexCollection>(tag));
  pixels_ = consumes<SiPixelRecHitCollection>(iConfig.getParameter<edm::InputTag>("pixel_tag"));

  fillhlt_ = iConfig.getParameter<bool>("fillhlt");
  if (fillhlt_) {
    hlt_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("hlt_tag"));
    paths_ = iConfig.getParameter<std::vector<std::string>>("hlt_paths");
  }

  fillcluscomp_ = iConfig.getParameter<bool>("fillcluscomp");
  if (fillcluscomp_) {
    cluscomp_ = consumes<reco::ClusterCompatibility>(iConfig.getParameter<edm::InputTag>("cluscomp_tag"));
  }

  fillhf_ = iConfig.getParameter<bool>("fillhf");
  if (fillhf_) {
    towers_ = consumes<CaloTowerCollection>(iConfig.getParameter<edm::InputTag>("hf_tag"));
    pfcands_ = consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("hf_tag"));
  }

  fillhfadc_ = iConfig.getParameter<bool>("fillhfadc");
  if (fillhfadc_) {
    hfqie_ = consumes<QIE10DigiCollection>(iConfig.getParameter<edm::InputTag>("hfadc_tag"));
  }

  fillgen_ = iConfig.getParameter<bool>("fillgen");
  if (fillgen_) {
    generator_ = consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("generator_tag"));
    genhi_ = consumes<edm::GenHIEvent>(iConfig.getParameter<edm::InputTag>("genhi_tag"));
    genvertex_ = consumes<edm::SimVertexContainer>(iConfig.getParameter<edm::InputTag>("genvertex_tag"));
  }
}

PixelPlant::~PixelPlant() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

void PixelPlant::fill_hlt(const edm::Event& iEvent) {
  edm::Handle<edm::TriggerResults> tr;
  iEvent.getByToken(hlt_, tr);
  const edm::TriggerResults* hlt = tr.product();

  if (!hlt->size()) {
    pix_.hlt = -1;
    return;
  }

  std::vector<std::string> allpaths;
  nameserv_->getTrigPaths(*hlt, allpaths);
  for (const auto& path : paths_) {
    const auto it = std::find(allpaths.begin(), allpaths.end(), path);
    if (hlt->accept(it - allpaths.begin())) {
      pix_.hlt = 1;
      return;
    }
  }

  pix_.hlt = 0;
}

void PixelPlant::fill_beamspot(const edm::Event& iEvent) {
  edm::Handle<reco::BeamSpot> bs;
  iEvent.getByToken(beamspot_, bs);
  const reco::BeamSpot* beamspot = bs.product();

  pix_.bsx = beamspot->x0();
  pix_.bsy = beamspot->y0();
  pix_.bsz = beamspot->z0();
}

void PixelPlant::fill_vertices(const edm::Event& iEvent) {
  // fill generator vertex
  pix_.vx[0] = -99;
  pix_.vy[0] = -99;
  pix_.vz[0] = -99;

  if (fillgen_) {
    edm::Handle<edm::SimVertexContainer> sv;
    iEvent.getByToken(genvertex_, sv);
    const edm::SimVertexContainer* genvertex = sv.product();

    if (!genvertex->empty()) {
      if (fabs((*genvertex)[0].position().z()) < 30) {
        pix_.vx[0] = (*genvertex)[0].position().x();
        pix_.vy[0] = (*genvertex)[0].position().y();
        pix_.vz[0] = (*genvertex)[0].position().z();
      }
    }
  }

  pix_.nv = 1;

  // fill reconstructed vertices
  for (const auto& vertex_ : vertices_) {
    edm::Handle<reco::VertexCollection> vs;
    iEvent.getByToken(vertex_, vs);
    const reco::VertexCollection* vertex = vs.product();

    reco::Vertex pv;
    std::size_t daughters = 0;
    std::size_t nvtxnotfake = 0;
    for (const auto& v : *vertex) {
      if (v.isFake()) continue;
      if (daughters <= v.tracksSize()) {
        daughters = v.tracksSize();
        pv = v;
      }
      nvtxnotfake++;
    }

    if (nvtxnotfake) {
      pix_.vx[pix_.nv] = pv.position().x();
      pix_.vy[pix_.nv] = pv.position().y();
      pix_.vz[pix_.nv] = pv.position().z();

      pix_.vxerr[pix_.nv] = pv.xError();
      pix_.vyerr[pix_.nv] = pv.yError();
      pix_.vzerr[pix_.nv] = pv.zError();
    } else {
      pix_.vx[pix_.nv] = -99;
      pix_.vy[pix_.nv] = -99;
      pix_.vz[pix_.nv] = -99;

      pix_.vxerr[pix_.nv] = 0;
      pix_.vyerr[pix_.nv] = 0;
      pix_.vzerr[pix_.nv] = 0;
    }

    pix_.nv++;
  }
}

void PixelPlant::fill_hf(const edm::Event& iEvent) {

  int nhfn = 0, nhfn_low = 0, nhfn_high = 0;
  int nhfp = 0, nhfp_low = 0, nhfp_high = 0;  
  
  float hftp = 0;
  float hftm = 0;
  
  edm::Handle<pat::PackedCandidateCollection> pfs;
  iEvent.getByToken(pfcands_, pfs);

  if (pfs.isValid()) {
    for (const auto& pfcand : *pfs) {
      if (pfcand.pdgId() == 1 || pfcand.pdgId() == 2) {
        const bool eta_plus = (pfcand.eta() > 3.0) && (pfcand.eta() < 6.0);
        const bool eta_minus = (pfcand.eta() < -3.0) && (pfcand.eta() > -6.0);
        if (pfcand.et() < 0.0) continue;
        if (eta_plus) {
          if (pfcand.energy() > 3.) { nhfp_low++; }
          if (pfcand.energy() > 4.) { nhfp++; }
          if (pfcand.energy() > 5.) { nhfp_high++; }
          hftp += pfcand.et();
        }
        if (eta_minus) {
          if (pfcand.energy() > 3.) { nhfn_low++; }
          if (pfcand.energy() > 4.) { nhfn++; }
          if (pfcand.energy() > 5.) { nhfn_high++; }
          hftm += pfcand.et();
        }
      }
    }
  } // if (pfs.isValid())
  else {
    edm::Handle<CaloTowerCollection> ts;
    iEvent.getByToken(towers_, ts);
    const CaloTowerCollection* towers = ts.product();
     
    if (ts.isValid()) {
      for (const auto& cal : *towers) {
        if (cal.ietaAbs() < 30) { continue; }
         
        // https://github.com/cms-sw/cmssw/blob/master/DataFormats/Candidate/interface/LeafCandidate.h#L124-L125
        if (cal.zside() > 0) {
          if (cal.energy() > 3.) { nhfp_low++; }
          if (cal.energy() > 4.) { nhfp++; }
          if (cal.energy() > 5.) { nhfp_high++; }
          hftp += cal.pt();
        } else {
          if (cal.energy() > 3.) { nhfn_low++; }
          if (cal.energy() > 4.) { nhfn++; }
          if (cal.energy() > 5.) { nhfn_high++; }
          hftm += cal.pt();
        }
      }
    } // else if (ts.isValid())
  }
  pix_.nhfp = nhfp;
  pix_.nhfn = nhfn;
  pix_.nhfp_low = nhfp_low;
  pix_.nhfn_low = nhfn_low;
  pix_.nhfp_high = nhfp_high;
  pix_.nhfn_high = nhfn_high;

  pix_.hft = hftp + hftm;
  pix_.hftp = hftp;
  pix_.hftm = hftm;
}

void PixelPlant::fill_hfadc(const edm::Event& iEvent) {

  int maxhfadcp = -1, maxhfadcn = -1;

  edm::Handle<QIE10DigiCollection> digi;
  iEvent.getByToken(hfqie_, digi);
  CaloSamples tool;

  for (auto& it : *digi) {
    const QIE10DataFrame& frame(it);
    const HcalDetId cell(frame.id());
    int sub = cell.subdet();
    if(sub != HcalSubdetector::HcalForward) continue; // DataFormats/HcalDetId/interface/HcalSubdetector.h
    if (cell.depth() > 4) continue;

    const HcalQIECoder* channelCoder = hcal_->getHcalCoder(cell);
    const HcalQIEShape* shape = hcal_->getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder, *shape);
    coder.adc2fC(frame, tool);
      
    if(frame.samples() != tool.size()) { edm::LogError("HFAdcToGeV") << "frame.samples() != tool.size()"; break; }
    
    int soi = tool.presamples();
    int ampl = 0;
    for (int ii = 0; ii < tool.size(); ii++) {
      QIE10DataFrame::Sample sam = frame[ii];
      if((sam.soi() && ii != soi) || (!sam.soi() && ii == soi)) { edm::LogError("HFAdcToGeV") << "Wrong soi information"; ampl = -1; break; }
      if(ii != soi) continue;
      ampl += sam.adc();
    }
    int ieta = cell.ieta();
    if (ieta > 0 && ampl > maxhfadcp) {
      maxhfadcp = ampl;
    }
    if (ieta < 0 && ampl > maxhfadcn) {
      maxhfadcn = ampl;
    }
  }

  pix_.maxhfadcp = maxhfadcp;
  pix_.maxhfadcn = maxhfadcn;
}

void PixelPlant::fill_pixels(const edm::Event& iEvent) {
  edm::Handle<SiPixelRecHitCollection> pix;
  iEvent.getByToken(pixels_, pix);
  const SiPixelRecHitCollection* pixdets = pix.product();

#define INIT_HITS(q)    pix_.nhits##q = 0;
  PIXELS1P(INIT_HITS)

    for (const auto& det : *pixdets) {
      DetId detid = det.detId();

      if (detid.det() != DetId::Tracker) { continue; }
      if (detid.subdetId() != PixelSubdetector::PixelBarrel &&
          detid.subdetId() != PixelSubdetector::PixelEndcap) { continue; }

      unsigned layer = topo_->layer(detid);
      if (detid.subdetId() == PixelSubdetector::PixelEndcap)
        layer += 4;

      // mask modules
      if (std::find(deads.begin(), deads.end(), detid.rawId()) != deads.end()) {
        continue;
      }

      //   // ! mask all outer modules in forward disks <- hotspot issue found in 2025 OO
      // auto rawid = static_cast<uint32_t>(detid.rawId());
      // uint16_t catid_z = rawid & 0xFFF;
      // uint8_t catid_m = (rawid >> 12) & 0xFF;
      // uint16_t catid_t = (rawid >> 20) & 0xFFF;
      // std::cout<<std::hex<<rawid<<": "<<catid_t<<" + "<<static_cast<int>(catid_m)<<" + "<<catid_z<<std::endl;
      // if (detid.subdetId() == PixelSubdetector::PixelEndcap) {
      //   if ((catid_m >= 0x41 && catid_m <= 0x56) ||
      //       (catid_m >= 0x81 && catid_m <= 0x96) ||
      //       (catid_m >= 0xC1 && catid_m <= 0xD6)) {
      //     continue;
      //   }
      // }

      const SiPixelRecHitCollection::DetSet dethits = *(pixdets->find(detid));
      for (const auto& hit : dethits) {
        const PixelGeomDetUnit* detunit = dynamic_cast<const PixelGeomDetUnit*>(geo_->idToDet(hit.geographicalId()));
        GlobalPoint gpos = detunit->toGlobal(hit.localPosition());
        math::XYZVector pos(gpos.x(), gpos.y(), gpos.z());

        double eta = pos.eta();
        double phi = pos.phi();
        double r = pos.rho();

        // if (detid.subdetId() == PixelSubdetector::PixelBarrel && layer == 1) { // ! smiling cut 
        //   if (hit.cluster()->size() < ( 0.78235*TMath::Abs(sinh(eta))-2.8373 ))
        //     continue;
        // }
      
#define FILL(q)                                                 \
        pix_.eta##q[pix_.nhits##q] = eta;                       \
        pix_.phi##q[pix_.nhits##q] = phi;                       \
        pix_.r##q[pix_.nhits##q] = r;                           \
        pix_.cs##q[pix_.nhits##q] = hit.cluster()->size();      \
        pix_.ch##q[pix_.nhits##q] = hit.cluster()->charge();    \
        pix_.nhits##q++;                                        \

#define CASE(q)                                 \
        case q: FILL(q) break;                  \

        switch (layer) {
          PIXELS1P(CASE)
            }
      }
    }
}

void PixelPlant::fill_particles(const edm::Event& iEvent) {
  edm::Handle<edm::HepMCProduct> gen;
  iEvent.getByToken(generator_, gen);
  const HepMC::GenEvent* generator = gen->GetEvent();
  pix_.process = generator->signal_process_id();

  pix_.hi_npart = -1;
  pix_.hi_b = -1;   
  edm::Handle<edm::GenHIEvent> mchievt;
  if(iEvent.getByToken(genhi_, mchievt)) {
    pix_.hi_b = mchievt->b();
    pix_.hi_npart = mchievt->Npart();
  }
   
  pix_.npart = 0;
  for (auto it = generator->particles_begin(); it != generator->particles_end(); ++it) {
    if ((*it)->status() != 1) { continue; }
    if (fabs((*it)->momentum().eta()) > 4) { continue; }

    int pdg = (*it)->pdg_id();
    int chg = (int)(pdt_->particle(pdg)->charge());
    if (chg == 0) { continue; }

    pix_.eta[pix_.npart] = (*it)->momentum().eta();
    pix_.phi[pix_.npart] = (*it)->momentum().phi();
    pix_.pt[pix_.npart] = (*it)->momentum().perp();

    pix_.pdg[pix_.npart] = pdg;
    pix_.chg[pix_.npart] = chg;

    pix_.npart++;
  }
}

void PixelPlant::fill_cluscomp(const edm::Event& iEvent) {
  edm::Handle<reco::ClusterCompatibility> cc;
  iEvent.getByToken(cluscomp_, cc);

  pix_.cluscomp = 1;
  
  std::vector<double> clusterPars_ = {0.0, 0.0045};
  int nhitsTrunc_ = 150;
  double clusterTrunc_ = 2.0;
  
  double clusVtxQual = determineQuality(*cc, -20.0, 20.05);
  double nPxlHits = cc->nValidPixelHits();

  // construct polynomial cut on cluster vertex quality vs. npixelhits
  double polyCut = 0;
  for (unsigned int i = 0; i < clusterPars_.size(); i++) {
    polyCut += clusterPars_[i] * std::pow((double)nPxlHits, (int)i);
  }
  if (nPxlHits < nhitsTrunc_)
    polyCut = 0;  // don't use cut below nhitsTrunc_ pixel hits
  if (polyCut > clusterTrunc_ && clusterTrunc_ > 0)
    polyCut = clusterTrunc_;  // no cut above clusterTrunc_

  // std::cout<<clusVtxQual<<" "<<polyCut<<std::endl;
  if (clusVtxQual < polyCut)
    pix_.cluscomp = 0;

}

double PixelPlant::determineQuality(const reco::ClusterCompatibility& cc, double minZ, double maxZ) {
  // will compare cluster compatibility at a determined best
  // z position to + and - 10 cm from the best position

  float best_z = 0.;
  int best_n = 0., low_n = 0., high_n = 0.;

  // look for best vertex z position within zMin to zMax range
  // best position is determined by maximum nHit with
  // chi used for breaking a tie

  int nhits_max = 0;
  double chi_max = 1e+9;
  for (int i = 0; i < cc.size(); i++) {
    if (cc.z0(i) > maxZ || cc.z0(i) < minZ)
      continue;
    if (cc.nHit(i) == 0)
      continue;
    if (cc.nHit(i) > nhits_max) {
      chi_max = 1e+9;
      nhits_max = cc.nHit(i);
    }
    if (cc.nHit(i) >= nhits_max && cc.chi(i) < chi_max) {
      chi_max = cc.chi(i);
      best_z = cc.z0(i);
      best_n = cc.nHit(i);
    }
  }

  // find compatible clusters at + or - 10 cm of the best,
  // or get as close as possible in terms of z position.

  double low_target = best_z - 10.0;
  double high_target = best_z + 10.0;
  double low_match = 1000., high_match = 1000.;
  for (int i = 0; i < cc.size(); i++) {
    if (fabs(cc.z0(i) - low_target) < low_match) {
      low_n = cc.nHit(i);
      low_match = fabs(cc.z0(i) - low_target);
    }
    if (fabs(cc.z0(i) - high_target) < high_match) {
      high_n = cc.nHit(i);
      high_match = fabs(cc.z0(i) - high_target);
    }
  }

  // determine vertex compatibility quality score

  double clusVtxQual = 0.0;
  if ((low_n + high_n) > 0)
    clusVtxQual = (2.0 * best_n) / (low_n + high_n);  // A/B
  else if (best_n > 0)
    clusVtxQual = 1000.0;  // A/0 (set to arbitrarily large number)
  else
    clusVtxQual = 0;

  return clusVtxQual;
}

// ------------ method called for each event ------------
void PixelPlant::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  pdt_ = iSetup.getHandle(pdt_token_);
  geo_ = iSetup.getHandle(geo_token_);
  topo_ = iSetup.getHandle(topo_token_);
  hcal_ = iSetup.getHandle(hcal_token_);

  pix_.run = (int)iEvent.id().run();
  pix_.lumi = (int)iEvent.luminosityBlock();
  pix_.event = (int)iEvent.id().event();
  pix_.bx = (int)iEvent.bunchCrossing();

  fill_beamspot(iEvent);
  fill_vertices(iEvent);
  fill_pixels(iEvent);
   
  if (fillhlt_) { fill_hlt(iEvent); } else { pix_.hlt = 1; }
  if (fillcluscomp_) { fill_cluscomp(iEvent); } else { pix_.cluscomp = 1; }
  if (fillhf_) { fill_hf(iEvent); } else {
    pix_.nhfp = 0; pix_.nhfn = 0;
    pix_.nhfp_low = 0; pix_.nhfn_low = 0;
    pix_.nhfp_high = 0; pix_.nhfn_high = 0;
    pix_.hft = 0; pix_.hftp = 0; pix_.hftm = 0; }
  if (fillhfadc_) { fill_hfadc(iEvent); } else {
    pix_.maxhfadcp = -2; pix_.maxhfadcn = -2; }
  if (fillgen_) { fill_particles(iEvent); } else {
    pix_.process = -1; pix_.npart = 0; }

  tpix_->Fill();
}

// ------------ method called once each job just before starting event loop ------------
void PixelPlant::beginJob() {
  tpix_ = fs_->make<TTree>("PixelTree", "pixels");

  tpix_->Branch("run", &pix_.run, "run/I");
  tpix_->Branch("lumi", &pix_.lumi, "lumi/I");
  tpix_->Branch("event", &pix_.event, "event/I");
  tpix_->Branch("bx", &pix_.bx, "bx/I");

  tpix_->Branch("bsx", &pix_.bsx, "bsx/F");
  tpix_->Branch("bsy", &pix_.bsy, "bsy/F");
  tpix_->Branch("bsz", &pix_.bsz, "bsz/F");

  tpix_->Branch("nv", &pix_.nv, "nv/I");
  tpix_->Branch("vx", pix_.vx, "vx[nv]/F");
  tpix_->Branch("vy", pix_.vy, "vy[nv]/F");
  tpix_->Branch("vz", pix_.vz, "vz[nv]/F");

  tpix_->Branch("hlt", &pix_.hlt, "hlt/I");
  tpix_->Branch("cluscomp", &pix_.cluscomp, "cluscomp/I");

  tpix_->Branch("nhfp", &pix_.nhfp, "nhfp/I");
  tpix_->Branch("nhfp_low", &pix_.nhfp_low, "nhfp_low/I");
  tpix_->Branch("nhfp_high", &pix_.nhfp_high, "nhfp_high/I");
  tpix_->Branch("nhfn", &pix_.nhfn, "nhfn/I");
  tpix_->Branch("nhfn_low", &pix_.nhfn_low, "nhfn_low/I");
  tpix_->Branch("nhfn_high", &pix_.nhfn_high, "nhfn_high/I");
  tpix_->Branch("hft", &pix_.hft, "hft/F");
  tpix_->Branch("hftp", &pix_.hftp, "hftp/F");
  tpix_->Branch("hftm", &pix_.hftm, "hftm/F");

  tpix_->Branch("maxhfadcp", &pix_.maxhfadcp, "maxhfadcp/I");
  tpix_->Branch("maxhfadcn", &pix_.maxhfadcn, "maxhfadcn/I");

#define BRANCH(q)                                                       \
  tpix_->Branch("nhits" #q, &pix_.nhits##q, "nhits" #q "/I");           \
  tpix_->Branch("eta" #q, pix_.eta##q, "eta" #q "[nhits" #q "]/F");     \
  tpix_->Branch("phi" #q, pix_.phi##q, "phi" #q "[nhits" #q "]/F");     \
  tpix_->Branch("r" #q, pix_.r##q, "r" #q "[nhits" #q "]/F");           \
  tpix_->Branch("cs" #q, pix_.cs##q, "cs" #q "[nhits" #q "]/F");        \
  tpix_->Branch("ch" #q, pix_.ch##q, "ch" #q "[nhits" #q "]/F");        \

  PIXELS1P(BRANCH);

  tpix_->Branch("process", &pix_.process, "process/I");
  tpix_->Branch("hi_npart", &pix_.hi_npart, "hi_npart/F");
  tpix_->Branch("hi_b", &pix_.hi_b, "hi_b/F");
  tpix_->Branch("npart", &pix_.npart, "npart/I");
  tpix_->Branch("pt", pix_.pt, "pt[npart]/F");
  tpix_->Branch("eta", pix_.eta, "eta[npart]/F");
  tpix_->Branch("phi", pix_.phi, "phi[npart]/F");
  tpix_->Branch("pdg", pix_.pdg, "pdg[npart]/I");
  tpix_->Branch("chg", pix_.chg, "chg[npart]/I");
}

// ------------ method called once each job just after ending the event loop ------------
void PixelPlant::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module ------------
void PixelPlant::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;

  desc.add<edm::InputTag>("beamspot_tag", edm::InputTag("offlineBeamSpot"));
  desc.add<std::vector<edm::InputTag>>("vertex_tags", {});
  desc.add<edm::InputTag>("pixel_tag", edm::InputTag("siPixelRecHits"));

  desc.ifValue(edm::ParameterDescription<bool>("fillhlt", false, true),
               true >> (edm::ParameterDescription<edm::InputTag>("hlt_tag", edm::InputTag("TriggerResults::HLT"), true) and
                        edm::ParameterDescription<std::vector<std::string>>("hlt_paths", true)) or
               false >> edm::EmptyGroupDescription());   
  desc.ifValue(edm::ParameterDescription<bool>("fillcluscomp", false, true),
               true >> edm::ParameterDescription<edm::InputTag>("cluscomp_tag", edm::InputTag("hiClusterCompatibility"), true) or
               false >> edm::EmptyGroupDescription());
  desc.ifValue(edm::ParameterDescription<bool>("fillhf", false, true),
               true >> edm::ParameterDescription<edm::InputTag>("hf_tag", edm::InputTag("towerMaker"), true) or
               false >> edm::EmptyGroupDescription());
  desc.ifValue(edm::ParameterDescription<bool>("fillhfadc", false, true),
               true >> edm::ParameterDescription<edm::InputTag>("hfadc_tag", edm::InputTag("hcalDigis"), true) or
               false >> edm::EmptyGroupDescription());
  desc.ifValue(edm::ParameterDescription<bool>("fillgen", false, true),
               true >> (edm::ParameterDescription<edm::InputTag>("generator_tag", edm::InputTag("generatorSmeared"), true) and
                        edm::ParameterDescription<edm::InputTag>("genhi_tag", edm::InputTag("heavyIon"), true) and
                        edm::ParameterDescription<edm::InputTag>("genvertex_tag", edm::InputTag("g4SimHits"), true)) or
               false >> edm::EmptyGroupDescription());

  descriptions.add("pixelplant", desc);
}

// define this as a plug-in
DEFINE_FWK_MODULE(PixelPlant);
