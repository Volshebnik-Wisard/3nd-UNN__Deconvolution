// DeconvolutionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Deconvolution.h"
#include "DeconvolutionDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int timer = 0;


// CDeconvolutionDlg dialog
CDeconvolutionDlg::CDeconvolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeconvolutionDlg::IDD, pParent),

	Ampl1(4),
	Ampl2(2),
	Ampl3(3),
	x1(8),
	x2(30),
	x3(40),
	Sigma1(4),
	Sigma2(3),
	Sigma3(5),
	AmplPulse(1),
	SigmaPulse(3),
	KolToch(50),
	InShym(0),
	OutShym(0),
	Pred(0.001),
	FuncZnach(0),
	fd(1)

	/*
	Ampl1(1),
	Ampl2(2),
	Ampl3(3),
	x1(8),
	x2(18),
	x3(25),
	Sigma1(2),
	Sigma2(3),
	Sigma3(2),
	AmplPulse(2),
	SigmaPulse(3),
	KolToch(32),
	InShym(0),
	OutShym(0),
	Pred(0.001),
	FuncZnach(0),
	fd(1) //этот параметр просто для красоты
	*/


{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeconvolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_KolToch, KolToch);

	DDX_Text(pDX, IDC_Ampl1, Ampl1);
	DDX_Text(pDX, IDC_Ampl2, Ampl2);
	DDX_Text(pDX, IDC_Ampl3, Ampl3);

	DDX_Text(pDX, IDC_x1, x1);
	DDX_Text(pDX, IDC_x2, x2);
	DDX_Text(pDX, IDC_x3, x3);

	DDX_Text(pDX, IDC_Sigma1, Sigma1);
	DDX_Text(pDX, IDC_Sigma2, Sigma2);
	DDX_Text(pDX, IDC_Sigma3, Sigma3);

	DDX_Text(pDX, IDC_AmplPulse, AmplPulse);
	DDX_Text(pDX, IDC_SigmaPulse, SigmaPulse);

	DDX_Text(pDX, IDC_FuncZnach, FuncZnach);
	DDX_Text(pDX, IDC_KolToch2, InShym);
	DDX_Text(pDX, IDC_KolToch3, OutShym);
	DDX_Text(pDX, IDC_FuncZnach2, Pred);
	DDX_Text(pDX, IDC_KolToch4, fd); //этот параметр просто для красоты
}

BEGIN_MESSAGE_MAP(CDeconvolutionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DrawSignalAndPulse, &CDeconvolutionDlg::OnBnClickedDrawsignalandpulse)
	ON_BN_CLICKED(IDC_DrawDeconvolution, &CDeconvolutionDlg::OnBnClickedDrawdeconvolution)
	ON_BN_CLICKED(IDC_DrawDeconvolution2, &CDeconvolutionDlg::OnBnClickedDrawdeconvolution2)
	ON_WM_TIMER()

END_MESSAGE_MAP()


// CDeconvolutionDlg message handlers



//-----------------------------------Отрисовка_Графиков----------------------------------------------



BOOL CDeconvolutionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// окно сигнала и импульсной характеристики
	SignalAndPulseWnd = GetDlgItem(IDC_SignalAndPulse);
	SignalAndPulseDc = SignalAndPulseWnd->GetDC();
	SignalAndPulseWnd->GetClientRect(&SignalAndPulsePic);
	// окно свёртки
	ConvolutionWnd = GetDlgItem(IDC_Convolution);
	ConvolutionDc = ConvolutionWnd->GetDC();
	ConvolutionWnd->GetClientRect(&ConvolutionPic);
	// окно деконволюции
	DeconvolutionWnd = GetDlgItem(IDC_Deconvolution);
	DeconvolutionDc = DeconvolutionWnd->GetDC();
	DeconvolutionWnd->GetClientRect(&DeconvolutionPic);
	// цвет исходного сигнала
	SignalColorWnd = GetDlgItem(IDC_SignalColor);
	SignalColorDc = SignalColorWnd->GetDC();
	SignalColorWnd->GetClientRect(&SignalColorPic);
	// цвет импульсной характеристики
	PulseColorWnd = GetDlgItem(IDC_PulseColor);
	PulseColorDc = PulseColorWnd->GetDC();
	PulseColorWnd->GetClientRect(&PulseColorPic);
	// цвет свёртки
	ConvolutionColorWnd = GetDlgItem(IDC_ConvolutionColor);
	ConvolutionColorDc = ConvolutionColorWnd->GetDC();
	ConvolutionColorWnd->GetClientRect(&ConvolutionColorPic);
	// цвет исходного сигнала
	DSignalColorWnd = GetDlgItem(IDC_DSignalColor);
	DSignalColorDc = DSignalColorWnd->GetDC();
	DSignalColorWnd->GetClientRect(&DSignalColorPic);
	// цвет деконволюции
	DeconvolutionColorWnd = GetDlgItem(IDC_DeconvolutionColor);
	DeconvolutionColorDc = DeconvolutionColorWnd->GetDC();
	DeconvolutionColorWnd->GetClientRect(&DeconvolutionColorPic);
	// цвет повторной свёртки
	ReconvolutionColorWnd = GetDlgItem(IDC_ReconvolutionColor);
	ReconvolutionColorDc = ReconvolutionColorWnd->GetDC();
	ReconvolutionColorWnd->GetClientRect(&ReconvolutionColorPic);

	// флаги
	DrawSignalAndPulseKoordFlag = true;
	DrawSignalAndPulseFlag = false;
	DrawConvolutionKoordFlag = true;
	DrawConvolutionFlag = false;
	DrawDeconvolutionKoordFlag = true;
	DrawDeconvolutionFlag = false;
	DrawReconvolutionFlag = false;
	killtimer = false;
	sm = false;
	pm = false;
	cm = false;
	l = false;
	rm = false;

	//RGB(222, 12, 98) - малиновый
	//RGB(255, 77, 0) - апельсиновый
	//RGB(90, 170, 160) - кобальтовый
	//RGB(84, 0, 153) - тёмно-фиолетовый

	// создание кисти для осей координат
	const COLORREF koordcolor = RGB(75, 75, 75);
	koordpen.CreatePen(PS_SOLID, 2, koordcolor);
	// создание кисти для сетки координат
	const COLORREF netkoordcolor = RGB(225, 225, 225);
	netkoordpen.CreatePen(PS_DOT, 1, netkoordcolor);
	// создание кисти для сигнала
	const COLORREF signalcolor = RGB(80, 160, 170);
	signalpen.CreatePen(PS_SOLID, 2, signalcolor);
	// создание кисти для импульсной характеристики
	const COLORREF pulsecolor = RGB(255, 77, 0);
	pulsepen.CreatePen(PS_SOLID, 2, pulsecolor);
	// создание кисти для свёртки
	const COLORREF convcolor = RGB(84, 0, 153);
	convpen.CreatePen(PS_SOLID, 2, convcolor);
	// создание кисти для деконволюции
	const COLORREF deconvcolor = RGB(222, 12, 98);
	deconvpen.CreatePen(PS_SOLID, 2, deconvcolor);
	// создание кисти для повторной свёртки
	const COLORREF reconvcolor = RGB(222, 12, 98);
	reconvpen.CreatePen(PS_SOLID, 2, reconvcolor);
	// создание шрифта для графиков
	LOGFONT logfont = {
		14,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial") };				// lpszFacename
	fontgraph.CreateFontIndirect(&logfont);
	/*
	SignalMass = new double[KolToch];
	PulseMass = new double[KolToch];
	lambda = new float[KolToch];
	DeconvSignalMass = new double[KolToch];
	*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeconvolutionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDeconvolutionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		UpdateData(false);
		if (DrawSignalAndPulseKoordFlag == true)
			DrawKoord(SignalAndPulseDc, SignalAndPulsePic, CString("x"), CString("A"));
		if (DrawConvolutionKoordFlag == true)
			DrawKoord(ConvolutionDc, ConvolutionPic, CString("x"), CString("A"));
		if (DrawDeconvolutionKoordFlag == true)
			DrawKoord(DeconvolutionDc, DeconvolutionPic, CString("x"), CString("A"));
		if (DrawSignalAndPulseFlag == true)
		{
			Draw2Graph(SignalMass, &signalpen, PulseMass, &pulsepen, SignalAndPulseDc, SignalAndPulsePic, (float)KolToch, CString("x"), CString("A"));
		}
		if (DrawConvolutionFlag == true)
		{
			Draw1Graph(ConvolutionMass, ConvolutionDc, ConvolutionPic, &convpen, KolToch, CString("x"), CString("A"));
		}
		if (DrawDeconvolutionFlag == true)
		{
			Draw2Graph(SignalMass, &signalpen, DeconvSignalMass, &deconvpen, DeconvolutionDc, DeconvolutionPic, (float)KolToch, CString("x"), CString("A"));
		}
		if (DrawReconvolutionFlag == true)
		{
			Draw2Graph(ConvolutionMass, &convpen, ReconvolutionMass, &reconvpen, ConvolutionDc, ConvolutionPic, (float)KolToch, CString("x"), CString("A"));
		}

		SignalColorDc->FillSolidRect(
			&SignalColorPic,
			RGB(80, 160, 170));
		PulseColorDc->FillSolidRect(
			&PulseColorPic,
			RGB(255, 77, 0));
		ConvolutionColorDc->FillSolidRect(
			&ConvolutionColorPic,
			RGB(84, 0, 153));
		DSignalColorDc->FillSolidRect(
			&DSignalColorPic,
			RGB(80, 160, 170));
		DeconvolutionColorDc->FillSolidRect(
			&DeconvolutionColorPic,
			RGB(222, 12, 98));
		ReconvolutionColorDc->FillSolidRect(
			&ReconvolutionColorPic,
			RGB(222, 12, 98));
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDeconvolutionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//Система координат
void CDeconvolutionDlg::DrawKoord(CDC* WinDc, CRect WinPic, CString Abs, CString Ord)
{
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	//заливка фона графика белым цветом
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	//отрисовка сетки координат
	pen = MemDc->SelectObject(&netkoordpen);
	//вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	//горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	//отрисовка осей
	pen = MemDc->SelectObject(&koordpen);
	//отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	//деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	//отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	//деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}
	//подписи осей
	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	//установка шрифта
	MemDc->SelectObject(&fontgraph);
	//подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	// подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	//вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}



//1-ый и 3-ий графики
void CDeconvolutionDlg::Draw2Graph(double* Mass1, CPen* graph1pen, double* Mass2, CPen* graph2pen, CDC* WinDc, CRect WinPic, float AbsMax, CString Abs, CString Ord)
{
	//----- поиск максимального и минимального значения -----------------------------
	Mass1Min = Mass1[0]; Mass1Max = Mass1[0];
	Mass2Min = Mass2[0]; Mass2Max = Mass2[0];
	for (int i = 1; i < KolToch; i++)
	{
		if (Mass1[i] < Mass1Min)
		{
			Mass1Min = Mass1[i];
		}
		if (Mass1[i] > Mass1Max)
		{
			Mass1Max = Mass1[i];
		}
		if (Mass2[i] < Mass2Min)
		{
			Mass2Min = Mass2[i];
		}
		if (Mass2[i] > Mass2Max)
		{
			Mass2Max = Mass2[i];
		}
	}
	if (Mass2Max > Mass1Max)
	{
		Max = Mass2Max;
	}
	else
	{
		Max = Mass1Max;
	}
	if (Mass2Min < Mass1Min)
	{
		Min = Mass2Min;
	}
	else
	{
		Min = Mass1Min;
	}
	//отрисовка
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	//заливка фона графика белым цветом
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	//отрисовка сетки координат
	pen = MemDc->SelectObject(&netkoordpen);
	// вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	// горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	//отрисовка осей
	pen = MemDc->SelectObject(&koordpen);
	// отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	//деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	//отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	//деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}
	//подписи осей
	//установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	// установка шрифта
	MemDc->SelectObject(&fontgraph);
	//подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	//подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	//выбор области для рисования
	xx0 = WinPic.Width() * 2 / 25;	xxmax = WinPic.Width() * 24 / 25;
	yy0 = WinPic.Height() / 10;	yymax = WinPic.Height() * 9 / 10;
	//отрисовка первого графика
	pen = MemDc->SelectObject(graph1pen);
	MemDc->MoveTo(xx0, yymax + (Mass1[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass1[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	//отрисовка второго графика
	pen = MemDc->SelectObject(graph2pen);
	MemDc->MoveTo(xx0, yymax + (Mass2[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass2[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	//вывод числовых значений
	//по оси абсцисс
	for (int i = 4; i < 25; i += 3)
	{
		sprintf(znach, "%5.1f", (i - 1) * (float)AbsMax / 22);
		MemDc->TextOut(i * WinPic.Width() / 25 + 2, WinPic.Height() * 9 / 10 + 2, CString(znach));
	}
	//по оси ординат
	sprintf(znach, "%5.1f", Max);	        MemDc->TextOut(32, WinPic.Height() / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.75 * Max);	MemDc->TextOut(32, WinPic.Height() * 5 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.5 * Max);  	MemDc->TextOut(32, WinPic.Height() * 9 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.25 * Max);	MemDc->TextOut(32, WinPic.Height() * 13 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0 * Max);	    MemDc->TextOutW(32, WinPic.Height() * 9 / 10 + 2, CString(znach));
	//вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}



//2-ой график
void CDeconvolutionDlg::Draw1Graph(double* Mass, CDC* WinDc, CRect WinPic, CPen* graphpen, int AbsMax, CString Abs, CString Ord)
{
	// поиск максимального и минимального значения
	Min = Mass[0];
	Max = Mass[0];
	for (int i = 1; i < KolToch; i++)
	{
		if (Mass[i] < Min)
		{
			Min = Mass[i];
		}
		if (Mass[i] > Max)
		{
			Max = Mass[i];
		}
	}
	//отрисовка
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	//заливка фона графика белым цветом
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	//отрисовка сетки координат
	pen = MemDc->SelectObject(&netkoordpen);
	// вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	// горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	//отрисовка осей
	pen = MemDc->SelectObject(&koordpen);
	//отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	//деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	//отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	//деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}
	//подписи осей
	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	// установка шрифта
	MemDc->SelectObject(&fontgraph);
	// подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	// подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	// выбор области для рисования
	xx0 = WinPic.Width() * 2 / 25;
	xxmax = WinPic.Width() * 24 / 25;
	yy0 = WinPic.Height() / 10;
	yymax = WinPic.Height() * 9 / 10;
	// отрисовка
	pen = MemDc->SelectObject(graphpen);
	MemDc->MoveTo(xx0, yymax + (Mass[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	//вывод числовых значений
	// по оси абсцисс
	for (int i = 4; i < 25; i += 3)
	{
		sprintf(znach, "%5.1f", (i - 1) * (float)AbsMax / 22);		MemDc->TextOut(i * WinPic.Width() / 25 + 2, WinPic.Height() * 9 / 10 + 2, CString(znach));
	}
	// по оси ординат
	sprintf(znach, "%5.1f", Max);	        MemDc->TextOut(32, WinPic.Height() / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.75 * Max); 	MemDc->TextOut(32, WinPic.Height() * 5 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.5 * Max);  	MemDc->TextOut(32, WinPic.Height() * 9 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.25 * Max);    MemDc->TextOut(32, WinPic.Height() * 13 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.0);	        MemDc->TextOutW(32, WinPic.Height() * 9 / 10 + 2, CString(znach));
	//вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}



//-----------------------------------Рассчёты--------------------------------------------------------



//Функция потока
DWORD dwThread;
HANDLE hThread;
DWORD WINAPI MyProc(PVOID pv)
{
	CDeconvolutionDlg* p = (CDeconvolutionDlg*)pv;
	p->MHJ(p->KolToch, p->lambda);
	return 0;
}

// вычисление сигнала и импульсной характеристики
void CDeconvolutionDlg::CalculateSignalAndPulse()
{
	// создание массивов исходного сигнала и импульсной характеристики
	UpdateData(true);
	if (sm == true)	delete[] SignalMass;
	if (pm == true)	delete[] PulseMass;
	SignalMass = new double[KolToch];
	PulseMass = new double[KolToch];
	// заполнение массива исходного сигнала
	for (int i = 0; i < KolToch; i++)
	{
		SignalMass[i] = Ampl1 * exp(-(x1 - i) * (x1 - i) / (Sigma1 * Sigma1)) + Ampl2 * exp(-(x2 - i) * (x2 - i) / (Sigma2 * Sigma2)) + Ampl3 * exp(-(x3 - i) * (x3 - i) / (Sigma3 * Sigma3));
	}
	FuncShym();
	sm = true;
	// заполнение массива импульсной характеристики
	for (int i = 0; i < KolToch; i++)
	{
		PulseMass[i] = AmplPulse * exp(-(i - KolToch / 2) * (i - KolToch / 2) / (SigmaPulse * SigmaPulse));
	}
	double* HelpMass = new double[KolToch];
	for (int i = 0; i < KolToch; i++)
	{
		HelpMass[i] = PulseMass[i];
	}
	for (int i = 0; i < KolToch / 2; i++)
	{
		PulseMass[i] = HelpMass[i + KolToch / 2];
		PulseMass[i + KolToch / 2] = HelpMass[i];
	}
	delete[] HelpMass;
	pm = true;
}



// вычисление свёртки/выходного сигнала
void CDeconvolutionDlg::CalculateConvolution()
{
	// формирование  массива свёртки
	UpdateData(true);
	if (cm == true)	delete[] ConvolutionMass;
	ConvolutionMass = new double[KolToch];
	// заполнение массива свертки
	for (int i = 0; i < KolToch; i++)
	{
		iter = 0;
		for (int j = 0; j < KolToch; j++)
		{
			int m = i - j;
			if (m < 0)
			{
				m += KolToch;
			}
			iter += PulseMass[m] * SignalMass[j];
		}
		ConvolutionMass[i] = iter;
	}
	ConvShym();
	cm = true;
}



// вычисление функционала среднеквадратичного отклонения
double CDeconvolutionDlg::function(float* lambda)
{
	double sumk = 0;
	for (int i = 0; i < KolToch; i++)
	{
		sumk = 0;
		for (int j = 0; j < KolToch; j++)
		{
			if ((i - j) < 0)
			{
				sumk += lambda[j] * PulseMass[i - j + KolToch];
			}
			else
				sumk += lambda[j] * PulseMass[i - j];
		}
		DeconvSignalMass[i] = exp(-1 - sumk);
	}
	double ret_value = 0;
	double ss = 0;
	for (int i = 0; i < KolToch; i++)
	{
		ss = 0;
		for (int j = 0; j < KolToch; j++)
		{
			if ((j - i) < 0)
			{
				ss += DeconvSignalMass[j] * PulseMass[j - i + KolToch];
			}
			else ss += DeconvSignalMass[j] * PulseMass[j - i];
		}
		ret_value += pow((ConvolutionMass[i] - ss), 2);
	}
	return ret_value;
}



// метод хука-дживса, функционал Шенона
double CDeconvolutionDlg::MHJ(int kk, float* x)
{
	// kk - количество параметров; x - массив параметров
	killtimer = false;
	double TAU1 = Pred;
	double  TAU = 1.e-6f; // Точность вычислений
	int i, j, bs, ps;
	double z, h, k, fi, fb;
	double* b = new double[kk];
	double* y = new double[kk];
	double* p = new double[kk];

	h = 1.;
	x[0] = 1.;
	for (i = 1; i < kk; i++)
	{
		x[i] = (double)rand() / RAND_MAX; // Задается начальное приближение
	}

	k = h;
	for (i = 0; i < kk; i++)
	{
		y[i] = p[i] = b[i] = x[i];
	}
	fi = function(x);
	ps = 0;
	bs = 1;
	fb = fi;

	j = 0;
	while (1)
	{
		//calc++; // Счетчик итераций. Можно игнорировать

		x[j] = y[j] + k;
		z = function(x);
		if (fb < TAU1)
		{
			break;
		}
		if (z >= fi)
		{
			x[j] = y[j] - k;
			z = function(x);
			if (z < fi)
			{
				y[j] = x[j];
			}
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = function(x);

		if (j < kk - 1)
		{
			j++;
			continue;
		}
		if (fi + 1e-8 >= fb)
		{
			if (ps == 1 && bs == 0)
			{
				for (i = 0; i < kk; i++)
				{
					p[i] = y[i] = x[i] = b[i];
				}
				z = function(x);
				bs = 1;
				ps = 0;
				fi = z;
				fb = z;
				j = 0;
				continue;
			}
			k /= 10.;
			if (k < TAU)
			{
				break;
			}
			j = 0;
			continue;
		}

		for (i = 0; i < kk; i++)
		{
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = function(x);
		fb = fi;
		ps = 1;
		bs = 0;
		fi = z;
		FuncZnach = fi;
		j = 0;
	} //  end of while(1)

	for (i = 0; i < kk; i++)
	{
		x[i] = p[i];
	}

	delete b;
	delete y;
	delete p;
	killtimer = true;
	return fb;
}



// обратная задача (деконволюция)
void CDeconvolutionDlg::CalculateDeconvolution()
{
	if (l == true)
		delete[] lambda;
	lambda = new float[KolToch];
	for (int i = 0; i < KolToch; i++)
		lambda[i] = 0;
	DeconvSignalMass = new double[KolToch];
	MHJ(KolToch, lambda);
	l = true;
}



// свёртка после обратной задачи (повтор свёртки)
void CDeconvolutionDlg::CalculateReconvolution()
{
	if (rm == true)
		delete[] ReconvolutionMass;
	ReconvolutionMass = new double[KolToch];
	for (int i = 0; i < KolToch; i++)
	{
		iter = 0;
		for (int j = 0; j < KolToch; j++)
		{
			int m = i - j;
			if (m < 0)
			{
				m += KolToch;
			}
			iter += PulseMass[m] * DeconvSignalMass[j];
		}
		ReconvolutionMass[i] = iter;
	}
	rm = true;
}



// вычисление и добавление шума для входного сигнала
void CDeconvolutionDlg::FuncShym()
{
	double* Shum = new double[KolToch];
	double* n = new double[KolToch];
	double ensig = 0;
	double ensh = 0;
	for (int i = 0; i < KolToch; i++)
	{
		Shum[i] = 0;
		for (int j = 0; j < 12; j++)
		{
			Shum[i] += (double)rand() - RAND_MAX / 2;
		}
		ensig += pow(SignalMass[i], 2);
		ensh += pow(Shum[i], 2);
	}
	double norm = sqrt(ensig / ensh * InShym / 100);
	for (int i = 0; i < KolToch; i++)
	{
		n[i] = Shum[i] * norm;
		SignalMass[i] = SignalMass[i] + n[i];
	}
	delete[] n;
	delete[] Shum;
}



// вычисление и добавление шума для выходного сигнала
void CDeconvolutionDlg::ConvShym()
{
	double* Shum = new double[KolToch];
	double* n = new double[KolToch];
	double ensig = 0;
	double ensh = 0;
	for (int i = 0; i < KolToch; i++)
	{
		Shum[i] = 0;
		for (int j = 0; j < 12; j++)
		{
			Shum[i] += (double)rand() - RAND_MAX / 2;
		}
		ensig += pow(ConvolutionMass[i], 2);
		ensh += pow(Shum[i], 2);
	}
	double norm = sqrt(ensig / ensh * OutShym / 100);
	for (int i = 0; i < KolToch; i++)
	{
		n[i] = Shum[i] * norm;
		ConvolutionMass[i] = ConvolutionMass[i] + n[i];
	}
	delete[] n;
	delete[] Shum;
}



//-----------------------------------Кнопки----------------------------------------------------------



// кнопка сигнал + имп. хар-ка + конволюция
void CDeconvolutionDlg::OnBnClickedDrawsignalandpulse()
{
	CalculateSignalAndPulse();
	DrawSignalAndPulseKoordFlag = false;
	DrawSignalAndPulseFlag = true;
	Draw2Graph(SignalMass, &signalpen, PulseMass, &pulsepen, SignalAndPulseDc, SignalAndPulsePic, (float)KolToch, CString("x"), CString("A"));
	CalculateConvolution();
	DrawConvolutionKoordFlag = false;
	DrawConvolutionFlag = true;
	Draw1Graph(ConvolutionMass, ConvolutionDc, ConvolutionPic, &convpen, KolToch, CString("x"), CString("A"));
}



// кнопка деконволюция
void CDeconvolutionDlg::OnBnClickedDrawdeconvolution()
{
	lambda = new float[KolToch];
	for (int i = 0; i < KolToch; i++)
		lambda[i] = 0;
	DeconvSignalMass = new double[KolToch];
	hThread = CreateThread(NULL, 0, MyProc, this, 0, &dwThread);
	timer = SetTimer(1, 1, NULL);
	DrawDeconvolutionKoordFlag = false;
	DrawDeconvolutionFlag = true;
	DrawConvolutionFlag = false;
	DrawReconvolutionFlag = true;
}



// кнопка стоп
void CDeconvolutionDlg::OnBnClickedDrawdeconvolution2()
{
	killtimer = true;
	SuspendThread(hThread);
	TerminateThread(hThread, 0);
	CloseHandle(hThread);
}



void CDeconvolutionDlg::OnTimer(UINT_PTR nIDEvent)
{
	CalculateReconvolution();
	Invalidate(0);
	if (killtimer)
	{
		KillTimer(timer);
	}
	CDialog::OnTimer(nIDEvent);
}

/*
void CDeconvolutionDlg::OnDestroy()
{
CDialog::OnDestroy();
/*
if(SignalMass)
delete [] SignalMass;
if(PulseMass)
delete [] PulseMass;
if(ConvolutionMass)
delete [] ConvolutionMass;
if(lambda)
delete [] lambda;
if(DeconvSignalMass)
delete [] DeconvSignalMass;
}
*/



