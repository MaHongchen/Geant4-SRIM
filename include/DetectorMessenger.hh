#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class DetectorMessenger : public G4UImessenger
{
public:

    DetectorMessenger(DetectorConstruction*);
    ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:

    DetectorConstruction* fDetector;

    G4UIdirectory* fTestemDir;
    G4UIdirectory* fDetDir;
    G4UIcmdWithAnInteger* fNbFilmCmd;
    G4UIcommand* fFilmCmd;
    G4UIcommand* fNdivCmd;
    G4UIcmdWithADoubleAndUnit* fSizeYZCmd;
};









#endif // !DetectorMessenger_h
