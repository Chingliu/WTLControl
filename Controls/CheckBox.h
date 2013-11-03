#pragma once

class CCheckBox : public CWindowImpl<CCheckBox, CButton>, public COwnerDraw<CCheckBox>
{
private:
    CFontHandle m_font;		// ����
    COLORREF m_clrText;	// �ı���ɫ
    COLORREF m_clrBk;	// ������ɫ
    BOOL m_bChecked;
public:
    BEGIN_MSG_MAP_EX( CCheckBox )
    MSG_WM_LBUTTONDOWN( OnLButtonDown )
    CHAIN_MSG_MAP_ALT( COwnerDraw<CCheckBox>, 1 )
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    
    CCheckBox()
    {
        m_bChecked = FALSE;
        m_clrBk =::GetSysColor( COLOR_BTNFACE );
        m_clrText =::GetSysColor( COLOR_BTNTEXT );
    }
    
    virtual ~CCheckBox()
    {
    
    }
    
    LRESULT OnLButtonDown( UINT uint, CPoint pt )
    {
        m_bChecked = !m_bChecked;
        SetCheck( m_bChecked );
        return 0;
    }
    
    BOOL OwnerDraw()
    {
        ModifyStyle( 0, BS_OWNERDRAW );
        SetFont( 12, _T( "����" ) );
        return TRUE;
    }
    
    void SetCheck( BOOL bCheck )
    {
        m_bChecked = bCheck;
        InvalidateRect( NULL );
    }
    
    void SetTextColor( COLORREF clrText )
    {
        m_clrText = clrText;
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
        if ( lpDrawItemStruct->CtlType != ODT_BUTTON )
        {
            return ;
        }
        CRect rcItem( lpDrawItemStruct->rcItem );
        CMemoryDC memDC( lpDrawItemStruct->hDC, rcItem );
        // �������
        memDC.FillSolidRect( &rcItem, m_clrBk );
        // ���ƾ���
        CRect rcCheck;
        rcCheck.SetRect( 0, 0, rcItem.Height(), rcItem.Height() );
        rcCheck.DeflateRect( 1, 1 );
        UINT uState = DFCS_BUTTONCHECK;
        if ( m_bChecked )
        {
            uState |= DFCS_CHECKED;
        }
        memDC.DrawFrameControl( &rcCheck, DFC_BUTTON, uState );
        // �����ı�
        HFONT hOldFont = memDC.SelectFont( m_font.m_hFont );
        memDC.SetBkMode( TRANSPARENT );
        memDC.SetTextColor( m_clrText );
        CString sText;
        GetWindowText( sText );
        if ( !sText.IsEmpty() )
        {
            CRect rcText;
            rcText.SetRect( rcCheck.Width() + 3, 0, rcItem.Width(), rcItem.Height() );
            memDC.DrawText( sText, sText.GetLength(), &rcText, DT_SINGLELINE | DT_VCENTER );
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