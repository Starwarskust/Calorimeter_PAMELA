#include "RunAction.hh"

RunAction::RunAction(G4int runNumber)
: fRunNumber(runNumber)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  std::string filename = "../data/output_" + std::to_string(fRunNumber) + ".csv";
  analysisManager->OpenFile(filename);

  analysisManager->CreateNtuple("Ntuple1", "Energy release");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("N_layer");
  analysisManager->CreateNtupleIColumn("N_plane");
  analysisManager->CreateNtupleIColumn("N_paddle");
  analysisManager->CreateNtupleIColumn("N_strip");
  analysisManager->CreateNtupleIColumn("N_pad");
  analysisManager->CreateNtupleDColumn("energy_desopit");
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
  analysisManager->CreateNtupleDColumn("E");
  analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
