#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
	: G4VUserActionInitialization(), fDetector(det)
{ }

ActionInitialization::~ActionInitialization()
{ }

void ActionInitialization::BuildForMaster() const
{
	SetUserAction(new RunAction(fDetector));
}

void ActionInitialization::Build() const
{

	PrimaryGeneratorAction* prim = new PrimaryGeneratorAction(fDetector);
	SetUserAction(prim);

	RunAction* run = new RunAction(fDetector, prim);
	SetUserAction(run);

	EventAction* event = new EventAction();
	SetUserAction(event);

	SetUserAction(new TrackingAction(prim,fDetector));

	SetUserAction(new SteppingAction(event,prim));

	SetUserAction(new StackingAction(prim));
}
