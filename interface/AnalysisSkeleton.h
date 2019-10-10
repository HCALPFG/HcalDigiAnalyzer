// -*- C++ -*-
//
// Package:    HcalAnalysis/AnalysisSkeleton
// Class:      AnalysisSkeleton
//
/**\class AnalysisSkeleton AnalysisSkeleton.cc HcalAnalysis/AnalysisSkeleton/plugins/AnalysisSkeleton.cc

 Description: skeleton code for writing an EDAnalyzer for HCAL RAW data

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  David Ren-Hwa Yu
//         Created:  Mon, 07 Oct 2019 23:18:36 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"

#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


class AnalysisSkeleton : public edm::one::EDAnalyzer<edm::one::SharedResources, edm::one::WatchRuns, edm::one::WatchLuminosityBlocks>  {
   public:
      explicit AnalysisSkeleton(const edm::ParameterSet&);
      ~AnalysisSkeleton();

   private:
      void beginJob();
      void endJob();
      void beginRun(edm::Run const&, edm::EventSetup const&);
      void endRun(edm::Run const&, edm::EventSetup const&);

      void beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const&);
      void endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const&);

      void analyze(const edm::Event&, const edm::EventSetup&);

      // ----------member data ---------------------------
      // Histograms and counters
      int _nevents;
      std::map<TString, TH1D*> _th1ds;
      // TH1D *_example_hist;

      // Infrastructure
      edm::ESHandle<HcalDbService> _dbService;  
      hcaldqm::electronicsmap::ElectronicsMap _ehashmap;
#ifdef USE_CQ      
      std::map<HcalDetId, uint32_t> _channelMasks;
      std::map<HcalDetId, bool> _channelQuality;
#endif      
      edm::Service<TFileService> _fs;

      // For loading digis
      edm::InputTag  _tagQIE10;
      edm::EDGetTokenT<QIE10DigiCollection> _tokQIE10;
      edm::InputTag  _tagQIE11;
      edm::EDGetTokenT<QIE11DigiCollection> _tokQIE11;
      edm::InputTag  _tagHBHE;
      edm::EDGetTokenT<HBHEDigiCollection> _tokHBHE;
      edm::InputTag  _tagHF;
      edm::EDGetTokenT<HFDigiCollection> _tokHF;
      edm::InputTag  _tagHO;
      edm::EDGetTokenT<HODigiCollection> _tokHO;


};