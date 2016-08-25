#ifndef __ASTAR__H__
#define __ASTAR__H__

#define MAPSIZE_Y 20
#define MAPSIZE_X 40
#define TILE_SIZE 30

#define UU 0
#define RU 1
#define RR 2
#define RD 3
#define DD 4
#define LD 5
#define LL 6
#define LU 7

#define BLANK 0
#define START 1
#define END 2
#define OBSTICLE 3
#define CREATE 4
#define DONE 5

struct NODE
{
	int iX;
	int iY;

	float fG;
	float fH;
	float fF;

	NODE *pParent;
};

struct lessF
{
	bool operator () (NODE *left, NODE *right) const
	{
		return left->fF < right->fF;
	}
};

/*---------------------------------------------------------------------------------*/
// MAP 초기화
/*---------------------------------------------------------------------------------*/
void		Init();
void		InitMap();

/*---------------------------------------------------------------------------------*/
// A* 길찾기
/*---------------------------------------------------------------------------------*/
void		JumpPointSearch(HWND hWnd);
void		CheckDirection(int iX, int iY, NODE * pParent, int iDir);
BOOL Jump(int iX, int iY, int iDir, int *pX, int *pY);

/*---------------------------------------------------------------------------------*/
// Node 관련
/*---------------------------------------------------------------------------------*/
NODE* 		CreateNode(int iX, int iY, float fG, NODE *pParent);
void		ReleaseList();

/*---------------------------------------------------------------------------------*/
// Draw 관련
/*---------------------------------------------------------------------------------*/
void		DrawMap(HDC hdc);
void		CreateStart(int iX, int iY);
void		CreateEnd(int iX, int iY);
void		CreateObsticle(int iX, int iY);
void		DrawPath(HDC hdc);


void		SetisObsc(BOOL bObsc);

using namespace std;

#endif