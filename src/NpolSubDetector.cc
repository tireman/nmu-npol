//%% NpolSubDetector.cc %% **

// Implements methods common to all objects in the setup
// Created: Daniel Wilbern December 2014

#include "G4LogicalVolume.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"

#include "NpolSubDetector.hh"

// Place a volume in its mother volume at a location specified by rectangular coordinates.
G4VPhysicalVolume *NpolSubDetector::PlaceRectangular(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,
		G4String detName, G4double x, G4double y, G4double z,
		G4double Rx, G4double Ry, G4double Rz) {

	G4RotationMatrix Rm; 
	G4ThreeVector Tv;
	G4Transform3D Tr;

	Rm.rotateX(Rx);
	Rm.rotateY(Ry);
	Rm.rotateZ(Rz);

	Tv.setX(x); Tv.setY(y); Tv.setZ(z);
	Tr = G4Transform3D(Rm,Tv);

	G4VPhysicalVolume *detPV = new G4PVPlacement(Tr, detLV,
			detName, motherLV, false, 0);

	return detPV;
}

// Place a volume in its mother volume at a location specified by cylindrical polar coordinates.
// The polar axis is the positive Z axis.
G4VPhysicalVolume *NpolSubDetector::PlaceCylindrical(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,
		G4String detName, G4double rho, G4double phi, G4double z) {

	G4RotationMatrix Rm;
	G4ThreeVector Tv;
	G4Transform3D Tr;

	Rm.rotateY(phi);
	Tv.setX(rho*sin(phi)); Tv.setY(z); Tv.setZ(rho*cos(phi));
	Tr = G4Transform3D(Rm,Tv);

	G4VPhysicalVolume *detPV = new G4PVPlacement(Tr, detLV,
			detName, motherLV, false, 0);

	return detPV;
}
