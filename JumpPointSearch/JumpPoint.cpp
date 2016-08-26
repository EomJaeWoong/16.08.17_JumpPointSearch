#include "stdafx.h"
#include "JumpPoint.h"

BYTE MAP[MAPSIZE_Y][MAPSIZE_X];
int iStartX, iStartY, iEndX, iEndY;
BOOL isObsc, bObsticle;
list<NODE *> ltOpenlist, ltCloselist;
NODE *EndNode;
int iDirColor;

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

		if (stSearchNode->iX == iEndX && stSearchNode->iY == iEndY)
		{
			EndNode = stSearchNode;
			break;
			//부모와의 경로 그리고 끝
		}

		ltCloselist.push_back(stSearchNode);
		MAP[stSearchNode->iY][stSearchNode->iX] = DONE;

		if (stSearchNode->pParent == NULL)
		{
			CheckDirection(stSearchNode->iX, stSearchNode->iY - 1, stSearchNode, UU);
			CheckDirection(stSearchNode->iX + 1, stSearchNode->iY - 1, stSearchNode, RU);
			CheckDirection(stSearchNode->iX + 1, stSearchNode->iY, stSearchNode, RR);
			CheckDirection(stSearchNode->iX + 1, stSearchNode->iY + 1, stSearchNode, RD);
			CheckDirection(stSearchNode->iX, stSearchNode->iY + 1, stSearchNode, DD);
			CheckDirection(stSearchNode->iX - 1, stSearchNode->iY + 1, stSearchNode, LD);
			CheckDirection(stSearchNode->iX - 1, stSearchNode->iY, stSearchNode, LL);
			CheckDirection(stSearchNode->iX - 1, stSearchNode->iY - 1, stSearchNode, LU);
		}

		else
		{
			//부모와 자신의 좌표를 통해 방향 구한후 checkDirection
			int iDirX = stSearchNode->iX - stSearchNode->pParent->iX;
			int iDirY = stSearchNode->iY - stSearchNode->pParent->iY;

			if (iDirX == 0 && iDirY < 0)							//UU
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, UU);

				if (MAP[stSearchNode->iY][stSearchNode->iX - 1] == OBSTICLE && 
					MAP[stSearchNode->iY - 1][stSearchNode->iX - 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LU);

				else if (MAP[stSearchNode->iY][stSearchNode->iX + 1] == OBSTICLE && 
					MAP[stSearchNode->iY - 1][stSearchNode->iX + 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RU);
			}

			else if (iDirX > 0 && iDirY < 0)						//RU
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, UU);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RR);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RU);
			}
			else if (iDirX > 0 && iDirY == 0)						//RR
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RR);

				if (MAP[stSearchNode->iY - 1][stSearchNode->iX] == OBSTICLE && 
					MAP[stSearchNode->iY - 1][stSearchNode->iX + 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RU);

				else if (MAP[stSearchNode->iY + 1][stSearchNode->iX] == OBSTICLE &&
				MAP[stSearchNode->iY + 1][stSearchNode->iX + 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RD);
			}

			else if (iDirX > 0 && iDirY > 0)						//RD
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RR);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, DD);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RD);
			}

			else if (iDirX == 0 && iDirY > 0)						//DD
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, DD);

				if (MAP[stSearchNode->iY][stSearchNode->iX - 1] == OBSTICLE &&
					MAP[stSearchNode->iY + 1][stSearchNode->iX - 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LD);

				else if (MAP[stSearchNode->iY][stSearchNode->iX + 1] == OBSTICLE &&
					MAP[stSearchNode->iY + 1][stSearchNode->iX + 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, RD);
			}

			else if (iDirX < 0 && iDirY > 0)						//LD
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LL);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, DD);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LD);
			}

			else if (iDirX < 0 && iDirY == 0)						//LL
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LL);

				if (MAP[stSearchNode->iY - 1][stSearchNode->iX] == OBSTICLE && 
					MAP[stSearchNode->iY - 1][stSearchNode->iX - 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LU);

				else if (MAP[stSearchNode->iY + 1][stSearchNode->iX] == OBSTICLE && 
					MAP[stSearchNode->iY + 1][stSearchNode->iX - 1] == BLANK)
					CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LD);
			}

			else if (iDirX < 0 && iDirY < 0)						//LU
			{
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LL);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, UU);
				CheckDirection(stSearchNode->iX, stSearchNode->iY, stSearchNode, LU);
			}
		}

		MAP[iStartY][iStartX] = START;
		MAP[iEndY][iEndX] = END;

		InvalidateRect(hWnd, NULL, false);
		SendMessage(hWnd, WM_PAINT, 0, 0);
	}
}

/*---------------------------------------------------------------------------------*/
// 방향 체크 함수
/*---------------------------------------------------------------------------------*/
void CheckDirection(int iX, int iY, NODE * pParent, int iDir)
{
	if (Jump(iX, iY, iDir, &iX, &iY))
	{
		ltOpenlist.push_back(CreateNode(iX, iY, pParent->fG + 1, pParent));
		MAP[iY][iX] = CREATE;
	}
}

/*---------------------------------------------------------------------------------*/
// 칸 이동 함수
/*---------------------------------------------------------------------------------*/
BOOL Jump(int iX, int iY, int iDir, int *pX, int *pY)
{
	if (((iX < 0) || (iY < 0) || (iX >= MAPSIZE_X) || (iY >= MAPSIZE_Y)) ||
		MAP[iY][iX] == OBSTICLE)
		return FALSE;

	*pX = iX;
	*pY = iY;
	if (MAP[iY][iX] == BLANK) MAP[iY][iX] = iDir;

	// 도착지점
	if (iX == iEndX && iY == iEndY)
		return TRUE;
	/*
	// o
	// xㅁ
	else if (MAP[iY][iX - 1] == OBSTICLE && MAP[iY - 1][iX - 1] == BLANK)
	{
		if (Jump(iX - 1, iY - 1, LU, &iX, &iY) || Jump(iX, iY - 1, UU, &iX, &iY))
			return TRUE;
	}

	//   o
	// ㅁx
	else if (MAP[iY][iX + 1] == OBSTICLE && MAP[iY - 1][iX + 1] == BLANK)
	{
		if (Jump(iX + 1, iY - 1, RU, &iX, &iY) || Jump(iX, iY - 1, UU, &iX, &iY))
			return TRUE;
	}

	// x o
	// ㅁ
	else if (MAP[iY - 1][iX] == OBSTICLE && MAP[iY - 1][iX + 1] == BLANK)
	{
		if (Jump(iX + 1, iY - 1, RU, &iX, &iY) || Jump(iX + 1, iY, RR, &iX, &iY))
			return TRUE;
	}

	// ㅁ
	// x o
	else if (MAP[iY + 1][iX] == OBSTICLE && MAP[iY + 1][iX + 1] == BLANK)
	{
		if (Jump(iX + 1, iY + 1, RD, &iX, &iY) || Jump(iX + 1, iY, RR, &iX, &iY))
			return TRUE;
	}

	// o x
	//   ㅁ
	else if (MAP[iY - 1][iX] == OBSTICLE && MAP[iY - 1][iX - 1] == BLANK)
	{
		if (Jump(iX - 1, iY - 1, LU, &iX, &iY) || Jump(iX - 1, iY, LL, &iX, &iY))
			return TRUE;
	}

	//   ㅁ
	// o x
	else if (MAP[iY + 1][iX] == OBSTICLE && MAP[iY + 1][iX - 1] == BLANK)
	{
		if (Jump(iX - 1, iY + 1, LD, &iX, &iY) || Jump(iX - 1, iY, LL, &iX, &iY))
			return TRUE;
	}

	// x ㅁ
	// o 
	else if (MAP[iY][iX - 1] == OBSTICLE && MAP[iY + 1][iX - 1] == BLANK)
	{
		if (Jump(iX - 1, iY + 1, LD, &iX, &iY) || Jump(iX, iY + 1, DD, &iX, &iY))
			return TRUE;

	}

	// ㅁ x
	//    o
	else if (MAP[iY][iX + 1] == OBSTICLE && MAP[iY + 1][iX + 1] == BLANK)
	{
		if (Jump(iX + 1, iY + 1, RD, &iX, &iY) || Jump(iX, iY + 1, DD, &iX, &iY))
			return TRUE;
	}
	*/
	switch (iDir)
	{
	case UU:
		if ((MAP[iY][iX - 1] == OBSTICLE && MAP[iY - 1][iX - 1] == BLANK) ||
			(MAP[iY][iX + 1] == OBSTICLE && MAP[iY - 1][iX + 1] == BLANK))
			return TRUE;
		MAP[iY][iX] = UU;
		Jump(iX, iY - 1, iDir, pX, pY);
		break;

	case RU:
		if ((MAP[iY][iX - 1] == OBSTICLE && MAP[iY - 1][iX - 1] == BLANK) ||
			(MAP[iY + 1][iX] == OBSTICLE && MAP[iY + 1][iX + 1] == BLANK))
			return TRUE;

		Jump(iX + 1, iY - 1, iDir, pX, pY);

		if		(Jump(iX, iY - 1, UU, pX, pY))  *pY = iY;
		else if (Jump(iX + 1, iY, RR, pX, pY)) *pX = iX;
			
		break;

	case RR:
		if ((MAP[iY + 1][iX] == OBSTICLE && MAP[iY + 1][iX + 1] == BLANK) ||
			(MAP[iY - 1][iX] == OBSTICLE && MAP[iY - 1][iX + 1] == BLANK))
			return TRUE;

		Jump(iX + 1, iY, iDir, pX, pY);
		break;

	case RD:
		if ((MAP[iY - 1][iX] == OBSTICLE && MAP[iY - 1][iX + 1] == BLANK) ||
			(MAP[iY][iX - 1] == OBSTICLE && MAP[iY + 1][iX - 1] == BLANK))
			return TRUE;

		if (Jump(iX, iY + 1, DD, pX, pY))	*pY = iY;
		else if (Jump(iX + 1, iY, RR, pX, pY))  *pX = iX;

		Jump(iX + 1, iY + 1, iDir, pX, pY);	
		
		break;

	case DD:
		if ((MAP[iY][iX - 1] == OBSTICLE && MAP[iY + 1][iX - 1] == BLANK) ||
			(MAP[iY][iX + 1] == OBSTICLE && MAP[iY + 1][iX + 1] == BLANK))
			return TRUE;
		MAP[iY][iX] = DD;
		Jump(iX, iY + 1, iDir, pX, pY);
		break;

	case LD:
		MAP[iY][iX] = LD;
		if (!Jump(iX - 1, iY + 1, iDir, pX, pY))
		{
			Jump(iX - 1, iY, LL, pX, pY);
			Jump(iX, iY + 1, DD, pX, pY);
		}
		break;

	case LL:
		if ((MAP[iY - 1][iX] == OBSTICLE && MAP[iY - 1][iX - 1] == BLANK)||
			(MAP[iY + 1][iX] == OBSTICLE && MAP[iY + 1][iX - 1] == BLANK))
		MAP[iY][iX] = LL;
		Jump(iX - 1, iY, iDir, pX, pY);
		break;

	case LU:
		MAP[iY][iX] = LU;
		if (!Jump(iX - 1, iY - 1, iDir, pX, pY))
		{
			Jump(iX - 1, iY, LL, pX, pY);
			Jump(iX, iY - 1, UU, pX, pY);
		}
		break;
	}
}

/*---------------------------------------------------------------------------------*/
// Node 생성
/*---------------------------------------------------------------------------------*/
NODE* CreateNode(int iX, int iY, float fG, NODE *pParent)
{
	NODE* stNode = new NODE;
	stNode->iX = iX;
	stNode->iY = iY;
	stNode->pParent = pParent;

	stNode->fG = fG;
	stNode->fH = abs(stNode->iX - iEndX) + abs(stNode->iY - iEndY);
	stNode->fF = stNode->fG + stNode->fH;

	return stNode;
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
	HBRUSH hBrush, hBrushOld;
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
				hBrush = CreateSolidBrush(RGB(150, 150, 150));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
			}

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

			else if (MAP[iCntY][iCntX] == DONE)
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

			switch (MAP[iCntY][iCntX])
			{
			case UU:
				hBrush = CreateSolidBrush(RGB(58, 250, 244));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case RU:
				hBrush = CreateSolidBrush(RGB(245, 169, 225));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case RR:
				hBrush = CreateSolidBrush(RGB(250, 75, 11));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case RD:
				hBrush = CreateSolidBrush(RGB(35, 11, 244));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case DD:
				hBrush = CreateSolidBrush(RGB(254, 11, 214));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case LD:
				hBrush = CreateSolidBrush(RGB(150, 30, 100));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case LL:
				hBrush = CreateSolidBrush(RGB(200, 150, 12));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;

			case LU:
				hBrush = CreateSolidBrush(RGB(255, 130, 100));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iCntX * TILE_SIZE + 1, iCntY * TILE_SIZE + 1,
					(iCntX + 1) * TILE_SIZE + 1, (iCntY + 1) * TILE_SIZE) + 2;

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				DeleteObject(hBrushOld);
				break;
			}
		}
	}
	SelectObject(hdc, hPenOld);

	DeleteObject(hPen);
	DeleteObject(hPenOld);
	DeleteObject(hNullPen);
}

void DrawPath(HDC hdc)
{
	HPEN hPen, hPenOld;
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	if (EndNode != NULL){
		while (EndNode->pParent != NULL)
		{
			MoveToEx(hdc, (EndNode->iX * TILE_SIZE) + (TILE_SIZE / 2),
				(EndNode->iY * TILE_SIZE) + (TILE_SIZE / 2), NULL);
			LineTo(hdc, (EndNode->pParent->iX * TILE_SIZE) + (TILE_SIZE / 2),
				(EndNode->pParent->iY * TILE_SIZE) + (TILE_SIZE / 2));
			EndNode = EndNode->pParent;
		}
	}
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
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