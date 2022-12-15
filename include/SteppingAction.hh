#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "PrimaryGeneratorAction.hh"

class EventAction;
class G4NIELCalculator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction*,PrimaryGeneratorAction*);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step*) override;
private:
    EventAction* fEventAction;
    G4NIELCalculator* fNIELCalculator;
    PrimaryGeneratorAction* fPrimary;
};

#endif