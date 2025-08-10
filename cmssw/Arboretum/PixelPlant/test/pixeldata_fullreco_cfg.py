import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('PIXEL', Run3_2025_OXY)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load("Configuration.StandardSequences.Reconstruction_Data_cff")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(10000)
)

# process.source = cms.Source("PoolSource",
# 			    fileNames = cms.untracked.vstring(
# 				    '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
# 			    )
# )
process.source = cms.Source("NewEventStreamFileReader",
        fileNames = cms.untracked.vstring(
                'file:/eos/cms/store/t0streamer/Data/PhysicsIonPhysics0/000/393/846/run393846_ls0005_streamPhysicsIonPhysics0_StorageManager.dat'
        )
)

process.TFileService = cms.Service('TFileService',
	fileName = cms.string(
		'pixeldata_fullreco.root'
	)
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
	process.GlobalTag,
        '150X_dataRun3_Prompt_v3',
	''
)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_MinimumBiasHF_OR_BptxAND_v1"])
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
	src = cms.InputTag("offlinePrimaryVertices"),
	cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
	filter = cms.bool(True),
)
process.filterSequence = cms.Sequence(process.hltfilter*process.primaryVertexFilter)
process.prefilter = cms.Path(process.filterSequence)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

process.load('Arboretum.PixelPlant.pixel_data_cfi')
process.pixel.fillhlt = cms.bool(True)
process.pixel.hlt_paths = cms.vstring('HLT_MinimumBiasHF_OR_BptxAND_v1')
process.output = cms.Path(process.pixel)

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output,
    process.endjob_step
)

for path in process.paths:
	getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

# process.pixel.superFilters = cms.vstring("prefilter")

# from CondCore.CondDB.CondDB_cfi import *
# process.es_pool = cms.ESSource("PoolDBESSource",
#                                toGet = cms.VPSet(
#                                    cms.PSet(
#                                        record = cms.string("HcalElectronicsMapRcd"),
#                                        tag = cms.string("HcalElectronicsMap_v10.0_offline")
#                                    )
#                                ),
#                                connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
# )

# process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')
# process.es_ascii = cms.ESSource(
#     'HcalTextCalibrations',
#     input = cms.VPSet(
#         cms.PSet(
#             object = cms.string('ElectronicsMap'),
#             file = cms.FileInPath("emap_2025_full.txt")
#              )
#     )
# )

process.MessageLogger.cerr.FwkReport.reportEvery = 100

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.inputFiles='file:/eos/cms/store/t0streamer/Data/PhysicsIonPhysics0/000/394/153/run394153_ls0070_streamPhysicsIonPhysics0_StorageManager.dat'
# ivars.outputFile=''
ivars.parseArguments()

process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
# process.TFileService.fileName = cms.string(ivars.outputFile)

