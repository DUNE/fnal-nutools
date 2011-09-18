///
/// \file    ScanWindow.cxx
/// \brief   window for hand scanning
/// \author  brebel@fnal.gov
/// \version $Id: ScanWindow.cxx,v 1.10 2011-09-18 18:46:26 brebel Exp $
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
static int kCategoryWidth = 150;
static int kFieldHeight = 50;

namespace evdb{

  //......................................................................
  ScanFrame::ScanFrame(const TGWindow* p,
		       unsigned int nCategories,
		       unsigned int maxFields)
  {
    // Create tile view container. Used to show colormap.
    
    fFrame = new TGGroupFrame(p, "Categories", kHorizontalFrame);
    
    TGLayoutHints* fLH3 = new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsTop,
					    2,2,2,2);
    
    // grab the ScanOptions service and loop over the categories to make a 
    // ScanFrame for each
    art::ServiceHandle<evdb::ScanOptions> opts;

    unsigned int pos = 0;
    for(unsigned int c = 0; c < nCategories; ++c){
      std::vector<std::string> types;
      std::vector<std::string> labels;
      
      for(unsigned int p = 0; p < opts->fFieldsPerCategory[c]; ++p){
	types.push_back(opts->fFieldTypes[pos+p]);
	labels.push_back(opts->fFieldLabels[pos+p]);
	
      }
      
      pos += opts->fFieldsPerCategory[c];

      // Create container for the current category.  
      TGGroupFrame *frame = 0;
      fCatFrames.push_back(new TGGroupFrame(fFrame, opts->fCategories[c].c_str(), 
					    kRaisedFrame|kVerticalFrame));
      frame = fCatFrames[fCatFrames.size()-1];
      //frame->SetLayoutManager(fML);
      // loop over the fields and determine what to draw
      for(unsigned int i = 0; i < types.size(); ++i){

	if(types[i] == "Text"){
	  TGLabel *l = new TGLabel(frame, labels[i].c_str());
	  frame->AddFrame(l, fLH3);
	  fTextBoxes.push_back(new TGTextEntry(frame));
	  frame->AddFrame(fTextBoxes[fTextBoxes.size()-1], fLH3);
	}
	else if(types[i] == "Number"){
	  TGLabel *l = new TGLabel(frame, labels[i].c_str());
	  frame->AddFrame(l, fLH3);
	  fNumberBoxes.push_back(new TGNumberEntry(frame, 0, 2, -1, TGNumberFormat::kNESInteger));
	  frame->AddFrame(fNumberBoxes[fNumberBoxes.size()-1], fLH3);
	}
	else if(types[i] =="CheckButton"){
	  fCheckButtons.push_back(new TGCheckButton(frame, labels[i].c_str(), kInputID));
	  frame->AddFrame(fCheckButtons[fCheckButtons.size()-1], fLH3);
	}
	else if(types[i] =="RadioButton"){
	  fRadioButtons.push_back(new TGRadioButton(frame, labels[i].c_str(), kInputID));
	  frame->AddFrame(fRadioButtons[fRadioButtons.size()-1], fLH3);
	  fRadioButtons[fRadioButtons.size()-1]->Connect("Clicked()","evdb::ScanFrame",
							 this,"RadioButton()");
	  fRadioButtonIds.push_back(kInputID);
	}

	++kInputID;
		
      }// end loop over types

      fFrame->AddFrame(frame, fLH3);

    } // end loop over categories
    
    fFrame->Resize(kCategoryWidth*nCategories, kFieldHeight*maxFields);
    
    fFrame->Connect("ProcessedEvent(Event_t*)", "evdb::ScanFrame", this,
		    "HandleMouseWheel(Event_t*)");
    fCanvas = 0;
    
    delete fLH3;
  }

  //......................................................................  
  ScanFrame::~ScanFrame()
  { 
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

    delete fFrame; 
  }  

  //......................................................................  
  int ScanFrame::GetHeight() const
  {
    if (fFrame) return fFrame->GetHeight();
    else        return 0;
  }
  
  //......................................................................
  int ScanFrame::GetWidth() const
  {
    if (fFrame) return fFrame->GetWidth();
    else        return 0;
  }
  
  //......................................................................
  void ScanFrame::HandleMouseWheel(Event_t *event)
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
  void ScanFrame::Record(std::string outfilename,
			 const char* comments)
  {
    art::ServiceHandle<evdb::ScanOptions> scanopt;

    // get the event information
    const art::Event *evt = evdb::EventHolder::Instance()->GetEvent();
    
    std::ofstream outfile(outfilename.c_str(), std::ios_base::app);

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
    
    // do we need to get the truth information?
    if(scanopt->fIncludeMCInfo){

      std::vector< art::Handle< std::vector<simb::MCTruth> > > mclist;

      try{
	evt->getManyByType(mclist);
	if(mclist.size() < 1){
	  mf::LogWarning("ScanWindow") << "MC truth information requested for output file"
				       << " but no MCTruth objects found in event - "
				       << " put garbage numbers into the file";
	  outfile << -999. << " " << -999. << " " << -999. << " " << -999.
		  << " " << -999. << " " << -999.;
	}
	  
	if ( mclist[0]->at(0).Origin() != simb::kBeamNeutrino ){
	  mf::LogWarning("ScanWindow") <<"Unknown particle source or truth information N/A"
				       << " put garbage numbers into the file";
	  outfile << -999. << " " << -999. << " " << -999. << " " << -999.
		  << " " << -999. << " " << -999.;
	}
	else{	      
	  // get the event vertex and energy information,
	  const simb::MCNeutrino& nu = mclist[0]->at(0).GetNeutrino();
	  
	  outfile << nu.Nu().PdgCode() << " " 
		  << nu.Nu().Vx()      << " " 
		  << nu.Nu().Vy()      << " " 
		  << nu.Nu().Vz()      << " " 
		  << nu.Nu().E()       << " " 
		  << nu.CCNC()         << " " 
		  << nu.InteractionType();
	}
      }
      catch(cet::exception &e){
	mf::LogWarning("ScanWindow") << "MC truth information requested for output file"
				     << " but no MCTruth objects found in event - "
				     << " put garbage numbers into the file";
	outfile << -999. << " " << -999. << " " << -999. << " " << -999.
		<< " " << -999. << " " << -999.;
      }

    }//end if using MC information

    // end this line for the event
    outfile << comments << std::endl;
  }

  //......................................................................
  void ScanFrame::RadioButton()
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


  //--------------------------------------------------------------------
  ScanWindow::ScanWindow() 
    : TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 50, 50)
  {  
    TGLayoutHints* fLHTopLeft   = new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 2,2,2,2);
    TGLayoutHints* fLHBotRight  = new TGLayoutHints(kLHintsBottom|kLHintsRight|kLHintsExpandX, 2,2,2,2);

    // set up the file name to store the information
    art::ServiceHandle<evdb::ScanOptions> opts;
    std::string user(gSystem->Getenv("USER"));
    fOutFileName.append(opts->fScanFileBase);
    fOutFileName.append(user);
    fOutFileName.append(".txt");

    std::ofstream outfile(fOutFileName.c_str());

    //output the labels so we know what each is
    outfile << "Run Event ";

    // figure out how many categories and maximum number of items for a category
    unsigned int ncat      = opts->fCategories.size();
    unsigned int maxFields = 1;
    unsigned int pos       = 0;
    for(unsigned int c = 0; c < opts->fCategories.size(); ++c){
      for(unsigned int p = 0; p < opts->fFieldsPerCategory[c]; ++p){
	if(opts->fFieldsPerCategory[c] > maxFields) maxFields = opts->fFieldsPerCategory[c];
 	outfile << opts->fCategories[c].c_str() << ":" << opts->fFieldLabels[pos+p].c_str() << " ";
      }
      pos += opts->fFieldsPerCategory[c];
    } // end loop over categories
    outfile << "comments" << std::endl;

    fF1 = new TGCompositeFrame(this, kCategoryWidth, 26, kVerticalFrame);
    
    fL1 = new TGLabel(fF1, "");
  
    fF1->AddFrame(fL1,fLHTopLeft);

    this->AddFrame(fF1);

    // make a canvas to hold the different fields
    fCanvas = new TGCanvas(this, kCategoryWidth*ncat, 1.1*kFieldHeight*maxFields);
    fScan = new ScanFrame(fCanvas->GetViewPort(), ncat, maxFields);
    fScan->SetCanvas(fCanvas);
    fCanvas->SetContainer(fScan->GetFrame());
    fScan->GetFrame()->SetCleanup(kDeepCleanup);

    this->AddFrame(fCanvas);

    // Button bar across the bottom
    fF3 = new TGCompositeFrame(this, kCategoryWidth, 26, kHorizontalFrame);
    fB3 = new TGTextButton(fF3, " Prev   ");
    fB4 = new TGTextButton(fF3, " Next   ");
    fB5 = new TGTextButton(fF3, " Record ");
    fComments = new TGTextEntry(fF3);
    //fComments->SetMaxLength(200);
    TGLabel *commentLabel = new TGLabel(fF3, "Comments:");

    Pixel_t red;
    gClient->GetColorByName("red", red);
    fB5->ChangeBackground(red);

    fF3->AddFrame(commentLabel, fLHTopLeft);
    fF3->AddFrame(fComments, fLHTopLeft);
    fF3->AddFrame(fB3, fLHTopLeft);
    fF3->AddFrame(fB4, fLHTopLeft);
    fF3->AddFrame(fB5, fLHTopLeft);

    fB3->Connect("Clicked()","evdb::ScanWindow",this,"Prev()");
    fB4->Connect("Clicked()","evdb::ScanWindow",this,"Next()");
    fB5->Connect("Clicked()","evdb::ScanWindow",this,"Rec()");

    this->AddFrame(fF3, fLHBotRight);
    
    this->Connect("CloseWindow()","evdb::ScanWindow",this,"CloseWindow()");

    this->Resize(1.05*kCategoryWidth*ncat+8, 1.15*kFieldHeight*maxFields+52);
    this->MapSubwindows();
    this->MapWindow();
    this->SetWindowName("Scan Dialog Window");

    delete fLHTopLeft;
    delete fLHBotRight;
  }

  //......................................................................
  ScanWindow::~ScanWindow() 
  {  
    if (fB3)            { delete fB3;             fB3             = 0; }
    if (fB4)            { delete fB4;             fB4             = 0; }
    if (fB5)            { delete fB5;             fB5             = 0; }
    if (fComments)      { delete fComments;       fComments       = 0; }
    if (fF3)            { delete fF3;             fF3             = 0; }

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
    fScan->Record(fOutFileName, fComments->GetText());

    evdb::NavState::Set(evdb::kNEXT_EVENT);
  }

}// namespace
