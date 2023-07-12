#pragma once
class CListCtrlEdit : public CEdit
{
	DECLARE_DYNAMIC(CListCtrlEdit)

public:
	CListCtrlEdit();
	virtual ~CListCtrlEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};



//2 CListCtrlCombo

class CListCtrlCombo : public CComboBox
{
	DECLARE_DYNAMIC(CListCtrlCombo)

public:
	CListCtrlCombo();
	virtual ~CListCtrlCombo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


//3  CEditableListCtrl

class CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)
	enum { listCtrl_LABEL = 0, listCtrl_EDIT = 1, listCtrl_COMBO = 2, listCtrl_STATIC = 3 };

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	void ShowEdit(bool bShow, int nItem, int nSubItem, CRect rcCtrl);
	void ShowCombo(bool bShow, int nItem, int nSubItem, CRect rcCtrl);
	afx_msg LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComboEnd(WPARAM wParam, LPARAM lParam);


private:
	CListCtrlEdit m_edit;
	CListCtrlCombo m_combo;
	int nItem;         // ��ǰ�����
	int nSubItem;            // ��ǰ�������
	bool m_bEditVisible;     // Edit�Ƿ�ɼ�
	bool m_bComboVisible;    // ComboBox�Ƿ�ɼ�

	//-----------------��������----------------------
public:
	void column_type(int nColumn = 0);//��ʼ�����е����� 
	void column_string(int nColumn = 0);//��ʼ�����е�ѡ����Ŀ���Ҫ�����Ͽ� 

	void OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult);//1 �Ի溯���ı���ɫ���� 

public:
	int m_column_num;//�еĸ���
	int* m_column_type;//�е�����, �����Ǳ༭��,��Ͽ�,������ͨ���ı���
	CString* m_column_string;//�����е���Ͽ����Ŀ�ַ�����


};