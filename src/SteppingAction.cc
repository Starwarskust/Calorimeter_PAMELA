#include "SteppingAction.hh"

SteppingAction::SteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
  if (step->GetTrack()->GetParticleDefinition()->GetPDGCharge() != 0 &&
      (step->GetTrack()->GetTrackID() == 1 || step->GetTrack()->GetParentID() == 1)) {
    analysisManager->FillNtupleIColumn(2, 0, step->GetTrack()->GetTrackID());
    analysisManager->FillNtupleIColumn(2, 1, step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    analysisManager->FillNtupleDColumn(2, 2, step->GetPreStepPoint()->GetPosition().x());
    analysisManager->FillNtupleDColumn(2, 3, step->GetPreStepPoint()->GetPosition().y());
    analysisManager->FillNtupleDColumn(2, 4, step->GetPreStepPoint()->GetPosition().z());
    analysisManager->AddNtupleRow(2);
    if (step->GetTrack()->GetTrackStatus() == fStopAndKill) {
      analysisManager->FillNtupleIColumn(2, 0, step->GetTrack()->GetTrackID());
      analysisManager->FillNtupleIColumn(2, 1, step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
      analysisManager->FillNtupleDColumn(2, 2, step->GetPostStepPoint()->GetPosition().x());
      analysisManager->FillNtupleDColumn(2, 3, step->GetPostStepPoint()->GetPosition().y());
      analysisManager->FillNtupleDColumn(2, 4, step->GetPostStepPoint()->GetPosition().z());
      analysisManager->AddNtupleRow(2);
    }
  }

  if (step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetTrackStatus() == fStopAndKill) {
    // ЭТО ТОЧКА ГИБЕЛИ ПЕРВИЧНОЙ ЧАСТИЦЫ, НУЖНА ИНФА ПО ТОЧКЕ СМЕРТИ И ПРОЦЕССЕ СМЕРТИ
    // "Process name: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << "\n"
  }
}
