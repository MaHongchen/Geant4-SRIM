#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmCalculator.hh"

#include <iomanip>

Run::Run(const DetectorConstruction* det)
	: G4Run(),
	fDetector(det),
	fParticle(0), fEkin(0.),
	fNbOfTraks0(0), fNbOfTraks1(0),
	fNbOfSteps0(0), fNbOfSteps1(0),
	fEdep(0.), fNIEL_calculator(0.),fNIEL_step(0),
	fTrueRange(0.), fTrueRange2(0.),
	fProjRange(0.), fProjRange2(0.),
	fTransvDev(0.), fTransvDev2(0.),
	NumOfSecondaries(0),fAbsnumber(0),
	fBSnumber(0),fTransnumber(0),fNdis(0)
{ }

Run::~Run()
{ }

void Run::SetPrimary(const G4ParticleDefinition* particle, G4double energy)
{
	fParticle = particle;
	fEkin = energy;
}

void Run::CountProcesses(const G4String& procName)
{
	std::map<G4String, G4int>::iterator it = fProcCounter.find(procName);
	if (it == fProcCounter.end()) {	
		fProcCounter[procName] = 1;	
	}
	else {
		fProcCounter[procName]++;	
	}
}

void Run::CountPrimProcesses(const G4String& procName)
{
	std::map<G4String, G4int>::iterator it = fPrimProcCounter.find(procName);
	if (it == fPrimProcCounter.end()) {
		fPrimProcCounter[procName] = 1;
	}
	else {
		fPrimProcCounter[procName]++;
	}
}

void Run::CountSeconProcesses(const G4String& procName)
{
	std::map<G4String, G4int>::iterator it = fSeconProcCounter.find(procName);
	if (it == fSeconProcCounter.end()) {
		fSeconProcCounter[procName] = 1;
	}
	else {
		fSeconProcCounter[procName]++;
	}
}

void Run::Merge(const G4Run* run)
{
	const Run* localRun = static_cast<const Run*>(run);

	// pass information about primary particle
	fParticle = localRun->fParticle;
	fEkin = localRun->fEkin;

	// accumulate sums
	//
	fNbOfTraks0 += localRun->fNbOfTraks0;
	fNbOfTraks1 += localRun->fNbOfTraks1;
	fNbOfSteps0 += localRun->fNbOfSteps0;
	fNbOfSteps1 += localRun->fNbOfSteps1;
	fEdep += localRun->fEdep;
	fNIEL_calculator += localRun->fNIEL_calculator;
	fNIEL_step += localRun->fNIEL_step;
	fTrueRange += localRun->fTrueRange;
	fTrueRange2 += localRun->fTrueRange2;
	fProjRange += localRun->fProjRange;
	fProjRange2 += localRun->fProjRange2;
	fTransvDev += localRun->fTransvDev;
	fTransvDev2 += localRun->fTransvDev2;
	NumOfSecondaries += localRun->NumOfSecondaries;
	fAbsnumber += localRun->fAbsnumber;
	fBSnumber += localRun->fBSnumber;
	fTransnumber += localRun->fTransnumber;
	fNdis += localRun->fNdis;
	//map: processes count
	std::map<G4String, G4int>::const_iterator it;
	for (it = localRun->fProcCounter.begin();
		it != localRun->fProcCounter.end(); ++it) {

		G4String procName = it->first;
		G4int localCount = it->second;
		if (fProcCounter.find(procName) == fProcCounter.end()) {
			fProcCounter[procName] = localCount;
		}
		else {
			fProcCounter[procName] += localCount;
		}
	}

	//map: Primary processes count
	std::map<G4String, G4int>::const_iterator Pit;
	for (Pit = localRun->fPrimProcCounter.begin();
		Pit != localRun->fPrimProcCounter.end(); ++Pit) {

		G4String procName = Pit->first;
		G4int localCount = Pit->second;
		if (fPrimProcCounter.find(procName) == fPrimProcCounter.end()) {
			fPrimProcCounter[procName] = localCount;
		}
		else {
			fPrimProcCounter[procName] += localCount;
		}
	}

	//map: Secondary processes count
	std::map<G4String, G4int>::const_iterator Sit;
	for (Sit = localRun->fSeconProcCounter.begin();
		Sit != localRun->fSeconProcCounter.end(); ++Sit) {

		G4String procName = Sit->first;
		G4int localCount = Sit->second;
		if (fSeconProcCounter.find(procName) == fSeconProcCounter.end()) {
			fSeconProcCounter[procName] = localCount;
		}
		else {
			fSeconProcCounter[procName] += localCount;
		}
	}

	G4Run::Merge(run);
}

void Run::EndOfRun()
{
	G4int prec = 5, wid = prec + 2;
	G4int dfprec = G4cout.precision(prec);

	//run condition
	//        
	G4String partName = fParticle->GetParticleName();
	G4Material* fFilmMaterial1 = fDetector->GetFilmMaterial(1);
	G4double density1 = fFilmMaterial1->GetDensity();
	G4Material* fFilmMaterial2 = fDetector->GetFilmMaterial(2);
	G4double density2 = fFilmMaterial2->GetDensity();
	G4Material* fFilmMaterial3 = fDetector->GetFilmMaterial(3);
	G4double density3 = fFilmMaterial3->GetDensity();

	G4cout << "\n ======================== run summary ======================\n";
	G4cout << "\n The run is: " << numberOfEvent << " " << partName << " of "
		<< G4BestUnit(fEkin, "Energy") << " through "

		<< G4BestUnit(fDetector->GetFilmThickness(1), "Length") << " of "
		<< fFilmMaterial1->GetName() << " (density: "
		<< G4BestUnit(density1, "Volumic Mass") << ")" <<" and "

		<< G4BestUnit(fDetector->GetFilmThickness(2), "Length") << " of "
		<< fFilmMaterial2->GetName() << " (density: "
		<< G4BestUnit(density2, "Volumic Mass") << ")" <<" and "

		<< G4BestUnit(fDetector->GetFilmThickness(3), "Length") << " of "
		<< fFilmMaterial3->GetName() << " (density: "
		<< G4BestUnit(density3, "Volumic Mass") << ")" << G4endl;

	if (numberOfEvent == 0) { G4cout.precision(dfprec);   return; }

	
	G4double dNbOfEvents = (G4double)numberOfEvent;
	G4cout << "\n Total energy deposited per event:   "
		<< G4BestUnit(fEdep / dNbOfEvents, "Energy") << G4endl;
	G4cout << " NIEL energy by NIELcalculator per  event: "
		<< G4BestUnit(fNIEL_calculator / dNbOfEvents, "Energy") << G4endl;
	G4cout << " NIEL energy by NIEDeposit() per  event: "
		<< G4BestUnit(fNIEL_step / dNbOfEvents, "Energy") << G4endl;

	//nb of tracks and steps per event
	//           
	G4cout << "\n Nb tracks/event"
		<< "   neutral: " << std::setw(wid) << fNbOfTraks0 / dNbOfEvents
		<< "   charged: " << std::setw(wid) << fNbOfTraks1 / dNbOfEvents
		<< "\n Nb  steps/event"
		<< "   neutral: " << std::setw(wid) << fNbOfSteps0 / dNbOfEvents
		<< "   charged: " << std::setw(wid) << fNbOfSteps1 / dNbOfEvents
		<< G4endl;

	//frequency of processes
	//
	G4cout << "\n Total Process calls frequency :" << G4endl;
	G4int index = 0;
	std::map<G4String, G4int>::iterator it;
	for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
		G4String procName = it->first;
		G4int    count = it->second;
		G4String space = " "; if (++index % 3 == 0) space = "\n";
		G4cout << " " << std::setw(20) << procName << "=" << std::setw(7) << count
			<< space;
	}
	G4cout << G4endl;

	G4cout << "\n Primary Process calls frequency :" << G4endl;
	G4int Pindex = 0;
	std::map<G4String, G4int>::iterator Pit;
	for (Pit = fPrimProcCounter.begin(); Pit != fPrimProcCounter.end(); Pit++) {
		G4String procName = Pit->first;
		G4int    count = Pit->second;
		G4String space = " "; if (++Pindex % 3 == 0) space = "\n";
		G4cout << " " << std::setw(20) << procName << "=" << std::setw(7) << count
			<< space;
	}
	G4cout << G4endl;

	G4cout << "\n Secondary Process calls frequency :" << G4endl;
	G4int Sindex = 0;
	std::map<G4String, G4int>::iterator Sit;
	for (Sit = fSeconProcCounter.begin(); Sit != fSeconProcCounter.end(); Sit++) {
		G4String procName = Sit->first;
		G4int    count = Sit->second;
		G4String space = " "; if (++Sindex % 3 == 0) space = "\n";
		G4cout << " " << std::setw(20) << procName << "=" << std::setw(7) << count
			<< space;
	}
	G4cout << G4endl;

	//BS,Abs,Trans number
	G4cout << "\n The BSnumber is :" << fBSnumber << G4endl;
	G4cout << " The Absnumber is :" << fAbsnumber << G4endl;
	G4cout << " The Transnumber is :" << fTransnumber << G4endl;
	G4cout << " The Total number is :" << fBSnumber+fAbsnumber+fTransnumber << G4endl;

	//number of Secondaries 
	G4cout << "\n The number of Primary Recoils is :" << NumOfSecondaries << G4endl;
	G4cout << " The average Primary Recoil number is :" << NumOfSecondaries / dNbOfEvents << G4endl;

	G4cout << "The number of Secondary Recoils is :" << fNdis << G4endl;
	G4cout << "The average Secondary Recoil number is :" << fNdis / dNbOfEvents << G4endl;

	G4cout << "The total average recoil is :" << NumOfSecondaries / dNbOfEvents + fNdis / dNbOfEvents << G4endl;

	//compute true and projected ranges, and transverse dispersion
	//

	fTrueRange /= fAbsnumber; fTrueRange2 /= fAbsnumber;
	G4double trueRms = fTrueRange2 - fTrueRange * fTrueRange;
	if (trueRms > 0.) trueRms = std::sqrt(trueRms); else trueRms = 0.;

	fProjRange /= fAbsnumber; fProjRange2 /= fAbsnumber;
	G4double projRms = fProjRange2 - fProjRange * fProjRange;
	if (projRms > 0.) projRms = std::sqrt(projRms); else projRms = 0.;

	fTransvDev /= 2 * fAbsnumber; fTransvDev2 /= 2 * fAbsnumber;
	G4double trvsRms = fTransvDev2 - fTransvDev * fTransvDev;
	if (trvsRms > 0.) trvsRms = std::sqrt(trvsRms); else trvsRms = 0.;


	//compare true range with csda range from PhysicsTables
	//
	/*G4EmCalculator emCalculator;
	G4double rangeTable = 0.;
	if (fParticle->GetPDGCharge() != 0.)
		rangeTable = emCalculator.GetCSDARange(fEkin, fParticle, material);*/

	G4cout << "\n---------------------------------------------------------\n";
	G4cout << " Primary particle : ";
	G4cout << "\n true Range = " << G4BestUnit(fTrueRange, "Length")
		<< "   rms = " << G4BestUnit(trueRms, "Length");

	G4cout << "\n proj Range = " << G4BestUnit(fProjRange, "Length")
		<< "   rms = " << G4BestUnit(projRms, "Length");

	G4cout << "\n proj/true  = " << fProjRange / fTrueRange;

	G4cout << "\n transverse dispersion at end = "
		<< G4BestUnit(trvsRms, "Length");

	/*G4cout << "\n      mass true Range from simulation = "
		<< G4BestUnit(fTrueRange * density, "Mass/Surface")
		<< "\n       from PhysicsTable (csda range) = "
		<< G4BestUnit(rangeTable * density, "Mass/Surface");*/
	G4cout << "\n---------------------------------------------------------\n";
	G4cout << G4endl;

	// remove all contents in fProcCounter 
	fProcCounter.clear();

	//restore default format         
	G4cout.precision(dfprec);
}
