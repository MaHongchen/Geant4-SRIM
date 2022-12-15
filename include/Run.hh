#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "globals.hh"
#include <map>

class DetectorConstruction;
class G4ParticleDefinition;

class Run : public G4Run
{
public:
    Run(const DetectorConstruction*);
    ~Run() override;

    void SetPrimary(const G4ParticleDefinition* particle, G4double energy);

    //Count Neutral Track Number
    void CountTraks0(G4int nt) { fNbOfTraks0 += nt; }

    //Count Charged Track Number
    void CountTraks1(G4int nt) { fNbOfTraks1 += nt; }

    //Count Neutral Step Number
    void CountSteps0(G4int ns) { fNbOfSteps0 += ns; }

    //Count Charged Step Number
    void CountSteps1(G4int ns) { fNbOfSteps1 += ns; }


    void CountProcesses(const G4String& procName);
    void CountPrimProcesses(const G4String& proName);
    void CountSeconProcesses(const G4String& proName);

    void AddEdep(G4double val) { fEdep += val; }
    void AddNIEL_calculator(G4double val) { fNIEL_calculator += val; }
    void AddNIEL_step(G4double val) { fNIEL_step += val; }

    //True Range
    void AddTrueRange(G4double l) { fTrueRange += l; fTrueRange2 += l * l; }

    //Projected Range
    void AddProjRange(G4double x) { fProjRange += x; fProjRange2 += x * x; }

    //Transverse Range
    void AddTransvDev(G4double y) { fTransvDev += y; fTransvDev2 += y * y; }

    //Secondaries Counts
    void AddSecondaries(G4int n) { NumOfSecondaries = NumOfSecondaries + n; }

    void AddAbs() { fAbsnumber = fAbsnumber + 1; }
    void AddBS() { fBSnumber = fBSnumber + 1; }
    void AddTrans() { fTransnumber = fTransnumber + 1; }

    void AddNdis(G4double val) { fNdis = fNdis + val; }

    void Merge(const G4Run*) override;
    void EndOfRun();

private:
    const DetectorConstruction* fDetector;
    const G4ParticleDefinition* fParticle;
    G4double  fEkin;

    G4int           fNbOfTraks0, fNbOfTraks1;
    G4int           fNbOfSteps0, fNbOfSteps1;
    G4double        fEdep, fNIEL_calculator,fNIEL_step;
    G4double        fTrueRange, fTrueRange2;
    G4double        fProjRange, fProjRange2;
    G4double        fTransvDev, fTransvDev2;
    std::map<G4String, G4int>    fProcCounter;
    std::map<G4String, G4int>    fPrimProcCounter;
    std::map<G4String, G4int>    fSeconProcCounter;
    G4int           NumOfSecondaries;
    G4int           fAbsnumber;//stop in films
    G4int           fBSnumber;
    G4int           fTransnumber;
    G4double           fNdis;


};




#endif