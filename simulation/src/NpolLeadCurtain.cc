//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	         *
//******************************************************************

// %% NpolLeadCurtain.cc %%

// Lead Curtain construction file
// Created: William Tireman - May 2017

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolLeadCurtain.hh"
#include "NpolDipole1.hh"
#include "NpolDipole2.hh"
#include "NpolShieldHut.hh"
#include "NpolParticleFluxTagger.hh"

G4double NpolLeadCurtain::leadThickness = 15.0*cm;  // thickness of the lead curtain
//G4double NpolLeadCurtain::PosLead = NpolParticleFluxTagger::taggerPos - leadThickness/2 + 18.50*cm; // Move D1 back 15*cm to fit lead shield
G4double NpolLeadCurtain::PosLead = NpolDipole2::PosD2 - NpolDipole2::gapLength/2 + leadThickness/2 - 58.25*cm; // offset of lead (58.25 cm for location 1, 55.25 cm for Location 2 )

NpolLeadCurtain::NpolLeadCurtain() {
   ConstructLeadCurtain();
}

NpolLeadCurtain::~NpolLeadCurtain() {}

G4String NpolLeadCurtain::GetName() {
  return G4String("Lead Curtain");
}

// Construct a lead shield for in front of the polarimeter
void NpolLeadCurtain::ConstructLeadCurtain(){

  // Make lead curtain 5*cm wider/taller than what is necessary
  G4double xlen = 120*cm; //2*(PosLead)*tan(NpolShieldHut::horAngle/2) + 5.0*cm;
  G4double ylen = 90*cm; //2*(PosLead)*tan(NpolShieldHut::vertAngle/2) + 5.0*cm; 

  G4Box *LeadCurtain = new G4Box("LeadCurtain",xlen/2,ylen/2,leadThickness/2);
  LeadCurtainLV = new G4LogicalVolume(LeadCurtain,NpolMaterials::GetInstance()->GetMaterial("Pb"),"LeadCurtainLV",0,0,0);
  G4VisAttributes *LeadCurtainVisAtt = new G4VisAttributes(G4Colour(1.0, 0.7, 0.2));
  LeadCurtainLV->SetVisAttributes(LeadCurtainVisAtt);
}


void NpolLeadCurtain::Place(G4LogicalVolume *motherLV) {
 
  PlaceCylindrical(LeadCurtainLV, motherLV, "LeadCurtain", PosLead,-NpolShieldHut::NpolAng, 0);

}
