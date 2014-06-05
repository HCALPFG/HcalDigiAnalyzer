#ifndef HCALPFG_HcalDigiAnalyzer_HcalDigiAnalyzer_h
#define HCALPFG_HcalDigiAnalyzer_HcalDigiAnalyzer_h

// System includes
#include <string> 

// HCAL includes
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibCalorimetry/HcalTPGAlgos/interface/HcaluLUTTPGCoder.h"

// Framework includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// My includes
#include "HCALPFG/HcalDigiAnalyzer/interface/DigiTree.h"
#include "HCALPFG/HcalDigiAnalyzer/interface/FillDigiTree.h"

class HcalDigiAnalyzer : public edm::EDAnalyzer {
public:
  explicit HcalDigiAnalyzer(const edm::ParameterSet&);
  ~HcalDigiAnalyzer();
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  template <class DigiCollection, class RecoCollection> 
  void analyzeDigiCollection ( const HcalDbService    & conditions,
			       const HcaluLUTTPGCoder & inputCoder,
			       const DigiCollection   & digis     , 
			       const RecoCollection   & recos       );
  
  //-----------------------------------------------------
  // Root tree objects
  //-----------------------------------------------------

  DigiTree     m_digiTree;
  FillDigiTree m_fillDigi;

  //-----------------------------------------------------
  // Out file info
  //-----------------------------------------------------

  std::string m_outPath;
  std::string m_outSuffix;
  std::string m_rootFile;  

  //-----------------------------------------------------
  // EDM input tags
  //-----------------------------------------------------
  
  edm::InputTag m_rawDataLabel;
  edm::InputTag m_hcalDigisLabel;
  edm::InputTag m_hbheRecHitsLabel;
  edm::InputTag m_hoRecHitsLabel;
  edm::InputTag m_hfRecHitsLabel;

};

#endif
