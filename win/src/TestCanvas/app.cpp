#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static ATOM MyRegisterClass(HINSTANCE);
static BOOL InitInstance(HINSTANCE, int);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow)
{
    MSG msg;

    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    memset(&wcex, 0, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);

    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc	= WndProc;
    wcex.hInstance		= hInstance;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName	= L"Test";

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"Test", L"TestCanvas", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
        NULL, NULL, hInstance, NULL);

    if (hwnd) {
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
    }

    return !!hwnd;
}

#include <GiGdipCanvas.h>
#include <testcanvas.h>

static void ondraw(HWND hwnd);
static void switchTest(HWND hwnd);
static void regen(HWND hwnd);
static void dyndraw(GiCanvas* canvas);

GiGdipCanvas    _canvas;
int             _tests = 0x400;
POINTS          _pt = { 0, 0 };

// �����ڵ���Ϣ������
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_ERASEBKGND:     // �����������������˸
        break;
    case WM_PAINT:              // �ػ�������
        ondraw(hwnd);
        break;
    case WM_SIZE:               // �ı䴰�ڴ�С
        regen(hwnd);
        break;
    case WM_DESTROY:            // �˳�
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDBLCLK:      // ˫���л�����ͼ��
        switchTest(hwnd);
        break;
    case WM_MOUSEMOVE:          // ����ƶ�
        _pt = MAKEPOINTS(lparam);
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    return 0;
}

void ondraw(HWND hwnd)
{
    DWORD tick = GetTickCount();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);            // ��ȡ�ػ���Ϣ�ľ������

    SetBkColor(hdc, GetSysColor(COLOR_WINDOW)); // ���ñ���ɫ�Ա㴫������

    if (_canvas.beginPaint(hwnd, hdc)) {        // ��ʼ�ڻ����ϻ���
        if (!_canvas.drawCachedBitmap()) {      // ��ʾ�ϴα��������
            _canvas.clearWindow();              // ʹ�ñ���ɫ�����ʾ
            TestCanvas::test(&_canvas, _tests); // ���Ʋ���ͼ��
            if ((_tests & 0x400) == 0)          // not testDynCurves
                _canvas.saveCachedBitmap();     // ������ʾ������
        }
        dyndraw(&_canvas);                      // ���ƶ�̬ͼ��
        _canvas.endPaint();                     // ��������
    }
    EndPaint(hwnd, &ps);
    tick = GetTickCount() - tick;
}

void dyndraw(GiCanvas* canvas)
{
    static float phase = 0;
    phase += 1;
    canvas->setPen(0, 0, 1, phase);
    canvas->setBrush(0x88005500, 0);
    canvas->drawEllipse((float)_pt.x - 50.f, (float)_pt.y - 50.f, 100.f, 100.f, true, false);
}

void regen(HWND hwnd)
{
    _canvas.clearCachedBitmap();            // ��Ҫ���¹�����ʾ
    InvalidateRect(hwnd, NULL, FALSE);      // �����ػ���Ϣ
}

void switchTest(HWND hwnd)
{
    int a[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
        0x80 | 0x40 | 0x02, 0x100, 0x200, 0x400 };
    int n = sizeof(a)/sizeof(a[0]);
    int i = n;

    while (--i > 0 && a[i] != _tests) ;
    _tests = a[(i + 1) % n];
    regen(hwnd);

    GiGdipCanvas canvas;

    if (canvas.beginPaintBuffered(1024, 768)) {
        TestCanvas::test(&canvas, _tests);

        WCHAR filename[MAX_PATH];
        GetModuleFileNameW(NULL, filename, MAX_PATH);
        wcscpy_s(wcsrchr(filename, L'.'), 5, L".png");
        canvas.save(filename);

        canvas.endPaint();
    }
}
