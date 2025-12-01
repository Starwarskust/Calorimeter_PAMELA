#pragma once

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>
#include <G4AnalysisManager.hh>

#include <G4EventManager.hh>
#include <G4VProcess.hh>

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    ~SteppingAction();
    void UserSteppingAction(const G4Step *step) override;

  private:
    G4AnalysisManager *fAnalysisManager = G4AnalysisManager::Instance();
};
