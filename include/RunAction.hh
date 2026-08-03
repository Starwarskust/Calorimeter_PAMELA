#pragma once

#include <G4UserRunAction.hh>
#include "SimConfig.hh"

class RunAction : public G4UserRunAction
{
  public:
    RunAction(const SimConfig& config);
    ~RunAction() override = default;
    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

  private:
    const SimConfig& fConfig;
};
