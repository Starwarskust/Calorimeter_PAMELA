#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name)
: G4VSensitiveDetector(name),
  fEventID(0)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *R0hist)
{
  G4double energyDeposit = step->GetTotalEnergyDeposit();
  if (energyDeposit == 0.)
    return true;
  fAnalysisManager->FillNtupleIColumn(0, fEventID);
  fAnalysisManager->FillNtupleIColumn(1, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(4));
  fAnalysisManager->FillNtupleIColumn(2, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(3));
  fAnalysisManager->FillNtupleIColumn(3, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(2));
  fAnalysisManager->FillNtupleIColumn(4, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(1));
  fAnalysisManager->FillNtupleIColumn(5, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(0));
  fAnalysisManager->FillNtupleDColumn(6, energyDeposit);
  fAnalysisManager->AddNtupleRow(0);
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *hitCollection)
{
  fEventID += 1;
}
