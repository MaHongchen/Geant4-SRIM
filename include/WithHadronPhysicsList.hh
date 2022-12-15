#ifndef WithHadronPhysicsList_h
#define WithHadronPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class WithHadronPhysicsList : public G4VModularPhysicsList
{
public:
	explicit WithHadronPhysicsList(G4int ver = 1, const G4String& type = "WithHadronPhysicsList");
	virtual ~WithHadronPhysicsList() = default;
	virtual void ConstrucParticle();

	//copy constructor and hide assignment operator
	WithHadronPhysicsList(const WithHadronPhysicsList&) = delete;
	WithHadronPhysicsList & operator=(const WithHadronPhysicsList& right) = delete;
};




#endif