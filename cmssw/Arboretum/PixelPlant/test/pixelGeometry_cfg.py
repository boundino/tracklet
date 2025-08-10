import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('PIXEL', Run3_2025_OXY)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    # '150X_mcRun3_2025_forOO_realistic_v1',
    # '150X_dataRun3_Prompt_v3',
    '150X_dataRun3_Express_v2',
    ''
)

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")


# process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.MessageLogger = cms.Service("MessageLogger",
                                    cerr = cms.untracked.PSet(
                                        enable = cms.untracked.bool(False)
                                    ),
                                    cout = cms.untracked.PSet(
                                        enable = cms.untracked.bool(True)
                                    ),
                                    files = cms.untracked.PSet(
                                        detailedInfo = cms.untracked.PSet(

                                        )
                                    )
)

process.prefer("GlobalTag")
process.es_prefer_trackerNumberingGeometryDB = cms.ESPrefer("TrackerGeometricDetESModule", "trackerNumberingGeometryDB")
process.es_prefer_trackerGeometryDB = cms.ESPrefer("TrackerDigiGeometryESModule", "trackerGeometryDB")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.dump = cms.EDAnalyzer("PIXGeometry",
		              outputFile = cms.untracked.string('tracklet/analysis/data/pixelGeo.root'),
		              outputTreename = cms.untracked.string('alignTree')
)

process.p = cms.Path(process.dump)


