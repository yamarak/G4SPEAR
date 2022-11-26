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
/// \file CrystalHit.h
/// \brief Definition of the CrystalHit class

#pragma once

#ifndef CRYSTALHIT_H
#define CRYSTALHIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>

class CrystalHit : public G4VHit
{
public:
	CrystalHit();
	CrystalHit(const CrystalHit&) = default;
	~CrystalHit() override;

	// Operators

	CrystalHit& operator=(const CrystalHit&) = default;
	G4bool operator==(const CrystalHit&) const;

	inline void* operator new(size_t);
	inline void operator delete(void* aHit);

	// Methods from base class

	void Draw() override {}
	void Print() override;

	// Methods to handle data

	void Add(G4double de, G4double ch) { fEdep += de; fChan += ch; }

	// Get methods

	G4double GetEdep() const { return fEdep; }
	G4double GetChan() const { return fChan; }
private:
	G4double fEdep{ 0.0 };
	G4double fChan{ 0.0 };
};

using CrystalHitsCollection = G4THitsCollection<CrystalHit>;

extern G4ThreadLocal G4Allocator<CrystalHit>* CrystalHitAllocator;

inline void* CrystalHit::operator new(size_t)
{
	if (!CrystalHitAllocator)
		CrystalHitAllocator = new G4Allocator<CrystalHit>;
	return (void*)CrystalHitAllocator->MallocSingle();
}

inline void CrystalHit::operator delete(void* aHit)
{
	CrystalHitAllocator->FreeSingle((CrystalHit*)aHit);
}

#endif // !CRYSTALHIT_H
