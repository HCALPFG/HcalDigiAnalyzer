import FWCore.ParameterSet.Config as cms
import os, sys
import string

if len (sys.argv) >= 3:
    run_number = int(sys.argv[2])
else:
    run_number = int(221165)

print "Analyze local run:", run_number

process = cms.Process("HCALPFG")

#------------------------------------------------------------------------------------
# Specify Event Source
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
      'root://eoscms//eos/cms/store/group/comm_hcal/LS1/USC_'+str(run_number)+'.root'
    ),
    streams   = cms.untracked.vstring(
    #HBHEa,b,c:
    'HCAL_DCC700','HCAL_DCC701','HCAL_DCC702','HCAL_DCC703','HCAL_DCC704','HCAL_DCC705',
    'HCAL_DCC706','HCAL_DCC707','HCAL_DCC708','HCAL_DCC709','HCAL_DCC710','HCAL_DCC711',
    'HCAL_DCC712','HCAL_DCC713','HCAL_DCC714','HCAL_DCC715','HCAL_DCC716','HCAL_DCC717',
    #HF:
    'HCAL_DCC718','HCAL_DCC719','HCAL_DCC720','HCAL_DCC721','HCAL_DCC722','HCAL_DCC723',
    #HO:
    'HCAL_DCC724','HCAL_DCC725','HCAL_DCC726','HCAL_DCC727','HCAL_DCC728','HCAL_DCC729',
    'HCAL_DCC730','HCAL_DCC731',
    'HCAL_Trigger','HCAL_SlowData'
    )
)

#------------------------------------------------------------------------------------
#  Message Logger stuff
#------------------------------------------------------------------------------------

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#------------------------------------------------------------------------------------
# Hcal Conditions: from Global Conditions Tag 
#------------------------------------------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V43D::All', '')

#------------------------------------------------------------------------------------
# Hcal DQM Source, including HitReconstrctor
#------------------------------------------------------------------------------------

process.load("Geometry.HcalEventSetup.HcalTopology_cfi")
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hbhe_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_ho_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hf_cfi")

#------------------------------------------------------------------------------------
# Define unpacker
#------------------------------------------------------------------------------------

process.tbunpack = cms.EDProducer("HcalTBObjectUnpacker",
    IncludeUnmatchedHits    = cms.untracked.bool(False),
    HcalTriggerFED          = cms.untracked.int32(1),
    fedRawDataCollectionTag = cms.InputTag('source')
)


#------------------------------------------------------------------------------------
# Define source for HCAL digis
#------------------------------------------------------------------------------------

process.hcalDigis.InputLabel = cms.InputTag("source")

#------------------------------------------------------------------------------------
# Define analyzer
#------------------------------------------------------------------------------------

process.analysis = cms.EDAnalyzer('HcalDigiAnalyzer',
    outPath   = cms.untracked.string ( "" ),
    outSuffix = cms.untracked.string ( str(run_number) )
)

#------------------------------------------------------------------------------------
# Define path
#------------------------------------------------------------------------------------

process.p = cms.Path(process.tbunpack
                     *process.hcalDigis
                     *process.horeco
                     *process.hfreco
                     *process.hbheprereco
                     *process.analysis
)

process.dump_module = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName       = cms.untracked.string ('dump.root')
)

process.dump = cms.EndPath (process.dump_module )

process.schedule = cms.Schedule(process.p,process.dump)

