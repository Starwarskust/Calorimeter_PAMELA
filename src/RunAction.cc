#include "RunAction.hh"

RunAction::RunAction(G4int runNumber)
: fRunNumber(runNumber)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  analysisManager->CreateNtuple("energy_release", "Energy release in sensitive volumes");
  analysisManager->CreateNtupleIColumn("event_id");
  analysisManager->CreateNtupleIColumn("n_layer");
  analysisManager->CreateNtupleIColumn("n_plane");
  analysisManager->CreateNtupleIColumn("n_paddle");
  analysisManager->CreateNtupleIColumn("n_strip");
  analysisManager->CreateNtupleIColumn("n_pad");
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

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  std::string filename = "../data/output_" + std::to_string(fRunNumber) + ".hdf5";
  analysisManager->SetFileName(filename);
  analysisManager->OpenFile(filename);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
