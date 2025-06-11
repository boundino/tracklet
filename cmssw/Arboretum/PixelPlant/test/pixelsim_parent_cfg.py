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
    input = cms.untracked.int32(10)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                'file:miniaod_PAT_14.root',
		                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/MINIAOD_250518_el8_Run3_2025_OXY_1506/250520_104022/0000/miniaod_PAT_14.root',
                                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/DIGIRAW_250518_el8_gcc12_1506/250519_145746/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_317.root', 
                           ),
                            secondaryFileNames = cms.untracked.vstring(
                                'file:digiraw_DIGI_L1_DIGI2RAW_HLT_317.root',
                                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/DIGIRAW_250518_el8_gcc12_1506/250519_145746/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_317.root',
                                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/DIGIRAW_250518_el8_gcc12_1506/250519_145746/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_318.root',
                                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/DIGIRAW_250518_el8_gcc12_1506/250519_145746/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_319.root',
                                # '/store/user/wangj/Hijing_MinimumBias_b015_OO_5362GeV/DIGIRAW_250518_el8_gcc12_1506/250519_145746/0000/digiraw_DIGI_L1_DIGI2RAW_HLT_32.root'
                            )
)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(
                                       'pixelsim_pf.root'
                                   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '141X_mcRun3_2024_realistic_HI_v13',
    ''
)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction_trackingOnly) # siPixelRecHits

process.load('Arboretum.PixelPlant.pixel_miniaod_cff')
process.output = cms.EndPath(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output
)
