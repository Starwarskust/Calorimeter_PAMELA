#pragma once

#include <G4VSensitiveDetector.hh>
#include <G4AnalysisManager.hh>

#include <G4EventManager.hh>

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(G4String name);
    ~SensitiveDetector();
    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
    void EndOfEvent(G4HCofThisEvent *hitCollection) override;

  private:
    G4AnalysisManager *fAnalysisManager = G4AnalysisManager::Instance();
};
