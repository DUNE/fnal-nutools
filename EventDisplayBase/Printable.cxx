////////////////////////////////////////////////////////////////////////
// $Id: Printable.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
//
// Base class for printable objects
//
// messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/Printable.h"
#include <iostream>
#include <cstdlib>
using namespace evdb;

// ClassImp(evdb::Printable)

static std::map<std::string,Printable*> gsPrintables;

//......................................................................

Printable::Printable() { }

//......................................................................

Printable::~Printable() 
{
  Printable::RemoveFromListOfPrintables(this);
}

//......................................................................

void Printable::AddToListOfPrintables(const char* name, 
					 Printable* p) 
 {
  std::string s(name);

  if (gsPrintables[s] == 0) {
    gsPrintables[s] = p;
  }
  else {
    if (gsPrintables[s] != p) {
      std::cerr << "Printable: Name " << name << " reused.\n";
      std::abort();
    }
  }
}

//......................................................................

void Printable::RemoveFromListOfPrintables(Printable* p) 
{
  std::map<std::string,Printable*>::iterator itr(gsPrintables.begin());
  std::map<std::string,Printable*>::iterator itrEnd(gsPrintables.end());
  for (; itr!=itrEnd; ++itr) {
    if ( itr->second == p) {
      gsPrintables.erase(itr);
      return;
    }
  }
}

//......................................................................

std::map<std::string,Printable*>& Printable::GetPrintables() 
{
  return gsPrintables;
}

////////////////////////////////////////////////////////////////////////

