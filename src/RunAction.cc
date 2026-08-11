#include "RunAction.hh"

#include <G4AnalysisManager.hh>

RunAction::RunAction(const SimConfig& config)
: fConfig(config)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);

  analysisManager->CreateNtuple("energy_release", "Energy release in sensitive volumes");
  analysisManager->CreateNtupleIColumn("event_id");
  analysisManager->CreateNtupleIColumn("n_layer");
  analysisManager->CreateNtupleIColumn("n_plane");
  analysisManager->CreateNtupleIColumn("n_strip");
  analysisManager->CreateNtupleDColumn("energy_deposit");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("primary_info", "Parameters of the primary particle");
  analysisManager->CreateNtupleIColumn("event_id");
  analysisManager->CreateNtupleDColumn("energy");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("theta");
  analysisManager->CreateNtupleDColumn("phi");
  analysisManager->CreateNtupleDColumn("z_end");
  analysisManager->CreateNtupleSColumn("last_process");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("track_info", "Tracks of all particles");
  analysisManager->CreateNtupleIColumn("event_id");
  analysisManager->CreateNtupleIColumn("track_id");
  analysisManager->CreateNtupleIColumn("pdg_code");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("energy");
  analysisManager->CreateNtupleDColumn("energy_deposit");
  analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  const std::string filename = "../data/output_" + std::to_string(fConfig.runNumber) + ".root";
  analysisManager->SetFileName(filename);
  analysisManager->OpenFile(filename);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
