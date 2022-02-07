///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* panelSizer;
	panelSizer = new wxBoxSizer( wxVERTICAL );

	drawPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 600,600 ), wxTAB_TRAVERSAL );
	drawPanel->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );

	panelSizer->Add( drawPanel, 1, wxEXPAND, 5 );


	mainSizer->Add( panelSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* menuSizer;
	menuSizer = new wxBoxSizer( wxVERTICAL );

	loadButton = new wxButton( this, wxID_ANY, wxT("Wczytaj"), wxDefaultPosition, wxDefaultSize, 0 );
	menuSizer->Add( loadButton, 0, wxALIGN_CENTER|wxALL, 5 );

	saveButton = new wxButton( this, wxID_ANY, wxT("Zapisz"), wxDefaultPosition, wxDefaultSize, 0 );
	menuSizer->Add( saveButton, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString vectorChoiceChoices[] = { wxT("[1,0,0]"), wxT("[0,1,0]"), wxT("[0,0,1]"), wxT("[1,1,0]"), wxT("[1,0,1]"), wxT("[0,1,1]") };
	int vectorChoiceNChoices = sizeof( vectorChoiceChoices ) / sizeof( wxString );
	vectorChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vectorChoiceNChoices, vectorChoiceChoices, 0 );
	vectorChoice->SetSelection( 0 );
	menuSizer->Add( vectorChoice, 0, wxALIGN_CENTER|wxALL, 5 );

	wxBoxSizer* leftRightSizer;
	leftRightSizer = new wxBoxSizer( wxHORIZONTAL );

	previousSliceButton = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxSize( 30,30 ), 0 );
	leftRightSizer->Add( previousSliceButton, 0, wxALL, 5 );

	nextSliceButton = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,30 ), 0 );
	leftRightSizer->Add( nextSliceButton, 0, wxALL, 5 );


	menuSizer->Add( leftRightSizer, 1, wxALIGN_CENTER, 5 );

	loadGauge = new wxGauge( this, wxID_ANY, 600, wxDefaultPosition, wxSize( 75,-1 ), wxGA_HORIZONTAL );
	loadGauge->SetValue( 0 );
	menuSizer->Add( loadGauge, 0, wxALIGN_CENTER|wxALL, 5 );


	mainSizer->Add( menuSizer, 0, wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	loadButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::loadButtonClick ), NULL, this );
	saveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::saveButtonClick ), NULL, this );
	vectorChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MyFrame1::OnChoice ), NULL, this );
	previousSliceButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::previousSliceClick ), NULL, this );
	nextSliceButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::nextSliceClick ), NULL, this );
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	loadButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::loadButtonClick ), NULL, this );
	saveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::saveButtonClick ), NULL, this );
	vectorChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MyFrame1::OnChoice ), NULL, this );
	previousSliceButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::previousSliceClick ), NULL, this );
	nextSliceButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::nextSliceClick ), NULL, this );

}
