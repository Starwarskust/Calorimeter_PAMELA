#include "RunAction.hh"

RunAction::RunAction(G4int runNumber)
: fRunNumber(runNumber)
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("../data/output.csv");

  analysisManager->CreateNtuple("Ntuple1", "Energy release");
  for (G4int i = 0; i < 4224; i++)
    analysisManager->CreateNtupleDColumn(std::to_string(i));
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("Ntuple2", "Initial parameters");
  analysisManager->CreateNtupleDColumn("Ekin");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("theta");
  analysisManager->CreateNtupleDColumn("phi");
  analysisManager->CreateNtupleDColumn("Z_end");
  analysisManager->CreateNtupleSColumn("LastProcess");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("Ntuple3", "Tracks");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("ParticlePDG");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
