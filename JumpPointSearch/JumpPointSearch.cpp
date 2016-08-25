// JumpPointSearch.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "JumpPoint.h"
#include "JumpPointSearch.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
HWND g_hWnd;
HDC g_hMemDC;
HBITMAP g_hMemBitmap;
HBITMAP g_hMemBitmapold;
RECT rWinRect;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUMPPOINTSEARCH));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_JUMPPOINTSEARCH);
	wcex.lpszClassName = L"JumpPointSearch";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	g_hWnd = CreateWindow(L"JumpPointSearch", L"JumpPointSearch", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//
//  �Լ�: WndProc(g_hWnd, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL isDrag = FALSE;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_CHAR:
		if (GetAsyncKeyState(VK_SPACE) & 0x8000){
			InitMap();
			JumpPointSearch(g_hWnd);
			InvalidateRect(g_hWnd, NULL, false);
			SendMessage(hWnd, WM_PAINT, 0, 0);
		}
		break;

	case WM_LBUTTONDOWN:
		CreateStart(LOWORD(lParam) / TILE_SIZE, HIWORD(lParam) / TILE_SIZE);
		InvalidateRect(g_hWnd, NULL, false);
		break;

	case WM_RBUTTONDBLCLK:
		CreateEnd(LOWORD(lParam) / TILE_SIZE, HIWORD(lParam) / TILE_SIZE);
		InvalidateRect(g_hWnd, NULL, false);
		break;

	case WM_RBUTTONDOWN:
		SetisObsc(TRUE);
		isDrag = TRUE;

		break;

	case WM_RBUTTONUP:
		isDrag = FALSE;
		break;

	case WM_MOUSEMOVE:
		if (isDrag)
		{
			CreateObsticle(LOWORD(lParam) / TILE_SIZE, HIWORD(lParam) / TILE_SIZE);
			InvalidateRect(g_hWnd, NULL, false);
		}
		break;

	case WM_PAINT:
		GetClientRect(g_hWnd, &rWinRect);

		hdc = GetDC(g_hWnd);
		g_hMemBitmap = CreateCompatibleBitmap(hdc, rWinRect.right, rWinRect.bottom);
		g_hMemDC = CreateCompatibleDC(hdc);
		g_hMemBitmapold = (HBITMAP)SelectObject(g_hMemDC, g_hMemBitmap);
		ReleaseDC(g_hWnd, hdc);

		PatBlt(g_hMemDC, 0, 0, rWinRect.right, rWinRect.bottom, WHITENESS);
		DrawMap(g_hMemDC);

		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, rWinRect.right, rWinRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}