#include "StripParameterisation.hh"

#include <G4ThreeVector.hh>

StripParameterisation::StripParameterisation(G4double stripWidth, G4double padGap)
: fStripWidth(stripWidth),
  fPadGap(padGap)
{}

void StripParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4int padNo = copyNo / 32;
  G4double y = fStripWidth * (copyNo - 47.5) + (padNo - 1) * fPadGap;
  physVol->SetTranslation(G4ThreeVector(0., y, 0.));
  physVol->SetRotation(nullptr);
}
