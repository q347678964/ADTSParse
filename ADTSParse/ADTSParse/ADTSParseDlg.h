
// ADTSParseDlg.h : ͷ�ļ�
//

#pragma once

// CADTSParseDlg �Ի���
class CADTSParseDlg : public CDialogEx
{
// ����
public:
	CADTSParseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ADTSPARSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	CString strFilePath_adts;
	void DebugInfo(CString String);
	void ADTSParseHandler(void);
};
