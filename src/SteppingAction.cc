#include "SteppingAction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"
#include "G4NIELCalculator.hh"
#include "G4ICRU49NuclearStoppingModel.hh"

#include "G4LindhardPartition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "qmath.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* event ,PrimaryGeneratorAction* primary)
    :G4UserSteppingAction(), fEventAction(event),fPrimary(primary)
{
    //???
    fNIELCalculator = new G4NIELCalculator(new G4ICRU49NuclearStoppingModel(), 1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    //Get current Run
    Run* run = static_cast<Run*>
        (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    //Get current AnalysisManager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    //Get PostStep position
    G4ThreeVector poststepP = aStep->GetPostStepPoint()->GetPosition();
    //Get PreStep position
    G4ThreeVector prestepP = aStep->GetPreStepPoint()->GetPosition();
    //Get Particle Gun Position
    G4ThreeVector vertex = fPrimary->GetParticleGun()->GetParticlePosition();
    //Get Deposited Energy of current step
    G4double EdepStep = aStep->GetTotalEnergyDeposit();
    if (EdepStep > 0.) {
        run->AddEdep(EdepStep);
        fEventAction->AddEdep(EdepStep);
    }
    analysisManager->FillH1(5, (poststepP - vertex).x(), EdepStep);//Deposit Energy Distribution
    //get NIEL of current step
    G4int ns = aStep->GetTrack()->GetCurrentStepNumber();
    G4double niel_calculator = fNIELCalculator->ComputeNIEL(aStep);
   //G4cout << "The NIEL of step "<<ns<<" compute by NIELCalculator is " << G4BestUnit(niel_calculator,"Energy") << G4endl;
    G4double niel_step = aStep->GetNonIonizingEnergyDeposit();
   //G4cout << "The NIEL of step "<<ns<<" compute by GetNonIonizingEnergyDeposit() is " <<G4BestUnit(niel_step,"Energy") << G4endl;
    if (niel_calculator > 0.) {
        run->AddNIEL_calculator(niel_calculator);
        fEventAction->AddNIEL_calculator(niel_calculator);
    }
    if (niel_step > 0.) {
        run->AddNIEL_step(niel_step);
        fEventAction->AddNIEL_step(niel_step);
    }
    analysisManager->FillH1(7, (poststepP - vertex).x(), niel_calculator);
    //Delta Energy of a step
    G4double deltaEnergy = aStep->GetDeltaEnergy();
    analysisManager->FillH1(6, (poststepP - vertex).x(), deltaEnergy);
    //get process of current step
    const G4VProcess* process =
        aStep->GetPostStepPoint()->GetProcessDefinedStep();
    if (process)
    {
        //count total process
        run->CountProcesses(process->GetProcessName());
        //count primary processes
        if (aStep->GetTrack()->GetParentID() == 0)
        {
            run->CountPrimProcesses(process->GetProcessName());
        }
        //count secondary processes
        else if (aStep->GetTrack()->GetParentID() == 1)
        {
            run->CountSeconProcesses(process->GetProcessName());
        }
        // Count Recoils
        if (process->GetProcessName() == "ScreenedElastic")
        {
            //G4cout << "There is a Screened Elastic Process" << G4endl;
            G4int NumOfSecondaries=aStep->GetNumberOfSecondariesInCurrentStep();
            run->AddSecondaries(NumOfSecondaries);
            if (NumOfSecondaries)
            {
                G4double recoilX = poststepP.x()-vertex.x();
                //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(2, recoilX);//PKA Distribution
            }
        }
    }
    // step length of primary particle
    G4int ID = aStep->GetTrack()->GetTrackID();
    G4double steplen = aStep->GetStepLength();
    if (ID == 1) analysisManager->FillH1(8, steplen);//Step Length of Primary

}
