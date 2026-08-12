#pragma once

#include <G4UserSteppingAction.hh>
#include <G4AnalysisManager.hh>

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction() = default;
    ~SteppingAction() override = default;
    void UserSteppingAction(const G4Step* step) override;

  private:
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
};
