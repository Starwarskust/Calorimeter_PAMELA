#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name)
{}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  const G4double energyDeposit = step->GetTotalEnergyDeposit();
  if (energyDeposit == 0.)
    return true;
  const G4VTouchable* touchableHandle = step->GetPreStepPoint()->GetTouchable();
  const G4int layer = touchableHandle->GetCopyNumber(2);
  const G4int plane = touchableHandle->GetCopyNumber(1);
  const G4int strip = touchableHandle->GetCopyNumber(0);
  fEnergyDeposit[layer][plane][strip] += energyDeposit;
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  for (G4int layer = 0; layer < nLayers; ++layer) {
    for (G4int plane = 0; plane < nPlanes; ++plane) {
      for (G4int strip = 0; strip < nStrips; ++strip) {
        const G4double energyDeposit = fEnergyDeposit[layer][plane][strip];
        if (energyDeposit == 0.)
          continue;
        fAnalysisManager->FillNtupleIColumn(0, 0, eventID);
        fAnalysisManager->FillNtupleIColumn(0, 1, layer);
        fAnalysisManager->FillNtupleIColumn(0, 2, plane);
        fAnalysisManager->FillNtupleIColumn(0, 3, strip);
        fAnalysisManager->FillNtupleDColumn(0, 4, energyDeposit);
        fAnalysisManager->AddNtupleRow(0);
        fEnergyDeposit[layer][plane][strip] = 0.;
      }
    }
  }
}
