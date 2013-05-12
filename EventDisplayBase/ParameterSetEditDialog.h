///
/// \file  ParameterSetEditDialog.h
/// \brief Pop-up window for editing parameter sets
///
#ifndef EVDB_PARAMETERSETEDITDIALOG_H
#define EVDB_PARAMETERSETEDITDIALOG_H
#include "TQObject.h"
#include "RQ_OBJECT.h"
#include "TGFrame.h"
class TGTab;
class TGCanvas;
class TGTableLayout;
class TGTableLayoutHints;
class TGListBox;
class TGDoubleSlider;
class TGTextEntry;
class TGRadioButton;
class TGCheckButton;
namespace fhicl { class ParameterSet; }

namespace evdb
{
  class ParameterSetEditFrame;
  ///===================================================================  
  ///
  /// \brief A single row for editing a single parameter in a set
  ///
  class ParameterSetEditRow
  {
    RQ_OBJECT("evdb::ParameterSetEditRow");
  public:
    ParameterSetEditRow(TGHorizontalFrame* lhs,
			TGHorizontalFrame* rhs,
			const fhicl::ParameterSet& ps,
			const std::string& key);
    ~ParameterSetEditRow();
    
    void TextEntryReturnPressed();
    void ListBoxSelectionChanged();
    void ListBoxSelected(int id);
    void RadioButtonClicked();
    void CheckButtonClicked();
    void SliderPositionChanged();
    
  private:
    void SetupTextEntry(TGCompositeFrame* f, 
                        unsigned int flags,
                        const std::vector<std::string>& value);

    void SetupListBox(TGCompositeFrame* f, 
                      unsigned int flags,
                      const std::vector<std::string>& choice,
                      const std::vector<std::string>& value,
                      bool ismulti);

    void SetupRadioButtons(TGCompositeFrame* f, 
                           unsigned int flags,
                           const std::vector<std::string>& choice,
                           const std::vector<std::string>& value);
    
    void SetupCheckButton(TGCompositeFrame* f, 
			  unsigned int flags,
			  const std::vector<std::string>& choice,
			  const std::vector<std::string>& value);
    
    void SetupSlider(TGCompositeFrame* f,
                     unsigned int flags,
                     const std::vector<std::string>& choice,
                     const std::vector<std::string>& value);

  private:
    static bool IsLegalGUItag(const std::string& s);
    static void ParseGUItag(const std::string&        guitag,
                            std::string&              frame,
                            std::vector<std::string>& choice);
    static void UnpackParameter(const fhicl::ParameterSet& ps,
                                const std::string&         key,
                                unsigned int&              flags,
                                std::string&               tag,
                                std::vector<std::string>&  choice,
                                std::vector<std::string>&  value,
                                std::string&               doc);
  public:
    TGHorizontalFrame* fMother; ///< Top level frame
    TGLayoutHints*     fRightLH;///< Align to right
    TGLayoutHints*     fLeftLH; ///< Align to left
    TGCompositeFrame*  fLHS;    ///< Container for stuff on the left
    TGCompositeFrame*  fRHS;    ///< Container for stuff on the right
    TGTextButton*      fLabel;  ///< Label on the left
  public:
    TGTextEntry*                fTextEntry;
    TGListBox*                  fListBox;
    TGDoubleSlider*             fSlider;
    std::vector<TGRadioButton*> fRadioButton;
    std::vector<TGCheckButton*> fCheckButton;
  public:
    unsigned int             fParamFlags;
    std::vector<std::string> fChoice;
    std::string              fValue;
  };

  ///===================================================================  
  ///
  /// \brief A frame for editing a single paramter set
  ///
  class ParameterSetEditFrame 
  {
    RQ_OBJECT("evdb:ParameterSetEditFrame");
  public:
    ParameterSetEditFrame(TGCompositeFrame* mother, unsigned int psetid);
  private:
    TGCompositeFrame* fTopFrame;
    TGCanvas*         fCanvas;
    TGLayoutHints*    fCanvasH;
    TGCompositeFrame* fContainer;
    TGTableLayout*    fLayout;
    std::vector<TGHorizontalFrame*>   fLHS;
    std::vector<TGHorizontalFrame*>   fRHS;
    std::vector<TGTableLayoutHints*>  fLHSHints;
    std::vector<TGTableLayoutHints*>  fRHSHints;
    std::vector<ParameterSetEditRow*> fRow;
  };

  ///===================================================================  
  ///
  /// \brief Top-level interface to all parameter sets
  ///
  class ParameterSetEditDialog : public TGTransientFrame
  {
    RQ_OBJECT("evdb::ParameterSetEditDialog");
  public:
    ParameterSetEditDialog(int which);
    ~ParameterSetEditDialog();

    void Apply();
    void Cancel();
    void Done();
    void CloseWindow();
    std::string TabName(const std::string& s);
    
  private:
    TGTab*             fTGTab;
    TGHorizontalFrame* fButtons;
    TGTextButton*      fApply;
    TGTextButton*      fCancel;
    TGTextButton*      fDone;
  };
}

#endif
////////////////////////////////////////////////////////////////////////
