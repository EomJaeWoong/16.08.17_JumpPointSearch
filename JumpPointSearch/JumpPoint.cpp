#include "stdafx.h"
#include "JumpPoint.h"

BYTE MAP[MAPSIZE_Y][MAPSIZE_X];
int iStartX, iStartY, iEndX, iEndY;
BOOL isObsc, bObsticle;
list<NODE *> ltOpenlist, ltCloselist;

void Init()
{
	for (int iCntY = 0; iCntY < MAPSIZE_Y; iCntY++)
	{
		for (int iCntX = 0; iCntX < MAPSIZE_X; iCntX++)
			MAP[iCntY][iCntX] = BLANK;
	}

	iStartX = -1;
	iStartY = -1;
	iEndX = -1;
	iEndY = -1;

	isObsc = TRUE;
	bObsticle = TRUE;
}

/*---------------------------------------------------------------------------------*/
// JumpPointSearch
/*---------------------------------------------------------------------------------*/
void JumpPointSearch(HWND hWnd)
{
	NODE* stSearchNode;

	if ((iStartX == -1 && iStartY == -1) || (iEndX == -1 && iEndY == -1))
		return;

	ltOpenlist.push_back(CreateNode(iStartX, iStartY, 0, NULL));
	while (1)
	{
		ltOpenlist.sort(lessF());
		stSearchNode = ltOpenlist.front();
		ltOpenlist.pop_front();

		CheckDirection(stSearchNode->iX, stSearchNode->iY - 1, stSearchNode, UU);
		CheckDirection(stSearchNode->iX + 1, stSearchNode->iY - 1, stSearchNode, RU);
		CheckDirection(stSearchNode->iX + 1, stSearchNode->iY, stSearchNode, RR);
		CheckDirection(stSearchNode->iX + 1, stSearchNode->iY + 1, stSearchNode, RD);
		CheckDirection(stSearchNode->iX, stSearchNode->iY + 1, stSearchNode, DD);
		CheckDirection(stSearchNode->iX - 1, stSearchNode->iY + 1, stSearchNode, LD);
		CheckDirection(stSearchNode->iX - 1, stSearchNode->iY, stSearchNode, LL);
		CheckDirection(stSearchNode->iX - 1, stSearchNode->iY - 1, stSearchNode, LU);

		InvalidateRect(hWnd, NULL, false);
		SendMessage(hWnd, WM_PAINT, 0, 0);
	}
}

/*---------------------------------------------------------------------------------*/
// 방향 체크 함수
/*---------------------------------------------------------------------------------*/
void CheckDirection(int iX, int iY, NODE * pParent, int iDir)
{
	int *iNodeX, *iNodeY;

	Jump(iX, iY, iDir, iNodeX, iNodeY);

}

/*---------------------------------------------------------------------------------*/
// 칸 이동 함수
/*---------------------------------------------------------------------------------*/
BOOL Jump(int iX, int iY, int iDir, int *pX, int *pY)
{
	switch (iDir)
	{
	case UU:

	case RU:

	case RR:
	case RD:
	case DD:
	case LD:
	case LL:
	case LU:
	}
}

/*---------------------------------------------------------------------------------*/
// Draw
/*---------------------------------------------------------------------------------*/
void DrawMap(HDC hdc)
{
	TCHAR text[20];
	list<NODE *>::iterator iter;

	/*---------------------------------------------------------------------------------*/
	// Map 그리기
	/*---------------------------------------------------------------------------------*/
	HPEN hPen, hPenOld, hNullPen;
	hNullPen = (HPEN)GetStockObject(NULL_PEN);
	hPen = CreatePen(PS_SOLID, 0, RGB(150, 150, 150));
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	for (int iCntX = 0; iCntX <= MAPSIZE_X; iCntX++)
	{
		MoveToEx(hdc, iCntX * TILE_SIZE, 0, NULL);
		LineTo(hdc, iCntX * TILE_SIZE, MAPSIZE_Y * TILE_SIZE);
	}

	for (int iCntY = 0; iCntY <= MAPSIZE_Y; iCntY++)
	{
		MoveToEx(hdc, 0, iCntY * TILE_SIZE, NULL);
		LineTo(hdc, MAPSIZE_X * TILE_SIZE, iCntY * TILE_SIZE);
	}

	SelectObject(hdc, hPenOld);
	hPenOld = (HPEN)SelectObject(hdc, hNullPen);

	for (int iCntY = 0; iCntY < MAPSIZE_Y; iCntY++)
	{
		for (int iCntX = 0; iCntX < MAPSIZE_X; iCntX++)
		{
			/*---------------------------------------------------------------------------------*/
			// 시작점 그리기
			/*---------------------------------------------------------------------------------*/
			if (MAP[iCntY][iCntX] == START)
			{
				HBRUSH hBrush, hBrushOld;
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);


				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
			}

			/*---------------------------------------------------------------------------------*/
			// 도착점 그리기
			/*---------------------------------------------------------------------------------*/
			else if (MAP[iCntY][iCntX] == END)
			{
				HBRUSH hBrush, hBrushOld;
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
			}

			/*---------------------------------------------------------------------------------*/
			// 장애물 그리기
			/*---------------------------------------------------------------------------------*/
			else if (MAP[iCntY][iCntX] == OBSTICLE)
			{
				HBRUSH hBrush, hBrushOld;
				hBrush = CreateSolidBrush(RGB(150, 150, 150));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
			}

			/*
			else if (MAP[iCntY][iCntX] == CREATE)
			{
			HBRUSH hBrush, hBrushOld;
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
			(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

			SelectObject(hdc, hBrushOld);
			DeleteObject(hBrush);
			DeleteObject(hBrushOld);
			}

			else if (MAP[iCntY][iCntX] == DONE )
			{
			HBRUSH hBrush, hBrushOld;
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
			(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

			SelectObject(hdc, hBrushOld);
			DeleteObject(hBrush);
			DeleteObject(hBrushOld);
			}
			*/
		}
	}
	SelectObject(hdc, hPenOld);

	DeleteObject(hPen);
	DeleteObject(hPenOld);
	DeleteObject(hNullPen);
}

/*---------------------------------------------------------------------------------*/
// 시작점 Setting
/*---------------------------------------------------------------------------------*/
void CreateStart(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		if (iStartX != -1 && iStartY != -1)
			MAP[iStartY][iStartX] = BLANK;

		iStartX = iX;
		iStartY = iY;

		MAP[iY][iX] = START;
	}
}

/*---------------------------------------------------------------------------------*/
// 도착점 Setting
/*---------------------------------------------------------------------------------*/
void CreateEnd(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		if (iEndX != -1 && iEndY != -1)
			MAP[iEndY][iEndX] = BLANK;

		iEndX = iX;
		iEndY = iY;

		MAP[iY][iX] = END;
	}
}

/*---------------------------------------------------------------------------------*/
// 장애물 Setting
/*---------------------------------------------------------------------------------*/
void CreateObsticle(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		if (MAP[iY][iX] == OBSTICLE && isObsc)
		{
			bObsticle = TRUE;
			isObsc = FALSE;
		}
		else if (MAP[iY][iX] == BLANK  && isObsc)
		{
			bObsticle = FALSE;
			isObsc = FALSE;
		}

		if (bObsticle)	MAP[iY][iX] = BLANK;
		else			MAP[iY][iX] = OBSTICLE;
	}
}

void SetisObsc(BOOL bObsc)	{ isObsc = bObsc; }