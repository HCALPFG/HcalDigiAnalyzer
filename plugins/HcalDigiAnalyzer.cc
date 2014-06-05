// system include files
#include <memory>
#include <iostream>

// My includes
#include "HCALPFG/HcalDigiAnalyzer/interface/HcalDigiAnalyzer.h"

// HCAL includes
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGCoder.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGRecord.h"
#include "CalibFormats/CaloObjects/interface/IntegerCaloSamples.h"
#include "CalibCalorimetry/HcalTPGAlgos/interface/HcaluLUTTPGCoder.h"

// Framework includes
#include "FWCore/Framework/interface/ESHandle.h"

HcalDigiAnalyzer::HcalDigiAnalyzer(const edm::ParameterSet& iConfig):
  m_outPath          (iConfig.getUntrackedParameter<std::string>  ("outPath"         ,"./" )),
  m_outSuffix        (iConfig.getUntrackedParameter<std::string>  ("outSuffix"       ,""   )),
  m_rawDataLabel     (iConfig.getUntrackedParameter<edm::InputTag>("RawDataLabel"    ,edm::InputTag("source"     ))),
  m_hcalDigisLabel   (iConfig.getUntrackedParameter<edm::InputTag>("HcalDigisLabel"  ,edm::InputTag("hcalDigis"  ))),
  m_hbheRecHitsLabel (iConfig.getUntrackedParameter<edm::InputTag>("HBHERecHitsLabel",edm::InputTag("hbheprereco"))),
  m_hoRecHitsLabel   (iConfig.getUntrackedParameter<edm::InputTag>("HORecHitsLabel"  ,edm::InputTag("horeco"     ))),
  m_hfRecHitsLabel   (iConfig.getUntrackedParameter<edm::InputTag>("HFRecHitsLabel"  ,edm::InputTag("hfreco"     )))
{
  m_rootFile = m_outPath + std::string("HcalFlatNtuple") + m_outSuffix + std::string(".root");
  m_fillDigi.init(m_rootFile, &m_digiTree);
}

HcalDigiAnalyzer::~HcalDigiAnalyzer(){}

template <class DigiCollection, class RecoCollection > 
void HcalDigiAnalyzer::analyzeDigiCollection ( const HcalDbService    & conditions,  
					       const HcaluLUTTPGCoder & inputCoder, 
					       const DigiCollection   & digis     , 
					       const RecoCollection   & recos      ){

  //-----------------------------------------------------
  // Get iterators
  //-----------------------------------------------------
   
  typename RecoCollection::const_iterator reco;
  typename RecoCollection::const_iterator reco_end = recos.end(); 
  typename DigiCollection::const_iterator digi     = digis.begin();
  typename DigiCollection::const_iterator digi_end = digis.end(); 

  //-----------------------------------------------------
  // Get setup information
  //-----------------------------------------------------
  
  //-----------------------------------------------------
  // Resize digis
  //-----------------------------------------------------

  int ndigis = (int) digis.size();
  int idigi = 0;

  m_digiTree.digi_timeslice_dv        -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_er        -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_raw       -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_adc       -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_ladc      -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_nomFC     -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_fiber     -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_fiberChan -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_capid     -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_allFC     -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_pedFC     -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_FC        -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_gain      -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_rcgain    -> resize ( ndigis ) ;
  m_digiTree.digi_timeslice_energy    -> resize ( ndigis ) ;

  m_digiTree.digi_subdet              -> resize ( ndigis ) ;
  m_digiTree.digi_ieta                -> resize ( ndigis ) ;
  m_digiTree.digi_iphi                -> resize ( ndigis ) ;
  m_digiTree.digi_depth               -> resize ( ndigis ) ;
  m_digiTree.digi_presamples          -> resize ( ndigis ) ;
  m_digiTree.digi_nTS                 -> resize ( ndigis ) ;
  m_digiTree.digi_fiberIdleOffset     -> resize ( ndigis ) ;
  
  //-----------------------------------------------------
  // Loop through digis
  //-----------------------------------------------------
  
  for (; digi != digi_end ; ++digi ) {

    //-----------------------------------------------------
    // Get digi-specific cc objects
    //-----------------------------------------------------
    
    const HcalDetId       * hcalDetId    = & digi -> id();
    const HcalQIECoder    * channelCoder =   conditions.getHcalCoder        (*hcalDetId);
    const HcalCalibrations* calibrations = & conditions.getHcalCalibrations (*hcalDetId);
    const HcalQIEShape    * shape        =   conditions.getHcalShape        (*hcalDetId);
    
    HcalCoderDb coder (*channelCoder, *shape); 
    CaloSamples tool;
    coder.adc2fC ( * digi, tool );

    IntegerCaloSamples integer_tool;
    integer_tool.setPresamples(digi -> presamples());
    inputCoder.adc2Linear (*digi, integer_tool );

    //-----------------------------------------------------
    // Get digi-specific values
    //-----------------------------------------------------
        
    int digi_subdet          = hcalDetId -> subdet();
    int digi_ieta            = hcalDetId -> ieta();
    int digi_iphi            = hcalDetId -> iphi();
    int digi_depth           = hcalDetId -> depth();
    int digi_presamples      = digi      -> presamples() ;
    int digi_nTS             = digi      -> size();
    int digi_fiberIdleOffset = digi      -> fiberIdleOffset();

    (*m_digiTree.digi_subdet         )[idigi] = digi_subdet          ;
    (*m_digiTree.digi_ieta           )[idigi] = digi_ieta            ;
    (*m_digiTree.digi_iphi           )[idigi] = digi_iphi            ;
    (*m_digiTree.digi_depth          )[idigi] = digi_depth           ;
    (*m_digiTree.digi_presamples     )[idigi] = digi_presamples      ;
    (*m_digiTree.digi_nTS            )[idigi] = digi_nTS             ;
    (*m_digiTree.digi_fiberIdleOffset)[idigi] = digi_fiberIdleOffset ;
    
    //-----------------------------------------------------
    // Loop through digi time slices
    //-----------------------------------------------------

    (*m_digiTree.digi_timeslice_dv        )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_er        )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_raw       )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_adc       )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_ladc      )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_nomFC     )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_fiber     )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_fiberChan )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_capid     )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_allFC     )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_pedFC     )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_FC        )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_gain      )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_rcgain    )[idigi].resize ( digi_nTS );
    (*m_digiTree.digi_timeslice_energy    )[idigi].resize ( digi_nTS );

    for ( int iTS = 0; iTS < digi_nTS ; ++iTS ) {
      
      //-----------------------------------------------------
      // Get slice-specific cc objects
      //-----------------------------------------------------

      const HcalQIESample * qieSample = & digi -> sample (iTS);

      //-----------------------------------------------------
      // Get slice-specific values
      //-----------------------------------------------------

      int    digi_timeslice_dv        = (int) qieSample -> dv();
      int    digi_timeslice_er        = (int) qieSample -> er();
      int    digi_timeslice_raw       = qieSample -> raw();
      int    digi_timeslice_adc       = qieSample -> adc();
      int    digi_timeslice_ladc      = integer_tool[iTS];
      int    digi_timeslice_nomFC     = qieSample -> nominal_fC();
      int    digi_timeslice_fiber     = qieSample -> fiber();
      int    digi_timeslice_fiberChan = qieSample -> fiberChan();
      int    digi_timeslice_capid     = qieSample -> capid();
      double digi_timeslice_allFC     = tool[iTS];
      double digi_timeslice_pedFC     = calibrations -> pedestal     ( digi_timeslice_capid );
      double digi_timeslice_FC        = digi_timeslice_allFC - digi_timeslice_pedFC;
      double digi_timeslice_gain      = calibrations -> rawgain      ( digi_timeslice_capid );
      double digi_timeslice_rcgain    = calibrations -> respcorrgain ( digi_timeslice_capid ) ;
      double digi_timeslice_energy    = ( digi_timeslice_allFC - digi_timeslice_pedFC ) * digi_timeslice_rcgain;

      (*m_digiTree.digi_timeslice_dv        )[idigi][iTS] = digi_timeslice_dv       ;
      (*m_digiTree.digi_timeslice_er        )[idigi][iTS] = digi_timeslice_er       ;
      (*m_digiTree.digi_timeslice_raw       )[idigi][iTS] = digi_timeslice_raw      ;
      (*m_digiTree.digi_timeslice_adc       )[idigi][iTS] = digi_timeslice_adc      ;
      (*m_digiTree.digi_timeslice_ladc      )[idigi][iTS] = digi_timeslice_ladc     ;
      (*m_digiTree.digi_timeslice_nomFC     )[idigi][iTS] = digi_timeslice_nomFC    ;
      (*m_digiTree.digi_timeslice_fiber     )[idigi][iTS] = digi_timeslice_fiber    ;
      (*m_digiTree.digi_timeslice_fiberChan )[idigi][iTS] = digi_timeslice_fiberChan;
      (*m_digiTree.digi_timeslice_capid     )[idigi][iTS] = digi_timeslice_capid    ;
      (*m_digiTree.digi_timeslice_allFC     )[idigi][iTS] = digi_timeslice_allFC    ;
      (*m_digiTree.digi_timeslice_pedFC     )[idigi][iTS] = digi_timeslice_pedFC    ;
      (*m_digiTree.digi_timeslice_FC        )[idigi][iTS] = digi_timeslice_FC      ;
      (*m_digiTree.digi_timeslice_gain      )[idigi][iTS] = digi_timeslice_gain     ;
      (*m_digiTree.digi_timeslice_rcgain    )[idigi][iTS] = digi_timeslice_rcgain   ;
      (*m_digiTree.digi_timeslice_energy    )[idigi][iTS] = digi_timeslice_energy   ;
      
    }

    //-----------------------------------------------------
    // For each digi, try to find a rechit
    //-----------------------------------------------------

    reco = recos.find ( * hcalDetId ) ;

    double reco_energy = -999.;
    double reco_time   = -999.;

    if ( reco != reco_end ) {
      reco_energy = reco -> energy();
      reco_time   = reco -> time();
    }
    
    (*m_digiTree.reco_energy).push_back ( reco_energy ) ;
    (*m_digiTree.reco_energy).push_back ( reco_time   ) ;

    ++idigi;
    
  } // end of loop over digis
}

void HcalDigiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {


  //-----------------------------------------------------
  // Fill the run and event number information
  //-----------------------------------------------------
  
  int run   = iEvent.id().run();
  int event = iEvent.id().event();  

  //-----------------------------------------------------
  // Initialize the Tree values
  //-----------------------------------------------------

  m_digiTree.clear();
  
  (*m_digiTree.run  ).push_back ( run   );
  (*m_digiTree.event).push_back ( event );
  
  //-----------------------------------------------------
  // edm::ESHandles
  //-----------------------------------------------------

  edm::ESHandle<HcalDbService> conditions;
  edm::ESHandle<HcalTPGCoder > inputCoder;
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  edm::Handle<FEDRawDataCollection>  rawData;
  edm::Handle<HBHEDigiCollection  >  hbheDigis;
  edm::Handle<HBHERecHitCollection>  hbheRecos;
  edm::Handle<HFDigiCollection    >  hfDigis;
  edm::Handle<HFRecHitCollection  >  hfRecos;
  edm::Handle<HODigiCollection    >  hoDigis;
  edm::Handle<HORecHitCollection  >  hoRecos;

  //-----------------------------------------------------
  // Get EventSetup objects
  //-----------------------------------------------------

  iSetup.get<HcalDbRecord >().get(conditions);
  iSetup.get<HcalTPGRecord>().get(inputCoder);

  const HcaluLUTTPGCoder* inputLUTCoder = dynamic_cast<const HcaluLUTTPGCoder*>(inputCoder.product());
  
  //-----------------------------------------------------
  // Get EDM event objects
  //-----------------------------------------------------

  bool gotRawData = iEvent.getByLabel(m_rawDataLabel, rawData);
  if (!gotRawData){
    std::cout << "Could not find FedRawDataCollection with tag: " << m_rawDataLabel << std::endl;
    return;
  }

  bool gotHBHEDigis = iEvent.getByLabel(m_hcalDigisLabel, hbheDigis);
  if (!gotHBHEDigis ) {
    std::cout << "Could not find HBHE Digis with tag " << m_hcalDigisLabel << std::endl;
    return;
  }

  bool gotHBHERecos = iEvent.getByLabel(m_hbheRecHitsLabel, hbheRecos);
  if (!gotHBHERecos ) {
    std::cout << "Could not find HBHE Recos with tag " << m_hbheRecHitsLabel << std::endl;
    return;
  }

  bool gotHFDigis = iEvent.getByLabel(m_hcalDigisLabel, hfDigis);
  if (!gotHFDigis ) {
    std::cout << "Could not find HF Digis with tag " << m_hcalDigisLabel << std::endl;
    return;
  }

  bool gotHFRecos = iEvent.getByLabel(m_hfRecHitsLabel, hfRecos);
  if (!gotHFRecos ) {
    std::cout << "Could not find HF Recos with tag " << m_hfRecHitsLabel << std::endl;
    return;
  }

  bool gotHODigis = iEvent.getByLabel(m_hcalDigisLabel, hoDigis);
  if (!gotHODigis ) {
    std::cout << "Could not find HO Digis with tag " << m_hcalDigisLabel << std::endl;
    return;
  }

  bool gotHORecos = iEvent.getByLabel(m_hoRecHitsLabel, hoRecos);
  if (!gotHORecos ) {
    std::cout << "Could not find HO Recos with tag " << m_hoRecHitsLabel << std::endl;
    return;
  }

  //-----------------------------------------------------
  // Analyze raw data
  //-----------------------------------------------------
  
  for(int fed_number=FEDNumbering::MINHCALFEDID;fed_number<=FEDNumbering::MAXHCALFEDID; fed_number++) {
    const FEDRawData& fedData = rawData->FEDData(fed_number);
    if ( fedData.size() < 24 ) continue ;
    const HcalDCCHeader * dccHeader = (HcalDCCHeader*) fedData.data();
    int orbit_number = dccHeader -> getOrbitNumber();
    int bunch_number = dccHeader -> getBunchId();
    
    (*m_digiTree.fed_number).push_back(fed_number  );
    (*m_digiTree.fed_bcn   ).push_back(bunch_number);
    (*m_digiTree.fed_orn   ).push_back(orbit_number);
  }
  
  //-----------------------------------------------------
  // Analyze digis
  //-----------------------------------------------------

  analyzeDigiCollection ( *conditions , *inputLUTCoder ,  *hbheDigis , *hbheRecos ) ;
  analyzeDigiCollection ( *conditions , *inputLUTCoder ,  *hfDigis   , *hfRecos   ) ;
  // analyzeDigiCollection ( *conditions , *inputLUTCoder ,  *hoDigis   , *hoRecos   ) ;

  //-----------------------------------------------------
  // Fill tree
  //-----------------------------------------------------

  m_fillDigi.fill();

}

void HcalDigiAnalyzer::beginJob(){}

void HcalDigiAnalyzer::endJob() {
  m_fillDigi.finalize();
}

DEFINE_FWK_MODULE(HcalDigiAnalyzer);
