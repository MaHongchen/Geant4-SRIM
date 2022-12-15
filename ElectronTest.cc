#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

#include "G4Types.hh"
#include "G4SteppingVerbose.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"





int main(int argc, char** argv)
{
	//detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;
	if (argc == 1) ui = new G4UIExecutive(argc, argv);

	//choose the Random engine
	//G4Random::setTheEngine(new CLHEP::RanecuEngine);

	//Use SteppingVerbose with Unit
	G4int precision = 4;
	G4SteppingVerbose::UseBestUnit(precision);

	//Creating run manager
	//auto runManager = G4RunManagerFactory::CreateRunManager();
	//Creating serial run manager for Debug test
	auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);

	if (argc == 3) {
		G4int nThreads = G4UIcommand::ConvertToInt(argv[2]);
		runManager->SetNumberOfThreads(nThreads);
	}

	//set mandatory initialization classes
	DetectorConstruction* det = new DetectorConstruction;
	runManager->SetUserInitialization(det);

	auto* phys = new PhysicsList();
	runManager->SetUserInitialization(phys);

	runManager->SetUserInitialization(new ActionInitialization(det));

	//initialize visualization
	G4VisManager* visManager = nullptr;

	//get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (ui) {
		//interactive mode
		visManager = new G4VisExecutive;
		visManager->Initialize();
		 G4String command1 = "/control/execute ";
		 G4String MacroFile = "vis.mac";
		 UImanager->ApplyCommand(command1 + MacroFile);
		ui->SessionStart();
		delete ui;
	}
	else {
		//batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}

	//job termination
	delete visManager;
	delete runManager;
}