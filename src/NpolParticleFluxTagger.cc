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

G4double NpolParticleFluxTagger::vertAngle = 4.927*deg;  // 1 degree more than gap height
G4double NpolParticleFluxTagger::horAngle =  10.385*deg; // 2 degree more than gap width
G4double NpolParticleFluxTagger::taggerPos = 1.500*m; 
G4double NpolParticleFluxTagger::NpolAng = 28.0*deg;

NpolParticleFluxTagger::NpolParticleFluxTagger() {
  ConstructParticleTagger();
}

NpolParticleFluxTagger::~NpolParticleFluxTagger() {}

G4String NpolParticleFluxTagger::GetName() {
  return G4String("Particle Flux Tagger");
}

// Construct a thin air box so we can tag particles before entering the first magnet.  
// Place it just a millimeter off the front steel wall
// Vertical and horizontal angles are calculated by taking the Dipole 1 specs and the distance
// to the front face of the magnet, then add 1 degree to that result so the solid angle of the
// target tagger will encompass more than just the opening.
void NpolParticleFluxTagger::ConstructParticleTagger(){  

  G4double xlen = 2*taggerPos*tan(horAngle); G4double ylen = 2*taggerPos*tan(vertAngle); G4double zlen = 0.010*cm;


  G4Box *ParticleTagger = new G4Box("ParticleTagger",xlen/2,ylen/2,zlen/2);
  ParticleTaggerLV = new G4LogicalVolume(ParticleTagger,NpolMaterials::GetInstance()->GetMaterial("Air"),"NPOLTaggerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  ParticleTaggerLV->SetVisAttributes(TaggerVisAtt);
}

void NpolParticleFluxTagger::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(ParticleTaggerLV, motherLV, "ParticleTagger",  taggerPos, -NpolAng, 0);

}

