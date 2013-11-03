#pragma once

class CStaticEx : public CWindowImpl<CStaticEx, CStatic>, public COwnerDraw<CStaticEx>
{
private:
    CFontHandle m_font;		// 字体
    COLORREF m_clrText;	// 文本颜色
    COLORREF m_clrBk;	// 背景颜色
public:
    BEGIN_MSG_MAP( CStaticEx )
    CHAIN_MSG_MAP_ALT( COwnerDraw<CStaticEx>, 1 )
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    
    CStaticEx()
    {
        m_clrBk =::GetSysColor( COLOR_BTNFACE );
        m_clrText =::GetSysColor( COLOR_BTNTEXT );
    }
    
    virtual ~CStaticEx()
    {
    
    }
    
    BOOL Attach( HWND hWnd )
    {
        SubclassWindow( hWnd );
        ModifyStyle( 0, SS_OWNERDRAW );
        SetFont( 12, _T( "宋体" ) );
        return TRUE;
    }
    
    void SetTextColor( COLORREF clrText )
    {
        m_clrText = clrText;
        InvalidateRect( NULL );
    }
    
    void SetBkColor( COLORREF clrBk )
    {
        m_clrBk = clrBk;
        InvalidateRect( NULL );
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
    
    void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
    {
        CRect rcItem( lpDrawItemStruct->rcItem );
        CMemoryDC memDC( lpDrawItemStruct->hDC, rcItem );
        // 背景填充
        memDC.FillSolidRect( &rcItem, m_clrBk );
        // 绘制文本
        HFONT hOldFont = memDC.SelectFont( m_font.m_hFont );
        memDC.SetBkMode( TRANSPARENT );
        memDC.SetTextColor( m_clrText );
        CString sText;
        GetWindowText( sText );
        if ( !sText.IsEmpty() )
        {
            memDC.DrawText( sText, sText.GetLength(), &rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
        }
        memDC.SelectFont( hOldFont );
    }
    
    void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
    {
        lpMeasureItemStruct->itemHeight = 30;
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
protected:
private:
};