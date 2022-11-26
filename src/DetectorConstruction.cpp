//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.cpp
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.h"
#include "CrystalHit.h"
#include "CrystalSD.h"

#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
//#include <G4CutTubs.hh>
//#include <G4Cons.hh>
//#include <G4Para.hh>
//#include <G4Trd.hh>
//#include <G4Trap.hh>
//#include <G4Sphere.hh>
//#include <G4Orb.hh>
//#include <G4Torus.hh>
//#include <G4Polycone.hh>
//#include <G4GenericPolycone.hh>
//#include <G4Polyhedra.hh>
//#include <G4EllipticalTube.hh>
//#include <G4Ellipsoid.hh>
//#include <G4EllipticalCone.hh>
//#include <G4Paraboloid.hh>
//#include <G4Hype.hh>
//#include <G4Tet.hh>
//#include <G4ExtrudedSolid.hh>
//#include <G4TwistedBox.hh>
//#include <G4TwistedTrap.hh>
//#include <G4TwistedTrd.hh>
//#include <G4GenericTrap.hh>
//#include <G4TwistedTubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Get nist material manager
	auto* nist = G4NistManager::Instance();

	// CZT Material
	auto* CZT = new G4Material("CZT",
							   5.78 * g / cm3,
							   3,
							   kStateSolid);
	CZT->AddElement(nist->FindOrBuildElement(48), 9);
	CZT->AddElement(nist->FindOrBuildElement(30), 1);
	CZT->AddElement(nist->FindOrBuildElement(52), 10);

	// Option to switch on/off checking of volumes overlaps
	constexpr G4bool fCheckOverlaps = true;

	// World

	constexpr G4double worldLength{ 1 * m };
	const G4String worldName{ "World" };
	auto* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	auto* worldSolid = new G4Box(worldName,
								 worldLength / 2,
								 worldLength / 2,
								 worldLength / 2);
	auto* worldLV = new G4LogicalVolume(worldSolid,
										worldMaterial,
										worldName);
	auto* worldPV = new G4PVPlacement(G4Transform3D(),
									  worldLV,
									  worldName,
									  nullptr,
									  false,
									  0,
									  fCheckOverlaps);

	// SPEAR

	constexpr G4double spearDiameter{ 13 * mm };
	constexpr G4double spearLength{ 89 * mm };
	const G4String spearName{ "SPEAR" };
	const G4Transform3D spearTransform3D =
		G4TranslateX3D(spearLength / 2) * G4RotateY3D(halfpi);
	auto* spearMaterial = nist->FindOrBuildMaterial("G4_AIR");
	auto* spearSolid = new G4Tubs(spearName,
								  0,
								  spearDiameter / 2,
								  spearLength / 2,
								  0,
								  twopi);
	auto* spearLV = new G4LogicalVolume(spearSolid,
										spearMaterial,
										spearName);
	new G4PVPlacement(spearTransform3D,
					  spearLV,
					  spearName,
					  worldLV,
					  false,
					  0,
					  fCheckOverlaps);

	// Crystal

	constexpr G4double crystalLength{ 5 * mm };
	const G4String crystalName{ "Crystal" };
	const G4Transform3D crystalTransform3D =
		G4TranslateZ3D((crystalLength - spearLength) / 2 + 8.38 * mm);
	auto* crystalSolid = new G4Box(crystalName,
								   crystalLength / 2,
								   crystalLength / 2,
								   crystalLength / 2);
	auto* crystalLV = new G4LogicalVolume(crystalSolid,
										  CZT,
										  "crystalLV");
	new G4PVPlacement(crystalTransform3D,
					  crystalLV,
					  crystalName,
					  spearLV,
					  false,
					  0,
					  fCheckOverlaps);

	return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
	//G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
	
	// Sensitive detector
	auto crystalSD = new CrystalSD("CrystalSD",
								   "CrystalHitsCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(crystalSD);
	SetSensitiveDetector("crystalLV", crystalSD);
}
