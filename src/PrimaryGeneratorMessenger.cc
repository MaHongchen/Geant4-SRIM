#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
    PrimaryGeneratorAction* Gun)
    :G4UImessenger(), fAction(Gun), fGunDir(0), fDefaultCmd(0), fRndmCmd(0)
{
    fGunDir = new G4UIdirectory("/testem/gun/");
    fGunDir->SetGuidance("gun control");

    fDefaultCmd = new G4UIcmdWithoutParameter("/testem/gun/setDefault", this);
    fDefaultCmd->SetGuidance("set/reset kinematic defined in PrimaryGenerator");
    fDefaultCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fRndmCmd = new G4UIcmdWithADoubleAndUnit("/testem/gun/rndm", this);
    fRndmCmd->SetGuidance("random lateral extension on the beam");
    fRndmCmd->SetParameterName("rBeam", false);
    fRndmCmd->SetRange("rBeam>=0.");
    fRndmCmd->SetUnitCategory("Length");
    fRndmCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete fDefaultCmd;
    delete fRndmCmd;
    delete fGunDir;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
    G4String newValue)
{
    if (command == fDefaultCmd)
    {
        fAction->SetDefaultKinematic();
    }

    if (command == fRndmCmd)
    {
        fAction->SetRndmBeam(fRndmCmd->GetNewDoubleValue(newValue));
    }
}