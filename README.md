# HcalDigiAnalyzer
EDM analyzer for looking at HCAL digis and rechits

First time setup and test run over an LED run:

```
mkdir WhereYouWantToWork
cd WhereYouWantToWork
cmsrel CMSSW_10_6_4_patch1 # 10_6 or newer for era=Run3
cd CMSSW_10_6_4_patch1/src
cmsenv
git clone https://github.com/HCALPFG/HcalDigiAnalyzer HCALPFG/HcalDigiAnalyzer
scram b -j8
cd HCALPFG/HcalDigiAnalyzer/test
cmsRun skeleton_cfg.py run=329864
```

You should end up with a file `skeletonanalysis_329864.root` containing a few histograms.

