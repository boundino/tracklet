import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_pp_on_PbPb_cff import Run3_pp_on_PbPb
process = cms.Process('PIXEL', Run3_pp_on_PbPb)

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Services_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:step3_apprximateCluster_22.root',
    )
)

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(
        'file:pixelsim_presplitting.root'
    )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    'auto:phase1_2022_realistic_hi',
    ''
)

process.pixel = cms.EDAnalyzer('PixelPlant',
	fillhf = cms.bool(False),
    fillgen = cms.bool(True),
	fillhlt = cms.bool(False),
    # hlt_tag = cms.InputTag("TriggerResults::HLT"),
    # hlt_paths = cms.vstring('HLT_HIL1MinimumBiasHF_OR_SinglePixelTrack_v1'),
	pixel_tag = cms.InputTag("siPixelRecHitsPreSplitting"),
)

process.rawtodigi = cms.Path(
	# process.RawToDigi
    # process.hcalDigis +
    # process.ecalDigis +
    process.siPixelDigis 
)

process.p1 = cms.Path(
	process.pixeltrackerlocalreco *
	process.offlineBeamSpot
)

process.output = cms.EndPath(
    process.pixel
)

process.schedule = cms.Schedule(
    process.rawtodigi,
	process.p1,
    process.output
)
