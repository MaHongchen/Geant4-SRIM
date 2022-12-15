#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger : public G4UImessenger
{
public:

    PhysicsListMessenger(PhysicsList*);
    ~PhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:

    PhysicsList* fPhysicsList;

    G4UIdirectory* fPhysDir;
    G4UIcmdWithAString* fListCmd;

};






#endif // !PhysicsListMessenger_h
