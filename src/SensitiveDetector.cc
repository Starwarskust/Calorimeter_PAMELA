#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name)
: G4VSensitiveDetector(name)
{
  calorimeterResponse.resize(4224);
}

SensitiveDetector::~SensitiveDetector() 
{}

void SensitiveDetector::Initialize(G4HCofThisEvent *hitCollection)
{
  std::fill(calorimeterResponse.begin(), calorimeterResponse.end(), 0.);
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *R0hist)
{
  G4double energyDeposit = step->GetTotalEnergyDeposit();
  G4int stripNumber = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
  if (energyDeposit == 0.)
    return true;
  calorimeterResponse[stripNumber] += energyDeposit;
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *hitCollection)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  for (G4int i = 0; i < 4224; i++)
    analysisManager->FillNtupleDColumn(0, i, calorimeterResponse[i]);
  analysisManager->AddNtupleRow(0);
}
