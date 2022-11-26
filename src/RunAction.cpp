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
/// \file RunAction.cpp
/// \brief Implementation of the :RunAction class

#include "RunAction.h"

#include <G4AnalysisManager.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

RunAction::RunAction()
{
	// Set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);

	// Create analysis manager
	// The choice of the output format is done via the specified
	// file extension.
	auto analysisManager = G4AnalysisManager::Instance();

	// Create directories
	//analysisManager->SetHistoDirectoryName("histograms");
	//analysisManager->SetNtupleDirectoryName("ntuple");
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	// Note: merging ntuples is available only with Root output

	// Book histograms, ntuple

	// Creating histograms
	analysisManager->CreateH1("Edep",
							  "Energy Deposition in crystal",
							  4096,
							  0.0,
							  753.664 * keV);
	analysisManager->CreateH1("Spec",
							  "Spectrum in crystal",
							  4096,
							  0.0,
							  753.664 * keV);

	// Creating ntuple
	analysisManager->CreateNtuple("spear", "Edep and Spectrum");
	analysisManager->CreateNtupleDColumn("Edep");
	analysisManager->CreateNtupleDColumn("Spec");
	analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
	// Inform the runManager to save random number seed
	//G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	// Open an output file

	G4String fileName = "spear.root";
	// Other supported output types:
	// G4String fileName = "spear.csv";
	// G4String fileName = "spear.hdf5";
	// G4String fileName = "spear.xml";
	analysisManager->OpenFile(fileName);
	G4cout << "Using " << analysisManager->GetType() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	// Print histogram statistics

	auto analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->GetH1(1))
	{
		G4cout << G4endl << " ----> print histograms statistic ";
		if (isMaster)
			G4cout << "for the entire run " << G4endl << G4endl;
		else
			G4cout << "for the local thread " << G4endl << G4endl;

		G4cout << " Edep : mean = "
			<< G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy")
			<< " rms = "
			<< G4BestUnit(analysisManager->GetH1(0)->rms(), "Energy") << G4endl;

		G4cout << " Spec : mean = "
			<< G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
			<< " rms = "
			<< G4BestUnit(analysisManager->GetH1(1)->rms(), "Energy") << G4endl;
	}

	// Save histograms & ntuple
	analysisManager->Write();
	analysisManager->CloseFile();
}
