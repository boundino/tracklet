import FWCore.ParameterSet.Config as cms

from Arboretum.PixelPlant.pixel_mc_cfi import pixel
pixel.vertex_tags = cms.VInputTag( cms.InputTag("offlineSlimmedPrimaryVertices") )
pixel.hf_tag = cms.InputTag("packedPFCandidates")
