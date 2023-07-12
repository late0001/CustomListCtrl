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
	int nItem;         // 当前点击项
	int nSubItem;            // 当前点击子项
	bool m_bEditVisible;     // Edit是否可见
	bool m_bComboVisible;    // ComboBox是否可见

	//-----------------新增部分----------------------
public:
	void column_type(int nColumn = 0);//初始化各列的类型 
	void column_string(int nColumn = 0);//初始化各列的选择数目项，主要针对组合框 

	void OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult);//1 自绘函数改变颜色声明 

public:
	int m_column_num;//列的个数
	int* m_column_type;//列的类型, 区分是编辑框,组合框,还是普通的文本框
	CString* m_column_string;//各个列的组合框的项目字符数足


};