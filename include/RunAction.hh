#pragma once

#include <G4UserRunAction.hh>

#include <G4AnalysisManager.hh>

class RunAction : public G4UserRunAction
{
  public:
    RunAction(G4int runNumber);
    ~RunAction();
    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

  private:
    G4int fRunNumber;
};
