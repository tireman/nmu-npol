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

