///
/// \file   ParameterSetEditDialog.cxx
/// \brief  Pop-up window for editing parameter sets
/// \author messier@indiana.edu
///
#include "EventDisplayBase/ParameterSetEditDialog.h"
#include <iostream>
#include "TGTab.h"
#include "TGButton.h"
#include "TGCanvas.h"
#include "TGTableLayout.h"
#include "TGLayout.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGListBox.h"
#include "TGDoubleSlider.h"
#include "fhiclcpp/ParameterSet.h"
#include "EventDisplayBase/ServiceTable.h"
using namespace evdb;

// Window and row sizes in units of pixels
static const unsigned int kWidth  = 500;
static const unsigned int kHeight = 650;
static const unsigned int kRowW = 348;
static const unsigned int kRowH = 18;

//
// Flags to help us decide what sort of parameter we need to build a
// GUI for.
//
static const int kSINGLE_VALUED_PARAM    = 0x01; // Expect single value
static const int kVECTOR_PARAM           = 0x02; // Expect multiple values
static const int kVECTOR_OF_VECTOR_PARAM = 0x04; // Expect multiple values
static const int kHAVE_GUI_TAGS          = 0x10; // GUI tags are present
static const int kNO_GUI_TAGS            = 0x20; // GUI tags are not present

//
// The short letter codes for the various GUI objects supported. Also
// provide a list of all possible tags.
//
#define GUITAG static const std::string
GUITAG kTEXT_ENTRY      = "te";  // A text edit box
GUITAG kLIST_BOX_SINGLE = "lbs"; // A list box, single choice allowed
GUITAG kLIST_BOX_MULTI  = "lbm"; // A list box, multuiple choices allowed
GUITAG kRADIO_BUTTONS   = "rb";  // Radio buttons
GUITAG kCHECK_BOX       = "cb";  // Check boxes
GUITAG kSLIDER          = "sl";  // Slider bar
#undef GUITAG
static const std::vector<std::string> gsGUITAG = {
  kTEXT_ENTRY,
  kLIST_BOX_SINGLE,
  kLIST_BOX_MULTI,
  kRADIO_BUTTONS,
  kCHECK_BOX,
  kSLIDER
};

//======================================================================
//
// ParameterSetEditRow methods
//
ParameterSetEditRow::ParameterSetEditRow(TGHorizontalFrame* lhs,
					 TGHorizontalFrame* rhs,
					 const fhicl::ParameterSet& ps,
					 const std::string& key) :
  fLabel(0),
  fTextEntry(0),
  fListBox(0),
  fSlider(0)
{
  //
  // Extract information about the parameter for which we are building
  // the GUI
  //
  std::string              tag;    // What sort of frame to build?
  std::vector<std::string> values; // What is the current value?
  std::string              doc;    // Documentation string
  this->UnpackParameter(ps, key, fParamFlags, tag, fChoice, values, doc);

  if (fParamFlags&kVECTOR_PARAM) {
    fValue = "[";
    for (unsigned int i=0; i<values.size(); ++i) {
      fValue += values[i];
      if (i+1<values.size()) fValue += ",";
      else                   fValue += "]";
    }
  }
  else {
    fValue = values[0];
  }

  fLeftLH  = new TGLayoutHints(kLHintsLeft, 1,1,0,0);
  fRightLH = new TGLayoutHints(kLHintsRight,1,1,0,0);
  
  fLabel = new TGTextButton(lhs,
			    key.c_str(),
			    -1,
			    TGButton::GetDefaultGC()(),
			    TGTextButton::GetDefaultFontStruct(),
			    0);
  lhs->AddFrame(fLabel);
  fLabel->SetToolTipText(doc.c_str());
  fLabel->SetTextJustify(kTextRight);
  
  if (tag==kTEXT_ENTRY) {
    this->SetupTextEntry(rhs, fParamFlags, values);
  }
  if (tag==kLIST_BOX_SINGLE) {
    this->SetupListBox(rhs, fParamFlags, fChoice, values, false);
  }
  if (tag==kLIST_BOX_MULTI) {
    this->SetupListBox(rhs, fParamFlags, fChoice, values, true);
  }
  if (tag==kRADIO_BUTTONS) {
    this->SetupRadioButtons(rhs, fParamFlags, fChoice, values);
  }
  if (tag==kCHECK_BOX) {
    this->SetupCheckButton(rhs, fParamFlags, fChoice, values);
  }
  if (tag==kSLIDER) {
    this->SetupSlider(rhs, fParamFlags, fChoice, values);
  }
}

//......................................................................

ParameterSetEditRow::~ParameterSetEditRow() 
{
}

//......................................................................

void ParameterSetEditRow::UnpackParameter(const fhicl::ParameterSet& p,
					  const std::string&         key,
					  unsigned int&              flag,
					  std::string&               tag,
					  std::vector<std::string>&  choice,
					  std::vector<std::string>&  value,
					  std::string&               doc)
{
  std::string guikey = key; guikey += ".gui";
  std::string dockey = key; dockey += ".doc";
  
  flag = 0;
  
  //
  // Try to extract GUI tags
  //
  std::string gui;
  try {
    gui = p.get< std::string >(guikey);
    doc = p.get< std::string >(dockey);
    flag |= kHAVE_GUI_TAGS;
  }
  catch (...) { 
    //
    // If they aren't there, try extracting it as a normal
    // parameter. Default to providing the user with a text entry box.
    //
    gui = kTEXT_ENTRY;
    doc = "See .fcl file for documentation...";
    flag |= kNO_GUI_TAGS;
  }
  
  //
  // Parse out the GUI string to find out what type of frame to build
  // and the choices we should present to the user
  //
  ParseGUItag(gui, tag, choice);

  //
  // Now extract the assigned value(s) of the parameter
  //
  // The key is either just the key, or in the case of GUI-enabled
  // parameters the key name with ".val" appended
  //
  std::string valkey = key;
  if ( flag&kHAVE_GUI_TAGS ) valkey += ".val";
  //
  // Try first to extract a single value.
  //
  try {
    std::string v = p.get<std::string>(valkey);
    value.push_back(v);
    flag |= kSINGLE_VALUED_PARAM;
  }
  catch (...) {
    //
    // If that fails, try extracting multiple values
    //
    try {
      value = p.get< std::vector<std::string> >(valkey);
      flag |= kVECTOR_PARAM;
      if (value.size()==0) value.push_back("");
    }
    catch (...) {
      //
      // Yikes - vector of vectors, perhaps?
      //
      try {
	std::vector< std::vector <std::string> > vv;
	vv = p.get<std::vector<std::vector<std::string> > >(valkey);
	flag |= kVECTOR_OF_VECTOR_PARAM;
	if (value.size()==0) value.push_back("");
      }
      catch (...) {
	//
	// If that fails we are very stuck. Abort to ensure the problem
	// gets fixed
	//
	std::cerr << "Failed to parse " << key << std::endl;
      }
    }
  }
}

//......................................................................
//
// Parse out what we can from the "gui" tag. Expected format is:
// "frame_tag:choice1,choice2,choice3"
//
void ParameterSetEditRow::ParseGUItag(const std::string&        guitag,
				      std::string&              frame,
				      std::vector<std::string>& choice)
{
  //
  // Get the frame name. Should be piece just before the ":"
  //
  choice.clear();
  size_t icolon = guitag.find(':');
  if (icolon == std::string::npos) frame = guitag;
  else                             frame = guitag.substr(0,icolon);
  if (!IsLegalGUItag(frame))       frame = kTEXT_ENTRY;
  
  //
  // Get the list of choices. Should be comma separated.
  //
  size_t icomma = icolon;
  size_t spos, epos;
  while (icomma!=std::string::npos) {
    spos = icomma+1;
    epos = guitag.find(',',spos);
    std::string s = guitag.substr(spos,epos-spos);
    choice.push_back(s);
    icomma = epos;
  }
}

//......................................................................

bool ParameterSetEditRow::IsLegalGUItag(const std::string& s) 
{
  for(unsigned int i=0; i<gsGUITAG.size(); ++i) {
    if (s==gsGUITAG[i]) return true;
  }
  std::cerr << __FILE__ << ":" << __LINE__ << " " 
            << s << " is not a legal GUI tag." << std::endl;
  return false;
}

//......................................................................

void ParameterSetEditRow::SetupTextEntry(TGCompositeFrame* f, 
                                        unsigned int flags,
                                        const std::vector<std::string>& value)
{
  fTextEntry = new TGTextEntry(f);
  f->AddFrame(fTextEntry);
  
  fTextEntry->Connect("ReturnPressed()",
                      "evdb::ParameterSetEditRow",
                      this,
                      "TextEntryReturnPressed()");
  
  std::string buff;
  if (flags&kVECTOR_PARAM) buff += "[";
  for (unsigned int i=0; i<value.size(); ++i) {
    buff += value[i];
    if ((i+1)!=value.size()) buff += ",";
  }
  if (flags&kVECTOR_PARAM) buff += "]";
  fTextEntry->SetText(buff.c_str(), 0);
  fTextEntry->Resize(kRowW,kRowH);
}

//......................................................................

void ParameterSetEditRow::SetupListBox(TGCompositeFrame* f, 
                                      unsigned int flags,
                                      const std::vector<std::string>& choice,
                                      const std::vector<std::string>& value,
                                      bool ismulti)
{
  fListBox = new TGListBox(f);
  f->AddFrame(fListBox);
  if (ismulti) fListBox->SetMultipleSelections();
  
  for (size_t i=0; i<choice.size(); ++i) {
    fListBox->AddEntry(choice[i].c_str(), i);
  }
  for (size_t i=0; i<value.size(); ++i) {
    TGLBEntry* e = fListBox->FindEntry(value[i].c_str());
    if (e) e->Activate(true);
  }
  
  fListBox->Connect("SelectionChanged()",
                    "evdb::ParameterSetEditRow",
                    this,
                    "ListBoxSelectionChanged()");
  fListBox->Connect("Selected(Int_t)",
                    "evdb::ParameterSetEditRow",
                    this,
                    "ListBoxSelected(int)");
  
  size_t h = kRowH*choice.size();
  if (h>3*kRowH) h = 3*kRowH;
  fListBox->Resize(kRowW,h);
}

//......................................................................

void ParameterSetEditRow::
SetupRadioButtons(TGCompositeFrame* f, 
                  unsigned int flags,
                  const std::vector<std::string>& choice,
                  const std::vector<std::string>& value)
{
  for (size_t i=0; i<choice.size(); ++i) {
    TGRadioButton* b = new TGRadioButton(f, choice[i].c_str(), i);
    f->AddFrame(b);
    
    b->SetTextJustify(kTextLeft);
    b->Connect("Clicked()",
               "evdb::ParameterSetEditRow",
               this,
               "RadioButtonClicked()");
    
    for (size_t j=0; j<value.size(); ++j) {
      if (value[j] == choice[i]) {
	b->SetState(kButtonDown);
      }
    }
    
    fRadioButton.push_back(b);
  }
}

//......................................................................

void ParameterSetEditRow::
SetupCheckButton(TGCompositeFrame* f, 
		 unsigned int flags,
		 const std::vector<std::string>& choice,
		 const std::vector<std::string>& value)
{
  for (size_t i=0; i<choice.size(); ++i) {
    TGCheckButton* b = new TGCheckButton(f, choice[i].c_str(), i);
    f->AddFrame(b);
    b->Connect("Clicked()",
               "evdb::ParameterSetEditRow",
               this,
               "CheckButtonClicked()");
    
    for (size_t j=0; j<value.size(); ++j) {
      if (value[j] == choice[i]) {
        b->SetState(kButtonDown);
      }
    }
    
    fCheckButton.push_back(b);
  }
}
//......................................................................

void ParameterSetEditRow::SetupSlider(TGCompositeFrame* f, 
				      unsigned int flags,
				      const std::vector<std::string>& choice,
				      const std::vector<std::string>& value)
{
  fTextEntry = new TGTextEntry(f);
  f->AddFrame(fTextEntry);
  
  std::string t;
  if (value.size()==1) { t = value[0]; }
  if (value.size()==2) { 
    t = "["; t += value[0]; t += ","; t += value[1]; t += "]";
  }
  fTextEntry->SetText(t.c_str());
  
  fTextEntry->Connect("ReturnPressed()",
                      "evdb::ParameterSetEditRow",
                      this,
                      "TextEntryReturnPressed()");

  fSlider = new TGDoubleHSlider(f, 100, kDoubleScaleBoth);
  f->AddFrame(fSlider);

  float min = atof(choice[0].c_str());
  float max = atof(choice[1].c_str());

  float pos1 = 0;
  float pos2 = 0;
  if (value.size()==1) {
    pos1 = atof(value[0].c_str());
    pos2 = pos1;
  }
  if (value.size()==2) {
    pos1 = atof(value[0].c_str());
    pos2 = atof(value[1].c_str());
  }

  fSlider->SetRange(min, max);
  fSlider->SetPosition(pos1, pos2);
  
  fSlider->Connect("PositionChanged()",
                   "evdb::ParameterSetEditRow",
                   this,
                   "SliderPositionChanged()");
  
  fTextEntry->Resize(kRowW*1/4,   kRowH);
  fSlider->   Resize(kRowW*3/4,10*kRowH);
}

//......................................................................

void ParameterSetEditRow::TextEntryReturnPressed()
{
  const char* text = fTextEntry->GetBuffer()->GetString();
  
  //
  // If we also have a slider connected to this frame, make sure its
  // state is updated
  //
  if (fSlider) {
    int   n=0;
    float f1=0, f2=0;
    n = sscanf(text, "[%f, %f]", &f1, &f2);
    if (n!=2) {
      n = sscanf(text, "%f", &f1);
      f2 = f1;
    }
    fSlider->SetPosition(f1, f2);
  }
  
  fValue = text;
}

//......................................................................

void ParameterSetEditRow::ListBoxSelectionChanged()
{
  //
  // Only need to handle list boxes where multiple selections are
  // allowed here.
  //
  if (fListBox->GetMultipleSelections()==0) return;
  
  fValue = "[";
  TList selections;
  fListBox->GetSelectedEntries(&selections);
  TGLBEntry* sel;
  bool isfirst = true;
  for (unsigned int i=0;;++i) {
    sel = (TGLBEntry*)selections.At(i);
    if (sel==0) break;
    if (!isfirst) fValue += ",";
    fValue += fChoice[sel->EntryId()];
    isfirst = false;
  }    
  fValue += "]";
}

//......................................................................

void ParameterSetEditRow::ListBoxSelected(int id)
{
  //
  // Only handle single selection list boxes here
  //
  if (fListBox->GetMultipleSelections()) return;
  
  fValue = fChoice[id];
}
//......................................................................

void ParameterSetEditRow::RadioButtonClicked() 
{
  TGButton* b = (TGButton*)gTQSender;
  int id = b->WidgetId();
  for (size_t i=0; i<fRadioButton.size(); ++i) {
    if (fRadioButton[i]->WidgetId() != id) {
      fRadioButton[i]->SetState(kButtonUp);
    }
    else fValue = fChoice[i];
  }
}

//......................................................................

void ParameterSetEditRow::CheckButtonClicked() 
{
  fValue = "[";
  
  bool isfirst = true;
  for (unsigned int i=0; i<fCheckButton.size(); ++i) {
    if (fCheckButton[i]->IsDown()) {
      if (!isfirst) fValue += ",";
      fValue += fChoice[i];
      isfirst = false;
    }
  }
  fValue += "]";
}

//......................................................................

void ParameterSetEditRow::SliderPositionChanged() 
{
  char buff[1024];
  float mn, mx;
  fSlider->GetPosition(mn, mx);
  
  if (fParamFlags & kVECTOR_PARAM) {
    sprintf(buff, "[%.1f, %.1f]",mn,mx);
  }
  else {
    sprintf(buff, "%.1f",0.5*(mn+mx));
  } 
  fTextEntry->SetText(buff);
  fValue = buff;
}

//======================================================================
//
// ParameterSetEditFrame methods
//
ParameterSetEditFrame::ParameterSetEditFrame(TGCompositeFrame* mother,
					     unsigned int psetid)
{
  fCanvas  = new TGCanvas(mother, kWidth-16, kHeight-16);
  fCanvasH = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY);
  mother->AddFrame(fCanvas, fCanvasH);
  
  fContainer = new TGCompositeFrame(fCanvas->GetViewPort());
  fCanvas->SetContainer(fContainer);

  const ServiceTable& st = ServiceTable::Instance();
  const fhicl::ParameterSet& pset = st.GetParameterSet(psetid);
  std::vector<std::string>   key  = pset.get_keys();
  unsigned int               nkey = key.size();
    
  fLayout = new TGTableLayout(fContainer, nkey, 2);
  fContainer->SetLayoutManager(fLayout);

  fLHS.     resize(nkey);
  fRHS.     resize(nkey);
  fLHSHints.resize(nkey);
  fRHSHints.resize(nkey);
  fRow.     resize(nkey);

  unsigned int i;
  for (i=0; i<nkey; ++i) {
    if (!((key[i]=="service_type") ||
	  (key[i]=="module_type")  ||
	  (key[i]=="module_label"))) {
      
      fLHS[i]      = new TGHorizontalFrame(fContainer);
      fLHSHints[i] = new TGTableLayoutHints(0,1,i,i+1);
      fContainer->AddFrame(fLHS[i], fLHSHints[i]);
      
      fRHS[i]      = new TGHorizontalFrame(fContainer);
      fRHSHints[i] = new TGTableLayoutHints(1,2,i,i+1);
      fContainer->AddFrame(fRHS[i], fRHSHints[i]);
      
      fRow[i] = new ParameterSetEditRow(fLHS[i], fRHS[i], pset, key[i]);
    }
  }
}

//======================================================================
//
// ParameterSetEditDialog methods
//
ParameterSetEditDialog::ParameterSetEditDialog(int which) :
  TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 4, 4)
{
  fTGTab = new TGTab(this);
  this->AddFrame(fTGTab);
  
  fButtons = new TGHorizontalFrame(this);
  this->AddFrame(fButtons);
  
  fApply  = new TGTextButton(fButtons, " Apply  ");
  fCancel = new TGTextButton(fButtons, " Cancel ");
  fDone   = new TGTextButton(fButtons, " Done   ");
  
  fButtons->AddFrame(fApply);
  fButtons->AddFrame(fCancel);
  fButtons->AddFrame(fDone);
  
  fApply-> Connect("Clicked()","evdb::ParameterSetEditDialog",this,"Apply()");
  fCancel->Connect("Clicked()","evdb::ParameterSetEditDialog",this,"Cancel()");
  fDone->  Connect("Clicked()","evdb::ParameterSetEditDialog",this,"Done()");
  
  //
  // Loop over all the parameter sets and build tabs for them
  //
  const ServiceTable& st = ServiceTable::Instance();
  unsigned int i;
  for (i=0; i<st.fServices.size(); ++i) {
    if (st.fServices[i].fCategory==which) {
      std::string tabnm = this->TabName(st.fServices[i].fName);
      TGCompositeFrame* f = fTGTab->AddTab(tabnm.c_str());
      new ParameterSetEditFrame(f, i);
    }
  }
  
  switch (which) {
  case kDRAWING_SERVICE:
    this->SetWindowName("Drawing Services");
    break;
  case kEXPERIMENT_SERVICE:
    this->SetWindowName("Experiment Services");
    break;
  case kART_SERVICE:
    this->SetWindowName("ART Services");
    break;
  default:
    this->SetWindowName("Services Configuration");
  }

  this->MapSubwindows();
  this->Resize(kWidth,kHeight);
  this->MapWindow();
}

//......................................................................

ParameterSetEditDialog::~ParameterSetEditDialog() {}

void ParameterSetEditDialog::Apply() {}
void ParameterSetEditDialog::Cancel() {}
void ParameterSetEditDialog::Done() {}
void ParameterSetEditDialog::CloseWindow() {}

//......................................................................
//
// Remove any redundant text from the tab name
//
std::string ParameterSetEditDialog::TabName(const std::string& s)
{
  size_t n = 0;
  
  n = s.find("DrawingOptions");
  if (n!=std::string::npos) return s.substr(0,n);
  
  return s;
}

////////////////////////////////////////////////////////////////////////

