//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu

#ifndef Npol_ActionInitialization_h
#define Npol_ActionInitialization_h

#include "G4VUserActionInitialization.hh"

class NpolActionInitialization : public G4VUserActionInitialization {

	public:
		NpolActionInitialization();
		virtual ~NpolActionInitialization();

		virtual void BuildForMaster() const;
		virtual void Build() const;
};

#endif

