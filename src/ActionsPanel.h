#pragma once
#include "wx/wx.h"
#include "IControls.h"
#include "ActionCode.h"
#include "ActionsListBox.h"

#include "bitmaps/new_action.xpm"
#include "bitmaps/rename_action.xpm"
#include "bitmaps/delete_action.xpm"

enum
{
	ID_ADD_ACTION,
	ID_REN_ACTION,
	ID_DEL_ACTION
};

class ActionsPanel : public wxPanel
{
	DECLARE_CLASS(ActionsPanel)
	DECLARE_EVENT_TABLE()

private:
	ActionsListBox		*_actList;
	wxBitmapButton		*_addActButton;
	wxBitmapButton		*_renActButton;
	wxBitmapButton		*_delActButton;
	IControls			*_controls;


	void OnAddAction( wxCommandEvent &event );
	void OnRenAction( wxCommandEvent &event );
	void OnDelAction( wxCommandEvent &event );

public:
	ActionsPanel( wxWindow *owner, ILocationPage *locPage, ActionCode *_actCode, IControls *controls );
	~ActionsPanel(void);

	ActionsListBox *GetActionsListBox() { return _actList; };
};
