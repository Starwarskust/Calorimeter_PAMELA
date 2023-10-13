#include "PhysicsList.hh"

PhysicsList::PhysicsList()
: FTFP_BERT(0)
{
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

// void PhysicsList::SetCuts()
// {
//   SetCutValue(1*mm, "proton");
//   SetCutValue(1*mm, "e-");
//   SetCutValue(1*mm, "e+");
//   SetCutValue(1*mm, "gamma");
// }
