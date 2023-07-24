import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_pp_on_PbPb_cff import Run3_pp_on_PbPb
process = cms.Process('PIXEL', Run3_pp_on_PbPb)
# from Configuration.Eras.Era_Run3_pp_on_PbPb_approxSiStripClusters_cff import Run3_pp_on_PbPb_approxSiStripClusters
# process = cms.Process('PIXEL', Run3_pp_on_PbPb_approxSiStripClusters)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
# process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load("Configuration.StandardSequences.Reconstruction_Data_cff")

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
			    fileNames = cms.untracked.vstring(
				    # 'file:/eos/cms/tier0/store/hidata/HIRun2022A/HITestRawPrime21/RAW/v1/000/362/229/00000/12bd02d0-825c-4d36-b3d1-278f83708cef.root',
				    '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
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
        '124X_dataRun3_Prompt_v10',
	# 'auto:run3_data_prompt', # 1255p1 => 124X_dataRun3_Prompt_frozen_v8 
	''
)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_HIMinimumBias_v2"])
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
					   src = cms.InputTag("offlinePrimaryVertices"),
					   cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
					   filter = cms.bool(True),   # otherwise it won't filter the events
)
process.filterSequence = cms.Sequence(process.hltfilter*process.primaryVertexFilter)
# process.prefilter = cms.Path(process.filterSequence)

process.pixel = cms.EDAnalyzer('PixelPlant',
			       pixel_tag = cms.InputTag("siPixelRecHits"),
			       vertex_tags = cms.VInputTag(cms.InputTag("offlinePrimaryVertices")),
			       fillhf = cms.bool(True),
			       fillhlt = cms.bool(True),
			       hlt_paths = cms.vstring('HLT_HIMinimumBias_v2'),
                               fillcluscomp = cms.bool(True),
)

# process.raw2digi_step = cms.Path(process.siPixelDigis + process.hcalDigis + process.ecalDigis)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
# process.L1Reco_step = cms.Path(process.L1Reco)

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

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process, new="rawPrimeDataRepacker", old="rawDataCollector")
MassReplaceInputTag(process, new="rawDataRepacker", old="rawDataCollector")
# MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")


# process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.cerr.FwkReport.reportEvery = 100 # => This will suppress err message even for 1st event


import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.maxEvents = -1
ivars.outputFile='pixeldata_fullreco.root'
ivars.inputFiles='/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root'
ivars.parseArguments()

process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(ivars.maxEvents))
process.TFileService.fileName = cms.string(ivars.outputFile)
