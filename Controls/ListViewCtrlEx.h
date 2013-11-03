#pragma once
#include <atlctrls.h>

class CListViewCtrlEx : public CWindowImpl<CListViewCtrlEx, CListViewCtrl>,
    public COwnerDraw<CListViewCtrlEx>
{
private:
    //    BOOL m_bMouseHover;
    int m_iHoverIndex;
    int m_iPreHoverIndex;
    UINT m_uItemHeight;	// ITEM高度
    COLORREF m_clrItemSelected;	// ITEM被选中时的颜色
    COLORREF m_clrItemNormal;	// ITEM 默认颜色
    CFontHandle m_font;		// 字体
    COLORREF m_clrText;	// 文本颜色
public:
    BEGIN_MSG_MAP( CListViewCtrlEx )
    MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove )
    MESSAGE_HANDLER( WM_MOUSEHOVER, OnMouseHover )
    MESSAGE_HANDLER( WM_MOUSELEAVE, OnMouseLeave )
    CHAIN_MSG_MAP_ALT( COwnerDraw<CListViewCtrlEx>, 1 )
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    
    CListViewCtrlEx()
    {
        //        m_bMouseHover = FALSE;
        m_iHoverIndex = -1;
        m_iPreHoverIndex = -1;
        m_uItemHeight = 29;
        m_clrItemSelected =::GetSysColor( COLOR_HIGHLIGHT );
        m_clrItemNormal =::GetSysColor( COLOR_WINDOW );
        m_clrText =::GetSysColor( COLOR_BTNTEXT );
        
        SetFont( 12, _T( "宋体" ) );
    }
    
    virtual ~CListViewCtrlEx()
    {
    
    }
    
    void SetFont( int iFontSize, LPCTSTR lpszFontName, BOOL bItalic = FALSE, BOOL bUnderline = FALSE )
    {
        LOGFONT lf;
        lf.lfWeight = FW_NORMAL;
        lf.lfHeight = iFontSize;
        lf.lfWidth = 0;
        lf.lfUnderline = bUnderline;
        lf.lfStrikeOut = FALSE;
        lf.lfItalic = bItalic;
        lf.lfEscapement = 0;
        lf.lfCharSet = DEFAULT_CHARSET;
        _tcscpy_s( lf.lfFaceName, lpszFontName );
        if ( !m_font.IsNull() )
        {
            m_font.DeleteObject();
        }
        m_font.CreateFontIndirect( &lf );
    }
    
    void Attach( HWND hWnd )
    {
        SubclassWindow( hWnd );
        ModifyStyle( 0, LVS_OWNERDRAWFIXED );
    }
    
    //     void SetItemHeight( UINT uHeight )
    //     {
    //         m_uItemHeight = uHeight;
    //     }
    
    LRESULT OnMouseMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        TRACKMOUSEEVENT csTME;
        csTME.cbSize = sizeof( csTME );
        csTME.dwFlags = TME_LEAVE | TME_HOVER;
        csTME.hwndTrack = m_hWnd;
        csTME.dwHoverTime = 10;
        ::_TrackMouseEvent( &csTME );
        return 0;
    }
    
    LRESULT OnMouseHover( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        CPoint pt;
        GetCursorPos( &pt );
        ::ScreenToClient( m_hWnd, &pt );
        UINT uFlag = 0;
        m_iHoverIndex = HitTest( pt, &uFlag );
        if ( LVHT_NOWHERE != uFlag )
        {
            CRect rcItem;
            GetItemRect( m_iHoverIndex, &rcItem, LVIR_BOUNDS );
            InvalidateRect( &rcItem );
            GetItemRect( m_iPreHoverIndex, &rcItem, LVIR_BOUNDS );
            InvalidateRect( &rcItem );
            m_iPreHoverIndex = m_iHoverIndex;
        }
        
        return 0;
    }
    
    LRESULT OnMouseLeave( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        CRect rcItem;
        GetItemRect( m_iHoverIndex, &rcItem, 3 );
        InvalidateRect( &rcItem );
        GetItemRect( m_iPreHoverIndex, &rcItem, 3 );
        InvalidateRect( &rcItem );
        m_iPreHoverIndex = m_iHoverIndex = -1;
        
        return 0;
    }
    
    void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
    {
        CMemoryDC memDC( lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem );
        
        // 正常状态
        memDC.FillSolidRect( &lpDrawItemStruct->rcItem, m_clrItemNormal );
        
        // 鼠标划过状态
        if ( lpDrawItemStruct->itemID == m_iHoverIndex && ( -1 != m_iHoverIndex ) )
        {
            memDC.FillSolidRect( &lpDrawItemStruct->rcItem, RGB( 255, 0, 0 ) );
        }
        
        // 选择状态
        if ( ODA_DRAWENTIRE == ( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE ) )
        {
            if ( ODS_SELECTED == ( lpDrawItemStruct->itemState & ODS_SELECTED ) )
            {
                memDC.FillSolidRect( &lpDrawItemStruct->rcItem, m_clrItemSelected );
            }
        }
        
        // 绘制文本
        HFONT hOldFont = memDC.SelectFont( m_font.m_hFont );
        memDC.SetBkMode( TRANSPARENT );
        memDC.SetTextColor( m_clrText );
        int columnCount = GetHeader().GetItemCount();
        for ( int i = 0; i < columnCount; i++ )
        {
            RECT rcSubItem;
            GetSubItemRect( lpDrawItemStruct->itemID, i, LVIR_LABEL, &rcSubItem );
            rcSubItem.left += 3;
            BSTR sText = NULL;
            GetItemText( lpDrawItemStruct->itemID, i, sText );
            if ( NULL != sText )
            {
                memDC.DrawText( sText, _tcslen( sText ), &rcSubItem, DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_LEFT | DT_VCENTER );
            }
        }
        memDC.SelectFont( hOldFont );
    }
    
    void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
    {
        lpMeasureItemStruct->itemHeight = m_uItemHeight;
    }
    
    int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct )
    {
        // all items are equal
        return 0;
    }
    
    void DeleteItem( LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/ )
    {
        // default - nothing
    }
};