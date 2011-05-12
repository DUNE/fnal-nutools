///
/// \file    ScanWindow.cxx
/// \brief   window for hand scanning
/// \author  brebel@fnal.gov
/// \version $Id: ScanWindow.cxx,v 1.5 2011-05-12 15:22:06 brebel Exp $
///
#include "TCanvas.h"
#include "TGFrame.h" // For TGMainFrame, TGHorizontalFrame
#include "TGLayout.h" // For TGLayoutHints
#include "TGButton.h" // For TGCheckButton
#include "TGDimension.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include "TMath.h"
#include "TString.h"
#include "TSystem.h"

#include "EventDisplayBase/ScanWindow.h"
#include "EventDisplayBase/ScanOptions.h"
#include "EventDisplayBase/NavState.h"
#include "EventDisplayBase/EventHolder.h"
#include "SimulationBase/simbase.h"

#include "art/Framework/Core/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

static int kInputID = 0;

namespace evdb{

  //--------------------------------------------------------------------
  ScanWindow::ScanWindow() : 
    TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 4, 4)
  {  
    int h = 50;
    int w = 50;

    // set a frame to hold the various scan boxes
    fMainFrame       = new TGCompositeFrame(this, w, h, kHorizontalFrame);
    fScanCompFrame   = new TGCompositeFrame(fMainFrame, w, h, kVerticalFrame);
    fButtonCompFrame = new TGCompositeFrame(fMainFrame, w, h, kHorizontalFrame);
    
    TGLayoutHints* fLHBotLeft   = new TGLayoutHints(kLHintsBottom|kLHintsLeft, 2,2,5,5);
    TGLayoutHints* fLHScan      = new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandX, 2,2,5,5);
    TGLayoutHints* fLHButton    = new TGLayoutHints(kLHintsLeft|kLHintsTop, 2,2,5,5);

    TGLayoutHints* lh  = new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,5,5);
  
    //grab the ScanOptions service and loop over the categories to make a 
    // ScanFrame for each
    art::ServiceHandle<evdb::ScanOptions> opts;

    // set up the file name to store the information
    std::string user(gSystem->Getenv("USER"));
    fOutFileName.append(opts->fScanFileBase);
    fOutFileName.append(user);
    fOutFileName.append(".txt");

    std::ofstream outfile(fOutFileName.c_str());

    //output the labels so we know what each is
    outfile << "Run Event ";

    unsigned int pos = 0;
    unsigned int maxfields = 0;
    for(unsigned int c = 0; c < opts->fCategories.size(); ++c){
      std::vector<std::string> types;
      std::vector<std::string> labels;

      bool button = false;

      for(unsigned int p = 0; p < opts->fFieldsPerCategory[c]; ++p){
	types.push_back(opts->fFieldTypes[pos+p]);
	labels.push_back(opts->fFieldLabels[pos+p]);
	
	if(types[p].find("Button") != std::string::npos) button = true;
	outfile << opts->fCategories[c].c_str() << ":" << opts->fFieldLabels[pos+p].c_str() << " ";
      }
      
      // figure out the max number of fields to resize the window later
      if(types.size() > maxfields){
	maxfields = types.size();
	w = 130*maxfields;
      }
      pos += opts->fFieldsPerCategory[c];

      // Create view container.  
      TGGroupFrame *frame = 0;
      if(!button){
	fCatFrames.push_back(new TGGroupFrame(fScanCompFrame, opts->fCategories[c].c_str(), 
					      kRaisedFrame|kHorizontalFrame));
	frame = fCatFrames[fCatFrames.size()-1];
      }
      else{
	fButtonFrames.push_back(new TGGroupFrame(fButtonCompFrame, opts->fCategories[c].c_str(),
						 kRaisedFrame|kVerticalFrame));
	frame = fButtonFrames[fButtonFrames.size()-1];
      }
      h += 60;

      // loop over the fields and determine what to draw
      for(unsigned int i = 0; i < types.size(); ++i){

	if(types[i] == "Text"){
	  // only add a label for text entry if the category is not Comments, otherwise
	  // the labe is redundant
	  if(opts->fCategories[c].compare("Comments") != 0){
	    TGLabel *l = new TGLabel(frame, labels[i].c_str());
	    frame->AddFrame(l, lh);
	  }
	  fTextBoxes.push_back(new TGTextEntry(frame));
	  frame->AddFrame(fTextBoxes[fTextBoxes.size()-1], lh);
	  ++kInputID;
	}
	else if(types[i] == "Number"){
	  TGLabel *l = new TGLabel(frame, labels[i].c_str());
	  frame->AddFrame(l, lh);
	  fNumberBoxes.push_back(new TGNumberEntry(frame, 0, 2, -1, TGNumberFormat::kNESInteger));
	  frame->AddFrame(fNumberBoxes[fNumberBoxes.size()-1], lh);
	  ++kInputID;
	}
	else if(types[i] =="CheckButton"){
	  fCheckButtons.push_back(new TGCheckButton(frame, labels[i].c_str(), kInputID));
	  frame->AddFrame(fCheckButtons[fCheckButtons.size()-1], lh);
	  ++kInputID;
	}
	else if(types[i] =="RadioButton"){
	  fRadioButtons.push_back(new TGRadioButton(frame, labels[i].c_str(), kInputID));
	  frame->AddFrame(fRadioButtons[fRadioButtons.size()-1], lh);
	  fRadioButtons[fRadioButtons.size()-1]->Connect("Clicked()","evdb::ScanWindow",this,"RadioButton()");
	  fRadioButtonIds.push_back(kInputID);
	  ++kInputID;
	}
		
      }// end loop over types

    }// end loop over categories

    // loop over the frames created and add them to the appropriate spot in the window
    // button frames on the left.  the constants are magic numbers determined by trial and error
    for(unsigned int bf = 0; bf < fButtonFrames.size(); ++bf){
      fButtonFrames[bf]->Resize(120, 100);
      fButtonCompFrame->AddFrame(fButtonFrames[bf], fLHButton);
    }

    for(unsigned int cf = 0; cf < fCatFrames.size(); ++cf){
      fCatFrames[cf]->Resize(w, 60);
      fScanCompFrame->AddFrame(fCatFrames[cf], fLHScan);
    }

    // pad out the size of the Scan and Button frames to accomodate the various fields
    fScanCompFrame->Resize(w, 60*fCatFrames.size());
    fButtonCompFrame->Resize(120*fButtonFrames.size(), fScanCompFrame->GetSize().fHeight);

    // add the Scan and ButtonCompFrames to the Main frame, and set its size
    fMainFrame->AddFrame(fButtonCompFrame, fLHButton);
    fMainFrame->AddFrame(fScanCompFrame, fLHButton);
    fMainFrame->Resize(fButtonCompFrame->GetSize().fWidth+fScanCompFrame->GetSize().fWidth, 
		       fScanCompFrame->GetSize().fHeight);

    this->AddFrame(fMainFrame, fLHButton);
    
    //make a new line in the output
    outfile << std::endl;
    
    // Button bar across the bottom
    fF3 = new TGCompositeFrame(this, w, 60, kHorizontalFrame);
    fB3 = new TGTextButton(fF3, " Prev   ");
    fB4 = new TGTextButton(fF3, " Next   ");
    fB5 = new TGTextButton(fF3, " Record ");
    Pixel_t red;
    gClient->GetColorByName("red", red);
    fB5->ChangeBackground(red);

    fF3->AddFrame(fB3, fLHBotLeft);
    fF3->AddFrame(fB4, fLHBotLeft);
    fF3->AddFrame(fB5, fLHBotLeft);

    // add the click button frame to the main window
    this->AddFrame(fF3, fLHBotLeft);
    
    fB3->Connect("Clicked()","evdb::ScanWindow",this,"Prev()");
    fB4->Connect("Clicked()","evdb::ScanWindow",this,"Next()");
    fB5->Connect("Clicked()","evdb::ScanWindow",this,"Rec()");
    
    this->Connect("CloseWindow()","evdb::ScanWindow",this,"CloseWindow()");
    this->DontCallClose();
    this->SetCleanup(kDeepCleanup);

    // pad out the main frame a bit to make sure nothing is cut off
    this->MapSubwindows();
    this->MapWindow();
    this->Resize(TMath::Nint(1.05*fMainFrame->GetSize().fWidth), 
		 fMainFrame->GetSize().fHeight+fF3->GetSize().fHeight+50);

    delete fLHBotLeft;
    delete fLHScan;
    delete fLHButton;
  }

  //......................................................................
  ScanWindow::~ScanWindow() 
  {  
    if (fB3)            { delete fB3;             fB3             = 0; }
    if (fB4)            { delete fB4;             fB4             = 0; }
    if (fB5)            { delete fB5;             fB5             = 0; }
    if (fF3)            { delete fF3;             fF3             = 0; }
    if (fScanCompFrame) { delete fScanCompFrame;  fScanCompFrame  = 0; }

    for(unsigned int i = 0; i < fTextBoxes.size(); ++i){
      if( fTextBoxes[i] ) delete fTextBoxes[i];
    }
    for(unsigned int i = 0; i < fNumberBoxes.size(); ++i){
      if( fNumberBoxes[i] ) delete fNumberBoxes[i];
    }
    for(unsigned int i = 0; i < fRadioButtons.size(); ++i){
      if( fRadioButtons[i] ) delete fRadioButtons[i];
    }
    for(unsigned int i = 0; i < fCheckButtons.size(); ++i){
      if( fCheckButtons[i] ) delete fCheckButtons[i];
    }
    for(unsigned int i = 0; i < fCatFrames.size(); ++i){
      if( fCatFrames[i] ) delete fCatFrames[i];
    }

  }

  //......................................................................
  void ScanWindow::RadioButton()
  {
    TGButton *b = (TGButton *)gTQSender;
    int id = b->WidgetId();
    
    if(fRadioButtonIds.size() < 2) return;

    if(id >= fRadioButtonIds[0] && id <= fRadioButtonIds[fRadioButtonIds.size()-1]){
      for(unsigned int i = 0; i < fRadioButtonIds.size(); ++i)
	if(fRadioButtons[i]->WidgetId() != id) fRadioButtons[i]->SetState(kButtonUp);
    }

    return;
  }

  //......................................................................

  void ScanWindow::CloseWindow() { delete this; }
  
  //......................................................................
  void ScanWindow::Prev() 
  {
    evdb::NavState::Set(kPREV_EVENT);
  }
  
  //......................................................................  
  void ScanWindow::Next() 
  {
    evdb::NavState::Set(kNEXT_EVENT);
  }

  //......................................................................
  void ScanWindow::Rec()
  {
    art::ServiceHandle<evdb::ScanOptions> scanopt;

    // get the event information
    const art::Event *evt = evdb::EventHolder::Instance()->GetEvent();
    
    std::ofstream outfile(fOutFileName.c_str(), std::ios_base::app);

    outfile << evt->run() << " " << evt->id().event() << " ";

    // loop over the input fields
    unsigned int txtctr = 0;
    unsigned int numctr = 0;
    unsigned int radctr = 0;
    unsigned int chkctr = 0;
    for(unsigned int t = 0; t < scanopt->fFieldTypes.size(); ++t){
  
      if(scanopt->fFieldTypes[t] == "Text"){
	if(txtctr < fTextBoxes.size()   ){
	  outfile << fTextBoxes[txtctr]->GetText() << " ";
	  fTextBoxes[txtctr]->Clear();
	}
	++txtctr;
      }
      else if(scanopt->fFieldTypes[t] == "Number"){
	if(numctr < fNumberBoxes.size() ){
	  outfile << fNumberBoxes[numctr]->GetNumber() << " ";
	  fNumberBoxes[numctr]->SetNumber(0);
	}
	++numctr;
      }
      else if(scanopt->fFieldTypes[t] == "RadioButton"){
	if(radctr < fRadioButtons.size()     ){
	  outfile << (fRadioButtons[radctr]->GetState() == kButtonDown) << " ";
	  fRadioButtons[radctr]->SetState(kButtonUp);
	}
	++radctr;
      }
      else if(scanopt->fFieldTypes[t] == "CheckButton"){
	if(chkctr < fCheckButtons.size()     ){
	  outfile << (fCheckButtons[chkctr]->GetState() == kButtonDown) << " ";
	  fCheckButtons[chkctr]->SetState(kButtonUp);
	}
	++chkctr;
      }
		
    }// end loop over input field types
    
//     // do we need to get the truth information?
//     if(scanopt->fIncludeMCInfo){

//       art::Handle< std::vector<simb::MCTruth> > mclist;
//       evt->getByLabel(mcopt->fMCTruthModules[0], mclist);

//       art::Ptr<simb::MCTruth> mc(mclist,0);
//       const simb::MCNeutrino& neutrinoInteraction = mc->GetNeutrino();
	  
//       if ( mc->Origin() != simb::kBeamNeutrino ){
// 	std::cout<<"Unknown particle source or truth information N/A"<<std::endl;
//       }
//       else{	      
// 	// get the event vertex and energy information,

// 	const simb::MCParticle& nu = mc->GetNeutrino().Nu();

// 	std::cout << nu.Vx() << " " << nu.Vy() << " " << nu.Vz() << " " << nu.E();
	      
//       } // MC Truth	


//     }//end if using MC information

    // end this line for the event
    outfile << std::endl;

    evdb::NavState::Set(evdb::kNEXT_EVENT);

  }

}// namespace
