#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

HistoManager::HistoManager()
	: fFileName("ElectronTest.root")
{
	Book();
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
	// Create or get analysis manager
	// The choice of analysis technology is done via selection of a namespace
	// in HistoManager.hh
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetDefaultFileType("root");
	analysisManager->SetFileName(fFileName);
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(1);     // start histogram numbering from 1

	//Create 1D Histogram
	analysisManager->CreateH1("proton position", "Range Distribution", 100, 0, 150 * nm); //id=1;
	analysisManager->CreateH1("PKA Distribution", "PKA Distribution", 100, 0, 150 * nm); //id=2;
	analysisManager->CreateH1("PKA Energy Spectrum", "PKA Energy Spectrum", 100, 0, 100 * eV); //id=3;

	analysisManager->CreateH1("Total Recoil Distribution", "Total Recoil Distribution", 100, 0, 150 * nm);
	//id=4, total recoil=PKA+Lindhard-Robinson Partition

	analysisManager->CreateH1("Deposit Energy Distribution", "Deposit Energy Distribution", 100, 0, 150 * nm);//id=5
	analysisManager->CreateH1("Delta Energy Distribution", "Delta Energy Distribution", 100, 0, 150 * nm);//id=6
	analysisManager->CreateH1("NIEL Distribution", "NIEL Distribution", 100, 0, 150 * nm);//id=7;

	analysisManager->CreateH1("Step Length of Primary", "Step Length of Primary", 100, 0, 100 * nm);//id=8
}