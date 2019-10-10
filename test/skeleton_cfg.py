#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('skipEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('run', 
    "308231", 
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "Run number")
#options.register('inputFiles',
#                 #"file:inputFile.root", #default value
#                 "file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run314692/USC_314692.root",
#                 VarParsing.VarParsing.multiplicity.list,
#                 VarParsing.VarParsing.varType.string,
#                 "Input files")
#
options.register('outputFile',
                 "", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('threads',
                 4, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of threads")

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "Run = ", options.run
inputFile = "root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run{}/USC_{}.root".format(options.run, options.run)

if options.outputFile == "":
    outputFile = "file:skeletonanalysis_{}.root".format(options.run)

#print "inputFiles =", options.inputFiles
print "outputFile =", outputFile


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
from Configuration.StandardSequences.Eras import eras
#if options.run < 329129:
#    process = cms.Process('HcalAnalysis', eras.Run2_2018)
#else:
process = cms.Process('HcalAnalysis', eras.Run3)


#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )
process.source = cms.Source("HcalTBSource",
    fileNames  = cms.untracked.vstring([inputFile]),
    skipEvents = cms.untracked.uint32(options.skipEvents),
    firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID([]),
    firstRun = cms.untracked.uint32(328000)
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(outputFile)
)

process.options = cms.untracked.PSet(
  numberOfThreads = cms.untracked.uint32(options.threads)
)


#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
#process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("CondCore.CondDB.CondDB_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
from Configuration.AlCa.autoCond import autoCond
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalDigiAnalyzer.AnalysisSkeleton_cfi")

#------------------------------------------------------------------------------------
# Since this is a local run, make sure we're looking for the FEDs in the right place
#------------------------------------------------------------------------------------
process.hcalDigis.InputLabel = cms.InputTag("source")

#------------------------------------------------------------------------------------
# FED numbers
#------------------------------------------------------------------------------------
#process.hcalDigis.FEDs = cms.untracked.vint32(1100, 1102, 1104, 1106, 1108, 1110, 1112, 1114, 1116)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = '106X_dataRun2_PromptLike_Candidate_2019_04_25_12_26_01'
process.GlobalTag.globaltag = '106X_dataRun3_Prompt_v3'

#------------------------------------------------------------------------------------
# Analysis sequence definition
#------------------------------------------------------------------------------------
process.analysis_step = cms.Sequence(
    process.analysisSkeleton
)


#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # Do energy reconstruction
#    process.hbheprereco*
#    process.hbhereco*
#    process.hfprereco*
#    process.hfreco*
#    process.horeco*
    # Make the ntuples
    process.analysis_step
)

#-----------------------------------------------------------------------------------
# Dump configuraiton
#-----------------------------------------------------------------------------------
#dump = file('dump.py', 'w')
#dump.write( process.dumpPython() )
#dump.close()
