﻿#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "SensitiveDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();
    virtual G4VPhysicalVolume *Construct() override;
    virtual void ConstructSDandField() override;

  private:
    G4LogicalVolume *pad_lvol;
};

#endif
