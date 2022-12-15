#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class Run;
class DetectorConstruction;
class PrimaryGeneratorAction;
class HistoManager;

class RunAction : public G4UserRunAction
{
public:
    RunAction(const DetectorConstruction* det,
        PrimaryGeneratorAction* prim = nullptr);
    ~RunAction() override;

    G4Run* GenerateRun() override;
    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

private:
    const DetectorConstruction* fDetector;
    PrimaryGeneratorAction* fPrimary;
    Run* fRun;
    HistoManager* fHistoManager;
};

#endif
