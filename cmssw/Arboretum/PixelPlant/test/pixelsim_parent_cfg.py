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
                                # '/store/user/wangj/MinBias_Hijing_OO_5362GeV/MINIAOD_250715_1509p1_realisticBS/250717_011926/0000/reco_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_20.root'
                                'file:/eos/cms/store/group/phys_heavyions/wangj/mc/reco_RAW2DIGI_L1Reco_RECO_PAT.root'
                           ),
                            secondaryFileNames = cms.untracked.vstring(
                                # '/store/user/wangj/MinBias_Hijing_OO_5362GeV/DIGIRAW_250715_el8_gcc12_1508/250716_123030/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_116.root'
                                'file:/eos/cms/store/group/phys_heavyions/wangj/mc/digiraw_DIGI_L1_DIGI2RAW_HLT_PU.root'
                            )
)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(
                                       '/eos/cms/store/group/phys_heavyions/wangj/mc/pixelsim_pf.root'
                                   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '150X_mcRun3_2025_forOO_realistic_v8',
    ''
)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction_trackingOnly) # siPixelRecHits

process.load('Arboretum.PixelPlant.pixel_mc_miniaod_cfi')
process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10
