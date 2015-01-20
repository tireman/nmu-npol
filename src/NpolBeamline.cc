// %% NpolBeamline.cc %%

// Beam line construction file
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "NpolMaterials.hh"
#include "NpolBeamline.hh"

NpolBeamline::NpolBeamline() {
  G4cout << "Initializing Beamline: Up-stream and Down-Stream" << G4endl;
}

NpolBeamline::~NpolBeamline() {
  G4cout << "Deleting Beamline: Up-stream and Down-Stream" << G4endl;
}

void NpolBeamline::ConstructBeamlineDown(){
  G4double dia = 2.5*cm;
  G4int gnum = 60;
  
  std::vector<G4TwoVector> polygon(gnum);
  for (int i = 0; i< gnum-1; i++) {
    polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i),dia*sin(360.0*deg/gnum*i));
  }
 
  std::vector<G4ExtrudedSolid::ZSection> zsections;
  zsections.push_back(G4ExtrudedSolid::ZSection(0.0*m,(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(+5.44*m,(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(+26.88*m,(0*cm,0*cm),2.0));

  G4ExtrudedSolid *BeamlineDown = new G4ExtrudedSolid("BeamlineDown",polygon,zsections); 
  BeamlineDownLV = new G4LogicalVolume(BeamlineDown,NpolMaterials::GetInstance()->GetSSteel(), "BeamlineDownLV", 0,0,0);
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineDownLV->SetVisAttributes(BeamlineVisAtt);
}

void NpolBeamline::ConstructBeamlineDownInner(){
G4double dia = 1.75*cm;
 G4int gnum =60;
  std::vector<G4TwoVector> polygon(gnum);
  for (int i = 0; i < gnum-1; i++) {
    polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i),dia*sin(360.0*deg/gnum*i));
  }

  std::vector<G4ExtrudedSolid::ZSection> zsections;
  zsections.push_back(G4ExtrudedSolid::ZSection(0.0*m,(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(+5.44*m,(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(+26.88*m,(0*cm,0*cm),2.0));
  G4ExtrudedSolid *BeamlineDownInner = new G4ExtrudedSolid("BeamlineDownInner",polygon,zsections); 
  BeamlineDownInnerLV = new G4LogicalVolume(BeamlineDownInner,NpolMaterials::GetInstance()->GetVacuum(), "BeamlineDownInnerLV", 0,0,0);
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  BeamlineDownInnerLV->SetVisAttributes(BeamlineVisAtt);  
  }



// Construct the Up stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineUpper() {
  G4double len= 9.02*m, inDia = 0.0*cm, outDia = 2.5*cm;
  G4Tubs *BeamlineUpper = new G4Tubs("BeamlineUpper", inDia, outDia, 
       len, 0.0*deg, 360.*deg);
  BeamlineUpperLV = new G4LogicalVolume(BeamlineUpper,
       NpolMaterials::GetInstance()->GetSSteel(),"BeamlineUpperLV",0,0,0);
  G4VisAttributes *UpperVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineUpperLV->SetVisAttributes(UpperVisAtt);
}

// Fill the up stream portion with a vacuum
void NpolBeamline::ConstructBeamlineUpperInner() {
  G4double len= 9.02*m, inDia = 0.0*cm, outDia = 1.75*cm;
  G4Tubs *BeamlineUpperInner = new G4Tubs("BeamlineUpperInner", inDia, outDia, 
      len, 0.0*deg, 360.*deg);
  BeamlineUpperInnerLV = new G4LogicalVolume(BeamlineUpperInner,
      NpolMaterials::GetInstance()->GetVacuum(),"BeamlineUpperInnerLV",0,0,0);
  BeamlineUpperInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

/*// Construct the Down stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineDown() {

  G4double len= 10.750*m, inDia = 0.0*cm, outDia = 4.0*cm;
  G4Cons *BeamlineDown = new G4Cons("BeamlineDown",inDia,outDia,inDia+11.0*cm,
         outDia+11.0*cm, len, 0.0*deg, 360.*deg);
  BeamlineDownLV = new G4LogicalVolume(BeamlineDown,
         NpolMaterials::GetInstance()->GetSSteel(),"BeamlineDownLV",0,0,0);
  G4VisAttributes *DownVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineDownLV->SetVisAttributes(DownVisAtt);
}

// Fill the Down-stream with a vacuum
void NpolBeamline::ConstructBeamlineDownInner() {

  G4double len= 10.750*m, inDia = 0.0*cm, outDia = 3.0*cm;
  G4Cons *BeamlineDownInner = new G4Cons("BeamlineDownInner", 
	inDia, outDia, inDia, outDia+11.0*cm, len, 0.0*deg, 360.*deg);
  BeamlineDownInnerLV = new G4LogicalVolume(BeamlineDownInner,
	NpolMaterials::GetInstance()->GetVacuum(),"BeamlineDownInnerLV",0,0,0);
  BeamlineDownInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  }

// Target Chamber exit beamline; it is a non-tappered piece of stainless steel
void NpolBeamline::ConstructBeamlineExit() {

  G4double len= 2.72*m, inDia = 0.0*cm, outDia = 4.0*cm;
  G4Tubs *BeamlineExit = new G4Tubs("BeamlineExit", inDia, outDia, len, 
	0.0*deg, 360.*deg);
  BeamlineExitLV = new G4LogicalVolume(BeamlineExit, 
        NpolMaterials::GetInstance()->GetSSteel(), "BeamlineExitLV", 0,0,0);
  G4VisAttributes *ExitVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineExitLV->SetVisAttributes(ExitVisAtt);
}
// Target Chamber ext beamline vacuum inner portion: it is a non-tappered piece.
void NpolBeamline::ConstructBeamlineExitInner() {

  G4double len= 2.72*m, inDia = 0.0*cm, outDia = 3.0*cm;
  G4Tubs *BeamlineExitInner = new G4Tubs("BeamlineExitInner", inDia, outDia, 
      len,0.0*deg, 360.*deg);
  BeamlineExitInnerLV = new G4LogicalVolume(BeamlineExitInner, 
      NpolMaterials::GetInstance()->GetVacuum(), "BeamlineExitInnerLV", 0,0,0);
  BeamlineExitInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  }*/

G4VPhysicalVolume *NpolBeamline::Construct(G4LogicalVolume *motherLV) {
  
  ConstructBeamlineUpper();
  ConstructBeamlineDown();
  ConstructBeamlineUpperInner();
  ConstructBeamlineDownInner();
  PlaceCylindrical(BeamlineUpperLV, motherLV, "BeamLineUpper", -9.125*m,0,0);
  PlaceCylindrical(BeamlineUpperInnerLV, BeamlineUpperLV, "BeamLineUpperInner",
      0,0,0);
  return PlaceCylindrical(BeamlineDownLV, motherLV, "BeamLineDown", +0.103*m,0,0);
  return PlaceCylindrical(BeamlineDownInnerLV,BeamlineDownLV,"BeamLineDownInner", 0,0,0);
}

