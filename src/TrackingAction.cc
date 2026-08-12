#include "TrackingAction.hh"

#include <G4Track.hh>
#include <G4VProcess.hh>

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  if (track->GetTrackID() != 1)
    return;
  const G4StepPoint* postPoint = track->GetStep()->GetPostStepPoint();
  const G4VProcess* process = postPoint->GetProcessDefinedStep();
  fAnalysisManager->FillNtupleDColumn(1, 6, postPoint->GetPosition().z());
  fAnalysisManager->FillNtupleSColumn(1, 7, process ? process->GetProcessName() : "");
  fAnalysisManager->AddNtupleRow(1);
}
