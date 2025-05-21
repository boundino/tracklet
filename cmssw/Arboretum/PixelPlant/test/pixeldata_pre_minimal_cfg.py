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
								# '/store/hidata/HIRun2022A/HITestRawPrime0/RAW/v1/000/362/217/00000/20df3859-e65f-45d6-9b0a-e30921880ff7.root',
								# 'file:/eos/cms/tier0/store/hidata/HIRun2022A/HITestRawPrime21/RAW/v1/000/362/229/00000/12bd02d0-825c-4d36-b3d1-278f83708cef.root',
								# '/store/hidata/HIRun2022A/HITestRawPrime14/RAW/v1/000/362/271/00000/54ba6c9b-2f21-401c-8908-06f5ff0cf06f.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/0ea79907-85a4-415b-915a-58ab2771a224.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/31fee8ef-acfd-438d-a40b-9e99ef1ff906.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/41a0deed-b66e-482b-84e2-0b8443b31140.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/76b61dd4-f4be-4bc6-afbb-956bb31b0f72.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/8012f4ef-02a6-44fa-9264-72039a97bc5c.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/91d3a6e0-703b-408b-81b1-6792d727e16e.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/98377dfe-3064-46cd-9152-20d1966d4010.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/a2bcc43f-f722-440a-bf82-804afbbc31f1.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/aef86815-4230-497c-bdc3-3f5313e61248.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/bfe72ab5-2eb8-496f-b2ca-f89ec8d1153c.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/e3c86ea3-61f8-4c41-9f58-13315a12efa1.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/e6d89c69-5016-4e0c-93f7-dcf7f30ea912.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/f9096b63-d8cf-42d6-b047-716090d3ec3f.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fbaef8bc-f55d-4445-a0f4-f6e66ae8323e.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fc283f2b-100e-45e3-a853-761711004eed.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fc2f7f9c-d67a-4fd8-aba3-49e47cffa191.root',
							)
)

process.TFileService = cms.Service('TFileService',
								   fileName = cms.string(
									   'file:/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/HITestRaw0/pixeldata_presplitting_362294.root'
								   )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
	process.GlobalTag,
	# '94X_dataRun2_Rereco17_ONLYXeXeBS_v1',
	# '94X_dataRun2_ReReco_EOY17_v2',
	'auto:run3_data_prompt',
	''
)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_HIMinimumBias_v2"])
process.filterSequence = cms.Sequence(process.hltfilter)
# process.prefilter = cms.Path(process.filterSequence)

process.pixel = cms.EDAnalyzer('PixelPlant',
							   fillhf = cms.bool(True),
							   pixel_tag = cms.InputTag("siPixelRecHitsPreSplitting"),
							   fillhlt = cms.bool(True),
							   hlt_tag = cms.InputTag("TriggerResults::HLT"),
							   hlt_paths = cms.vstring('HLT_HIMinimumBias_v2'),
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

for path in process.paths:
	getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process)
# MassReplaceInputTag(process, new="rawPrimeDataRepacker", old="rawDataCollector")
MassReplaceInputTag(process, new="rawDataRepacker", old="rawDataCollector")
# MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')

ivars.maxEvents = -1
ivars.outputFile='pixeldata_presplitting.root'
ivars.inputFiles='/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root'
ivars.parseArguments()

process.source.fileNames = cms.untracked.vstring(ivars.inputFiles)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(ivars.maxEvents))
process.TFileService.fileName = cms.string(ivars.outputFile)