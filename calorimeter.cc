#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char *argv[])
{
  // Read input
  if (argc != 4) {
    G4cout << "Wrong number of input parameters" << G4endl;
    return 0;
  }
  G4int particle  = atoi(argv[1]);
  G4double energy = atoi(argv[2]); // MeV
  G4int n         = atoi(argv[3]); // number of events
  G4cout << "Input particle: " << particle << "\n"
         << "Input energy: " << energy << " MeV" << "\n"
         << "Input n: " << n << G4endl;

  // Construct RunManager and initialize G4 kernel
  G4RunManager *runManager = new G4RunManager;
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization(particle, energy));
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/tracking/verbose 0");

  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();
  G4UIExecutive *UI = new G4UIExecutive(argc, argv);
  UImanager->ApplyCommand("/control/execute vis.mac");
  UI->SessionStart();
  delete UI;

  // Run 1 particle
  G4int numberOfEvent = n;
  runManager->BeamOn(numberOfEvent);

  // Job termination
  delete runManager;

  return 0;
}
