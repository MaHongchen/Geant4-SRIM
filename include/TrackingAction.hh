#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "DetectorConstruction.hh"
class PrimaryGeneratorAction;
class DetectorConstruction;

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction(PrimaryGeneratorAction*,DetectorConstruction*);
    ~TrackingAction() {};

    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

private:
    PrimaryGeneratorAction* fPrimary;
    DetectorConstruction* fdetector;

};


#endif
