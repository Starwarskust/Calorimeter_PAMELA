#include "SteppingAction.hh"

#include <G4EventManager.hh>
#include <G4Step.hh>

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (step->GetTrack()->GetParticleDefinition()->GetPDGCharge() != 0 &&
      (step->GetTrack()->GetTrackID() == 1 || step->GetTrack()->GetParentID() == 1)) {
    const G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    fAnalysisManager->FillNtupleIColumn(2, 0, eventID);
    fAnalysisManager->FillNtupleIColumn(2, 1, step->GetTrack()->GetTrackID());
    fAnalysisManager->FillNtupleIColumn(2, 2, step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    fAnalysisManager->FillNtupleDColumn(2, 3, step->GetPreStepPoint()->GetPosition().x());
    fAnalysisManager->FillNtupleDColumn(2, 4, step->GetPreStepPoint()->GetPosition().y());
    fAnalysisManager->FillNtupleDColumn(2, 5, step->GetPreStepPoint()->GetPosition().z());
    fAnalysisManager->FillNtupleDColumn(2, 6, step->GetPreStepPoint()->GetKineticEnergy());
    fAnalysisManager->FillNtupleDColumn(2, 7, step->GetTotalEnergyDeposit());
    fAnalysisManager->AddNtupleRow(2);
    if (step->GetTrack()->GetTrackStatus() == fStopAndKill) {
      fAnalysisManager->FillNtupleIColumn(2, 0, eventID);
      fAnalysisManager->FillNtupleIColumn(2, 1, step->GetTrack()->GetTrackID());
      fAnalysisManager->FillNtupleIColumn(2, 2, step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
      fAnalysisManager->FillNtupleDColumn(2, 3, step->GetPostStepPoint()->GetPosition().x());
      fAnalysisManager->FillNtupleDColumn(2, 4, step->GetPostStepPoint()->GetPosition().y());
      fAnalysisManager->FillNtupleDColumn(2, 5, step->GetPostStepPoint()->GetPosition().z());
      fAnalysisManager->FillNtupleDColumn(2, 6, step->GetPostStepPoint()->GetKineticEnergy());
      fAnalysisManager->FillNtupleDColumn(2, 7, step->GetTotalEnergyDeposit());
      fAnalysisManager->AddNtupleRow(2);
    }
  }
}
