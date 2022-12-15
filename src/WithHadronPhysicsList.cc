#include "WithHadronPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "PhysListEmStandardNR.hh"
#include "PhysListEmStandard.hh"

#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"

#include "G4NuclearLevelData.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"



WithHadronPhysicsList::WithHadronPhysicsList(G4int ver, const G4String&)
{
	if (ver > 0)
		G4cout << "<<< Reference Physics List WithHadronPhysicsList " << G4endl;

	SetDefaultCutValue(1.0*nm);
	SetVerboseLevel(ver);


	//EM Physics
	RegisterPhysics(new PhysListEmStandard());

	// Decays
	RegisterPhysics(new G4DecayPhysics(ver));

	// Hadron Physics
	RegisterPhysics(new G4HadronElasticPhysicsXS(ver));
	RegisterPhysics(new G4HadronElasticPhysics(ver));

	RegisterPhysics(new G4StoppingPhysics(ver));

	RegisterPhysics(new G4IonPhysicsXS(ver));

	RegisterPhysics(new G4IonElasticPhysics(ver));

	RegisterPhysics(new G4HadronInelasticQBBC(ver));

}

void WithHadronPhysicsList::ConstrucParticle()
{
	
		G4BosonConstructor  pBosonConstructor;
		pBosonConstructor.ConstructParticle();

		G4LeptonConstructor pLeptonConstructor;
		pLeptonConstructor.ConstructParticle();

		G4MesonConstructor pMesonConstructor;
		pMesonConstructor.ConstructParticle();

		G4BaryonConstructor pBaryonConstructor;
		pBaryonConstructor.ConstructParticle();

		G4IonConstructor pIonConstructor;
		pIonConstructor.ConstructParticle();

		G4ShortLivedConstructor pShortLivedConstructor;
		pShortLivedConstructor.ConstructParticle();
}
