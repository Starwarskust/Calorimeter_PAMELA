#pragma once

#include <FTFP_BERT.hh>

#include <G4RadioactiveDecayPhysics.hh>
// #include <G4SystemOfUnits.hh>

class PhysicsList : public FTFP_BERT
{
  public:
    PhysicsList();
    ~PhysicsList();
    // void SetCuts() override;
};
