import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('PIXEL', Run3_2025_OXY)

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
# process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Services_cff")

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
)

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring('file:/eos/cms/store/t0streamer/Data/PhysicsSpecialRandom0/000/393/649/run393649_ls0026_streamPhysicsSpecialRandom0_StorageManager.dat')
)

# process.source = cms.Source("PoolSource",
# 			    fileNames = cms.untracked.vstring(
# 				    '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
# 			    )
# )

process.TFileService = cms.Service('TFileService',
				   fileName = cms.string(
					   'pixeldata_pre.root'
				   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
	process.GlobalTag,
	'150X_dataRun3_Prompt_v3',
	''
)

process.pixel = cms.EDAnalyzer('PixelPlant',
			       fillhf = cms.bool(False),
                               # hf_tag = cms.InputTag("towerMaker"),
			       pixel_tag = cms.InputTag("siPixelRecHitsPreSplitting"),
                               vertex_tags = cms.VInputTag( ),
                               fillhlt = cms.bool(False),
			       # hlt_tag = cms.InputTag("TriggerResults::HLT"),
                               # hlt_paths = cms.vstring('HLT_HIMinimumBias_v2'),
                               fillcluscomp = cms.bool(False),
                               # cluscomp_tag = cms.InputTag("hiClusterCompatibility"),
                               fillgen = cms.bool(False),
)

process.raw2digi_step = cms.Path(process.siPixelDigis + process.hcalDigis + process.ecalDigis)

process.recotowers = cms.Path(
	process.bunchSpacingProducer *
	process.calolocalreco *
	process.hcalGlobalRecoSequence *
	process.caloTowersRec
)

process.p1 = cms.Path(
	process.pixeltrackerlocalreco *
	process.offlineBeamSpot
)

process.output = cms.Path(process.pixel)

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(
	process.raw2digi_step,
	process.p1,
	process.output,
	process.endjob_step
)

# from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
# process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_HIMinimumBias_v2"])
# process.filterSequence = cms.Sequence(process.hltfilter)

# for path in process.paths:
# 	getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process)
# MassReplaceInputTag(process, new="rawPrimeDataRepacker", old="rawDataCollector")
# MassReplaceInputTag(process, new="rawDataRepacker", old="rawDataCollector")
# MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.maxEvents = -1
ivars.outputFile='pixeldata_pre.root'
ivars.inputFiles='/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root'
ivars.parseArguments()

# process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
# process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(ivars.maxEvents))
# process.TFileService.fileName = cms.string(ivars.outputFile)
