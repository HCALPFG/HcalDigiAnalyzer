#include "HCALPFG/HcalDigiAnalyzer/interface/FillDigiTree.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

FillDigiTree::FillDigiTree(){}

FillDigiTree::~FillDigiTree(){}

void 
FillDigiTree::init(std::string filename, DigiTree* treePtr) {
  
   m_treePtr = treePtr;
   m_treePtr -> init();
   
   m_file = new TFile( filename.c_str(), "RECREATE" );
   
   m_tree = new TTree( "digitree", "HCAL Digi Info", 1 );   
   
   m_tree->Branch("run"                       , &(m_treePtr->run                      )); 
   m_tree->Branch("event"		      , &(m_treePtr->event		      ));
   m_tree->Branch("fed_number"                , &(m_treePtr->fed_number               ));
   m_tree->Branch("fed_bcn"                   , &(m_treePtr->fed_bcn                  ));
   m_tree->Branch("fed_orn"                   , &(m_treePtr->fed_orn                  ));
   m_tree->Branch("digi_subdet"	              , &(m_treePtr->digi_subdet	      ));
   m_tree->Branch("digi_ieta"		      , &(m_treePtr->digi_ieta		      ));
   m_tree->Branch("digi_iphi"		      , &(m_treePtr->digi_iphi		      ));
   m_tree->Branch("digi_depth"		      , &(m_treePtr->digi_depth	              ));
   m_tree->Branch("digi_presamples"	      , &(m_treePtr->digi_presamples	      ));
   m_tree->Branch("digi_nTS"		      , &(m_treePtr->digi_nTS		      ));
   m_tree->Branch("digi_fiberIdleOffset"      , &(m_treePtr->digi_fiberIdleOffset     ));
   m_tree->Branch("digi_timeslice_dv"	      , &(m_treePtr->digi_timeslice_dv	      ));
   m_tree->Branch("digi_timeslice_er"	      , &(m_treePtr->digi_timeslice_er	      ));
   m_tree->Branch("digi_timeslice_raw"	      , &(m_treePtr->digi_timeslice_raw       ));
   m_tree->Branch("digi_timeslice_adc"	      , &(m_treePtr->digi_timeslice_adc       ));
   m_tree->Branch("digi_timeslice_ladc"	      , &(m_treePtr->digi_timeslice_ladc      ));
   m_tree->Branch("digi_timeslice_nomFC"      , &(m_treePtr->digi_timeslice_nomFC     ));
   m_tree->Branch("digi_timeslice_fiber"      , &(m_treePtr->digi_timeslice_fiber     ));
   m_tree->Branch("digi_timeslice_fiberChan"  , &(m_treePtr->digi_timeslice_fiberChan ));
   m_tree->Branch("digi_timeslice_capid"      , &(m_treePtr->digi_timeslice_capid     ));
   m_tree->Branch("digi_timeslice_allFC"      , &(m_treePtr->digi_timeslice_allFC     ));
   m_tree->Branch("digi_timeslice_pedFC"      , &(m_treePtr->digi_timeslice_pedFC     ));
   m_tree->Branch("digi_timeslice_FC"         , &(m_treePtr->digi_timeslice_FC        ));
   m_tree->Branch("digi_timeslice_gain "      , &(m_treePtr->digi_timeslice_gain      ));
   m_tree->Branch("digi_timeslice_rcgain"     , &(m_treePtr->digi_timeslice_rcgain    ));
   m_tree->Branch("digi_timeslice_energy"     , &(m_treePtr->digi_timeslice_energy    ));
   m_tree->Branch("reco_energy" 	      , &(m_treePtr->reco_energy 	      ));
   m_tree->Branch("reco_time"                 , &(m_treePtr->reco_time                )); 
   
}

DigiTree* FillDigiTree::getTreePtr() {
  return m_treePtr;
}

void FillDigiTree::fill() {
   m_tree->Fill();
}

void FillDigiTree::finalize() {
   m_file->Write();
   m_file->Close();
}
