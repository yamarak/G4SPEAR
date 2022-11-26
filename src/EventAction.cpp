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
/// \file EventAction.cpp
/// \brief Implementation of the EventAction class

#include "EventAction.h"

#include <G4AnalysisManager.hh>
#include <G4Event.hh>
#include <G4SDManager.hh>

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
	// Get hits collections IDs (only once)
	if (fHCID == -1)
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalHitsCollection");

	// Get hits collection
	auto crystalHC = GetHitsCollection(fHCID, anEvent);

	// Get hit with total values
	auto crystalHit = (*crystalHC)[crystalHC->entries() - 1];

	// Fill histograms, ntuple

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	// Fill histograms
	analysisManager->FillH1(0, crystalHit->GetEdep());
	analysisManager->FillH1(1, crystalHit->GetChan());

	// Fill ntuple
	analysisManager->FillNtupleDColumn(0, crystalHit->GetEdep());
	analysisManager->FillNtupleDColumn(1, crystalHit->GetChan());
	analysisManager->AddNtupleRow();
}

CrystalHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* anEvent) const
{
	auto hitsCollection = static_cast<CrystalHitsCollection*>(anEvent->GetHCofThisEvent()->GetHC(hcID));

	if (!hitsCollection)
	{
		G4ExceptionDescription msg;
		msg << "Cannot access hitsCollection ID " << hcID;
		G4Exception("EventAction::GetHitsCollection()",
					"MyCode0003", FatalException, msg);
	}

	return hitsCollection;
}
