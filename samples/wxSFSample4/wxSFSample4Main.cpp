/***************************************************************
 * Name:      wxSFSample4Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/
#include "wx_pch.h"

#include "wxSFSample4Main.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << wxT("-Windows");
#elif defined(__WXMAC__)
        wxbuild << wxT("-Mac");
#elif defined(__UNIX__)
        wxbuild << wxT("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << wxT("-Unicode build");
#else
        wxbuild << wxT("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(wxSFSample4Frame, wxFrame)
    EVT_CLOSE(wxSFSample4Frame::OnClose)
    EVT_MENU(idMenuQuit, wxSFSample4Frame::OnQuit)
    EVT_MENU(idMenuAbout, wxSFSample4Frame::OnAbout)
END_EVENT_TABLE()

wxSFSample4Frame::wxSFSample4Frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    SetSize(800, 600);

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(wxT(""));
    fileMenu->Append(idMenuQuit, wxT("&Quit\tAlt-F4"), wxT("Quit the application"));
    mbar->Append(fileMenu, wxT("&File"));

    wxMenu* helpMenu = new wxMenu(wxT(""));
    helpMenu->Append(idMenuAbout, wxT("&About\tF1"), wxT("Show info about this application"));
    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

    m_nControlType = 0;

    // set some diagram manager properties if necessary...
    // set accepted shapes (accept only wxSFControlShape)
    m_Manager.AcceptShape(wxT("wxSFControlShape"));

    // create shape canvas and associate it with shape manager
    m_pCanvas = new wxSFShapeCanvas(&m_Manager, this);
    // set some shape canvas properties if necessary...
    m_pCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_USE);

    // connect event handlers to shape canvas
    m_pCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxSFSample4Frame::OnLeftClickCanvas), NULL, this);
    m_pCanvas->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxSFSample4Frame::OnRightClickCanvas), NULL, this);

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxT("Hello wxShapeFramework user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    Center();
}


wxSFSample4Frame::~wxSFSample4Frame()
{
}

void wxSFSample4Frame::OnLeftClickCanvas(wxMouseEvent& event)
{
    // HINT: perform your user actions here...

    // ... and then process standard canvas operations
    event.Skip();
}

void wxSFSample4Frame::OnRightClickCanvas(wxMouseEvent& event)
{
    // add new rectangular shape to the diagram ...
    wxSFControlShape* pShape = (wxSFControlShape*)m_Manager.AddShape(CLASSINFO(wxSFControlShape), event.GetPosition());
    // set some shape's properties...
    if(pShape)
    {
        // set accepted child shapes for the new shape
        pShape->ClearAcceptedChilds();

        // Some visual aspects can be set here:
        pShape->SetControlOffset(5);
        //pShape->SetModBorder(*wxTRANSPARENT_PEN);
        //pShape->SetModFill(wxBrush(*wxRED, wxCROSSDIAG_HATCH));

        // Assign managed GUI control to the canvas. Remember the control shape now owns the GUI control and it is
        // deleted by the shape control in its destructor. If you want to remove the GUI control from the parent shape
        // just assign a new control or NULL value to it. You can also specify whether managed GUI control
        // is resized in accordance to dimensions of its parent control shape or vice versa.
        pShape->SetControl(CreateGUIControl(), sfFIT_SHAPE_TO_CONTROL);

        // You can specify whether events generated by the managed control are processed by the shape canvas
        // or/and the widget as well. Note that GUI controls differ in a way how they process events
        // so the behaviour can be different for various widgets.
        pShape->SetEventProcessing(wxSFControlShape::evtMOUSE2CANVAS | wxSFControlShape::evtKEY2CANVAS);
        //pShape->SetEventProcessing(wxSFControlShape::evtMOUSE2GUI | wxSFControlShape::evtKEY2GUI);
    }

    // ... and process standard canvas operations
    event.Skip();
}

void wxSFSample4Frame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample4Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample4Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg = wxbuildinfo(long_f);

    msg += wxT("\nWelcome to wxSFShapeFramework Sample 4 (c) Michal Bliznak, 2008\n\n");
    msg += wxT("Sample demostrates posibility to manage GUI controls (widgets) by the wxSFControlShape.\n");
    msg += wxT("wxSF event handlers are connected via 'Connect' function.\n\n");
    msg += wxT("Usage:\n");
    msg += wxT(" - Left mouse click operates with inserted GUI controls\n");
    msg += wxT(" - Right mouse click inserts a GUI control to the canvas\n");
    msg += wxT(" - DEL key removes selected shape\n");

    wxMessageBox(msg, wxT("wxShapeFramework Sample 4"));
}

wxWindow* wxSFSample4Frame::CreateGUIControl()
{
    m_nControlType = ( m_nControlType + 1 ) % 5;

    switch( m_nControlType )
    {
        case 0:
            return new wxButton(m_pCanvas, wxID_ANY, wxT("Hello World!"), wxDefaultPosition, wxSize(100, 50));

        case 1:
            return new wxTextCtrl(m_pCanvas, wxID_ANY, wxT("Hello World!"), wxDefaultPosition, wxSize(150, 100), wxTE_MULTILINE);

        case 2:
            return new wxSlider(m_pCanvas, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize(150, -1));

        case 3:
        {
            wxGauge *pCtrl = new wxGauge(m_pCanvas, wxID_ANY, 50, wxDefaultPosition, wxSize(100, -1));
            pCtrl->SetValue(25);
            return pCtrl;
        }

        case 4:
            return new TestPanel(m_pCanvas);

        default:
            return NULL;
    }
}

TestPanel::TestPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 100))
{
    fBig = false;
    btnResize = new wxButton(this, idResize, wxT("Resize me!!!"), wxPoint(10, 10), wxDefaultSize);

    Connect( idResize, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TestPanel::OnBtnResize ) );

    SetBackgroundColour(wxColour(100, 100, 200));
}

void TestPanel::OnBtnResize(wxCommandEvent& WXUNUSED(event))
{
    if( !fBig ) SetClientSize(GetClientSize() + wxSize(50, 50));
    else
        SetClientSize(GetClientSize() - wxSize(50, 50));

    fBig = !fBig;
}
