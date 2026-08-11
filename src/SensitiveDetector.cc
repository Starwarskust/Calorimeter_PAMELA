#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name)
{}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double energyDeposit = step->GetTotalEnergyDeposit();
  if (energyDeposit == 0.)
    return true;
  G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  const G4VTouchable* touchableHandle = step->GetPreStepPoint()->GetTouchable();
  fAnalysisManager->FillNtupleIColumn(0, 0, eventID);
  fAnalysisManager->FillNtupleIColumn(0, 1, touchableHandle->GetCopyNumber(2));
  fAnalysisManager->FillNtupleIColumn(0, 2, touchableHandle->GetCopyNumber(1));
  fAnalysisManager->FillNtupleIColumn(0, 3, touchableHandle->GetCopyNumber(0));
  fAnalysisManager->FillNtupleDColumn(0, 4, energyDeposit);
  fAnalysisManager->AddNtupleRow(0);
  return true;
}
