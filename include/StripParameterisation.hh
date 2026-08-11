#pragma once

#include <G4VPVParameterisation.hh>

class StripParameterisation : public G4VPVParameterisation
{
  public:
    StripParameterisation(G4double stripWidth, G4double padGap);
    void ComputeTransformation(G4int copyNo, G4VPhysicalVolume* physVol) const override;

  private:
    G4double fStripWidth;
    G4double fPadGap;
};
