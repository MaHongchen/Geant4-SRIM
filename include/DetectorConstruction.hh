#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

const G4int kMaxFilms = 10;                        // 0 + 9

class G4GlobalMagFieldMessenger;




class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

    DetectorConstruction();
    ~DetectorConstruction();

public:

    void SetNbOfFilms(G4int);
    void SetFilmMaterial(G4int, const G4String&);
    void SetFilmThickness(G4int, G4double);

    void SetFilmSizeYZ(G4double);
    void SetNbOfDivisions(G4int, G4int);

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

public:

    G4int       GetNbOfFilms() { return fNbOfFilms; }
    G4Material* GetFilmMaterial(G4int i) const { return fFilmMaterial[i]; };
    G4double    GetFilmThickness(G4int i) const { return fFilmThickness[i]; };
    G4double    GetXfront(G4int i) { return fXfront[i]; };

    G4double GetFilmSizeX() { return fFilmSizeX; };
    G4double GetFilmSizeYZ() { return fFilmSizeYZ; };

    G4int  GetNbOfDivisions(G4int i) { return fNbOfDivisions[i]; };

    void PrintParameters();

private:

    G4int              fNbOfFilms;
    G4Material* fFilmMaterial[kMaxFilms];
    G4double           fFilmThickness[kMaxFilms];
    G4double           fXfront[kMaxFilms];

    G4int              fNbOfDivisions[kMaxFilms];

    G4double           fFilmSizeX;
    G4double           fWorldSizeX;
    G4double           fFilmSizeYZ;
    G4Material* fDefaultMaterial;

    G4VPhysicalVolume* fPhysiWorld;

    DetectorMessenger* fDetectorMessenger;
    G4Cache<G4GlobalMagFieldMessenger*> fFieldMessenger;

private:

    void DefineMaterials();
    void ComputeParameters();
    G4VPhysicalVolume* ConstructVolumes();
};

#endif

