#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(const SimConfig& config)
: fConfig(config)
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(fConfig));
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(fConfig));
  SetUserAction(new RunAction(fConfig));
  SetUserAction(new SteppingAction());
}
