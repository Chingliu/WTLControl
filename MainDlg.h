// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Controls\ListViewCtrlEx.h"
#include "Controls\ColorButton.h"
#include "Controls\AppBar.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
    public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>,
    public CAppBar<CMainDlg>
{
private:
    CListViewCtrlEx m_listViewCtrlEx;
    CColorButton m_btnColor;
    CHyperLink m_hyperLink;
    CBitmapButton m_bmpBtn;
    CImageList m_imgList;
public:
    enum { IDD = IDD_MAINDLG };
    
    virtual BOOL PreTranslateMessage( MSG* pMsg )
    {
        return CWindow::IsDialogMessage( pMsg );
    }
    
    virtual BOOL OnIdle()
    {
        UIUpdateChildWindows();
        return FALSE;
    }
    
    BEGIN_UPDATE_UI_MAP( CMainDlg )
    END_UPDATE_UI_MAP()
    
    BEGIN_MSG_MAP( CMainDlg )
    MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
    MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
    COMMAND_ID_HANDLER( ID_APP_ABOUT, OnAppAbout )
    COMMAND_ID_HANDLER( IDOK, OnOK )
    COMMAND_ID_HANDLER( IDCANCEL, OnCancel )
    COMMAND_ID_HANDLER( IDC_BUTTON2, OnBmpBtn )
    REFLECT_NOTIFICATIONS()
    CHAIN_MSG_MAP( CAppBar<CMainDlg> )
    END_MSG_MAP()
    
    BEGIN_DDX_MAP( CMainDlg )
    DDX_CONTROL( IDC_BUTTON1, m_btnColor )
    DDX_CONTROL( IDC_STATICLINK, m_hyperLink )
    DDX_CONTROL( IDC_BUTTON2, m_bmpBtn )
    END_DDX_MAP()
    
    // Handler prototypes (uncomment arguments if needed):
    //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
    
    LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        DoDataExchange();
        // center the dialog on the screen
        CenterWindow();
        
        // set icons
        HICON hIcon = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics( SM_CXICON ), ::GetSystemMetrics( SM_CYICON ) );
        SetIcon( hIcon, TRUE );
        HICON hIconSmall = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ) );
        SetIcon( hIconSmall, FALSE );
        
        // register object for message filtering and idle updates
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT( pLoop != NULL );
        pLoop->AddMessageFilter( this );
        pLoop->AddIdleHandler( this );
        
        UIAddChildWindowContainer( m_hWnd );
        
        InitControls();
        
        return TRUE;
    }
    
    LRESULT OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        // unregister message filtering and idle updates
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT( pLoop != NULL );
        pLoop->RemoveMessageFilter( this );
        pLoop->RemoveIdleHandler( this );
        
        return 0;
    }
    
    LRESULT OnAppAbout( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }
    
    LRESULT OnOK( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        // TODO: Add validation code
        CloseDialog( wID );
        return 0;
    }
    
    LRESULT OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        CloseDialog( wID );
        return 0;
    }
    
    void CloseDialog( int nVal )
    {
        DestroyWindow();
        ::PostQuitMessage( nVal );
    }
    
    LRESULT OnBmpBtn( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        MessageBox( _T( "CBitmapButton" ), 0, 0 );
        return 0;
    }
    
    void InitControls()
    {
        // ListViewCtrlEx
        m_listViewCtrlEx.Attach( GetDlgItem( IDC_LIST1 ) );
        //		m_listViewCtrlEx.SetItemHeight(180);
        
        m_listViewCtrlEx.SetExtendedListViewStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, 0 );
        m_listViewCtrlEx.InsertColumn( 0, _T( "Packet" ), LVCFMT_LEFT, 50 );
        m_listViewCtrlEx.InsertColumn( 1, _T( "Source" ), LVCFMT_LEFT, 150 );
        m_listViewCtrlEx.InsertColumn( 2, _T( "Destination" ), LVCFMT_LEFT, 150 );
        m_listViewCtrlEx.InsertColumn( 3, _T( "size" ), LVCFMT_LEFT, 50 );
        m_listViewCtrlEx.InsertColumn( 4, _T( "Function" ), LVCFMT_LEFT, 100 );
        m_listViewCtrlEx.InsertColumn( 5, _T( "Content" ), LVCFMT_LEFT, 600 );
        
        for ( int i = 0; i < 20; i++ )
        {
            CString s;
            s.Format( _T( "测试---------%d" ), i );
            int id = m_listViewCtrlEx.InsertItem( i, s );
            m_listViewCtrlEx.SetItemText( id, 1, _T( "hello" ) );
            m_listViewCtrlEx.SetItemText( id, 2, s );
        }
        
        // 颜色选择按钮
        m_btnColor.SetColor( RGB( 255, 0, 0 ) );
        
        // 超链接
        m_hyperLink.SetHyperLink( _T( "www.163.com" ) );
        
        // 位图按钮
        m_imgList.Create( IDB_BITMAP1, 45, 1, 0 );
        m_bmpBtn.SetImageList( m_imgList );
        m_bmpBtn.SetBitmapButtonExtendedStyle( BMPBTN_HOVER );
        m_bmpBtn.SetImages( 0, 1, 2, 3 );
        
        // AppBar
        // 		InitAppBar(APPBAR_DOCKING_ALL);
        // 		SetAutoHide(TRUE);
        // 		SetKeepSize(TRUE);
        // 		DockAppBar(APPBAR_DOCKING_LEFT);
    }
};
