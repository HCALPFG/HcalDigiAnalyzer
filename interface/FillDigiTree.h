#ifndef HCALPFG_HcalDigiAnalyzer_FillDigiTree_h
#define HCALPFG_HcalDigiAnalyzer_FillDigiTree_h

#include <string>

#include "HCALPFG/HcalDigiAnalyzer/interface/DigiTree.h"

class TFile;
class TTree;

class FillDigiTree
{

   public:
      FillDigiTree();
      virtual ~FillDigiTree();
      
      void init(std::string filename, DigiTree* treePtr);
      DigiTree* getTreePtr();
      void fill();
      void finalize();

   private:
      FillDigiTree(const FillDigiTree&); // stop default

      const FillDigiTree& operator=(const FillDigiTree&); // stop default
      
      DigiTree* m_treePtr;
      TFile* m_file;
      TTree* m_tree;

};


#endif
