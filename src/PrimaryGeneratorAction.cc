#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(G4int particlePDG, G4double energy)
: fParticleGun(0),
  fParticlePDG(particlePDG),
  fEnergy(energy)
{
  auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  CLHEP::HepRandom::setTheSeed(seed);

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  G4ParticleDefinition *particle = 0;
  if (G4ParticleTable::GetParticleTable()->FindParticle(fParticlePDG))
    particle = G4ParticleTable::GetParticleTable()->FindParticle(fParticlePDG);
  else if (G4IonTable::GetIonTable()->GetIon(fParticlePDG))
    particle = G4IonTable::GetIonTable()->GetIon(fParticlePDG);
  else
    std::cerr << "Error: particle was not found in G4ParticleTable and G4IonTable" << std::endl;

  G4double Ekin = CLHEP::RandFlat::shoot(80., 360.); // MeV
  G4double X = CLHEP::RandFlat::shoot(-10., 10.); // mm
  G4double Y = CLHEP::RandFlat::shoot(-10., 10.); // mm
  G4double theta = asin(CLHEP::RandFlat::shoot());
  G4double phi = CLHEP::RandFlat::shoot(CLHEP::twopi);

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(1, 0, Ekin);
  analysisManager->FillNtupleDColumn(1, 1, X);
  analysisManager->FillNtupleDColumn(1, 2, Y);
  analysisManager->FillNtupleDColumn(1, 3, theta);
  analysisManager->FillNtupleDColumn(1, 4, phi);
  analysisManager->AddNtupleRow(1);

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(Ekin*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(X*mm, Y*mm, 0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta) * cos(phi),
                                                           sin(theta) * sin(phi),
                                                           cos(theta)));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
