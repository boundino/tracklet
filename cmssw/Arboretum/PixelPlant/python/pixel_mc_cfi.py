import FWCore.ParameterSet.Config as cms

from Arboretum.PixelPlant.pixel_data_cfi import pixel
pixel.hfadc_tag = cms.InputTag("simHcalUnsuppressedDigis","HFQIE10DigiCollection")
pixel.fillgen = cms.bool(True)
pixel.generator_tag = cms.InputTag("generatorSmeared")
pixel.genvertex_tag = cms.InputTag("g4SimHits")
pixel.genhi_tag = cms.InputTag("heavyIon")

