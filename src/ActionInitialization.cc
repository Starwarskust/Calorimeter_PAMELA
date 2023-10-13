#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization(G4int particlePDG, G4double energy)
: fParticlePDG(particlePDG),
  fEnergy(energy)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(fParticlePDG, fEnergy));
  SetUserAction(new RunAction);
  SetUserAction(new SteppingAction);
}
