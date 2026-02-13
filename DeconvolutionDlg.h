// DeconvolutionDlg.h : header file
//

#pragma once


// CDeconvolutionDlg dialog
class CDeconvolutionDlg : public CDialog
{
	// Construction
public:
	CDeconvolutionDlg(CWnd* pParent = NULL);	// standard constructor

	void DrawKoord(CDC*, CRect, CString, CString);
	void Draw1Graph(double*, CDC*, CRect, CPen*, int, CString, CString);
	void Draw2Graph(double*, CPen*, double*, CPen*, CDC*, CRect, float, CString, CString);
	void CalculateSignalAndPulse();
	void CalculateConvolution();
	double MHJ(int, float*);
	void FuncShym();
	void ConvShym();
	double function(float*);
	void CalculateDeconvolution();
	void CalculateReconvolution();

	CWnd* SignalAndPulseWnd;
	CWnd* ConvolutionWnd;
	CWnd* DeconvolutionWnd;
	CWnd* SignalColorWnd;
	CWnd* PulseColorWnd;
	CWnd* ConvolutionColorWnd;
	CWnd* DSignalColorWnd;
	CWnd* DeconvolutionColorWnd;
	CWnd* ReconvolutionColorWnd;

	CDC* SignalAndPulseDc;
	CDC* ConvolutionDc;
	CDC* DeconvolutionDc;
	CDC* SignalColorDc;
	CDC* PulseColorDc;
	CDC* ConvolutionColorDc;
	CDC* DSignalColorDc;
	CDC* DeconvolutionColorDc;
	CDC* ReconvolutionColorDc;

	CRect SignalAndPulsePic;
	CRect ConvolutionPic;
	CRect DeconvolutionPic;
	CRect SignalColorPic;
	CRect PulseColorPic;
	CRect ConvolutionColorPic;
	CRect DSignalColorPic;
	CRect DeconvolutionColorPic;
	CRect ReconvolutionColorPic;

	CPen koordpen, netkoordpen, signalpen, pulsepen, convpen, deconvpen, reconvpen;
	CPen* pen;
	CFont fontgraph;
	CFont* font;


	// Dialog Data
	enum { IDD = IDD_DECONVOLUTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int
		KolToch;
	double
		Ampl1,
		Ampl2,
		Ampl3,
		x1,
		x2,
		x3,
		Sigma1,
		Sigma2,
		Sigma3,
		AmplPulse,
		xPulse,
		SigmaPulse,
		InShym,
		OutShym,
		Pred,
		FuncZnach,
		fd; //этот параметр просто для красоты
	double
		* SignalMass,
		* PulseMass,
		* ConvolutionMass,
		* DeconvSignalMass,
		* ReconvolutionMass;
	float
		* lambda;
	double
		Min,
		Max,
		SignalMin,
		SignalMax,
		PulseMin,
		PulseMax,
		ConvolutionMin,
		ConvolutionMax,
		DeconvSignalMin,
		DeconvSignalMax,
		ReconvolutionMin,
		ReconvolutionMax,
		Mass1Min,
		Mass1Max,
		Mass2Min,
		Mass2Max;
	double
		xx0,
		xxmax,
		yy0,
		yymax,
		xxi,
		yyi,
		iter;
	char
		znach[10000];
	bool
		DrawSignalAndPulseKoordFlag,
		DrawSignalAndPulseFlag,
		DrawConvolutionKoordFlag,
		DrawConvolutionFlag,
		DrawDeconvolutionKoordFlag,
		DrawDeconvolutionFlag,
		DrawReconvolutionFlag,
		killtimer,
		sm,
		pm,
		cm,
		l,
		rm;
public:
	afx_msg void OnBnClickedDrawsignalandpulse();
	afx_msg void OnBnClickedDrawdeconvolution();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDrawdeconvolution2();
};
