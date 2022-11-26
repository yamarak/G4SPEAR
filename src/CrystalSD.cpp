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
/// \file CrystalSD.cpp
/// \brief Implementation of the CrystalSD class

#include "CrystalSD.h"

#include <G4SDManager.hh>

CrystalSD::CrystalSD(G4String name, G4String hitsCollectionName)
	: G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);
}

CrystalSD::~CrystalSD()
{
}

void CrystalSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection
	fHitsCollection = new CrystalHitsCollection(SensitiveDetectorName,
												collectionName[0]);
	hce->AddHitsCollection(G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]), fHitsCollection);

	// Create hits
	// TODO: unerstand this
	fHitsCollection->insert(new CrystalHit());
}

void CrystalSD::EndOfEvent(G4HCofThisEvent*)
{
	if (verboseLevel > 1)
	{
		auto nofHits = fHitsCollection->entries();
		G4cout << G4endl
			<< "-------->Hits Collection: in this event they are "
			<< nofHits << " hits in the tracker chambers: " << G4endl;
		for (size_t i = 0; i < nofHits; i++)
			(*fHitsCollection)[i]->Print();
	}
}

G4bool CrystalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	// Energy deposit
	auto edep = aStep->GetTotalEnergyDeposit();

	if (edep == 0.0) return false;

	// Get hit accounting data for this cell
	auto hit = (*fHitsCollection)[0];
	if (!hit)
	{
		G4ExceptionDescription msg;
		msg << "Cannot access the hit.";
		G4Exception("CrystalSD::ProcessHits()",
					"MyCode0004", FatalException, msg);
	}

	// Get hit for total accounting
	auto hitTotal = (*fHitsCollection)[fHitsCollection->entries() - 1];

	// Add values
	hit->Add(edep, edep);
	if (hit && hitTotal)
		if (!(*hit == *hitTotal))
			hitTotal->Add(edep, edep);
	
	return true;
}
