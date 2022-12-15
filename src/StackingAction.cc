//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm1/src/StackingAction.cc
/// \brief Implementation of the StackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "StackingAction.hh"
#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "G4LindhardPartition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "Run.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction(PrimaryGeneratorAction* primary)
 : G4UserStackingAction(),
    fPrimary(primary)
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* track)
{  
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //
  //energy spectrum of secondaries(PKA)
  //
  //energy
  G4double energy = track->GetKineticEnergy();
  //charge
  G4double charge = track->GetDefinition()->GetPDGCharge();
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (charge != 0.) analysisManager->FillH1(3, energy);//PKA Energy Spectrum


  //current run
  Run* run = static_cast<Run*>
      (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  //PKA z1
  G4int z1 = track->GetDefinition()->GetAtomicNumber();
  //PKA a1
  G4double a1 = track->GetDefinition()->GetAtomicMass();
  G4LindhardRobinsonPartition* LRPartition = new G4LindhardRobinsonPartition();
  //current material
  G4Material* mat = track->GetTouchable()->GetVolume()->GetLogicalVolume()->GetMaterial();
  // Lindhard-Robinson Partition
  G4double lrpartition = LRPartition->PartitionNIEL(z1, a1, mat, energy);

  //output information , for Debug test
  //G4cout << "This Track has initial kinetic energy " << G4BestUnit(energy, "Energy") << G4endl;
  //G4cout << "The PKA AtomicNumber is " << z1 << G4endl;
  //G4cout << "The PKA AtomicMass is " << G4BestUnit(z1, "Mass") << G4endl;
  //G4cout << "The Material is " << mat->GetName() << G4endl;
  //G4cout << "The Lindhard-Robinson partition is " << lrpartition << G4endl;

  G4double niel = energy * lrpartition;
  G4double Ed = 25 * eV;

  G4double Ndis = 0.;
  if (niel <= Ed)
  {
      G4double Ndis = 0.;
      run->AddNdis(Ndis);
  }
  else if (niel > Ed && niel <= 2 * Ed / 0.8)
  {
      G4double Ndis = 1.;
      run->AddNdis(Ndis);
  }
  else if (niel > 2 * Ed / 0.8)
  {
      G4double Ndis = niel * 0.8 / (2 * Ed);
      run->AddNdis(Ndis);
  }
   //position and Recoil X
  G4ThreeVector position = track->GetPosition();
  G4ThreeVector vertex = fPrimary->GetParticleGun()->GetParticlePosition();
  G4double TotalRecoilX = position.x() - vertex.x();
  analysisManager->FillH1(4, TotalRecoilX, Ndis);

  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
