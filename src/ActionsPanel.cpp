#include "ActionsPanel.h"

IMPLEMENT_CLASS(ActionsPanel, wxPanel)

BEGIN_EVENT_TABLE(ActionsPanel, wxPanel)
	EVT_BUTTON(ID_ADD_ACTION, ActionsPanel::OnAddAction)
	EVT_BUTTON(ID_REN_ACTION, ActionsPanel::OnRenAction)
	EVT_BUTTON(ID_DEL_ACTION, ActionsPanel::OnDelAction)
END_EVENT_TABLE()

ActionsPanel::ActionsPanel( wxWindow *owner, ILocationPage *locPage, ActionCode *actCode, IControls *controls ) : wxPanel( owner )
{
	_controls = controls;

	wxBitmap bitmaps[3];
	bitmaps[0] = wxBitmap(new_action_xpm);
	bitmaps[1] = wxBitmap(rename_action_xpm);
	bitmaps[2] = wxBitmap(delete_action_xpm);

	_actList = new ActionsListBox( owner, wxID_ANY, locPage, actCode, controls );
	wxSizer *sizer2 = new wxBoxSizer ( wxHORIZONTAL );

	_addActButton = new wxBitmapButton( this, ID_ADD_ACTION, bitmaps[0], wxDefaultPosition, wxSize(24,24) );
	_renActButton = new wxBitmapButton( this, ID_REN_ACTION, bitmaps[1], wxDefaultPosition, wxSize(24,24) );
	_delActButton = new wxBitmapButton( this, ID_DEL_ACTION, bitmaps[2], wxDefaultPosition, wxSize(24,24) );

	sizer2->Add( _addActButton );
	sizer2->Add( _renActButton );
	sizer2->Add( _delActButton );

	wxSizer *sizer1 = new wxBoxSizer( wxVERTICAL );

	sizer1->Add( sizer2 );
	sizer1->Add( _actList, 1, wxALL|wxGROW );

	bool isAnyAction = (_actList->GetSelection() < 0);
	_renActButton->Enable(!isAnyAction);
	_delActButton->Enable(!isAnyAction);

	SetSizerAndFit( sizer1 );
	SetAutoLayout( true );
}

ActionsPanel::~ActionsPanel(void)
{
}

void ActionsPanel::OnAddAction( wxCommandEvent &event )
{
	_controls->AddActionOnSelectedLoc();
	bool isAnyAction = (_actList->GetSelection() < 0);
	_renActButton->Enable(!isAnyAction);
	_delActButton->Enable(!isAnyAction);
}

void ActionsPanel::OnRenAction( wxCommandEvent &event )
{
	_controls->RenameSelectedAction();
}

void ActionsPanel::OnDelAction( wxCommandEvent &event )
{
	_controls->DeleteSelectedAction();
	bool isAnyAction = (_actList->GetSelection() < 0);
	_renActButton->Enable(!isAnyAction);
	_delActButton->Enable(!isAnyAction);
}