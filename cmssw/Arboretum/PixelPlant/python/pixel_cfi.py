import FWCore.ParameterSet.Config as cms

pixel = cms.EDAnalyzer('PixelPlant',
                       pixel_tag = cms.InputTag("siPixelRecHits"),
                       vertex_tags = cms.VInputTag( cms.InputTag("offlinePrimaryVertices") ),
                       fillhf = cms.bool(True),
                       hf_tag = cms.InputTag("towerMaker"),
                       fillhlt = cms.bool(False),
                       # hlt_paths = cms.vstring('HLT_HIMinimumBias_v2'),
                       fillcluscomp = cms.bool(True),
                       cluscomp_tag = cms.InputTag("hiClusterCompatibility"),
                       fillgen = cms.bool(True),
                       generator_tag = cms.InputTag("generatorSmeared"),
                       genvertex_tag = cms.InputTag("g4SimHits")
)

