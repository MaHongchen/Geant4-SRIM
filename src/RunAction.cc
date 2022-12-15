#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(const DetectorConstruction* det,
    PrimaryGeneratorAction* kin)
    : G4UserRunAction(), fDetector(det), fPrimary(kin), fRun(nullptr)
{
    fHistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{
    fRun = new Run(fDetector);
    return fRun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
    // show Rndm status
    if (isMaster)  G4Random::showEngineStatus();

    // keep run condition
    if (fPrimary) {
        G4ParticleDefinition* particle
            = fPrimary->GetParticleGun()->GetParticleDefinition();
        G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
        fRun->SetPrimary(particle, energy);
    }

    //histograms
    //
    //fHistoManager->Book();
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
    // compute and print statistic //Only the master run print statistic!
    if (isMaster) fRun->EndOfRun();

    //save histograms      
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    // show Rndm status
    if (isMaster)  G4Random::showEngineStatus();
}