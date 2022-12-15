#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
    :G4VUserPrimaryGeneratorAction(),
    fDetector(det), fRndmBeam(0.), fGunMessenger(0)
{
    fParticleGun = new G4ParticleGun(1);
    SetDefaultKinematic();

    //create a messenger for this class
    fGunMessenger = new PrimaryGeneratorMessenger(this);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fGunMessenger;
}

void PrimaryGeneratorAction::SetDefaultKinematic()
{
    G4ParticleDefinition* particle
        = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(10 * keV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
    G4double position = -0.5 * (fDetector->GetFilmSizeX());
    fParticleGun->SetParticlePosition(G4ThreeVector(position, 0. * cm, 0. * cm));
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //this function is called at the begining of event

    //randomize the beam, if requested.
    //
    if (fRndmBeam > 0.)
    {
        G4ThreeVector oldPosition = fParticleGun->GetParticlePosition();
        if (fRndmBeam > fDetector->GetFilmSizeYZ())
            fRndmBeam = fDetector->GetFilmSizeYZ();
        G4double rbeam = 0.5 * fRndmBeam;
        G4double x0 = oldPosition.x();
        G4double y0 = oldPosition.y() + (2 * G4UniformRand() - 1.) * rbeam;
        G4double z0 = oldPosition.z() + (2 * G4UniformRand() - 1.) * rbeam;
        fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
        fParticleGun->GeneratePrimaryVertex(anEvent);
        fParticleGun->SetParticlePosition(oldPosition);
    }

    else fParticleGun->GeneratePrimaryVertex(anEvent);
}