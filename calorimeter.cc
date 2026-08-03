#include <G4MTRunManager.hh>

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "SimConfig.hh"

#ifdef USE_VISUALIZATION
  #include <G4UImanager.hh>
  #include <G4UIExecutive.hh>
  #include <G4VisExecutive.hh>
#endif

int main(int argc, char* argv[])
{
  // Read input
  // Input example: ./calorimeter 0 2212 80 360 1 $(date +%s)
  if (argc != 7) {
    G4cout << "Wrong number of input parameters" << G4endl;
    return 0;
  }
  SimConfig simConfig;
  simConfig.runNumber = std::stoi(argv[1]); // run number
  simConfig.particlePDG = std::stoi(argv[2]); // PDG code of particle
  simConfig.energyMin = std::stod(argv[3]); // MeV
  simConfig.energyMax = std::stod(argv[4]); // MeV
  G4int numberOfEvents = std::stoi(argv[5]); // number of events
  G4long seed = std::stoi(argv[6]); // seed
  G4cout << "Input runNumber: " << simConfig.runNumber << "\n"
         << "Input particlePDG: " << simConfig.particlePDG << "\n"
         << "Input energyMin: " << simConfig.energyMin << " MeV" << "\n"
         << "Input energyMax: " << simConfig.energyMax << " MeV" << "\n"
         << "Input numberOfEvents: " << numberOfEvents << "\n"
         << "Input seed: " << seed << G4endl;

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(seed + simConfig.runNumber);

  // Construct RunManager and initialize G4 kernel
  G4MTRunManager* runManager = new G4MTRunManager();
  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization(simConfig));
  runManager->Initialize();

  #ifdef USE_VISUALIZATION
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    G4UIExecutive* UI = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute vis.mac");
    UI->SessionStart();
    delete UI;
  #else
    // Run 1 particle
    runManager->BeamOn(numberOfEvents);
  #endif

  // Job termination
  delete runManager;

  return 0;
}
