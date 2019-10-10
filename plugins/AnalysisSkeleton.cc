#include "HCALPFG/HcalDigiAnalyzer/interface/AnalysisSkeleton.h"

AnalysisSkeleton::AnalysisSkeleton(const edm::ParameterSet& ps) {
    // Comment out the ones you don't need
    _tagHBHE  = ps.getUntrackedParameter<edm::InputTag>("tagHBHE", edm::InputTag("hcalDigis"));
    _tagHF    = ps.getUntrackedParameter<edm::InputTag>("tagHF", edm::InputTag("hcalDigis"));
    _tagHO    = ps.getUntrackedParameter<edm::InputTag>("tagHO", edm::InputTag("hcalDigis"));
    _tagQIE10    = ps.getUntrackedParameter<edm::InputTag>("tagQIE10", edm::InputTag("hcalDigis"));
    _tagQIE11  = ps.getUntrackedParameter<edm::InputTag>("tagQIE11", edm::InputTag("hcalDigis"));

    _tokHBHE  = consumes<HBHEDigiCollection>(_tagHBHE);
    _tokHF  = consumes<HFDigiCollection>(_tagHF);
    _tokHO  = consumes<HODigiCollection>(_tagHO);
    _tokQIE10  = consumes<QIE10DigiCollection>(_tagQIE10);
    _tokQIE11  = consumes<QIE11DigiCollection>(_tagQIE11);

    _nevents = 0;   
}


AnalysisSkeleton::~AnalysisSkeleton() {}


// ------------ method called for each event  ------------
void AnalysisSkeleton::analyze(const edm::Event& event, const edm::EventSetup& es)
{
    _nevents += 1;

    // Load digis
    edm::Handle<HBHEDigiCollection> hbhe_digis;
    if (!event.getByToken(_tokHBHE, hbhe_digis)) {
        edm::LogError("Collection HBHEDigiCollection" + _tagHBHE.label() + " " + _tagHBHE.instance());
    }

    edm::Handle<HFDigiCollection> hf_digis;
    if (!event.getByToken(_tokHF, hf_digis)) {
        edm::LogError("Collection HFDigiCollection" + _tagHF.label() + " " + _tagHF.instance());
    }

    edm::Handle<HODigiCollection> ho_digis;
    if (!event.getByToken(_tokHO, ho_digis)) {
        edm::LogError("Collection HODigiCollection" + _tagHO.label() + " " + _tagHO.instance());
    }

    edm::Handle<QIE10DigiCollection> qie10_digis;
    if (!event.getByToken(_tokQIE10, qie10_digis)) {
        edm::LogError("Collection QIE10DigiCollection" + _tagQIE10.label() + " " + _tagQIE10.instance());
    }

    edm::Handle<QIE11DigiCollection> qie11_digis;
    if (!event.getByToken(_tokQIE11, qie11_digis)) {
        edm::LogError("Collection QIE11DigiCollection" + _tagQIE11.label() + " " + _tagQIE11.instance());
    }

    // Loop over digis
    /* Example HBHE loop (same code for HO and HF) */
    for (HBHEDigiCollection::const_iterator digi = hbhe_digis->begin(); digi != hbhe_digis->end(); ++digi) {
        // Digi physical and electronic ID
        HcalDetId const& did = digi->id();
        if (did.subdet() != HcalBarrel && did.subdet() != HcalEndcap) {
            continue;
        }
        uint32_t rawid = _ehashmap.lookup(did);
        HcalElectronicsId const& eid(rawid);

#ifdef USE_CQ
        // Filter out channels that are masked out
        if (!(_channelQuality[did])) {
            continue;
        }
#endif

        int nsamples = digi->size();

        // Load adc to fC conversion
        CaloSamples fc_ts; // fc_ts[i] returns the fC in timeslice i
        const HcalQIECoder *channelCoder = _dbService->getHcalCoder(did);
        const HcalQIEShape *shape = _dbService->getHcalShape(channelCoder);
        HcalCoderDb coder(*channelCoder, *shape);
        coder.adc2fC(*digi, fc_ts);

        // Load pedestals (in fC)
        HcalCalibrations calibrations = _dbService->getHcalCalibrations(did);
        double pedestals[nsamples];
        double sumq = 0.;
        for (int iTS = 0; iTS < nsamples; ++iTS) {
            int capid = digi->sample(iTS).capid();
            pedestals[nsamples] = calibrations.pedestal(capid);
            sumq += fc_ts[iTS] - pedestals[iTS];
        }

        // Fill histograms
        for (int iTS = 0; iTS < nsamples; ++iTS) {
            _th1ds["adc_hbhe_qie8"]->Fill(digi->sample(iTS).adc());
            _th1ds["fc_hbhe_qie8"]->Fill(fc_ts[iTS] - pedestals[iTS]);
        }
        _th1ds["sumq_hbhe_qie8"]->Fill(sumq);        
    }

    /* Example QIE10 loop (similar code for QIE11) */
    for (QIE10DigiCollection::const_iterator it = qie10_digis->begin(); it != qie10_digis->end(); ++it) {
        const QIE10DataFrame digi = static_cast<const QIE10DataFrame>(*it);
        HcalDetId const& did = digi.detid();

        if (did.subdet() != HcalForward) {
            continue;
        }

        uint32_t rawid = _ehashmap.lookup(did);
        HcalElectronicsId const& eid(rawid);

#ifdef USE_CQ
        // Filter out channels that are masked out
        if (!(_channelQuality[did])) {
            continue;
        }
#endif        

        int nsamples = digi.samples();

        // Load adc to fC conversion
        CaloSamples fc_ts; // fc_ts[i] returns the fC in timeslice i
        const HcalQIECoder *channelCoder = _dbService->getHcalCoder(did);
        const HcalQIEShape *shape = _dbService->getHcalShape(channelCoder);
        HcalCoderDb coder(*channelCoder, *shape);
        coder.adc2fC(digi, fc_ts);

        // Load pedestals (in fC)
        HcalCalibrations calibrations = _dbService->getHcalCalibrations(did);
        double pedestals[nsamples];
        double sumq = 0.;
        for (int iTS = 0; iTS < nsamples; ++iTS) {
            int capid = digi[iTS].capid();
            pedestals[nsamples] = calibrations.pedestal(capid);
            sumq += fc_ts[iTS] - pedestals[iTS];
        }

        // Fill histograms
        for (int iTS = 0; iTS < nsamples; ++iTS) {
            _th1ds["adc_hf_qie10"]->Fill(digi[iTS].adc());
            _th1ds["fc_hf_qie10"]->Fill(fc_ts[iTS] - pedestals[iTS]);
        }
        _th1ds["sumq_hf_qie10"]->Fill(sumq);        

    }
}

// ------------ method called once each job just before starting event loop  ------------
void
AnalysisSkeleton::beginJob() {    
    _nevents = 0;   
    _th1ds["adc_hbhe_qie8"] =  new TH1D("adc_hbhe_qie8", "ADC (HBHE QIE8)", 256, -0.5, 255.5);
    _th1ds["fc_hbhe_qie8"] =  new TH1D("fc_hbhe_qie8", "fC (HBHE QIE8)", 1000, 0., 1000.);
    _th1ds["sumq_hbhe_qie8"] =  new TH1D("sumq_hbhe_qie8", "SumQ (HBHE QIE8)", 1000, 0., 1000.);

    _th1ds["adc_hf_qie10"] =  new TH1D("adc_hf_qie10", "ADC (HBHE QIE10)", 256, -0.5, 255.5);
    _th1ds["fc_hf_qie10"] =  new TH1D("fc_hf_qie10", "fC (HBHE QIE10)", 1000, 0., 1000.);
    _th1ds["sumq_hf_qie10"] =  new TH1D("sumq_hf_qie10", "SumQ (HBHE QIE10)", 1000, 0., 1000.);
}

// ------------ method called once each job just after ending the event loop  ------------
void
AnalysisSkeleton::endJob() {
    _fs->file().cd();
    for (auto& it_hist : _th1ds) {
        it_hist.second->Write();
    }
    
}

void AnalysisSkeleton::beginRun(edm::Run const& run, edm::EventSetup const& es) {
    // Database service
    es.get<HcalDbRecord>().get(_dbService);

    // Get channels from emap (each stream gets its own emap copy)  
    HcalElectronicsMap const* emap = _dbService->getHcalMapping();
    _ehashmap.initialize(emap, hcaldqm::electronicsmap::fD2EHashMap);   
}

void AnalysisSkeleton::endRun(edm::Run const& run, edm::EventSetup const& es) {

}

void AnalysisSkeleton::beginLuminosityBlock(edm::LuminosityBlock const &lb, edm::EventSetup const& es) {
#ifdef USE_CQ
    // Build channel quality map
    edm::ESHandle<HcalChannelQuality> hcq;
    es.get<HcalChannelQualityRcd>().get("withTopo", hcq);
    const HcalChannelQuality *cq = hcq.product();
    for (auto& it_did : cq->getAllChannels()) {
        if (HcalGenericDetId(it_did).genericSubdet() == HcalGenericDetId::HcalGenUnknown) {
            continue;
        }
        if (HcalGenericDetId(it_did).isHcalDetId()) {
            HcalDetId did(it_did);
            _channelQuality[did] = true;
            uint32_t mask = (cq->getValues(did))->getValue();
            if (mask != 0) {
                _channelMasks[did] = mask;
                HcalChannelStatus cs(did.rawId(), mask);
                if (cs.isBitSet(HcalChannelStatus::HcalCellMask) || cs.isBitSet(HcalChannelStatus::HcalCellDead)) {
                    _channelQuality[did] = false;
                }
            }
        }
    }
#endif
}

void AnalysisSkeleton::endLuminosityBlock(edm::LuminosityBlock const &lb, edm::EventSetup const& es) {}


//define this as a plug-in
DEFINE_FWK_MODULE(AnalysisSkeleton);
