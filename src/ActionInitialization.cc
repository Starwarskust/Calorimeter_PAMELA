#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "TrackingAction.hh"
#ifdef SAVE_TRACKS
#include "SteppingAction.hh"
#endif

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
  SetUserAction(new TrackingAction());
#ifdef SAVE_TRACKS
  SetUserAction(new SteppingAction());
#endif
}
