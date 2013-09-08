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
    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"Test", L"TestView", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
        NULL, NULL, hInstance, NULL);

    if (hwnd) {
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
    }

    return !!hwnd;
}

#include "ViewAdapter.h"
#include <windowsx.h>

static ViewAdapter _adapter;

// �����ڵ���Ϣ������
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	_adapter.setWnd(hwnd);

    switch (message)
    {
    case WM_ERASEBKGND:			// �����������������˸
        break;
    case WM_PAINT:              // �ػ�������
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
			_adapter.onDraw(hdc);
			EndPaint(hwnd, &ps);
		}
        break;
    case WM_SIZE:               // �ı䴰�ڴ�С
		_adapter.onSize(LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_DESTROY:            // �˳�
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDBLCLK:      // ˫���л�����ͼ��
        {
            LPCSTR names[] = { "splines", "line", "select", "triangle" };
            static int index = -1;
            index = (index + 1) % (sizeof(names) / sizeof(names[0]));
            _adapter.setCommand(names[index]);
        }
        break;
    case WM_LBUTTONDOWN:		// ��갴��
		_adapter.onLButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), wparam);
        break;
	case WM_LBUTTONUP:			// ���̧��
		_adapter.onLButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        break;
	case WM_MOUSEMOVE:          // ����ƶ�
		_adapter.onMouseMove(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), wparam);
        break;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    return 0;
}
