import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('PIXEL', Run3_2025_OXY)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load("Configuration.StandardSequences.Reconstruction_Data_cff")
process.load("Configuration.StandardSequences.Services_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            '/store/hidata/OORun2025/IonPhysics0/MINIAOD/PromptReco-v1/000/394/153/00000/7aa4a885-8c8a-4f33-b207-cb7f666fab8d.root'
        ),
        secondaryFileNames = cms.untracked.vstring(
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/3522a287-95e8-4412-b93b-7e2f8bdbbbda.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/3da364dd-90d7-4673-837f-6f393734b7d6.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/5c363dc9-ccad-4981-9960-bf8cdd52c689.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/6f108202-e827-4f41-8297-1b74cf08bdd0.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/7f6cc2d6-ddeb-4f85-9079-8171f8701b13.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/bd9e9130-405e-4a13-9723-b4e993b392e0.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/ccca82e1-c80f-4982-875f-098d2787127b.root',
            '/store/hidata/OORun2025/IonPhysics0/RAW/v1/000/394/153/00000/ec7957b4-2118-4e3f-be1b-dc1c5f107a31.root',
        )
)

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(
            'pixeldata_pf.root'
        )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '150X_dataRun3_Prompt_v3',
    ''
)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction_trackingOnly) # siPixelRecHits

process.load('Arboretum.PixelPlant.pixel_data_miniaod_cfi')
process.pixel.fillhlt = cms.bool(True)
process.pixel.hlt_paths = cms.vstring('HLT_MinimumBiasHF_OR_BptxAND_v1')
process.output = cms.Path(process.pixel)

process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.output
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
for path in process.paths:
        getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

process.MessageLogger.cerr.FwkReport.reportEvery = 100

