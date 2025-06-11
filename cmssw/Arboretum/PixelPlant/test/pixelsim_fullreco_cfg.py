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
		                '/store/user/wangj/MinBias_AMPT_StringMelting_b015_OO_5362GeV/DIGIRAW_250527_el8_gcc12_1506p1_141X/250527_152140/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_274.root'
                            ),
)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(
                                       'pixelsim.root'
                                   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '150X_mcRun3_2025_forOO_realistic_v1',
    ''
)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

process.load('Arboretum.PixelPlant.pixel_cfi')
process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output
)


process.MessageLogger.cerr.FwkReport.reportEvery = 10
