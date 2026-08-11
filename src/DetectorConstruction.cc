#include "DetectorConstruction.hh"

#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4PVParameterised.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4SDManager.hh>

#include "StripParameterisation.hh"
#include "SensitiveDetector.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4bool check_overlaps = false;

  struct LayerComposition
  {
    G4double absorber   = 2.63*mm;
    G4double kapton     = 0.05*mm;
    G4double glue       = 0.10*mm;
    G4double g10c       = 1.20*mm;
    G4double kaolinite  = 0.05*mm;
    G4double strip      = 0.38*mm;
  } layer;
  G4double gas_gap = 1.90*mm;
  G4double layer_thickness = layer.absorber + 2 * (layer.kapton + layer.glue +
                             layer.g10c + layer.kaolinite + layer.strip) + gas_gap;

  G4int n_layers = 22;
  G4double calorimeter_thickness = n_layers * layer_thickness;
  G4double calorimeter_side_length = 246.*mm;

  G4double strip_length = 78.08*mm;
  G4double strip_width  =  2.44*mm;
  G4double pad_gap      =  2.42*mm;

  G4double density = 0.;
  G4int n_elem = 0;

  //------ World -----------------------------------------------------------------------------------

  G4double sizeX = 1.2 * calorimeter_side_length;
  G4double sizeY = 1.2 * calorimeter_side_length;
  G4double sizeZ = 2.4 * calorimeter_thickness;
  G4Box* world_svol = new G4Box("world", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume* world_lvol = new G4LogicalVolume(world_svol, world_mat, "world");

  G4VPhysicalVolume* world_pvol =
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
  G4Box* calo_svol = new G4Box("calorimeter", sizeX / 2, sizeY / 2, sizeZ / 2);

  G4Material* calo_mat = nist->FindOrBuildMaterial("G4_N");
  G4LogicalVolume* calo_lvol = new G4LogicalVolume(calo_svol, calo_mat, "calorimeter");

  new G4PVPlacement(nullptr,                        // no rotation
                    G4ThreeVector(0, 0, sizeZ / 2), // position
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
  G4Box* layer_svol = new G4Box("layer", sizeX / 2, sizeY / 2, sizeZ / 2);

  G4Material* layer_mat = nist->FindOrBuildMaterial("G4_C");
  G4LogicalVolume* layer_lvol = new G4LogicalVolume(layer_svol, layer_mat, "layer");

  new G4PVReplica("layer",              // name
                  layer_lvol,           // logical volume
                  calo_lvol,            // mother volume
                  kZAxis,               // axis of replication
                  n_layers,             // number of replica
                  layer_thickness);     // width of replica

  //------ Absorber (CAAB) -------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = layer.absorber;
  G4Box* absorber_svol = new G4Box("absorber", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* absorber_mat = new G4Material("Tunga", density = 18.925*g/cm3, n_elem = 3);
  absorber_mat->AddElement(nist->FindOrBuildElement("Ni"), 0.83*perCent);
  absorber_mat->AddElement(nist->FindOrBuildElement("Cu"), 0.89*perCent);
  absorber_mat->AddElement(nist->FindOrBuildElement("W"), 98.28*perCent);
  G4LogicalVolume* absorber_lvol = new G4LogicalVolume(absorber_svol, absorber_mat, "absorber");

  G4double shift = -gas_gap/2;
  new G4PVPlacement(nullptr,                        // no rotation
                    G4ThreeVector(0, 0, shift),     // position
                    absorber_lvol,                  // logical volume
                    "absorber",                     // name
                    layer_lvol,                     // mother volume
                    false,                          // no boolean operation
                    0,                              // copy number
                    check_overlaps);                // overlaps checking

  //------ Kapton (CAKP) ---------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = layer.kapton;
  G4Box* kapton_svol = new G4Box("kapton", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* kapton_mat = nist->FindOrBuildMaterial("G4_KAPTON");
  G4LogicalVolume* kapton_lvol = new G4LogicalVolume(kapton_svol, kapton_mat, "kapton");

  for (G4int i = 0; i < 2; i++) {
    shift = -gas_gap/2 - std::pow(-1, i) * (layer.absorber/2 + layer.kapton/2);
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
  sizeZ = layer.glue;
  G4Box* glue_svol = new G4Box("glue", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* glue_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume* glue_lvol = new G4LogicalVolume(glue_svol, glue_mat, "glue");

  for (G4int i = 0; i < 2; i++) {
    shift = -gas_gap/2 - std::pow(-1, i) * (layer.absorber/2 + layer.kapton + layer.glue/2);
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
  sizeZ = layer.g10c;
  G4Box* g10c_svol = new G4Box("G10C", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* g10c_mat = new G4Material("G10C", density = 1.70*g/cm3, n_elem = 4);
  g10c_mat->AddElement(nist->FindOrBuildElement("H"),   2*perCent);
  g10c_mat->AddElement(nist->FindOrBuildElement("C"),  15*perCent);
  g10c_mat->AddElement(nist->FindOrBuildElement("O"),  30*perCent);
  g10c_mat->AddElement(nist->FindOrBuildElement("Si"), 53*perCent);
  G4LogicalVolume* g10c_lvol = new G4LogicalVolume(g10c_svol, g10c_mat, "G10C");

  for (G4int i = 0; i < 2; i++) {
    shift = -gas_gap/2 - std::pow(-1, i) * (layer.absorber/2 + layer.kapton + layer.glue + layer.g10c/2);
    new G4PVPlacement(nullptr,                        // no rotation
                      G4ThreeVector(0, 0, shift),     // position
                      g10c_lvol,                      // logical volume
                      "G10C",                         // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Kaolinite (CAKA) ------------------------------------------------------------------------

  sizeX = calorimeter_side_length;
  sizeY = calorimeter_side_length;
  sizeZ = layer.kaolinite;
  G4Box* kaolin_svol = new G4Box("kaolinite", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* kaolin_mat = new G4Material("KAOLINITE", density = 2.594*g/cm3, n_elem = 4);
  kaolin_mat->AddElement(nist->FindOrBuildElement("H"),   1.56*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("O"),  55.78*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("Al"), 20.90*perCent);
  kaolin_mat->AddElement(nist->FindOrBuildElement("Si"), 21.76*perCent);
  G4LogicalVolume* kaolin_lvol = new G4LogicalVolume(kaolin_svol, kaolin_mat, "kaolinite");

  for (G4int i = 0; i < 2; i++) {
    shift = -gas_gap/2 - std::pow(-1, i) * (layer.absorber/2 + layer.kapton + layer.glue + layer.g10c + layer.kaolinite/2);
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
  sizeZ = layer.strip;
  G4Box* plane_svol = new G4Box("plane", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* plane_mat = nist->FindOrBuildMaterial("G4_N");
  G4LogicalVolume* plane_lvol = new G4LogicalVolume(plane_svol, plane_mat, "plane");

  for (G4int i = 0; i < 2; i++) {
    G4RotationMatrix rotation(0, 0, 0);
    rotation.rotateZ(i * CLHEP::halfpi);
    shift = -gas_gap/2 - std::pow(-1, i) * (layer.absorber/2 + layer.kapton + layer.glue + layer.g10c + layer.kaolinite + layer.strip/2);
    G4Transform3D transform(rotation, G4ThreeVector(0, 0, shift));
    new G4PVPlacement(transform,		   		            //rotation + position
                      plane_lvol,                     // logical volume
                      "plane",                        // name
                      layer_lvol,                     // mother volume
                      false,                          // no boolean operation
                      i,                              // copy number
                      check_overlaps);                // overlaps checking
  }

  //------ Strip (CAST) ----------------------------------------------------------------------------

  sizeX = 3 * strip_length + 2 * pad_gap;
  sizeY = strip_width;
  sizeZ = layer.strip;
  G4Box* strip_svol = new G4Box("strip", sizeX/2, sizeY/2, sizeZ/2);

  G4Material* strip_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume* strip_lvol = new G4LogicalVolume(strip_svol, strip_mat, "strip");
  sensitive_lvol = strip_lvol;

  G4VPVParameterisation* strip_pvol = new StripParameterisation(strip_width, pad_gap);
  new G4PVParameterised("strip",
                        strip_lvol,
                        plane_lvol,
                        kYAxis,
                        96,
                        strip_pvol);

  //------ Visualization attributes ----------------------------------------------------------------

  absorber_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Brown()));
  kapton_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));
  glue_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));
  g10c_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
  kaolin_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  strip_lvol->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  return world_pvol;
}

void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector* sd = new SensitiveDetector("SensitiveDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  sensitive_lvol->SetSensitiveDetector(sd);
}
