#ifndef HCALPFG_HcalDigiAnalyzer_DigiTree_h
#define HCALPFG_HcalDigiAnalyzer_DigiTree_h

#include <vector>

class DigiTree {
  
 public:
  
  DigiTree();
  virtual ~DigiTree();
  
  std::vector<int> * run;
  std::vector<int> * event;

  std::vector < int > * fed_number;
  std::vector < int > * fed_bcn;
  std::vector < int > * fed_orn;

  std::vector < int > * digi_subdet         ;
  std::vector < int > * digi_ieta           ;
  std::vector < int > * digi_iphi           ;
  std::vector < int > * digi_depth          ;
  std::vector < int > * digi_presamples     ;
  std::vector < int > * digi_nTS            ;
  std::vector < int > * digi_fiberIdleOffset;
  
  std::vector < std::vector<int   > > * digi_timeslice_dv        ;
  std::vector < std::vector<int   > > * digi_timeslice_er        ;
  std::vector < std::vector<int   > > * digi_timeslice_raw       ;
  std::vector < std::vector<int   > > * digi_timeslice_adc       ;
  std::vector < std::vector<int   > > * digi_timeslice_ladc      ;
  std::vector < std::vector<int   > > * digi_timeslice_nomFC     ;
  std::vector < std::vector<int   > > * digi_timeslice_fiber     ;
  std::vector < std::vector<int   > > * digi_timeslice_fiberChan ;
  std::vector < std::vector<int   > > * digi_timeslice_capid     ;
  std::vector < std::vector<double> > * digi_timeslice_allFC     ;
  std::vector < std::vector<double> > * digi_timeslice_pedFC     ;
  std::vector < std::vector<double> > * digi_timeslice_FC        ;
  std::vector < std::vector<double> > * digi_timeslice_gain      ;
  std::vector < std::vector<double> > * digi_timeslice_rcgain    ;
  std::vector < std::vector<double> > * digi_timeslice_energy    ;

  std::vector < double > * reco_energy;
  std::vector < double > * reco_time;
  
  void init () { 

    run   =  0;
    event =  0;

    fed_number           = 0;
    fed_bcn              = 0;
    fed_orn              = 0;
    
    digi_subdet          = 0;
    digi_ieta            = 0;
    digi_iphi            = 0;
    digi_depth           = 0;
    digi_presamples      = 0;
    digi_nTS             = 0;
    digi_fiberIdleOffset = 0;
    
    digi_timeslice_dv         = 0;
    digi_timeslice_er         = 0;
    digi_timeslice_raw        = 0;
    digi_timeslice_adc        = 0;
    digi_timeslice_ladc       = 0;
    digi_timeslice_nomFC      = 0;
    digi_timeslice_fiber      = 0;
    digi_timeslice_fiberChan  = 0;
    digi_timeslice_capid      = 0;
    digi_timeslice_allFC      = 0;
    digi_timeslice_pedFC      = 0;
    digi_timeslice_FC         = 0;
    digi_timeslice_gain       = 0;
    digi_timeslice_rcgain     = 0;
    digi_timeslice_energy     = 0;
    
    reco_energy = 0;
    reco_time   = 0;
    
  }

  void clear() { 
    run -> clear();
    event -> clear();

    fed_number          -> clear();
    fed_bcn             -> clear();
    fed_orn             -> clear();
    
    digi_subdet         -> clear();
    digi_ieta           -> clear();
    digi_iphi           -> clear();
    digi_depth          -> clear();
    digi_presamples     -> clear();
    digi_nTS            -> clear();
    digi_fiberIdleOffset-> clear();
    
    digi_timeslice_dv        -> clear();
    digi_timeslice_er        -> clear();
    digi_timeslice_raw       -> clear();
    digi_timeslice_adc       -> clear();
    digi_timeslice_ladc      -> clear();
    digi_timeslice_nomFC     -> clear();
    digi_timeslice_fiber     -> clear();
    digi_timeslice_fiberChan -> clear();
    digi_timeslice_capid     -> clear();
    digi_timeslice_allFC     -> clear();
    digi_timeslice_pedFC     -> clear();
    digi_timeslice_FC        -> clear();
    digi_timeslice_gain      -> clear();
    digi_timeslice_rcgain    -> clear();
    digi_timeslice_energy    -> clear();

    reco_energy-> clear();
    reco_time  -> clear();
    
  }
  
 private:
  DigiTree(const DigiTree&); // stop default
  const DigiTree& operator=(const DigiTree&); // stop default

};

#endif
