#ifndef SensitiveDetector_hh
#define SensitiveDetector_hh

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

#include "G4EventManager.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(G4String name);
    ~SensitiveDetector();
    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
    virtual void EndOfEvent(G4HCofThisEvent *hitCollection) override;

  private:
    G4AnalysisManager *fAnalysisManager = G4AnalysisManager::Instance();
};

#endif
