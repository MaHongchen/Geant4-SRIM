#include "DetectorMessenger.hh"

#include <sstream>

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
    :G4UImessenger(), fDetector(Det),
    fTestemDir(0),
    fDetDir(0),
    fNbFilmCmd(0),
    fFilmCmd(0),
    fNdivCmd(0),
    fSizeYZCmd(0)
{
    fTestemDir = new G4UIdirectory("/testem/");
    fTestemDir->SetGuidance(" detector control.");

    fDetDir = new G4UIdirectory("/testem/det/");
    fDetDir->SetGuidance("detector construction commands");

    fNbFilmCmd = new G4UIcmdWithAnInteger("/testem/det/setNbOfFilms", this);
    fNbFilmCmd->SetGuidance("Set number of Films.");
    fNbFilmCmd->SetParameterName("NbFilm", false);
    fNbFilmCmd->SetRange("NbAbsor>0");
    fNbFilmCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fNbFilmCmd->SetToBeBroadcasted(false);

    fFilmCmd = new G4UIcommand("/testem/det/setAbsor", this);
    fFilmCmd->SetGuidance("Set the Film nb, the material, the thickness.");
    fFilmCmd->SetGuidance("  Film number : from 1 to NbOfFilms");
    fFilmCmd->SetGuidance("  material name");
    fFilmCmd->SetGuidance("  thickness (with unit) : t>0.");
    //
    G4UIparameter* FilmNbPrm = new G4UIparameter("AbsorNb", 'i', false);
    FilmNbPrm->SetGuidance("film number : from 1 to NbOfAbsor");
    FilmNbPrm->SetParameterRange("FilmNb>0");
    fFilmCmd->SetParameter(FilmNbPrm);
    //
    G4UIparameter* MatPrm = new G4UIparameter("material", 's', false);
    MatPrm->SetGuidance("material name");
    fFilmCmd->SetParameter(MatPrm);
    //    
    G4UIparameter* ThickPrm = new G4UIparameter("thickness", 'd', false);
    ThickPrm->SetGuidance("thickness of Film");
    ThickPrm->SetParameterRange("thickness>0.");
    fFilmCmd->SetParameter(ThickPrm);
    //
    G4UIparameter* unitPrm = new G4UIparameter("unit", 's', false);
    unitPrm->SetGuidance("unit of thickness");
    G4String unitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("nm"));
    unitPrm->SetParameterCandidates(unitList);
    fFilmCmd->SetParameter(unitPrm);
    //
    fFilmCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFilmCmd->SetToBeBroadcasted(false);

    fNdivCmd = new G4UIcommand("/testem/det/nDivFilm", this);
    fNdivCmd->SetGuidance("Divide the Film nb : number of divisions");
    fNdivCmd->SetGuidance("  Film number : from 1 to NbOfFilms");
    fNdivCmd->SetGuidance("  number of divisions >= 0");
    //
    G4UIparameter* FilmNbPar = new G4UIparameter("FilmNb", 'i', false);
    FilmNbPar->SetGuidance("film number : from 1 to NbOfFilms");
    FilmNbPar->SetParameterRange("FilmNb>0");
    fNdivCmd->SetParameter(FilmNbPar);
    //
    G4UIparameter* NdivPrm = new G4UIparameter("NdivNb", 'i', false);
    NdivPrm->SetGuidance("nb of divisions > 0");
    NdivPrm->SetParameterRange("NdivNb>0");
    fNdivCmd->SetParameter(NdivPrm);
    //
    fNdivCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fNdivCmd->SetToBeBroadcasted(false);

    fSizeYZCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSizeYZ", this);
    fSizeYZCmd->SetGuidance("Set sizeYZ of the Film");
    fSizeYZCmd->SetParameterName("SizeYZ", false);
    fSizeYZCmd->SetRange("SizeYZ>0.");
    fSizeYZCmd->SetUnitCategory("Length");
    fSizeYZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSizeYZCmd->SetToBeBroadcasted(false);
}

DetectorMessenger::~DetectorMessenger()
{
    delete fNbFilmCmd;
    delete fFilmCmd;
    delete fNdivCmd;
    delete fSizeYZCmd;
    delete fDetDir;
    delete fTestemDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fNbFilmCmd)
    {
        fDetector->SetNbOfFilms(fNbFilmCmd->GetNewIntValue(newValue));
    }

    if (command == fFilmCmd)
    {
        G4int num; G4double tick;
        G4String unt, mat;
        std::istringstream is(newValue);
        is >> num >> mat >> tick >> unt;
        G4String material = mat;
        tick *= G4UIcommand::ValueOf(unt);
        fDetector->SetFilmMaterial(num, material);
        fDetector->SetFilmThickness(num, tick);
    }

    if (command == fNdivCmd)
    {
        G4int num, ndiv;
        std::istringstream is(newValue);
        is >> num >> ndiv;
        fDetector->SetNbOfDivisions(num, ndiv);
    }

    if (command == fSizeYZCmd)
    {
        fDetector->SetFilmSizeYZ(fSizeYZCmd->GetNewDoubleValue(newValue));
    }

}