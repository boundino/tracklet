// system include files
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Alignment/TrackerAlignment/interface/AlignableTracker.h"

#include <algorithm>
#include "TTree.h"
#include "TFile.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "CondFormats/Alignment/interface/DetectorGlobalPosition.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/Alignment/interface/AlignmentSurfaceDeformations.h"

#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentErrorExtendedRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerSurfaceDeformationRcd.h"

#include "CondFormats/GeometryObjects/interface/PTrackerParameters.h"
#include "CondFormats/GeometryObjects/interface/PTrackerAdditionalParametersPerDet.h"
#include "Geometry/Records/interface/PTrackerParametersRcd.h"
#include "Geometry/Records/interface/PTrackerAdditionalParametersPerDetRcd.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeomBuilderFromGeometricDet.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include "Geometry/CommonTopologies/interface/GeometryAligner.h"

#include "Alignment/CommonAlignment/interface/Alignable.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

// To access kinks and bows
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonTopologies/interface/SurfaceDeformation.h"

#include "CLHEP/Matrix/SymMatrix.h"

//
// class decleration
//

class PIXGeometry : public edm::one::EDAnalyzer<> {
public:
  explicit PIXGeometry(const edm::ParameterSet&);
  ~PIXGeometry() override;

private:
  void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  void addBranches();

  // ----------member data ---------------------------
  const edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> topoToken_;
  const edm::ESGetToken<GeometricDet, IdealGeometryRecord> geomDetToken_;
  const edm::ESGetToken<PTrackerParameters, PTrackerParametersRcd> ptpToken_;
  const edm::ESGetToken<PTrackerAdditionalParametersPerDet, PTrackerAdditionalParametersPerDetRcd> ptitpToken_;
  const edm::ESGetToken<Alignments, TrackerAlignmentRcd> aliToken_;
  const edm::ESGetToken<AlignmentErrorsExtended, TrackerAlignmentErrorExtendedRcd> aliErrorToken_;
  const edm::ESGetToken<AlignmentSurfaceDeformations, TrackerSurfaceDeformationRcd> surfDefToken_;
  const edm::ESGetToken<Alignments, GlobalPositionRcd> gprToken_;

  //std::vector<AlignTransform> m_align;
  AlignableTracker* theCurrentTracker;

  uint32_t m_rawid;
  double m_x, m_y, m_z;
  double m_eta, m_phi, m_rho;
  // double m_alpha, m_beta, m_gamma;
  int m_subdetid, m_layer;

  // Deformation parameters: stored in same tree as the alignment parameters
  // UInt_t numDeformationValues_;
  // enum { kMaxNumPar = 20 };  // slighly above 'two bowed surfaces' limit
  // Float_t deformationValues_[kMaxNumPar];

  TTree* m_tree;
  std::string m_outputFile;
  std::string m_outputTreename;
  TFile* m_file;
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
PIXGeometry::PIXGeometry(const edm::ParameterSet& iConfig)
    : topoToken_(esConsumes()),
      geomDetToken_(esConsumes()),
      ptpToken_(esConsumes()),
      ptitpToken_(esConsumes()),
      aliToken_(esConsumes()),
      aliErrorToken_(esConsumes()),
      surfDefToken_(esConsumes()),
      gprToken_(esConsumes()),
      theCurrentTracker(nullptr),
      m_rawid(0),
      m_x(0.),
      m_y(0.),
      m_z(0.),
      m_eta(0.),
      m_phi(0.),
      m_rho(0.),
      // m_alpha(0.),
      // m_beta(0.),
      // m_gamma(0.),
      m_subdetid(0),
      m_layer(0) {
  m_outputFile = iConfig.getUntrackedParameter<std::string>("outputFile");
  m_outputTreename = iConfig.getUntrackedParameter<std::string>("outputTreename");
  m_file = new TFile(m_outputFile.c_str(), "RECREATE");
  m_tree = new TTree(m_outputTreename.c_str(), m_outputTreename.c_str());
}

PIXGeometry::~PIXGeometry() { delete theCurrentTracker; }

//
// member functions
//

// ------------ method called to for each event  ------------
void PIXGeometry::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // retrieve tracker topology from geometry
  const TrackerTopology* const tTopo = &iSetup.getData(topoToken_);

  edm::LogInfo("beginJob") << "Begin Job";

  //accessing the initial geometry
  const GeometricDet* theGeometricDet = &iSetup.getData(geomDetToken_);
  const PTrackerParameters* ptp = &iSetup.getData(ptpToken_);
  const PTrackerAdditionalParametersPerDet* ptitp = &iSetup.getData(ptitpToken_);

  TrackerGeomBuilderFromGeometricDet trackerBuilder;
  //currernt tracker
  TrackerGeometry* theCurTracker = trackerBuilder.build(theGeometricDet, ptitp, *ptp, tTopo);

  //build the tracker
  const Alignments* alignments = &iSetup.getData(aliToken_);
  const AlignmentErrorsExtended* alignmentErrors = &iSetup.getData(aliErrorToken_);
  const AlignmentSurfaceDeformations* surfaceDeformations = &iSetup.getData(surfDefToken_);

  //apply the latest alignments
  const Alignments* globalPositionRcd = &iSetup.getData(gprToken_);
  GeometryAligner aligner;
  aligner.applyAlignments<TrackerGeometry>(&(*theCurTracker),
                                           alignments,
                                           alignmentErrors,
                                           align::DetectorGlobalPosition(*globalPositionRcd, DetId(DetId::Tracker)));
  aligner.attachSurfaceDeformations<TrackerGeometry>(&(*theCurTracker), &(*surfaceDeformations));

  theCurrentTracker = new AlignableTracker(&(*theCurTracker), tTopo);

  Alignments* theAlignments = theCurrentTracker->alignments();

  //alignments
  addBranches();
  for (std::vector<AlignTransform>::const_iterator i = theAlignments->m_align.begin();
       i != theAlignments->m_align.end();
       ++i) {
    m_rawid = i->rawId();

    DetId detid(m_rawid);
    if (detid.det() != DetId::Tracker) { continue; }
    if (detid.subdetId() != PixelSubdetector::PixelBarrel &&
	detid.subdetId() != PixelSubdetector::PixelEndcap) { continue; }

    m_subdetid = detid.subdetId();
    m_layer = tTopo->layer(detid);
    if (detid.subdetId() == PixelSubdetector::PixelEndcap)
      m_layer += 4;
    
    CLHEP::Hep3Vector translation = i->translation();
    m_x = translation.x();
    m_y = translation.y();
    m_z = translation.z();
    // math::XYZVector pos(m_x, m_y, m_z);
    ROOT::Math::XYZVector pos(m_x, m_y, m_z);
    m_eta = pos.eta();
    m_phi = pos.phi();
    m_rho = pos.rho();

    // CLHEP::HepRotation rotation = i->rotation();
    // m_alpha = rotation.getPhi();
    // m_beta = rotation.getTheta();
    // m_gamma = rotation.getPsi();
    m_tree->Fill();

    const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(geo_->idToDet(detid));
    

    delete theDet;

  }

  delete theAlignments;

  // Get GeomDetUnits for the current tracker
  // auto const& detUnits = theCurTracker->detUnits();
  // int detUnit(0);
  // //\\for (unsigned int iDet = 0; iDet < detUnits.size(); ++iDet) {
  // for (auto iunit = detUnits.begin(); iunit != detUnits.end(); ++iunit) {
  //   DetId detid = (*iunit)->geographicalId();
  //   m_rawid = detid.rawId();
  //   m_subdetid = detid.subdetId();

  //   ++detUnit;
  //   //\\GeomDetUnit* geomDetUnit = detUnits.at(iDet) ;
  //   auto geomDetUnit = *iunit;

  //   // Get SurfaceDeformation for this GeomDetUnit
  //   if (geomDetUnit->surfaceDeformation()) {
  //     std::vector<double> surfaceDeformParams = (geomDetUnit->surfaceDeformation())->parameters();
  //     //edm::LogInfo("surfaceDeformParamsSize") << " surfaceDeformParams size  = " << surfaceDeformParams.size() << std::endl ;
  //     m_dNpar = surfaceDeformParams.size();
  //     m_dtype = (geomDetUnit->surfaceDeformation())->type();
  //     m_d1 = surfaceDeformParams.at(0);
  //     m_d2 = surfaceDeformParams.at(1);
  //     m_d3 = surfaceDeformParams.at(2);
  //     mp_dpar->clear();
  //     for (std::vector<double>::const_iterator it = surfaceDeformParams.begin(); it != surfaceDeformParams.end();
  //          ++it) {
  //       mp_dpar->push_back((*it));
  //       //edm::LogInfo("surfaceDeformParamsContent") << " surfaceDeformParam = " << (*it) << std::endl ;
  //     }
  //     m_treeDeformations->Fill();
  //   }
  // }

  //write out
  m_file->cd();
  m_tree->Write();
  m_file->Close();
}

void PIXGeometry::addBranches() {
  m_tree->Branch("rawid", &m_rawid, "rawid/I");
  m_tree->Branch("x", &m_x, "x/D");
  m_tree->Branch("y", &m_y, "y/D");
  m_tree->Branch("z", &m_z, "z/D");
  m_tree->Branch("eta", &m_eta, "eta/D");
  m_tree->Branch("phi", &m_phi, "phi/D");
  m_tree->Branch("rho", &m_rho, "rho/D");
  // m_tree->Branch("alpha", &m_alpha, "alpha/D");
  // m_tree->Branch("beta", &m_beta, "beta/D");
  // m_tree->Branch("gamma", &m_gamma, "gamma/D");
  m_tree->Branch("subdetid", &m_subdetid, "subdetid/I");
  m_tree->Branch("layer", &m_layer, "layer/I");
}

//define this as a plug-in
DEFINE_FWK_MODULE(PIXGeometry);
