#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name)
: G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *R0hist)
{
  G4double energyDeposit = step->GetTotalEnergyDeposit();
  if (energyDeposit == 0.)
    return true;
  G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  fAnalysisManager->FillNtupleIColumn(0, 0, eventID);
  fAnalysisManager->FillNtupleIColumn(0, 1, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(4));
  fAnalysisManager->FillNtupleIColumn(0, 2, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(3));
  fAnalysisManager->FillNtupleIColumn(0, 3, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(2));
  fAnalysisManager->FillNtupleIColumn(0, 4, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(1));
  fAnalysisManager->FillNtupleIColumn(0, 5, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(0));
  fAnalysisManager->FillNtupleDColumn(0, 6, energyDeposit);
  fAnalysisManager->AddNtupleRow(0);
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *hitCollection)
{}
