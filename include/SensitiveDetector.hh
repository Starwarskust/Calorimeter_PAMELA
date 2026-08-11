#pragma once

#include <G4VSensitiveDetector.hh>
#include <G4AnalysisManager.hh>

#include <G4EventManager.hh>

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override = default;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

  protected:
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

  private:
    static constexpr G4int nLayers = 22;
    static constexpr G4int nPlanes = 2;
    static constexpr G4int nStrips = 96;
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    G4double fEnergyDeposit[nLayers][nPlanes][nStrips]{};
};
