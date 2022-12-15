#include "PhysListEmStandardNR.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4LossTableManager.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4eSingleCoulombScatteringModel.hh"
#include "G4UrbanMscModel.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "G4Generator2BS.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eplusAnnihilation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

#include "G4ScreenedNuclearRecoil.hh"
#include "G4NuclearStopping.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmParameters.hh"
#include "StepMax.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandardNR::PhysListEmStandardNR(const G4String& name)
	: G4VPhysicsConstructor(name)
{
	G4EmParameters* param = G4EmParameters::Instance();
	param->SetDefaults();
	param->SetStepFunction(0.02, 10* nm);
	param->SetStepFunctionMuHad(0.02, 10 * nm);
	param->SetStepFunctionLightIons(0.01, 10 * nm);
	param->SetStepFunctionIons(0.01, 10 * nm);
	param->SetFluo(true);
    param->SetMinEnergy(0.1 * eV);
	SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandardNR::~PhysListEmStandardNR()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmStandardNR::ConstructProcess()
{
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // muon & hadron bremsstrahlung and pair production
    G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
    G4MuPairProduction* mup = new G4MuPairProduction();

    G4ScreenedNuclearRecoil* nucr = new G4ScreenedNuclearRecoil("ScreenedElastic", "zbl", true, 0.1269 * eV, 10* eV);
    G4double energyLimit = 100. * MeV;
    nucr->SetMFPScaling(0.020);
    //nucr->SetCrossSectionHardening(1, 100);
    nucr->SetMaxEnergyForScattering(energyLimit);
    G4eCoulombScatteringModel* csm = new G4eCoulombScatteringModel();
    csm->SetActivationLowEnergyLimit(energyLimit);

    auto particleIterator = GetParticleIterator();
    particleIterator->reset();
    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {

            // Compton scattering
            G4ComptonScattering* cs = new G4ComptonScattering;
            cs->SetEmModel(new G4KleinNishinaModel());
            ph->RegisterProcess(cs, particle);

            // Photoelectric
            G4PhotoElectricEffect* pe = new G4PhotoElectricEffect();
            G4VEmModel* theLivermorePEModel = new G4LivermorePhotoElectricModel();
            theLivermorePEModel->SetHighEnergyLimit(10 * GeV);
            pe->SetEmModel(theLivermorePEModel);
            ph->RegisterProcess(pe, particle);

            // Gamma conversion
            G4GammaConversion* gc = new G4GammaConversion();
            G4VEmModel* thePenelopeGCModel = new G4PenelopeGammaConversionModel();
            thePenelopeGCModel->SetHighEnergyLimit(1 * GeV);
            gc->SetEmModel(thePenelopeGCModel);
            ph->RegisterProcess(gc, particle);

            // Rayleigh scattering
            ph->RegisterProcess(new G4RayleighScattering(), particle);

        }
        else if (particleName == "e-") {

            // ionisation
            G4eIonisation* eIoni = new G4eIonisation();
            ph->RegisterProcess(eIoni, particle);

            // bremsstrahlung
            G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
            ph->RegisterProcess(eBrem, particle);

            //Multiple Scattering
            ph->RegisterProcess(new G4eMultipleScattering(), particle);

            //Electron-Nuclear Coulomb Scattering
            G4CoulombScattering* cs = new G4CoulombScattering();
            G4eSingleCoulombScatteringModel* ecsModel = new G4eSingleCoulombScatteringModel();
            ecsModel->SetLowEnergyLimit(1 * MeV);
            ecsModel->SetRecoilThreshold(34 * eV);
            cs->SetEmModel(ecsModel);
            ph->RegisterProcess(cs, particle);
            
        }
        else if (particleName == "e+") {
            // ionisation
            G4eIonisation* eIoni = new G4eIonisation();

            // bremsstrahlung
            G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();

            ph->RegisterProcess(new G4eMultipleScattering(), particle);
            ph->RegisterProcess(eIoni, particle);
            ph->RegisterProcess(eBrem, particle);

            // annihilation at rest and in flight
            ph->RegisterProcess(new G4eplusAnnihilation(), particle);

        }
        else if (particleName == "mu+" ||
            particleName == "mu-") {

            G4MuIonisation* muIoni = new G4MuIonisation();

            ph->RegisterProcess(muIoni, particle);
            ph->RegisterProcess(mub, particle);
            ph->RegisterProcess(mup, particle);
            ph->RegisterProcess(new G4CoulombScattering(), particle);

        }
        else if (particleName == "alpha" || particleName == "He3") {

            G4hMultipleScattering* msc = new G4hMultipleScattering();
            G4UrbanMscModel* model = new G4UrbanMscModel();
            model->SetActivationLowEnergyLimit(energyLimit);
            msc->SetEmModel(model);
            ph->RegisterProcess(msc, particle);

            G4ionIonisation* ionIoni = new G4ionIonisation();
            ph->RegisterProcess(ionIoni, particle);

            ph->RegisterProcess(nucr, particle);

        }
        else if (particleName == "GenericIon") {

            G4hMultipleScattering* msc = new G4hMultipleScattering();
            G4UrbanMscModel* model = new G4UrbanMscModel();
            model->SetActivationLowEnergyLimit(energyLimit);
            msc->SetEmModel(model);
            ph->RegisterProcess(msc, particle);

            G4ionIonisation* ionIoni = new G4ionIonisation();
            //ionIoni->SetEmModel(new G4IonParametrisedLossModel());
            ph->RegisterProcess(ionIoni, particle);
            G4ScreenedNuclearRecoil* nucr1 = new G4ScreenedNuclearRecoil("ScreenedElastic", "zbl", true, 0.1269 * eV, 1 * eV);
            nucr1->SetMaxEnergyForScattering(energyLimit);
            nucr1->BuildPhysicsTable(*particle);
            //nucr1->SetMFPScaling(10);
            nucr1->AvoidNuclearReactions(false);
            ph->RegisterProcess(nucr1, particle);
            /*StepMax* stepMax = new StepMax();
            if (stepMax->IsApplicable(*particle)) {
                stepMax->SetMaxStep(0.3 * nm);
                ph->RegisterProcess(stepMax, particle);
            }*/

        }
        else if (particleName == "proton" ||
            particleName == "deuteron" ||
            particleName == "triton") {

            G4hMultipleScattering* msc = new G4hMultipleScattering();
            G4UrbanMscModel* model = new G4UrbanMscModel();
            model->SetActivationLowEnergyLimit(energyLimit);
            msc->SetEmModel(model);
            ph->RegisterProcess(msc, particle);

            G4hIonisation* hIoni = new G4hIonisation();
            ph->RegisterProcess(hIoni, particle);
            ph->RegisterProcess(nucr, particle);
            //ph->RegisterProcess(new G4NuclearStopping(), particle);
        }
        else if ((!particle->IsShortLived()) &&
            (particle->GetPDGCharge() != 0.0) &&
            (particle->GetParticleName() != "chargedgeantino")) {
            //all others charged particles except geantino

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4hIonisation(), particle);
        }
    }

    // Deexcitation
    G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}
