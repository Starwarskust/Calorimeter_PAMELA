#pragma once

#include <G4UserTrackingAction.hh>
#include <G4AnalysisManager.hh>

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction() = default;
    ~TrackingAction() override = default;
    void PostUserTrackingAction(const G4Track* track) override;

  private:
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
};
