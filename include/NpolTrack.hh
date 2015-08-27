//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

class NpolTrack {

	public:
		int trackId;
		int parentId;
		double posX;
		double posY;
		double posZ;
		double momX;
		double momY;
		double momZ;
		double time;
		double energy;
		const char *particle;
		const char *process;
		const char *volume;
};

