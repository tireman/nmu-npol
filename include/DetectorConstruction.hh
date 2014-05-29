

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: DetectorConstruction.hh,v 1.10 2008/09/22 16:41:20 maire Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "NMUSensitiveDetector.hh"
//class G4AssemblyVolume;
class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class NMUSensitiveDetector;
//class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
  virtual ~DetectorConstruction();

public:
  
  virtual G4VPhysicalVolume* Construct();
  
  
private:
  
  //G4Box*             solidWorld;
  //G4LogicalVolume*   logicWorld;
  //G4VPhysicalVolume* physiWorld;
  
  //G4VSolid  *TopDet, *TopVeto, *BottomDet, *BottomVeto, *FrontDet, *FrontTag;
  //G4LogicalVolume *TopDetLV, *TopVetoLV, *BottomDetLV, *BottomVetoLV, *FrontDetLV, *FrontTagLV;
  NMUSensitiveDetector *TopDetSD, *TopVetoSD, *BottomDetSD, *BottomVetoSD, *FrontDetSD, *FrontTagSD;
  //G4AssemblyVolume *TopArray, *TopVetoArray, *BottomArray, *BottomVetoArray, *FrontArray1, *FrontArray2, *FrontTagger1, *FrontTagger2;

};

#endif
