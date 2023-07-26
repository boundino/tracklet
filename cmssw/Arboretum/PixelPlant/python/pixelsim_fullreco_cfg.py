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
        'file:pixelsim.root'
    )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    # 'auto:phase1_2022_realistic_hi', # CMSSW_12_5_5_patch1 -> v8
    '125X_mcRun3_2022_realistic_HI_v13',
    ''
)

process.pixel = cms.EDAnalyzer('PixelPlant',
	                       pixel_tag = cms.InputTag("siPixelRecHits"),
	                       vertex_tags = cms.VInputTag(cms.InputTag("offlinePrimaryVertices")),
	                       fillhf = cms.bool(True),
	                       fillhlt = cms.bool(True),
                               hlt_paths = cms.vstring('HLT_HIMinimumBias_v2'),
                               fillcluscomp = cms.bool(True),
                               fillgen = cms.bool(True),
)

# process.raw2digi_step = cms.Path(process.hcalDigis + process.ecalDigis + process.siPixelDigis)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output
)

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.maxEvents = -1
ivars.outputFile='pixelsim_fullreco.root'
ivars.inputFiles='/store/mc/HINPbPbAutumn22DR/MinBias_Drum5F_5p36TeV_hydjet/GEN-SIM-RAW/NoPU_125X_mcRun3_2022_realistic_HI_v13-v2/60003/4fb43d80-0dbe-4920-b7b6-02e62a53145a.root'
# ivars.inputFiles='/store/user/wangj/MinBias_EposLHC_ReggeGribovParton_PbPb_5360GeV/MC_230324_DIGIRAW_1254_GTv9/230325_191449/0000/step2_digiraw_354.root'
ivars.parseArguments()

process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(ivars.maxEvents))
process.TFileService.fileName = cms.string(ivars.outputFile)
