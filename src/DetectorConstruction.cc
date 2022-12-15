#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"


#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4RunManager.hh"
#include <iomanip>

DetectorConstruction::DetectorConstruction()
    :G4VUserDetectorConstruction(), fDefaultMaterial(0), fPhysiWorld(0),
    fDetectorMessenger(0)
{
    // default parameter values of the absorbers
    fNbOfFilms = 3;
    fFilmThickness[0] = 0 * mm;        //dummy, for initialization   
    fFilmThickness[1] = 1500 * nm;
    fFilmThickness[2] = 5 * nm;
    fFilmThickness[3] = 1 * um;

    fFilmSizeYZ = 3. * um;
    for (G4int iFilm = 0; iFilm < kMaxFilms; iFilm++) {
        fNbOfDivisions[iFilm] = 1;
    }
    ComputeParameters();

    // materials
    DefineMaterials();
    SetFilmMaterial(1, "G4_Au");
    SetFilmMaterial(2, "G4_Cr");
    SetFilmMaterial(3, "G4_Si");

    // create commands for interactive definition of the calorimeter
    fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return ConstructVolumes();
}

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* man = G4NistManager::Instance();

    man->FindOrBuildMaterial("G4_Al");
    man->FindOrBuildMaterial("G4_Au");
    man->FindOrBuildMaterial("G4_Cr");
    man->FindOrBuildMaterial("G4_Pt");
    man->FindOrBuildMaterial("G4_Ir");
    man->FindOrBuildMaterial("G4_Si");
    man->FindOrBuildMaterial("G4_Fe");
    man->FindOrBuildMaterial("G4_Cu");
    man->FindOrBuildMaterial("G4_Ge");
    man->FindOrBuildMaterial("G4_Mo");
    man->FindOrBuildMaterial("G4_Ta");
    man->FindOrBuildMaterial("G4_W");
    man->FindOrBuildMaterial("G4_Au");
    man->FindOrBuildMaterial("G4_Pb");
    man->FindOrBuildMaterial("G4_PbWO4");
    man->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    man->FindOrBuildMaterial("G4_AIR");
    man->FindOrBuildMaterial("G4_WATER");

    G4Element* H = man->FindOrBuildElement("H");
    G4Element* O = man->FindOrBuildElement("O");

    G4Material* H2O =
        new G4Material("Water", 1.000 * g / cm3, 2);
    H2O->AddElement(H, 2);
    H2O->AddElement(O, 1);
    H2O->GetIonisation()->SetMeanExcitationEnergy(78.0 * eV);

    G4double density = universe_mean_density;    //from PhysicalConstants.h
    G4double pressure = 3.e-18 * pascal;
    G4double temperature = 2.73 * kelvin;
    G4Material* Galactic =
        new G4Material("Galactic", 1., 1.008 * g / mole, density,
            kStateGas, temperature, pressure);

    fDefaultMaterial = Galactic;

    //  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::ComputeParameters()
{
    // Compute total thickness of absorbers
    fFilmSizeX = 0.;
    for (G4int iFilm = 1; iFilm <= fNbOfFilms; iFilm++) {
        fFilmSizeX += fFilmThickness[iFilm];
    }
    fWorldSizeX = fFilmSizeX + 1000 * nm;
}

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
    // complete the Calor parameters definition
    ComputeParameters();

    // Cleanup old geometry
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    //
    // World
    //
    G4Box* solidWorld =
        new G4Box("World",                                             //name
            fWorldSizeX / 2, fFilmSizeYZ / 2, fFilmSizeYZ / 2);       //size

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,              //solid
            fDefaultMaterial,        //material
            "World");                //name

    fPhysiWorld =
        new G4PVPlacement(0,                        //no rotation
            G4ThreeVector(),        //at (0,0,0)
            logicWorld,               //logical volume
            "World",                  //name
            0,                       //mother volume
            false,                   //no boolean operation
            0);                      //copy number

//
// Films
//
    fXfront[0] = -0.5 * fFilmSizeX;
    //
    for (G4int k = 1; k <= fNbOfFilms; k++) {
        G4Material* material = fFilmMaterial[k];
        G4String matname = material->GetName();

        G4Box* solidFilm =
            new G4Box(matname, fFilmThickness[k] / 2, fFilmSizeYZ / 2, fFilmSizeYZ / 2);

        G4LogicalVolume* logicFilm =
            new G4LogicalVolume(solidFilm,           // solid
                material,             // material
                matname);             // name

        fXfront[k] = fXfront[k - 1] + fFilmThickness[k - 1];
        G4double xcenter = fXfront[k] + 0.5 * fFilmThickness[k];
        G4ThreeVector position = G4ThreeVector(xcenter, 0., 0.);

        new G4PVPlacement(0,                     //no rotation
            position,        //position
            logicFilm,            //logical volume        
            matname,               //name
            logicWorld,            //mother
            false,                 //no boulean operat
            k);                    //copy number

// divisions, if any
//
        G4double LayerThickness = fFilmThickness[k] / fNbOfDivisions[k];
        G4Box* solidLayer =
            new G4Box(matname, LayerThickness / 2, fFilmSizeYZ / 2, fFilmSizeYZ / 2);

        G4LogicalVolume* logicLayer =
            new G4LogicalVolume(solidLayer,      //solid
                material,        //material
                matname);        //name

        new G4PVReplica(matname,             //name
            logicLayer,    //logical volume
            logicFilm,    //mother
            kXAxis,              //axis of replication
            fNbOfDivisions[k],   //number of replica
            LayerThickness);     //witdth of replica    
    }

    PrintParameters();

    //always return the physical World
    //
    return fPhysiWorld;
}

void DetectorConstruction::PrintParameters()
{
    G4cout << "\n-------------------------------------------------------------"
        << "\n ---> The Film is " << fNbOfFilms << " layers of:";
    for (G4int i = 1; i <= fNbOfFilms; i++)
    {
        G4cout << "\n \t" << std::setw(12) << fFilmMaterial[i]->GetName() << ": "
            << std::setw(6) << G4BestUnit(fFilmThickness[i], "Length")
            << "  divided in " << fNbOfDivisions[i] << " slices";
    }
    G4cout << "\n-------------------------------------------------------------\n"
        << G4endl;
}

void DetectorConstruction::SetNbOfFilms(G4int ival)
{
    // set the number of Absorbers
    //
    if (ival < 1 || ival >(kMaxFilms - 1))
    {
        G4cout << "\n ---> warning from SetfNbOfFilms: "
            << ival << " must be at least 1 and and most " << kMaxFilms - 1
            << ". Command refused" << G4endl;
        return;
    }
    fNbOfFilms = ival;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetFilmMaterial(G4int iabs, const G4String& material)
{
    // search the material by its name
    //
    if (iabs > fNbOfFilms || iabs <= 0)
    {
        G4cout << "\n --->warning from SetfFilmMaterial: Film number "
            << iabs << " out of range. Command refused" << G4endl;
        return;
    }

    G4Material* pttoMaterial =
        G4NistManager::Instance()->FindOrBuildMaterial(material);
    if (pttoMaterial) {
        fFilmMaterial[iabs] = pttoMaterial;
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
        G4cout << "\n " << pttoMaterial << G4endl;
    }
}

void DetectorConstruction::SetFilmThickness(G4int iabs, G4double val)
{
    // change Absorber thickness
    //
    if (iabs > fNbOfFilms || iabs <= 0)
    {
        G4cout << "\n --->warning from SetfFilmThickness: Film number "
            << iabs << " out of range. Command refused" << G4endl;
        return;
    }
    if (val <= DBL_MIN)
    {
        G4cout << "\n --->warning from SetfFilmThickness: thickness "
            << val << " out of range. Command refused" << G4endl;
        return;
    }
    fFilmThickness[iabs] = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetFilmSizeYZ(G4double val)
{
    // change the transverse size
    //
    if (val <= DBL_MIN)
    {
        G4cout << "\n --->warning from SetfFilmSizeYZ: thickness "
            << val << " out of range. Command refused" << G4endl;
        return;
    }
    fFilmSizeYZ = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetNbOfDivisions(G4int iabs, G4int ival)
{
    // set the number of divisions
    //
    if (iabs > fNbOfFilms || iabs < 1)
    {
        G4cout << "\n --->warning from SetNbOfDivisions: Film number "
            << iabs << " out of range. Command refused" << G4endl;
        return;
    }

    if (ival < 1)
    {
        G4cout << "\n --->warning from SetNbOfDivisions: "
            << ival << " must be at least 1. Command refused" << G4endl;
        return;
    }
    fNbOfDivisions[iabs] = ival;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::ConstructSDandField()
{
    if (fFieldMessenger.Get() == 0) {
        // Create global magnetic field messenger.
        // Uniform magnetic field is then created automatically if
        // the field value is not zero.
        G4ThreeVector fieldValue = G4ThreeVector();
        G4GlobalMagFieldMessenger* msg =
            new G4GlobalMagFieldMessenger(fieldValue);
        //msg->SetVerboseLevel(1);
        G4AutoDelete::Register(msg);
        fFieldMessenger.Put(msg);
    }
}

