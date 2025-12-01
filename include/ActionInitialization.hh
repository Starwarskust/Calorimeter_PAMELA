#pragma once

#include <G4VUserActionInitialization.hh>

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(G4int particlePDG, G4double energyMin, G4double energyMax, G4int runNumber);
    ~ActionInitialization();
    void Build() const override;

  private:
    G4int fParticlePDG;
    G4double fEnergyMin;
    G4double fEnergyMax;
    G4int fRunNumber;
};
