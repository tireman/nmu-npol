//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

//%% NpolDetectorFactory.cc %% **

// Implements methods common to all objects in the setup
// Created: Daniel Wilbern December 2014

#include "G4LogicalVolume.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "NpolDetectorFactory.hh"

// Place a volume in its mother volume at a location specified by rectangular coordinates.
G4VPhysicalVolume *NpolDetectorFactory::PlaceRectangular(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,G4String detName, G4double x, G4double y, G4double z,G4double Rx, G4double Ry, G4double Rz) {

  G4RotationMatrix Rm; 
  G4ThreeVector Tv;
  G4Transform3D Tr;
  
  Rm.rotateX(Rx);
  Rm.rotateY(Ry);
  Rm.rotateZ(Rz);
  
  Tv.setX(x); Tv.setY(y); Tv.setZ(z);
  Tr = G4Transform3D(Rm,Tv);
  
  G4VPhysicalVolume *detPV = new G4PVPlacement(Tr, detLV,
	detName, motherLV, false, 0,false); // last bool for overlap check
  
  return detPV;
}

// Place a volume in its mother volume at a location specified by cylindrical polar coordinates.
// The polar axis is the positive Z axis.
G4VPhysicalVolume *NpolDetectorFactory::PlaceCylindrical(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,G4String detName, G4double rho, G4double phi, G4double z) {

  G4RotationMatrix Rm;
  G4ThreeVector Tv;
  G4Transform3D Tr;
  
  Rm.rotateY(phi);
  Tv.setX(rho*sin(phi)); Tv.setY(z); Tv.setZ(rho*cos(phi));
  Tr = G4Transform3D(Rm,Tv);
  
  G4VPhysicalVolume *detPV = new G4PVPlacement(Tr, detLV,
       detName, motherLV, false, 0,false); // last bool for overlap check

  return detPV;
}


