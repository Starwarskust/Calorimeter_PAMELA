#pragma once

#include <G4VSensitiveDetector.hh>
#include <G4AnalysisManager.hh>

#include <G4EventManager.hh>

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override = default;

  protected:
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

  private:
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
};
