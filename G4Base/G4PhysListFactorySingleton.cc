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
// $Id: G4PhysListFactorySingleton.cc,v 1.1 2011-10-17 17:16:07 rhatcher Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:  G4PhysListFactorySingleton
//
// Author: 2011-10-06  R. Hatcher
//
// Modified:
//
//----------------------------------------------------------------------------
//

#include "G4PhysListFactorySingleton.hh"

// Define static variable which holds the one-and-only instance
G4PhysListFactorySingleton* G4PhysListFactorySingleton::fgTheInstance;

G4PhysListFactorySingleton::G4PhysListFactorySingleton() 
{
  defName = "QGSP_BERT";  // default list to use if no PHYSLIST defined
  fgTheInstance = this;   // record created self in static pointer
}

G4PhysListFactorySingleton::~G4PhysListFactorySingleton()
{
  fgTheInstance = 0;
}

G4PhysListFactorySingleton& G4PhysListFactorySingleton::Instance()
{
  // Clean dtor calls G4PhysListFactorySingleton dtor at job end
  static Cleaner cleaner;

  if ( ! fgTheInstance ) {
    // need to create one
    cleaner.UseMe();   // dummy call to quiet compiler warnings
    fgTheInstance = new G4PhysListFactorySingleton();
  }
  
  return *fgTheInstance;
}

G4VModularPhysicsList* G4PhysListFactorySingleton::ReferencePhysList()
{
  // instantiate PhysList by environment variable "PHYSLIST"
  G4String name = "";
  char* path = getenv("PHYSLIST");
  if (path) {
    name = G4String(path);
  } else {
    name = defName;
    G4cout << "### G4PhysListFactorySingleton WARNING: "
	   << " environment variable PHYSLIST is not defined"
	   << G4endl
	   << "    Default Physics Lists " << name 
	   << " is instantiated" 
	   << G4endl;
  }
  return GetReferencePhysList(name);
}

G4VModularPhysicsList* 
G4PhysListFactorySingleton::GetReferencePhysList(const G4String& name)
{
  G4VModularPhysicsList* p = 0;
  
  // Replacing the very fixed list of if/else with a std::map lookup
  /*
  if(name == "CHIPS")              {p = new CHIPS();}
  else if(name == "FTFP_BERT")     {p = new FTFP_BERT();}
  else if(name == "FTFP_BERT_EMV") {p = new FTFP_BERT_EMV();}
  else if(name == "FTFP_BERT_EMX") {p = new FTFP_BERT_EMX();}
  else if(name == "FTFP_BERT_TRV") {p = new FTFP_BERT_TRV();}
  else if(name == "FTF_BIC")       {p = new FTF_BIC();}
  else if(name == "LBE")           {p = new LBE();}
  else if(name == "LHEP")          {p = new LHEP();}
  else if(name == "LHEP_EMV")      {p = new LHEP_EMV();}
  else if(name == "QBBC")          {p = new QBBC();}
  else if(name == "QGSC_BERT")     {p = new QGSC_BERT();}
  else if(name == "QGSP")          {p = new QGSP();}
  else if(name == "QGSP_BERT")     {p = new QGSP_BERT();}
  else if(name == "QGSP_BERT_EMV") {p = new QGSP_BERT_EMV();}
  else if(name == "QGSP_BERT_EMX") {p = new QGSP_BERT_EMX();}
  else if(name == "QGSP_BERT_HP")  {p = new QGSP_BERT_HP();}
  else if(name == "QGSP_BERT_NOLEP") {p = new QGSP_BERT_NOLEP();}
  else if(name == "QGSP_BERT_TRV") {p = new QGSP_BERT_TRV();}
  else if(name == "QGSP_BERT_CHIPS") {p = new QGSP_BERT_CHIPS();}
  else if(name == "QGSP_BIC")      {p = new QGSP_BIC();}
  else if(name == "QGSP_BIC_EMY")  {p = new QGSP_BIC_EMY();}
  else if(name == "QGSP_BIC_HP")   {p = new QGSP_BIC_HP();}
  else if(name == "QGSP_FTFP_BERT"){p = new QGSP_FTFP_BERT();}
  else if(name == "QGS_BIC")       {p = new QGS_BIC();}
  else if(name == "QGSP_INCL_ABLA"){p = new QGSP_INCL_ABLA();}
  else if(name == "Shielding")     {p = new Shielding();}
  */

  // we don't want map creating an entry if it doesn't exist
  // so use map::find() not map::operator[]
  std::map<G4String, PhysListCtorFuncPtr_t>::iterator itr
    = fFunctionMap.find(name);
  if ( fFunctionMap.end() != itr ) { 
    // found an appropriate entry in the list
    PhysListCtorFuncPtr_t foo = itr->second;  // this is the function
    p = (*foo)();  // use function to create the physics list
  }
  if ( ! p ) {
    G4cout << "### G4PhysListFactorySingleton WARNING: "
	   << "PhysicsList " << name << " is not known"
	   << G4endl;
  }
  return p;
}
  
G4bool G4PhysListFactorySingleton::IsReferencePhysList(const G4String& name)
{
  //  check if we know the name
  G4bool res = false;
  std::map<G4String, PhysListCtorFuncPtr_t>::iterator itr
    = fFunctionMap.find(name);
  if ( fFunctionMap.end() != itr ) res = true;
  return res;
}

const std::vector<G4String>& 
G4PhysListFactorySingleton::AvailablePhysLists() const
{
  // list of names might be out of date due to new registrations
  // rescan the std::map on each call (which won't be frequent)
  listnames.clear();

  // scan map for registered names
  std::map<G4String, PhysListCtorFuncPtr_t>::const_iterator itr;
  for ( itr = fFunctionMap.begin(); itr != fFunctionMap.end(); ++itr )
    listnames.push_back(itr->first);

  return listnames;
}

G4bool G4PhysListFactorySingleton::RegisterCreator(G4String name, 
                                                   PhysListCtorFuncPtr_t foo )
{
  // record new functions for creating lists
  fFunctionMap[name] = foo;
  return true;
}

/// !!!!!! register existing classes without disturbing their .cc files (yet)
#include "G4PhysListRegisterOld.icc"
