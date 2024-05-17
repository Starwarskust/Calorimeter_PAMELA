#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4NistManager *nist = G4NistManager::Instance();
  G4bool check_overlaps = false;

  G4double absorber_thickness  = 2.63*mm;
  G4double kapton_thickness    = 0.05*mm;
  G4double glue_thickness      = 0.10*mm;
  G4double G10C_thickness      = 1.20*mm;
  G4double kaolinite_thickness = 0.05*mm;
  G4double strip_thickness     = 0.38*mm;
  G4double gap_thickness       = 1.90*mm;

  G4int n_strips = 32;
  G4double strip_lenght = 78.08*mm;
  G4double strip_width  =  2.44*mm;

  G4int n_layers = 22;
  G4double layer_thickness = absorber_thickness + 2 * (kapton_thickness + glue_thickness + 
                             G10C_thickness + kaolinite_thickness + strip_thickness) + gap_thickness;
  G4double calorimeter_thickness = n_layers * layer_thickness;
  G4double calorimeter_side_length = 246.*mm;

  G4double density = 0.;
  G4int n_elem = 0;

  //------ World -----------------------------------------------------------------------------------

  G4double sizeX = 1.2 * calorimeter_side_length;
  G4double sizeY = 1.2 * calorimeter_side_length;
  G4double sizeZ = 2.4 * calorimeter_thickness;
  G4Box *world_svol = new G4Box("world", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume *world_lvol = new G4LogicalVolume(world_svol, world_mat, "world");

  G4VPhysicalVolume *world_pvol =
  new G4PVPlacement(nullptr,            // no rotation
                    G4ThreeVector(),    // position
                    world_lvol,         // logical volume
                    "world",            // name
                    nullptr,            // mother volume
                    false,              // no boolean operation
                    0,                  // copy number
                    check_overlaps);    // overlaps checking

  //------ Calorimeter -----------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = calorimeter_thickness;
  G4Box *calo_svol = new G4Box("calorimeter", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *calo_mat = nist->FindOrBuildMaterial("G4_N");
  G4LogicalVolume *calo_lvol = new G4LogicalVolume(calo_svol, calo_mat, "calorimeter");

  new G4PVPlacement(nullptr,                        // no rotation
                    G4ThreeVector(0, 0, sizeZ/2),   // position
                    calo_lvol,                      // logical volume
                    "calorimeter",                  // name
                    world_lvol,                     // mother volume
                    false,                          // no boolean operation
                    0,                              // copy number
                    check_overlaps);                // overlaps checking

  //------ Layer -----------------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = layer_thickness;
  G4Box *layer_svol = new G4Box("layer", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *layer_mat = nist->FindOrBuildMaterial("G4_C");
  G4LogicalVolume *layer_lvol = new G4LogicalVolume(layer_svol, layer_mat, "layer");

  new G4PVReplica("layer",              // name
                  layer_lvol,           // logical volume
                  calo_lvol,            // mother volume
                  kZAxis,               // axis of replication
                  n_layers,             // number of replica
                  layer_thickness);     // witdth of replica

  //------ Absorber (CAAB) -------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = absorber_thickness;
  G4Box *absorb_svol = new G4Box("absorber", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *absorb_mat = new G4Material("Tunga", density = 18.925*g/cm3, n_elem = 3);
  absorb_mat->AddElement(nist->FindOrBuildElement("Ni"), 0.83*perCent);
  absorb_mat->AddElement(nist->FindOrBuildElement("Cu"), 0.89*perCent);
  absorb_mat->AddElement(nist->FindOrBuildElement("W"), 98.28*perCent);
  G4LogicalVolume *absorb_lvol = new G4LogicalVolume(absorb_svol, absorb_mat, "absorber");

  G4double shift = -gap_thickness/2;
  new G4PVPlacement(nullptr,                        // no rotation
                    G4ThreeVector(0, 0, shift),     // position
                    absorb_lvol,                    // logical volume
                    "absorber",                     // name
                    layer_lvol,                     // mother volume
                    false,                          // no boolean operation
                    0,                              // copy number
                    check_overlaps);                // overlaps checking

  //------ Kapton (CAKP) ---------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = kapton_thickness;
  G4Box *kapton_svol = new G4Box("kapton", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *kapton_mat = nist->FindOrBuildMaterial("G4_KAPTON");
  G4LogicalVolume *kapton_lvol = new G4LogicalVolume(kapton_svol, kapton_mat, "kapton");

  for (G4int i = 0; i < 2; i++) {
    shift = -gap_thickness/2 - std::pow(-1, i) * (absorber_thickness/2 + kapton_thickness/2);
    new G4PVPlacement(nullptr,                        // no rotation
                      G4ThreeVector(0, 0, shift),     // position
                      kapton_lvol,                    // logical volume
                      "kapton",                       // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Glue (CAGL) -----------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = glue_thickness;
  G4Box *glue_svol = new G4Box("glue", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *glue_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume *glue_lvol = new G4LogicalVolume(glue_svol, glue_mat, "glue");

  for (G4int i = 0; i < 2; i++) {
    shift = -gap_thickness/2 - std::pow(-1, i) * (absorber_thickness/2 + kapton_thickness +
            glue_thickness/2);
    new G4PVPlacement(nullptr,                        // no rotation
                      G4ThreeVector(0, 0, shift),     // position
                      glue_lvol,                      // logical volume
                      "glue",                         // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Electronics (G10C) ----------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = G10C_thickness;
  G4Box *G10C_svol = new G4Box("G10C", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *G10C_mat = new G4Material("G10C", density = 1.70*g/cm3, n_elem = 4);
  G10C_mat->AddElement(nist->FindOrBuildElement("H"),  2*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("C"), 15*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("O"), 30*perCent);
  G10C_mat->AddElement(nist->FindOrBuildElement("Si"),53*perCent);
  G4LogicalVolume *G10C_lvol = new G4LogicalVolume(G10C_svol, G10C_mat, "G10C");

  for (G4int i = 0; i < 2; i++) {
    shift = -gap_thickness/2 - std::pow(-1, i) * (absorber_thickness/2 + kapton_thickness +
            glue_thickness + G10C_thickness/2);
    new G4PVPlacement(nullptr,                        // no rotation
                      G4ThreeVector(0, 0, shift),     // position
                      G10C_lvol,                      // logical volume
                      "G10C",                         // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Kaolinite (CAKA) ------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = kaolinite_thickness;
  G4Box *kaolin_svol = new G4Box("kaolinite", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *kaolin_mat = new G4Material("KAOLINITE", density = 2.594*g/cm3, n_elem = 4);
  kaolin_mat->AddElement(nist->FindOrBuildElement("H"),  1.56*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("O"), 55.78*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("Al"),20.90*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("Si"),21.76*perCent);
  G4LogicalVolume *kaolin_lvol = new G4LogicalVolume(kaolin_svol, kaolin_mat, "kaolinite");

  for (G4int i = 0; i < 2; i++) {
    shift = -gap_thickness/2 - std::pow(-1, i) * (absorber_thickness/2 + kapton_thickness +
            glue_thickness + G10C_thickness + kaolinite_thickness/2);
    new G4PVPlacement(nullptr,                        // no rotation
                      G4ThreeVector(0, 0, shift),     // position
                      kaolin_lvol,                    // logical volume
                      "kaolinite",                    // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Plane (CAPL) ----------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = strip_thickness;
  G4Box *plane_svol = new G4Box("plane", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *plane_mat = nist->FindOrBuildMaterial("G4_N");
  G4LogicalVolume *plane_lvol = new G4LogicalVolume(plane_svol, plane_mat, "plane");

  for (G4int i = 0; i < 2; i++) {
    G4RotationMatrix rotation(0, 0, 0);
    rotation.rotateZ(i * CLHEP::halfpi);
    shift = -gap_thickness/2 - std::pow(-1, i) * (absorber_thickness/2 + kapton_thickness +
            glue_thickness + G10C_thickness + kaolinite_thickness + strip_thickness/2);
    G4Transform3D transform(rotation, G4ThreeVector(0, 0, shift));
    new G4PVPlacement(transform,		   		            //rotation + position
                      plane_lvol,                     // logical volume
                      "plane",                        // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Paddle (CASI) ---------------------------------------------------------------------------

  sizeX = strip_lenght;
  sizeY = strip_lenght;
  sizeZ = strip_thickness;
  G4Box *paddle_svol = new G4Box("paddle", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *paddle_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume *paddle_lvol = new G4LogicalVolume(paddle_svol, paddle_mat, "paddle");

  G4int ind = 0;
  for (G4int iy = -1; iy <= 1; iy++) {
    for (G4int ix = -1; ix <= 1; ix++) {
      G4ThreeVector paddle_position = G4ThreeVector(ix * (strip_lenght + 2.42*mm),
                                                    iy * (strip_lenght + 2.42*mm), 0);
      new G4PVPlacement(nullptr,                        // no rotation
                        paddle_position,                // position
                        paddle_lvol,                    // logical volume
                        "paddle",                       // name
                        plane_lvol,                     // mother volume
                        false,                          // no boolean operation
                        ind,                            // copy number
                        check_overlaps);                // overlaps checking
      ind++;
    }
  }

  //------ Strip (CAST) ----------------------------------------------------------------------------

  sizeX = strip_lenght;
  sizeY = strip_width;
  sizeZ = strip_thickness;
  G4Box *strip_svol = new G4Box("strip", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *strip_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume *strip_lvol = new G4LogicalVolume(strip_svol, strip_mat, "strip");

  new G4PVReplica("strip",              // name
                  strip_lvol,           // logical volume
                  paddle_lvol,          // mother volume
                  kYAxis,               // axis of replication
                  n_strips,             // number of replica
                  strip_width);         // witdth of replica

  //------ Pad -------------------------------------------------------------------------------------

  sizeX = strip_width;
  sizeY = strip_width;
  sizeZ = strip_thickness;
  G4Box *pad_svol = new G4Box("pad", sizeX/2, sizeY/2, sizeZ/2);

  G4Material *pad_mat = nist->FindOrBuildMaterial("G4_Si");
  pad_lvol = new G4LogicalVolume(pad_svol, pad_mat, "pad");

  new G4PVReplica("pad",                // name
                  pad_lvol,             // logical volume
                  strip_lvol,           // mother volume
                  kXAxis,               // axis of replication
                  n_strips,             // number of replica
                  strip_width);         // witdth of replica

  //------ Visualization attributes ----------------------------------------------------------------

  absorb_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Brown()));
  kapton_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));
  glue_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));
  G10C_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
  kaolin_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  strip_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  return world_pvol;
}

void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector *SD = new SensitiveDetector("SensitiveDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(SD);
  pad_lvol->SetSensitiveDetector(SD);
}
