#include "pch.h"
#include "ListCtrlEdit.h"
#define WM_USER_EDIT_END WM_USER+101
// CListCtrlEdit
IMPLEMENT_DYNAMIC(CListCtrlEdit, CEdit)
CListCtrlEdit::CListCtrlEdit()
{
}
CListCtrlEdit::~CListCtrlEdit()
{
}
BEGIN_MESSAGE_MAP(CListCtrlEdit, CEdit)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
// CListCtrlEdit message handlers
void CListCtrlEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    // TODO: Add your message handler code here
    CWnd* pParent = this->GetParent();
    ::PostMessage(pParent->GetSafeHwnd(), WM_USER_EDIT_END, 0, 0);
}

//2
//#include "ListCtrlCombo.h"
#define WM_USER_COMBO_END WM_USER+102
// CListCtrlCombo
IMPLEMENT_DYNAMIC(CListCtrlCombo, CComboBox)
CListCtrlCombo::CListCtrlCombo()
{

}
CListCtrlCombo::~CListCtrlCombo()
{
}
BEGIN_MESSAGE_MAP(CListCtrlCombo, CComboBox)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
// CListCtrlCombo message handlers
void CListCtrlCombo::OnKillFocus(CWnd* pNewWnd)
{
    CComboBox::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
    CWnd* pParent = this->GetParent();
    ::PostMessage(pParent->GetSafeHwnd(), WM_USER_COMBO_END, 0, 0);
}

//3
//#include "EditableListCtrl.h"

#define WM_USER_EDIT_END  WM_USER+101
#define WM_USER_COMBO_END WM_USER+102

// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

CEditListCtrl::CEditListCtrl()
    : m_bEditVisible(false), m_bComboVisible(false), nItem(-1), nSubItem(-1)
{
    m_column_string = NULL;
    m_column_type = NULL;
}

CEditListCtrl::~CEditListCtrl()
{
    if (m_column_type) delete[]m_column_type;
    for (int i = 0; i < m_column_num; i++)
    {
        //if (m_column_string[i])
        {
            m_column_string[i] = "";//.ReleaseBuffer();
            // delete []m_column_string[i];
            // m_column_string[i]=NULL;
        }
        //delete m_column_string;
    }
    m_column_string = NULL;

}

void CEditListCtrl::column_type(int nColumn)//
{
    if (nColumn == 0)
    {
        nColumn = CEditListCtrl::GetHeaderCtrl()->GetItemCount();
    }
    m_column_num = nColumn;//�еĸ���

    if (m_column_type) delete[]m_column_type;
    m_column_type = new int[m_column_num];//���������������
    for (int i = 0; i < m_column_num; i++)
    {
        m_column_type[i] = 0;
    }
}
void CEditListCtrl::column_string(int nColumn)//
{
    if (nColumn == 0)
    {
        nColumn = CEditListCtrl::GetHeaderCtrl()->GetItemCount();
    }
    m_column_num = nColumn;//�еĸ���

    if (m_column_string)
    {
        for (int i = 0; i < m_column_num; i++)
        {
            // if (m_column_string[i])
            {
                m_column_string[i] = "";
                // delete []m_column_string[i];
                // m_column_string[i]=NULL;
            }
        }
        delete m_column_string;
        m_column_string = NULL;
    }

    m_column_string = new CString[m_column_num];
    for (int i = 0; i < m_column_num; i++)
    {
        m_column_string[i] = "";
    }
}

void CEditListCtrl::ShowEdit(bool bShow, int nItem, int nSubItem, CRect rcCtrl)
{
    m_bEditVisible = bShow;

    if (m_edit.m_hWnd == NULL)
    {
        m_edit.Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER,
            CRect(0, 0, 0, 0), this, 1001);
        m_edit.ShowWindow(SW_HIDE);

        CFont tpFont;
        tpFont.CreateStockObject(DEFAULT_GUI_FONT);
        m_edit.SetFont(&tpFont);
        tpFont.DeleteObject();
    }

    if (bShow == TRUE)
    {
        CString cstrItem = GetItemText(nItem, nSubItem);
        m_edit.MoveWindow(&rcCtrl);
        m_edit.ShowWindow(SW_SHOW);
        m_edit.SetWindowText(cstrItem);
        m_edit.SetFocus();
        m_edit.SetSel(-1);
    }
    else
        m_edit.ShowWindow(SW_HIDE);
}

void CEditListCtrl::ShowCombo(bool bShow, int nItem, int nSubItem, CRect rcCtrl)
{
    m_bComboVisible = bShow;

    if (m_combo.m_hWnd == NULL)
    {
        m_combo.Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | ES_LEFT | CBS_DROPDOWNLIST,
            CRect(0, 0, 0, 0), this, 1002);
        m_combo.ShowWindow(SW_HIDE);

        CFont tpFont;
        tpFont.CreateStockObject(DEFAULT_GUI_FONT);
        m_combo.SetFont(&tpFont);
        tpFont.DeleteObject();

        // m_combo.AddString(TEXT("Male"));
        // m_combo.AddString(TEXT("Female"));

    }

    if (bShow == TRUE)
    {
        //1 ���ݱ���ʵ�ʵ���Ŀm_column_string[nSubItem], ����̬��� ��Ͽ�
        CString str1 = _T(""), str2 = _T(""), str3 = _T("");
        int nPos = 0;
        str3 = "\n";//�����з����,����ȷ�����ݿ����Ǹ����ַ�
        str1 = m_column_string[nSubItem];
        str1.TrimLeft(); str1.TrimRight();

        if (str1 != "")//!str1.IsEmpty())
        {
            nPos = str1.FindOneOf(str3);
            while (nPos >= 0)
            {
                str2 = str1.Mid(0, nPos);
                m_combo.AddString(str2);
                str1 = str1.Mid(nPos + 1);
                nPos = str1.FindOneOf(str3);

            }
            if (str1 != "") { str2 = str1; m_combo.AddString(str2); }
        }

        m_combo.SetCurSel(-1);

        CString cstrItem = GetItemText(nItem, nSubItem);
        //2  ������Ͽ��(��Ŀ��+1),�༭����ռ��һ������+1, ��������Ͽ��������С
        // m_combo.MoveWindow(&rcCtrl);
        CRect rect;
        m_combo.GetDroppedControlRect(&rect);
        int nn = m_combo.GetCount(), hh = rcCtrl.Height();
        rect.SetRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, rcCtrl.top + (nn + 1) * hh);
        m_combo.MoveWindow(&rect);

        m_combo.ShowWindow(SW_SHOW);
        m_combo.SetWindowText(cstrItem);
        m_combo.SetFocus();
        int nItems = m_combo.GetCount();
        for (int i = 0; i < nItems; ++i)
        {
            CString cstrItemText;
            m_combo.GetLBText(i, cstrItemText);
            if (cstrItem == cstrItemText)
            {
                m_combo.SetCurSel(i);
                break;
            }
        }
    }
    else
    {//����ʱ ������Ͽ�, �ٴ���ʾʱ, ����ʵ�ʵ��� ,��̬���
        m_combo.ResetContent();//����
        m_combo.ShowWindow(SW_HIDE);
    }

}

BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
    ON_WM_LBUTTONDBLCLK()
    ON_NOTIFY(HDN_BEGINTRACKA, 0, CEditListCtrl::OnHdnBegintrack)
    ON_NOTIFY(HDN_BEGINTRACKW, 0, CEditListCtrl::OnHdnBegintrack)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_MESSAGE(WM_USER_EDIT_END, OnEditEnd)
    ON_MESSAGE(WM_USER_COMBO_END, OnComboEnd)
    // ON_NOTIFY ( NM_CUSTOMDRAW,IDC_LIST, OnCustomdrawMyList )  //2 �ֹ�������ϢӰ��
END_MESSAGE_MAP()

// CEditListCtrl message handlers
////3 �ֹ���ӱ�����,����ListCtrlĳ��ĳ�е�Ԫ���ǰ��,������ɫ
void CEditListCtrl::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{
    //This code based on Michael Dunn's excellent article on  
    //list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp  
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    // Take the default processing unless we set this to something else below.  
    *pResult = CDRF_DODEFAULT;
    // First thing - check the draw stage. If it's the control's prepaint  
    // stage, then tell Windows we want messages for every item.  
    if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
    {
        // This is the notification message for an item.  We'll request  
        // notifications before each subitem's prepaint stage.  
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
    {
        COLORREF m_clrText, m_clrTextBk;//ǰ��ɫ������ɫ
        int nRow, nCol;//�� �� ��
        nRow = static_cast <int>(pLVCD->nmcd.dwItemSpec);
        nCol = pLVCD->iSubItem;
        // ��ȡListCtrl��ǰ��Ԫ����ı�
        CString str = GetItemText(nRow, nCol);//m_list.

        m_clrText = pLVCD->clrText;//����ǰ���ı�ɫ
        m_clrTextBk = pLVCD->clrTextBk;//���ñ���ɫ
        if (m_column_type[nCol] == 3)//�ɱ䱳����
        {
            m_clrTextBk = (DWORD)_ttoi(str);
        }
        pLVCD->clrText = m_clrText;//����ǰ���ı�ɫ
        pLVCD->clrTextBk = m_clrTextBk;//���ñ���ɫ

        *pResult = CDRF_DODEFAULT;
    }
}


LRESULT CEditListCtrl::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
    CString strText;
    m_edit.GetWindowText(strText);
    SetItemText(nItem, nSubItem, strText);

    ShowEdit(false, -1, -1, CRect());

    return 0;
}

LRESULT CEditListCtrl::OnComboEnd(WPARAM wParam, LPARAM lParam)
{
    int nSel = m_combo.GetCurSel();
    CString cstrTextSel;
    m_combo.GetLBText(nSel, cstrTextSel);
    SetItemText(nItem, nSubItem, cstrTextSel);

    ShowCombo(false, -1, -1, CRect());

    return 0;
}

void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CRect rcCtrl;
    LVHITTESTINFO lvhti;
    lvhti.pt = point;
    nItem = SubItemHitTest(&lvhti);
    if (nItem == -1)
        return;

    nSubItem = lvhti.iSubItem;
    GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);

    //�鿴�е�����
    switch (m_column_type[lvhti.iSubItem])
    {
    case 0://listCtrl_LABEL
        break;
    case 1://listCtrl_EDIT
        ShowEdit(TRUE, nItem, nSubItem, rcCtrl);
        break;
    case 2://listCtrl_COMBO
        ShowCombo(TRUE, nItem, nSubItem, rcCtrl);
        break;
    case 3://listCtrl_STATIC
    {
        CString str = GetItemText(nItem, nSubItem);
        COLORREF clr = _ttol(str);//RGB(255,0,0);
        CColorDialog  dlg;
        dlg.m_cc.lpCustColors = &clr;//ԭ����ɫ
        if (IDOK == dlg.DoModal())
        {
            clr = dlg.GetColor();//��ѡ�����ɫ
            str.Format(L"%ld", clr);
            SetItemText(nItem, nSubItem, str);//��ɫ��ֵ����Ϊ��Ԫ����ı�
            this->Invalidate();//ˢ���б�
        }
    }
    break;


    default:
        break;

    }


    CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CEditListCtrl::OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: Add your control notification handler code here
    if (m_edit.m_hWnd != NULL && m_bEditVisible)
        OnEditEnd(0, 0);
    if (m_combo.m_hWnd != NULL && m_bComboVisible)
        OnComboEnd(0, 0);
    *pResult = 0;
}

void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    if (m_edit.m_hWnd != NULL && m_bEditVisible)
        OnEditEnd(0, 0);
    if (m_combo.m_hWnd != NULL && m_bComboVisible)
        OnComboEnd(0, 0);
    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    if (m_edit.m_hWnd != NULL && m_bEditVisible)
        OnEditEnd(0, 0);
    if (m_combo.m_hWnd != NULL && m_bComboVisible)
        OnComboEnd(0, 0);
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
