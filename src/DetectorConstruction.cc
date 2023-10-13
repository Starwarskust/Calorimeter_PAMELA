#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4NistManager *nist = G4NistManager::Instance();
  G4bool checkOverlaps = false;
  G4int nLayer = 22;

  G4double density = 0.;
  G4int nElem  = 0;
  G4int nAtom = 0;

  //-------------------------------------- Volumes -------------------------------------------------

  //--------------------------------------- World --------------------------------------------------

  G4Material *world_mat = nist->FindOrBuildMaterial("G4_N");
  // G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4double world_sizeXY = 400.*mm;
  G4double world_sizeZ  = 500.*mm;

  G4Box *world_solid =
    new G4Box("worldSolid",               // its name
              0.5*world_sizeXY,           // its size X
              0.5*world_sizeXY,           // its size Y
              0.5*world_sizeZ);           // its size Z

  G4LogicalVolume *world_log = 
    new G4LogicalVolume(world_solid,      // its solid
                        world_mat,        // its material
                        "worldLog");      // its name

  G4VPhysicalVolume *world_phys = 
    new G4PVPlacement(0,                  // no rotation
                      G4ThreeVector(),    // at its position
                      world_log,          // its logical volume
                      "worldPhys",        // its name
                      0,                  // its mother volume
                      false,              // no boolean operation
                      0,                  // copy number
                      checkOverlaps);     // overlaps checking

  //--------------------------------------- ToFS3 --------------------------------------------------

  // G4Material *ToFS3_mat = new G4Material("C10H11", density = 1.032*g/cm3, nElem = 2);
  // ToFS3_mat->AddElement(nist->FindOrBuildElement("H"), nAtom = 11);
  // ToFS3_mat->AddElement(nist->FindOrBuildElement("C"), nAtom = 10);

  // G4double ToFS3_sizeXY = 246*mm;
  // G4double ToFS3_sizeZ  =  14*mm;

  // G4Box *ToFS3_solid =
  //   new G4Box("ToFS3",                    // its name
  //             0.5*ToFS3_sizeXY,           // its size X
  //             0.5*ToFS3_sizeXY,           // its size Y
  //             0.5*ToFS3_sizeZ);           // its size Z

  // G4LogicalVolume *ToFS3_log =
  //   new G4LogicalVolume(ToFS3_solid,      // its solid
  //                       ToFS3_mat,        // its material
  //                       "ToFS3");         // its name

  // G4ThreeVector ToFS3_position = G4ThreeVector(0.,0.,-7*mm);
  // new G4PVPlacement(0,                    // no rotation
  //                   ToFS3_position,       // at its position
  //                   ToFS3_log,            // its logical volume
  //                   "ToFS3",              // its name
  //                   world_log,            // its mother  volume
  //                   false,                // no boolean operation
  //                   0,                    // copy number
  //                   checkOverlaps);       // overlaps checking

  //---------------------------------------- CAST --------------------------------------------------

  G4Material *CAST_mat = nist->FindOrBuildMaterial("G4_Si");

  G4double CAST_sizeX = 239.08*mm; // strip length
  G4double CAST_sizeY = 2.44*mm;
  G4double CAST_sizeZ = 0.38*mm;

  G4Box *CAST_solid =
    new G4Box("CAST",                     // its name
              0.5*CAST_sizeX,             // its size X
              0.5*CAST_sizeY,             // its size Y
              0.5*CAST_sizeZ);            // its size Z

  CAST_log = new G4LogicalVolume(CAST_solid,       // its solid
                                 CAST_mat,         // its material
                                 "CAST");          // its name

  for (G4int iLayer = 0; iLayer < nLayer; iLayer++) {
    for (G4int iSide = 0; iSide < 2; iSide++) { // 0 means along x axis; 1 means along y axis
      G4RotationMatrix *CAST_rotation = new G4RotationMatrix(iSide*CLHEP::halfpi, 0., 0.);
      for (G4int iBlock = -1; iBlock < 2; iBlock++) {
        for (G4int iStrip = 0; iStrip < 32; iStrip++) {
          G4double shift = -37.82 + iStrip*2.44 + iBlock*80.5;
          G4int CAST_index = iLayer*192 + iSide*96 + (iBlock+1)*32 + iStrip;
          G4ThreeVector CAST_position = G4ThreeVector(   iSide *shift*mm,
                                                      (1-iSide)*shift*mm,
                                                      (0.19 + iSide*5.81 + iLayer*8.09)*mm);
          new G4PVPlacement(CAST_rotation,      // no rotation
                            CAST_position,      // at its position
                            CAST_log,           // its logical volume
                            "CAST",             // its name
                            world_log,          // its mother volume
                            false,              // no boolean operation
                            CAST_index,         // copy number
                            checkOverlaps);     // overlaps checking
        }
      }
    }
  }

  //---------------------------------------- CAKP --------------------------------------------------

  G4Material *CAKP_mat = nist->FindOrBuildMaterial("G4_KAPTON");

  G4double CAKP_sizeXY =  246*mm;
  G4double CAKP_sizeZ  = 0.05*mm;

  G4Box *CAKP_solid =
    new G4Box("CAKP",                     // its name
              0.5*CAKP_sizeXY,            // its size X
              0.5*CAKP_sizeXY,            // its size Y
              0.5*CAKP_sizeZ);            // its size Z

  G4LogicalVolume *CAKP_log = 
    new G4LogicalVolume(CAKP_solid,       // its solid
                        CAKP_mat,         // its material
                        "CAKP");          // its name

  for (G4int i = 0; i < nLayer; i++) {
    for (G4int j = 0; j < 2; j++) {
      G4ThreeVector CAKP_position = G4ThreeVector(0., 0., (0.405 + j*4.03 + i*8.09)*mm);
      new G4PVPlacement(0,                  // no rotation
                        CAKP_position,      // at its position
                        CAKP_log,           // its logical volume
                        "CAKP",             // its name
                        world_log,          // its mother volume
                        false,              // no boolean operation
                        i*2 + j,            // copy number
                        checkOverlaps);     // overlaps checking
    }
  }

  //---------------------------------------- CAGL --------------------------------------------------

  G4Material *CAGL_mat = nist->FindOrBuildMaterial("G4_Si");

  G4double CAGL_sizeXY =  246*mm;
  G4double CAGL_sizeZ  = 0.10*mm;

  G4Box *CAGL_solid =
    new G4Box("CAGL",                     // its name
              0.5*CAGL_sizeXY,            // its size X
              0.5*CAGL_sizeXY,            // its size Y
              0.5*CAGL_sizeZ);            // its size Z

  G4LogicalVolume *CAGL_log = 
    new G4LogicalVolume(CAGL_solid,       // its solid
                        CAGL_mat,         // its material
                        "CAGL");          // its name

  for (G4int i = 0; i < nLayer; i++) {
    for (G4int j = 0; j < 2; j++) {
      G4ThreeVector CAGL_position = G4ThreeVector(0., 0., (0.48 + j*4.03 + i*8.09)*mm);
      new G4PVPlacement(0,                  // no rotation
                        CAGL_position,      // at its position
                        CAGL_log,           // its logical volume
                        "CAGL",             // its name
                        world_log,          // its mother volume
                        false,              // no boolean operation
                        i*2 + j,            // copy number
                        checkOverlaps);     // overlaps checking
    }
  }

  //---------------------------------------- G10C --------------------------------------------------

  G4Material *G10C_mat = new G4Material("G10C", density = 1.70*g/cm3, nElem = 4);
  G10C_mat->AddElement(nist->FindOrBuildElement("H"),  2*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("C"), 15*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("O"), 30*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("Si"),53*perCent);

  G4double G10C_sizeXY =  246*mm;
  G4double G10C_sizeZ  = 1.20*mm;

  G4Box *G10C_solid =
    new G4Box("G10C",                     // its name
              0.5*G10C_sizeXY,            // its size X
              0.5*G10C_sizeXY,            // its size Y
              0.5*G10C_sizeZ);            // its size Z

  G4LogicalVolume *G10C_log = 
    new G4LogicalVolume(G10C_solid,       // its solid
                        G10C_mat,         // its material
                        "G10C");          // its name

  for (G4int i = 0; i < nLayer; i++) {
    for (G4int j = 0; j < 2; j++) {
      G4ThreeVector G10C_position = G4ThreeVector(0., 0., (1.13 + j*4.03 + i*8.09)*mm);
      new G4PVPlacement(0,                  // no rotation
                        G10C_position,      // at its position
                        G10C_log,           // its logical volume
                        "G10C",             // its name
                        world_log,          // its mother volume
                        false,              // no boolean operation
                        i*2 + j,            // copy number
                        checkOverlaps);     // overlaps checking
    }
  }

  //---------------------------------------- CAKA --------------------------------------------------

  G4Material *CAKA_mat = new G4Material("KAOLINITE", density = 2.594*g/cm3, nElem = 4);
  CAKA_mat->AddElement(nist->FindOrBuildElement("H"),  1.56*perCent);
  CAKA_mat->AddElement(nist->FindOrBuildElement("O"), 55.78*perCent);
  CAKA_mat->AddElement(nist->FindOrBuildElement("Al"),20.90*perCent);
  CAKA_mat->AddElement(nist->FindOrBuildElement("Si"),21.76*perCent);

  G4double CAKA_sizeXY =  246*mm;
  G4double CAKA_sizeZ  = 0.05*mm;

  G4Box *CAKA_solid =
    new G4Box("CAKA",                     // its name
              0.5*CAKA_sizeXY,            // its size X
              0.5*CAKA_sizeXY,            // its size Y
              0.5*CAKA_sizeZ);            // its size Z

  G4LogicalVolume *CAKA_log = 
    new G4LogicalVolume(CAKA_solid,       // its solid
                        CAKA_mat,         // its material
                        "CAKA");          // its name

  for (G4int i = 0; i < nLayer; i++) {
    for (G4int j = 0; j < 2; j++) {
      G4ThreeVector CAKA_position = G4ThreeVector(0., 0., (1.755 + j*4.03 + i*8.09)*mm);
      new G4PVPlacement(0,                  // no rotation
                        CAKA_position,      // at its position
                        CAKA_log,           // its logical volume
                        "CAKA",             // its name
                        world_log,          // its mother volume
                        false,              // no boolean operation
                        i*2 + j,            // copy number
                        checkOverlaps);     // overlaps checking
    }
  }

  //---------------------------------------- CAAB --------------------------------------------------

  G4Material *CAAB_mat = new G4Material("Tunga", density = 18.925*g/cm3, nElem = 3);
  CAAB_mat->AddElement(nist->FindOrBuildElement("Ni"), 0.83*perCent);
  CAAB_mat->AddElement(nist->FindOrBuildElement("Cu"), 0.89*perCent);
  CAAB_mat->AddElement(nist->FindOrBuildElement("W"), 98.28*perCent);

  G4double CAAB_sizeXY =  246*mm;
  G4double CAAB_sizeZ  = 2.63*mm;

  G4Box *CAAB_solid =
    new G4Box("CAAB",                     // its name
              0.5*CAAB_sizeXY,            // its size X
              0.5*CAAB_sizeXY,            // its size Y
              0.5*CAAB_sizeZ);            // its size Z

  G4LogicalVolume *CAAB_log = 
    new G4LogicalVolume(CAAB_solid,       // its solid
                        CAAB_mat,         // its material
                        "CAAB");          // its name

  for (G4int i = 0; i < nLayer; i++) {
    G4ThreeVector CAAB_position = G4ThreeVector(0., 0., (3.095 + i*8.09)*mm);
    new G4PVPlacement(0,                  // no rotation
                      CAAB_position,      // at its position
                      CAAB_log,           // its logical volume
                      "CAAB",             // its name
                      world_log,          // its mother volume
                      false,              // no boolean operation
                      i,                  // copy number
                      checkOverlaps);     // overlaps checking
  }

  //--------------------------------------- Return -------------------------------------------------

  return world_phys;
}

void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector *SD = new SensitiveDetector("SensitiveDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(SD);
  CAST_log->SetSensitiveDetector(SD);
}
