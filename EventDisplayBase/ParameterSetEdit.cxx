///
/// \file  ParameterSetEdit.cxx
/// \brief Popup to edit configuration data
///
/// \version $Id: ParameterSetEdit.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
///
#include "EventDisplayBase/ParameterSetEdit.h"
#include "EventDisplayBase/NavState.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
extern "C" {
#include <sys/types.h>
#include <unistd.h>
}
#include "TROOT.h"
#include "TApplication.h"
#include "TGCanvas.h"
#include "TGLabel.h"
#include "TGWindow.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGScrollBar.h"
using namespace evdb;

static void parse_pset_string(const std::string& pset, 
			      std::vector<std::string>& names,
			      std::vector<std::string>& types,
			      std::vector<std::string>& values)
{
  // Parse out the content of the parameter set
  size_t istart = 0;
  size_t iend   = 0;
  while (1) {
    iend = pset.find(';',istart);
    
    std::string param = pset.substr(istart, iend-istart);

    size_t ieq = param.find('=');
    if (ieq == param.npos) { abort(); }
    
    std::string nm   = param.substr(0,ieq);
    std::string assn = param.substr(ieq+1,param.size());
    
    size_t paren = assn.find('(');
    if (paren==assn.npos) abort();
    
    std::string type  = assn.substr(0,paren);
    std::string value = assn.substr(paren+1, assn.size());

    size_t openbracket;
    size_t closebracket;
    size_t openparen;
    size_t closeparen;
    size_t opencurly;
    size_t closecurly;
    while (1) {
      openbracket = nm.find('<');
      if (openbracket!=nm.npos) nm.erase(openbracket,1);

      closebracket = nm.find('>');
      if (closebracket!=nm.npos) nm.erase(closebracket,1);

      openparen = nm.find('(');
      if (openparen!=nm.npos) nm.erase(openparen,1);

      closeparen = nm.find(')');
      if (closeparen!=nm.npos) nm.erase(closeparen,1);

      opencurly = nm.find('{');
      if (opencurly!=nm.npos) nm.erase(opencurly,1);

      closecurly = nm.find('}');
      if (closecurly!=nm.npos) nm.erase(closecurly,1);

      if (openbracket==nm.npos && closebracket==nm.npos &&
	  openparen  ==nm.npos && closeparen  ==nm.npos &&
	  opencurly  ==nm.npos && closecurly  ==nm.npos) break;
    }
    while (1) {
      openbracket = value.find('<');
      if (openbracket!=value.npos) value.erase(openbracket,1);

      closebracket = value.find('>');
      if (closebracket!=value.npos) value.erase(closebracket,1);

      openparen = value.find('(');
      if (openparen!=value.npos) value.erase(openparen,1);

      closeparen = value.find(')');
      if (closeparen!=value.npos) value.erase(closeparen,1);

      opencurly = value.find('{');
      if (opencurly!=value.npos) value.erase(opencurly,1);

      closecurly = value.find('}');
      if (closecurly!=value.npos) value.erase(closecurly,1);

      if (openbracket==value.npos && closebracket==value.npos &&
	  openparen  ==value.npos && closeparen  ==value.npos &&
	  opencurly  ==value.npos && closecurly  ==value.npos) break;
    }
    names. push_back(nm);
    types. push_back(type);
    values.push_back(value);
    
    if (iend==pset.npos) break;
    istart = iend+1;
  }
}


//......................................................................

ParamFrame::ParamFrame(const TGWindow* p, 
		       std::vector<std::string>&  name,
		       std::vector<std::string>&  type,
		       std::vector<std::string>&  value,
		       std::vector<TGTextEntry*>& fT2)
{
  // Create tile view container. Used to show colormap.
  
  fFrame = new TGGroupFrame(p, "Parameters", kVerticalFrame);

  TGLayoutHints* fLH3 = new TGLayoutHints(kLHintsCenterX|kLHintsExpandX,
					  2,2,2,2);

  fML = new TGMatrixLayout(fFrame, 0, 2, 2);
  fFrame->SetLayoutManager(fML);
  int h=0;
  
  // Start at "2" since first two are module and label
  for (unsigned int i=2; i<name.size(); ++i) {
    // Build the parameter label
    TGTextButton*
      b = new TGTextButton(fFrame, 
			   name[i].c_str(),
			   -1, 
			   TGButton::GetDefaultGC()(),
			   TGTextButton::GetDefaultFontStruct(), 
			   0);
    b->SetToolTipText(type[i].c_str());
    fFrame->AddFrame(b, fLH3);
    
    // Build the text edit box for the values
    TGTextEntry* t = new TGTextEntry(fFrame, value[i].c_str());
    
    // Set the size of the edit box
    t->Resize(225,18);
    fFrame->AddFrame(t, fLH3);
    fT2.push_back(t);
    h += 26;
  }
  if (h>30*26) h = 30*26;

  fFrame->Resize(fFrame->GetWidth(), h);

  fFrame->Connect("ProcessedEvent(Event_t*)", "evdb::ParamFrame", this,
		  "HandleMouseWheel(Event_t*)");
  fCanvas = 0;

  delete fLH3;
}

//......................................................................

int ParamFrame::GetHeight() const
{
  if (fFrame) return fFrame->GetHeight();
  else        return 0;
}

//......................................................................

int ParamFrame::GetWidth() const
{
  if (fFrame) return fFrame->GetWidth();
  else        return 0;
}

//......................................................................

void ParamFrame::HandleMouseWheel(Event_t *event)
{
  // Handle mouse wheel to scroll.
  
  if (event->fType != kButtonPress && event->fType != kButtonRelease)
    return;
  
  Int_t page = 0;
  if (event->fCode == kButton4 || event->fCode == kButton5) {
    if (!fCanvas) return;
    if (fCanvas->GetContainer()->GetHeight())
      page = Int_t(Float_t(fCanvas->GetViewPort()->GetHeight() *
			   fCanvas->GetViewPort()->GetHeight()) /
		   fCanvas->GetContainer()->GetHeight());
  }
  
  if (event->fCode == kButton4) {
    //scroll up
    Int_t newpos = fCanvas->GetVsbPosition() - page;
    if (newpos < 0) newpos = 0;
    fCanvas->SetVsbPosition(newpos);
  }
  if (event->fCode == kButton5) {
    // scroll down
    Int_t newpos = fCanvas->GetVsbPosition() + page;
    fCanvas->SetVsbPosition(newpos);
  }
}

//......................................................................

ParameterSetEdit::ParameterSetEdit(TGMainFrame* mf,
				   const std::string& module,
				   const std::string& label,
				   const std::string& pset,
				   std::string*       newpset) :
  TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 4, 4),
  fResult(newpset)
{
  int h = 800;
  int w = 500;
  
  // Convert the parameter set to a list of names, types, and values.
  parse_pset_string(pset, fName, fType, fValue);
  
  fLH1 = new TGLayoutHints(kLHintsLeft|kLHintsExpandX,   2,2,2,2);
  fLH2 = new TGLayoutHints(kLHintsRight|kLHintsExpandX,  2,2,2,2);
  fLH3 = new TGLayoutHints(kLHintsCenterX|kLHintsExpandX,2,2,2,2);
  fLH4 = new TGLayoutHints(kLHintsLeft|kLHintsExpandY,   4,4,4,4);

  // Add the heading at the top of the window
  h = 0;
  fF1 = new TGCompositeFrame(this, w, h, kVerticalFrame);
  std::ostringstream lbl1;
  lbl1 << "Module " << module << " - " << label;
  
  fL1 = new TGLabel(fF1, lbl1.str().c_str());
  
  fF1->AddFrame(fL1,fLH3);

  fL1->SetHeight(26);
  this->AddFrame(fF1);
  h = 30;

  // Add the parameter fields and edit boxes
  fCanvas = new TGCanvas(this, w, h);
  fParam = new ParamFrame(fCanvas->GetViewPort(),
			  fName,
			  fType, 
			  fValue,
			  fT2);
  fParam->SetCanvas(fCanvas);
  fCanvas->SetContainer(fParam->GetFrame());
  fParam->GetFrame()->SetCleanup(kDeepCleanup);

  h = fParam->GetHeight();
  if (h>800) h = 800;
  fCanvas->Resize(w,h);
  
  this->AddFrame(fCanvas);

  // Button bar across the bottom
  fF3 = new TGCompositeFrame(this, w, 16, kHorizontalFrame);
  this->AddFrame(fF3);

  fB3 = new TGTextButton(fF3, " Apply ");
  fB4 = new TGTextButton(fF3, " Cancel ");
  fB5 = new TGTextButton(fF3, " Done ");
  fF3->AddFrame(fB3, fLH1);
  fF3->AddFrame(fB4, fLH1);
  fF3->AddFrame(fB5, fLH1);

  fB3->Connect("Clicked()","evdb::ParameterSetEdit",this,"Apply()");
  fB4->Connect("Clicked()","evdb::ParameterSetEdit",this,"Cancel()");
  fB5->Connect("Clicked()","evdb::ParameterSetEdit",this,"Done()");

  this->Connect("CloseWindow()","evdb::ParameterSetEdit",this,"CloseWindow()");

  h += 50;

  this->Resize(w+8,h);
  this->MapSubwindows();
  this->MapWindow();

  (*fResult) = "";
}

//......................................................................

int ParameterSetEdit::Edit()
{
  unsigned int i;
  const char* values;
  std::ostringstream pset;
  
  pset << "<" <<
    fName[0] << "=" << fType[0] << "(" << fValue[0] << ");" <<
    fName[1] << "=" << fType[1] << "(" << fValue[1] << ");";
  
  for (i=2; i<fName.size(); ++i) {
    values = fT2[i-2]->GetText();
    pset << fName[i] << "=" << fType[i] << "({" << values << "});";
  }
  pset << ">";
  
  (*fResult) = pset.str();
    
  return 1;
}

//......................................................................

ParameterSetEdit::~ParameterSetEdit()
{
  unsigned int i;
  delete fB5;
  delete fB4;
  delete fB3;
  for (i=0; i<fT2.size(); ++i) delete fT2[i];
  delete fL1;
  delete fF3;
  delete fF1;
  delete fLH4;
  delete fLH3;
  delete fLH2;
  delete fLH1;
}
 
//......................................................................

void ParameterSetEdit::CloseWindow() { delete this; }

//......................................................................

void ParameterSetEdit::Cancel() 
{
  this->SendCloseMessage(); 
}

//......................................................................

void ParameterSetEdit::Done() 
{
  this->Edit(); 
  this->SendCloseMessage();
  NavState::Set(kRELOAD_EVENT);
}

//......................................................................

void ParameterSetEdit::Apply() 
{
  this->Edit(); 
  NavState::Set(kRELOAD_EVENT);
}

////////////////////////////////////////////////////////////////////////
