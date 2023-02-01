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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
		'file:step3_apprximateCluster_22.root',
    ),
)

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(
        'file:pixelsim_presplitting.root'
    )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    # 'auto:phase1_2022_realistic_hi', # -> v6
    '125X_mcRun3_2022_realistic_HI_v8',
    ''
)

process.pixel = cms.EDAnalyzer('PixelPlant',
	pixel_tag = cms.InputTag("siPixelRecHitsPreSplitting"),
	vertex_tags = cms.VInputTag(cms.InputTag("offlinePrimaryVertices")),
	fillhf = cms.bool(True),
    fillgen = cms.bool(True),
	fillhlt = cms.bool(False),
    # hlt_tag = cms.InputTag("TriggerResults::HLT"),
    # hlt_paths = cms.vstring('HLT_HIL1MinimumBiasHF_OR_SinglePixelTrack_v1'),
)

# process.raw2digi_step = cms.Path(process.hcalDigis + process.ecalDigis + process.siPixelDigis)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

# process.recotowers = cms.Path(
# 	process.bunchSpacingProducer *
# 	process.calolocalreco *
# 	process.hcalGlobalRecoSequence *
# 	process.caloTowersRec
# )

# process.p1 = cms.Path(
# 	process.pixeltrackerlocalreco *
# 	process.offlineBeamSpot
# )

process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
	# process.recotowers,
	# process.p1,
    process.output
)

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.maxEvents = -1
ivars.outputFile='pixelsim_pre_fullreco.root'
ivars.inputFiles='/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/MC_221213_DIGIRAW_1252p1_privateZ/221215_152406/0000/step2_digiraw_268.root'
ivars.parseArguments()

process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(ivars.maxEvents))
process.TFileService.fileName = cms.string(ivars.outputFile)