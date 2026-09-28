#include "PrimaryGeneratorAction.hh"

#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4AnalysisManager.hh>
#include <Randomize.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction(const SimConfig& config)
: fConfig(config)
{
  fParticleGun = new G4ParticleGun();

  G4ParticleDefinition* particle = nullptr;
  if (G4ParticleTable::GetParticleTable()->FindParticle(fConfig.particlePDG))
    particle = G4ParticleTable::GetParticleTable()->FindParticle(fConfig.particlePDG);
  else if (G4IonTable::GetIonTable()->GetIon(fConfig.particlePDG))
    particle = G4IonTable::GetIonTable()->GetIon(fConfig.particlePDG);
  else
    G4cerr << "Error: particle was not found in G4ParticleTable and G4IonTable" << G4endl;
  fParticleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // G4double Ekin = CLHEP::RandFlat::shoot(fConfig.energyMin, fConfig.energyMax); // MeV
  G4double logEkin = CLHEP::RandFlat::shoot(
    std::log(fConfig.energyMin),
    std::log(fConfig.energyMax)
  );
  G4double Ekin = std::exp(logEkin); // MeV
  G4double X = CLHEP::RandFlat::shoot(-120., 120.); // mm
  G4double Y = CLHEP::RandFlat::shoot(-120., 120.); // mm
  G4double theta = asin(CLHEP::RandFlat::shoot());
  G4double phi = CLHEP::RandFlat::shoot(CLHEP::twopi);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(1, 0, anEvent->GetEventID());
  analysisManager->FillNtupleDColumn(1, 1, Ekin);
  analysisManager->FillNtupleDColumn(1, 2, X);
  analysisManager->FillNtupleDColumn(1, 3, Y);
  analysisManager->FillNtupleDColumn(1, 4, theta);
  analysisManager->FillNtupleDColumn(1, 5, phi);

  fParticleGun->SetParticleEnergy(Ekin*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(X*mm, Y*mm, -1.*mm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta) * cos(phi),
                                                           sin(theta) * sin(phi),
                                                           cos(theta)));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
