import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('PIXEL', Run3_2025_OXY)

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Services_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
		                'file:digiraw_DIGI_L1_DIGI2RAW_HLT_317.root',
                            ),
)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(
                                       'file:pixelsim_pre.root'
                                   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '141X_mcRun3_2024_realistic_HI_v13',
    ''
)

## Not for running but show the corresponding sequences
process.raw2digi_step = cms.Path(process.hcalDigis + process.ecalDigis + process.siPixelDigis)
process.recotowers = cms.Path(process.bunchSpacingProducer * process.calolocalreco * process.hcalGlobalRecoSequence * process.caloTowersRec)
process.p1 = cms.Path(process.pixeltrackerlocalreco * process.offlineBeamSpot)

process.load('Arboretum.PixelPlant.pixel_cfi')
process.pixel.pixel_tag = cms.InputTag("siPixelRecHitsPreSplitting")
process.pixel.vertex_tags = cms.VInputTag()
process.pixel.fillcluscomp = cms.bool(False)
process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.recotowers,
    process.p1,
    process.output
)
