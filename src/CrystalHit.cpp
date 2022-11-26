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
/// \file CrystalHit.cpp
/// \brief Implementation of the CrystalHit class

#include "CrystalHit.h"

#include <G4UnitsTable.hh>

#include <iomanip>

G4ThreadLocal G4Allocator<CrystalHit>* CrystalHitAllocator{ nullptr };

CrystalHit::CrystalHit()
{
}

CrystalHit::~CrystalHit()
{
}

G4bool CrystalHit::operator==(const CrystalHit& right) const
{
	return (this == &right) ? true : false;
}

void CrystalHit::Print()
{
	G4cout
		<< "Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy")
		<< "Chan: " << std::setw(7) << G4BestUnit(fChan, "Energy")
		<< G4endl;
}