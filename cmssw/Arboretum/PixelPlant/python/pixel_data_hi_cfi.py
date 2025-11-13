import FWCore.ParameterSet.Config as cms

from Arboretum.PixelPlant.pixel_cfi import pixel
pixel.fillhf = cms.bool(True)
pixel.hf_tag = cms.InputTag("towerMaker")
pixel.fillhfadc = cms.bool(True)
pixel.hfadc_tag = cms.InputTag("hcalDigis")
pixel.fillcluscomp = cms.bool(True)
pixel.cluscomp_tag = cms.InputTag("hiClusterCompatibility")

