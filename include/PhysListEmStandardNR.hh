#ifndef PhysListEmStandardNR_h
#define PhysListEmStandardNR_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysListEmStandardNR : public G4VPhysicsConstructor
{
public:
	PhysListEmStandardNR(const G4String& name = "standardNR");
	~PhysListEmStandardNR();

public:
	// This method is dummy for physics
	virtual void ConstructParticle() {};

	// This method will be invoked in the Construct() method.
	// each physics process will be instantiated and
	// registered to the process manager of each particle type 
	virtual void ConstructProcess();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif