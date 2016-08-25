// JumpPointSearch.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "JumpPoint.h"
#include "JumpPointSearch.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
HWND g_hWnd;
HDC g_hMemDC;
HBITMAP g_hMemBitmap;
HBITMAP g_hMemBitmapold;
RECT rWinRect;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
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

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindow(L"JumpPointSearch", L"JumpPointSearch", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//
//  함수: WndProc(g_hWnd, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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