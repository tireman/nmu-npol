// @(#)root/physics:$Name:  $:$Id: TGenPhaseSpace.h,v 1.2 2000/09/11 06:16:26 brun Exp $
// Author: Rene Brun , Valerio Filippini  06/09/2000 
// Modified: Joern Langheinrich, 28/09/2000
//           specialize three particle decay
//           generate a plain distribution in dalitz

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   Phase Space Generator, based on the GENBOD routine of CERNLIB           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
   
#ifndef JLAB_JGenPhaseSpace
#define JLAB_JGenPhaseSpace

#include "TLorentzVector.h"
#define NFWSUM 1001

class JGenPhaseSpace : public TObject {
 private:  
  Int_t        fNt;             // number of decay particles
  Double_t     fMass[4];        // masses of particles
  Double_t     fBeta[4];        // betas of decaying particle
  Double_t     fTeCm;           // total energy in the C.M.
  Double_t     fTeCmTm;         // total energy in the C.M. minus the total mass
  Double_t     fWtMax;          // maximum weigth 
  TLorentzVector fDecPro[4];    // kinematics of the generated particles 
  Double_t     fwSum [NFWSUM];  // integrated weight function

  Double_t pdk(Double_t a, Double_t b, Double_t c);  
  Double_t cheatRnd(Double_t rnd);
  Double_t wtRnd(Double_t rnd);

 public:
  JGenPhaseSpace() {}
  JGenPhaseSpace(const JGenPhaseSpace &gen);
  virtual ~JGenPhaseSpace() {}

  Bool_t          SetDecay(TLorentzVector &P, 
			   Int_t nt, Double_t *mass, Option_t *opt="");
  Double_t        Generate();
  TLorentzVector* GetDecay(Int_t n); 

  Int_t           GetNt()      const { return fNt;}
  Double_t        GetWtMax()   const { return fWtMax;}

  //  ClassDef(JGenPhaseSpace,1) Simple Phase Space Generator
};

#endif
