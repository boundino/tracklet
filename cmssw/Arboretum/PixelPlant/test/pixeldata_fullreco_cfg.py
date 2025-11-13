import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_cff import Run3_2025
process = cms.Process('PIXEL', Run3_2025)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_Data_cff') # Check official raw2digi whether Data_cff or DataMapper_cff is used
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("NewEventStreamFileReader",
                            fileNames = cms.untracked.vstring(
                                'file:/eos/cms/store/t0streamer/Data/PhysicsVRZeroBias14/000/398/864/run398864_ls0004_streamPhysicsVRZeroBias14_StorageManager.dat'
                            )
)
# process.source = cms.Source("PoolSource",
# 			    fileNames = cms.untracked.vstring(
# 				    '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
# 			    )
# )

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

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

process.load('Arboretum.PixelPlant.pixel_data_cfi')
process.pixel.fillhlt = cms.bool(True)
process.pixel.hlt_paths = cms.vstring('HLT_ZeroBias_HighRate_v5')
process.output = cms.Path(process.pixel)

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output,
    process.endjob_step
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10

# # ! Cross check how official digi2raw did
# from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")

# # HLT filter and primary vertex filter
# from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
# process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_MinimumBiasHF_OR_BptxAND_v1"])
# process.primaryVertexFilter = cms.EDFilter("VertexSelector",
# 	                                   src = cms.InputTag("offlinePrimaryVertices"),
# 	                                   cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
# 	                                   filter = cms.bool(True),
# )
# process.filterSequence = cms.Sequence(process.hltfilter*process.primaryVertexFilter)
# for path in process.paths:
# 	getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

