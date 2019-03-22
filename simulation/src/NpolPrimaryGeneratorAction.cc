//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolPrimaryGeneratorAction.cc %%

// Generates the primary particle for each event
// Created: William Tireman
// Modified: 26-June-2016  Changed to General Particle Source -- W.T.

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "TMath.h"
#include "TRandom3.h"
#include <TLorentzVector.h>
#include "JGenPhaseSpace.h"
#include "JGenFermiMomentum.h"

#include "NpolPrimaryGeneratorAction.hh"
#include "NpolPolarimeter.hh"

/* ----------- constants ----------- */ 
#define massElectron 0.000510998956 // GeV
#define massNeutron 0.939565378 // GeV
#define massDeuteron 1.875612928 // GeV
#define massProton 0.93827231 // GeV
#define alpha 0.0072973525664 // fine structure constant
#define dcsConversion 2.56819e-6 // const for conversion from 1ub to 1GeV^-2
/* ----------- constants ----------- */

G4double NpolPrimaryGeneratorAction::NpolAng = (NpolPolarimeter::NpolAng)*180./TMath::Pi();

NpolPrimaryGeneratorAction::NpolPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  
  //fParticleGun = new G4GeneralParticleSource();

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* pType = particleTable->FindParticle("geantino");
  fParticleGun->SetParticleDefinition(pType);
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
  std::cout << "Deleting Particle Gun" << std::endl;
  delete fParticleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double NpolAng = -NpolPolarimeter::NpolAng;
  TLorentzVector Vector = GenerateNeutronEvent();
  G4double nMom = Vector.P();
  G4double nTheta = Vector.Theta();
  G4double nPhi = Vector.Phi();
   
  G4double xDir = sin(nTheta)*cos(nPhi);
  G4double yDir = sin(nTheta)*sin(nPhi);
  G4double zDir = cos(nTheta);
     
  G4double xPrimeDir = xDir*cos(NpolAng) + zDir*sin(NpolAng);
  G4double yPrimeDir = 1*yDir;
  G4double zPrimeDir = -xDir*sin(NpolAng) + zDir*cos(NpolAng);
 
  fParticleGun->SetParticleMomentum(nMom);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xPrimeDir,yPrimeDir,zPrimeDir));
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  fParticleGun->SetParticlePolarization(G4ThreeVector(1.,0.,0.));
  fParticleGun->GeneratePrimaryVertex(anEvent); 

}

//  Generator from Tongtong Cao (post-doc, Hampton U.) for computation of a Neutron Lorentz 4-vector
//  using the differential cross sections for polarized and unpolarized (e,e'n) reaction.  See
//  document for more details in nmu-npol/simulation/npol-doc folder. 
TLorentzVector NpolPrimaryGeneratorAction::GenerateNeutronEvent(){

  bool evtFlag = false;
  int channel = 3; // channel
  char filter='p';  // Choose which filter is used to select events: n - no filter; u - unpolarized differential cross section; p - polarized differential cross section
  double maxDCS=0.0438455; // maximum of differential cross section (Q^2=3.95)
  double beamEnergy=4.40*GeV; // beam energy (GeV) (for Q^2=3.95)
  double helicityRatio=1; // ratio of events with helicity + to with helicity - for the electron beam
  double polBeam=0.8; // polarization of beam
  double openAngle=5; // opening angle of the detector for scattered electrons (deg)
  double thetaNeutronFree=NpolAng;// polar angle of neutron polarimeter (deg)
  double gen=0; // electronic form factor of neutron
  double gmn=0; // magnetic form factor of neutron

  double thetaNeutronFreeRad=thetaNeutronFree/180.*TMath::Pi();
  double pSNeutronFree=2*massNeutron*beamEnergy*(massNeutron+beamEnergy)*cos(thetaNeutronFreeRad)/((2*beamEnergy*massNeutron+massNeutron*massNeutron+beamEnergy*beamEnergy*sin(thetaNeutronFreeRad)*sin(thetaNeutronFreeRad)));
  double pSElectronFree=sqrt(pow(pSNeutronFree*sin(thetaNeutronFreeRad),2)+pow(beamEnergy-pSNeutronFree*cos(thetaNeutronFreeRad),2));
  double q2Free=2*(beamEnergy-pSElectronFree)*massNeutron;
  double thetaSElectronFree=asin(sqrt((beamEnergy-pSElectronFree)*massNeutron/2/beamEnergy/pSElectronFree))*2/TMath::Pi()*180;

  //// Variables defination ////
  int event = 0; //Serial number of events
  double x=0, y=0, z=0; // vertex
  double fcos=0, fphi=0,fpx=0, fpy=0, fpz=0, fEn=0; //variables for Fermi Momentum
  double masses[2]={massElectron,massNeutron}; // masses of product paritcles for en
  double massesENP[3]={massElectron,massNeutron, massProton}; // masses of product paritcles for ed
  double q2=0, energySElectron=0, thetaSElectron=0, pRNeutron=0, thetaRNeutron=0, tau=0, epsilon=0;
  double unpolDCS=0, polDCS=0; // unploarized and polarized differential cross section
  double polTrans=0, polLongi=0, phaseShift=0; // transverse and longitudinal components and phase-shift of recoiled neutron polarization
  int helicity=1; // helicity

  TLorentzVector *pBeam = NULL;
  TLorentzVector beam;
  pBeam = &beam;
  TLorentzVector *pTarget = NULL;
  TLorentzVector target;
  pTarget = &target;
  TLorentzVector *pSpectator = NULL;
  TLorentzVector spectator;
  pSpectator = &spectator;
  TLorentzVector *pW = NULL;
  TLorentzVector w;
  pW = &w;
  TLorentzVector *pP1 = NULL;
  TLorentzVector p1;
  pP1 = &p1;
  TLorentzVector *pP2 = NULL;
  TLorentzVector p2;
  pP2 = &p2;
  TLorentzVector *pP3 = NULL; // the third produced particle is proton for ed->enp
  TLorentzVector p3;
  pP3 = &p3;			

   ////Some preparation////
  TRandom3 randomNum(0);  // randomNum is used in the main function
  TRandom3 *myRNG=new TRandom3(0);
  gRandom = myRNG;  //gRandom is used in the JGenPhaseSpace class

  double ran1=0, ran2=0, ran3=0; //random number: ran1 and ran2 are for extraction of Fermi momentum, ran3 is for fitlering events by DCS

  JGenPhaseSpace eventEN, eventED; 
  TLorentzVector deuteron;
  deuteron.SetXYZT(0,0,0,massDeuteron);
  double thetaSElectronRad=0; // polar angle of scattered electron (rad)
  double mottDCS=0; // Mott differential cross section
  double percentPosHeli=helicityRatio/(1+helicityRatio); // perecent of events with positive helicity
  ////Some preparation////


  if(gen==0) gen = genCalc(q2Free);  // If gen is not assigned, gen is calculated by the formula.
  if(gmn==0) gmn = gmnCalc(q2Free);  // If gem is not assigned, gen is calculated by the formula.

  /* channel 1 */
  if(channel == 1){
	do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(0, 0, 0, massNeutron);
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=='n'){
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=='u'){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=='p'){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
      }
    } while(!(evtFlag)); 
  } 
  /* End channel 1 */

  /* channel 2 */
  if(channel == 2){
	do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      // Use reject-accept method to select a value of Fermi Momentum
      ran1 = 0.5*randomNum.Rndm();
      ran2 = 11.0*randomNum.Rndm();
      if (JGenFermiMomentum::Instance().Spectral(ran1)<ran2) continue;
      fcos = (2*randomNum.Rndm())-1;
      fphi = 2*3.141592653*randomNum.Rndm();
      fpx = ran1*sqrt(1-fcos*fcos)*cos(fphi);
      fpy = ran1*sqrt(1-fcos*fcos)*sin(fphi);
      fpz = ran1*fcos;
      fEn = sqrt(fpx*fpx+fpy*fpy+fpz*fpz+massNeutron*massNeutron);
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(fpx, fpy, fpz, fEn);
      spectator=deuteron-target;
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=='n'){
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=='u'){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=='p'){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
      }
    } while(!(evtFlag)); 
  }
  /* End channel 2 */
  
  /* channel 3 */
  if(channel == 3){
    do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
	  }
	  
      // Use reject-accept method to select a value of Fermi Momentum
      ran1 = 0.5*randomNum.Rndm();
      ran2 = 11.0*randomNum.Rndm();
      if (JGenFermiMomentum::Instance().Spectral(ran1)<ran2) continue;
      fcos = (2*randomNum.Rndm())-1;
      fphi = 2*3.141592653*randomNum.Rndm();
      fpx = ran1*sqrt(1-fcos*fcos)*cos(fphi);
      fpy = ran1*sqrt(1-fcos*fcos)*sin(fphi);
      fpz = ran1*fcos;
      fEn = sqrt(fpx*fpx+fpy*fpy+fpz*fpz+massNeutron*massNeutron);
	  
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(fpx, fpy, fpz, fEn);
	  
      spectator=deuteron-target;
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=='n'){
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){	
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=='u'){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			double k1=q2*pow(gmn,2);
			double k2=pow(2*massNeutron,2)*(pow(gen,2)+tau*pow(gmn,2))/(1+tau);
			double term1=beam.E()*pP1->E()-beam.Px()*pP1->Px()-beam.Py()*pP1->Py()-beam.Pz()*pP1->Pz()-2*pow(massElectron,2);
			double term2=(beam.E()*target.E()-beam.Px()*target.Px()-beam.Py()*target.Py()-beam.Pz()*target.Pz())*(pP1->E()*target.E()-pP1->Px()*target.Px()-pP1->Py()*target.Py()-pP1->Pz()*target.Pz())/pow(massNeutron,2)-q2/4;
			double M2=pow(2*alpha,2)/pow(q2,2)*(k1*term1+k2*term2);
			unpolDCS=1./4*pow(pP1->P(),3)*M2/(beam.P()*massNeutron*fabs(beamEnergy*pP1->E()*massNeutron-pP2->E()*pow(massElectron,2)))/dcsConversion;
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=='p'){
		  ran3 = maxDCS*randomNum.Rndm();
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){
			double k1=q2*pow(gmn,2);
			double k2=pow(2*massNeutron,2)*(pow(gen,2)+tau*pow(gmn,2))/(1+tau);
			double term1=beam.E()*pP1->E()-beam.Px()*pP1->Px()-beam.Py()*pP1->Py()-beam.Pz()*pP1->Pz()-2*pow(massElectron,2);
			double term2=(beam.E()*target.E()-beam.Px()*target.Px()-beam.Py()*target.Py()-beam.Pz()*target.Pz())*(pP1->E()*target.E()-pP1->Px()*target.Px()-pP1->Py()*target.Py()-pP1->Pz()*target.Pz())/pow(massNeutron,2)-q2/4;
			double M2=pow(2*alpha,2)/pow(q2,2)*(k1*term1+k2*term2);
			unpolDCS=1./4*pow(pP1->P(),3)*M2/(beam.P()*massNeutron*fabs(beamEnergy*pP1->E()*massNeutron-pP2->E()*pow(massElectron,2)))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
	  }
	} while(!(evtFlag));  
  }
  /* End channel 3 */
  
 /* channel 4 */
  if(channel == 4){
    do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target=deuteron;
      w = beam + target;
	  
      if (eventED.SetDecay (w, 3, massesENP)){
		eventED.Generate ();
		pP1 = eventED.GetDecay (0);
		pP2 = eventED.GetDecay (1);
		pP3 = eventED.GetDecay (2);
		
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=='n'){
		  if(thetaSElectron>thetaSElectronFree-openAngle/2 && thetaSElectron <thetaSElectronFree+openAngle/2){	
			evtFlag = true;
			event++;
		  }	event++;
		}
      }
    } while(!(evtFlag)); 
  }
  /* End channel 4 */

  TLorentzVector Vector = *pP2;
  return Vector;
}


double NpolPrimaryGeneratorAction::genCalc(double q2){
  double tau=q2/4/massNeutron/massNeutron;
  double gD=pow(1+q2/0.71,-2);
  double gen=-0.886*(-1.913)*tau*gD/(1+3.29*tau);
  return gen;
}

double NpolPrimaryGeneratorAction::gmnCalc(double q2){
  double b[5]={3.26,-0.272, 0.0123, -2.52, 2.55};
  double gmn=-1.913/(1+b[0]*q2/(1+b[1]*q2/(1+b[2]*q2/(1+b[3]*q2/(1+b[4]*q2)))));
  return gmn;
}
