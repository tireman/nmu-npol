//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolParticleFluxTagger.cc %%

// Particle Flux Tagger construction file
// Created: William Tireman - December 2015

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolParticleFluxTagger.hh"

G4double NpolParticleFluxTagger::vertAngle = 0.600*rad; // 300 mrad more than enough
G4double NpolParticleFluxTagger::horAngle = 0.850*rad;  // 450 mrad more than enough
G4double NpolParticleFluxTagger::taggerPos = 1.500*m;   // tagger position from target
G4double NpolParticleFluxTagger::NpolAng = 28.0*deg;    // nominal NPOL location 28.0*deg;

NpolParticleFluxTagger::NpolParticleFluxTagger() {
  ConstructParticleTagger();
}

NpolParticleFluxTagger::~NpolParticleFluxTagger() {}

G4String NpolParticleFluxTagger::GetName() {
  return G4String("Particle Flux Tagger");
}

// Construct a thin vacuum box so we can tag particles before entering the first magnet.  
// Place it just a few millimeters off the front steel wall
// Tagger height/width larger than opening just to make sure we encompass possibilities
// In analysis, will cut down via acceptance cuts
void NpolParticleFluxTagger::ConstructParticleTagger(){  

  G4double xlen = 2*taggerPos*tan(horAngle/2); G4double ylen = 2*taggerPos*tan(vertAngle/2);
  G4double zlen = 0.10*cm;

  G4Box *ParticleTagger = new G4Box("ParticleTagger",xlen/2,ylen/2,zlen/2);
  ParticleTaggerLV = 
	new G4LogicalVolume(ParticleTagger,NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"ParticleTaggerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  ParticleTaggerLV->SetVisAttributes(TaggerVisAtt);
}

void NpolParticleFluxTagger::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(ParticleTaggerLV, motherLV, "ParticleTagger",  taggerPos, -NpolAng, 0);

}

