#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class StepMax;
class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList : public G4VModularPhysicsList
{
public:

    PhysicsList();
    ~PhysicsList();

    virtual void ConstructParticle();

    void AddPhysicsList(const G4String& name);
    virtual void ConstructProcess();
    void AddIonGasModels();
   

    void AddStepMax();
    StepMax* GetStepMaxProcess() { return fStepMaxProcess; };

private:

    G4bool   fHelIsRegisted;
    G4bool   fBicIsRegisted;
    G4bool   fBiciIsRegisted;

    G4String                            fEmName;
    G4VPhysicsConstructor*              fEmPhysicsList;
    G4VPhysicsConstructor*              fDecPhysicsList;
    std::vector<G4VPhysicsConstructor*> fHadronPhys;
    StepMax*                            fStepMaxProcess;

    PhysicsListMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif