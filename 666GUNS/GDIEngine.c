#include <tchar.h>
#include "GDIEngine.h"
#include "AABB_OBB.h"

// 모듈 내에서 쓸 전역 변수
#define BMPSIZE 64
#define PRINTBMPSIZE 256
#define GREENCOLOR RGB(0, 255, 0)
#define BLACKCOLOR RGB(0, 0, 0)
#define WHITECOLOR RGB(255, 255, 255)
#define FRAMEINTERVAL 50

#define PLAYERIDLECOUNT 6
#define PLAYERDASHCOUNT 10
#define PLAYERMOVECOUNT 12
#define PLAYERDEADCOUNT 7

#define ENEMYIDLECOUNT 1
#define ENEMYMOVECOUNT 10
#define ENEMYDEADCOUNT 7
#define ENEMYATTACKCOUNT 11

#define WEAPONSHEETCOUNT 2
#define WEAPONSHOTGUNCOUNT 4

#define SLIDESPEED 18
#define FADESPEED 10

RECT Rect;

/// 그림을 그릴 대상 DC
HDC MainDC;						// 화면에 그려지는 DC
HDC BufferDC;					// MainDC에 복사할 DC
HDC PlayerBufferDC;				// 플레이어 및 무기를 그릴 DC
HDC EnemyBufferDC;				// 적 및 무기를 그릴 버퍼 DC
HDC enemyDC;					// 적 캐릭터를 그릴 DC

/// BufferDC의 도화지
HBITMAP BufferBit;
HBITMAP PlayerBufferBit;
HBITMAP EnemyBufferBit;

/// 미리 로드해둔 비트맵 파일들
// 메인화면
HBITMAP mainBackgroundBitmap;
HBITMAP mouseHoverBitmap;

// 팝업
HBITMAP optionFrameBitmap;		// 옵션 팝업 프레임
HBITMAP leftButtonBitmap;		// 볼륨 조절 버튼(왼쪽)
HBITMAP rightButtonBitmap;		// 볼륨 조절 버튼(오른쪽)
HBITMAP mainButtonBitmap;		// 메인화면 이동 버튼
HBITMAP continueButtonBitmap;	// 계속하기 버튼
HBITMAP effectSoundBitmap;		// 효과음 
HBITMAP backgroundSoundBitmap;	// 배경음
HBITMAP creaditBitmap;			// 크레딧 팝업

// 인게임 UI 및 배경 관련
HBITMAP backgroundBitmap;		// 배경
HBITMAP ObjectBitmap[16];		// 건물 
HBITMAP OccupationBitmap;		// 점령지
HBITMAP CarBitmap[5];			// 자동차 오브젝트
HBITMAP playerHeartBitmap;		// 플레이어 체력? 생명력?	
HBITMAP playerDashBitmap;		// 플레이어 대쉬
HBITMAP NumberBitmap[11];		// 타이머 및 기타 UI 숫자
HBITMAP weaponSlotBitmap;		// 무기 슬롯
HBITMAP addSlotBitmap;			// 무기 획득시 나오는 슬롯
HBITMAP overHeatBitmap;			// 오버히트 비트맵

// 인게임 총UI
HBITMAP weaponUIBitmap;

// 캐릭터
HBITMAP playerBitmap;
HBITMAP enemyRangeBitmap;
HBITMAP enemyMeleeBitmap;

// 총 관련
HBITMAP weaponBitmap;
HBITMAP bulletBitmap;
HBITMAP mouseBitmap;
HBITMAP mouseLeftFillBitmap;
HBITMAP mouseRightFillBitmap;
HBITMAP explodeBitmap;

// 엔딩
HBITMAP gameOverBitmap;
HBITMAP gameClearBitmap[3];
HBITMAP sixBitmap;
HBITMAP coninueBitmap;
HBITMAP endingCreatditBitmap;
HBITMAP blackBitmap;

// 기타
HBITMAP ArrowBitmap;

/// 미리 지정해둔 브러쉬
HBRUSH blackBrush;
HBRUSH oldBrush;

void GDI_Initialize(HWND hWnd)
{
	GetClientRect(hWnd, &Rect);
	MainDC = GetDC(hWnd);

	blackBrush = CreateSolidBrush(BLACKCOLOR);
	oldBrush = SelectObject(BufferDC, blackBrush);

	// 메인 화면
	mainBackgroundBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Main_BG.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mouseHoverBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Main_BG_C.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 메인화면 팝업
	optionFrameBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Option_Frame.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	leftButtonBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\T_Point_L.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	rightButtonBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\T_Point_R.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mainButtonBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Main_View_W.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	continueButtonBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Carry_On_W.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	effectSoundBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Effect_Sound.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	backgroundSoundBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Background_Sound.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);	
	creaditBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Main\\Credit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 인게임 UI 
	backgroundBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	playerHeartBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\heart_R.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	playerDashBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\Dash.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	weaponSlotBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\Bottom.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	addSlotBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\Bottom_Slot.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	OccupationBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\666_LONG.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	overHeatBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\InGame\\Over_Hit_Stop_2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// 인게임 총 UI
	weaponUIBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\weaponsUI_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 캐릭터
	playerBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Character\\666_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	enemyRangeBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Character\\police_1_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	enemyMeleeBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Character\\police_2_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 무기
	weaponBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\weapons_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bulletBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\Effect_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mouseBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\Aim_main.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mouseLeftFillBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\Aim_left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mouseRightFillBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\Aim_right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	explodeBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Weapon\\Explosion_Sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


	// 엔딩
	gameOverBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Ending\\GAME_OVER.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	sixBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Ending\\Delete_666.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	coninueBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Ending\\Continue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	endingCreatditBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Ending\\Ending_Credit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	blackBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\Ending\\black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// 기타
	ArrowBitmap = (HBITMAP)LoadImage(NULL, L"Resources\\bmp\\RedPoint.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	/// 묶음 비트맵
	// 숫자 UI
	for (int i = 0; i < 11; i++)
	{
		TCHAR number[50] = _T("");
		wsprintf(number, TEXT("Resources\\bmp\\Number\\Number_%d.bmp"), i);

		if (i == 10)
		{
			_tcscpy_s(number, 50, _T("Resources\\bmp\\Number\\Number_.bmp"));
		}

		NumberBitmap[i] = (HBITMAP)LoadImage(NULL, number, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	// 인게임 건물 UI
	for (int i = 0; i < 16; i++)
	{
		TCHAR object[50] = _T("");
		wsprintf(object, TEXT("Resources\\bmp\\Building\\Building%d.bmp"), i + 1);

		ObjectBitmap[i] = (HBITMAP)LoadImage(NULL, object, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	// 인게임 자동차 UI
	for (int i = 0; i < 5; i++)
	{
		TCHAR car[60] = _T("");
		wsprintf(car, TEXT("Resources\\bmp\\Object\\car%d.bmp"), i);

		CarBitmap[i] = (HBITMAP)LoadImage(NULL, car, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	
	// 게임 클리어 
	for (int i = 0; i < 3; i++)
	{
		TCHAR clear[60] = _T("");
		wsprintf(clear, TEXT("Resources\\bmp\\Ending\\666_T_%d.bmp"), i + 1);

		gameClearBitmap[i] = (HBITMAP)LoadImage(NULL, clear, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

}

void GDI_StartDC()
{
	BufferDC = CreateCompatibleDC(MainDC);
	BufferBit = CreateCompatibleBitmap(MainDC, Rect.right, Rect.bottom);
	SelectObject(BufferDC, BufferBit);
	FillRect(BufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));
}

void GDI_EndDC()
{
	DeleteDC(BufferDC);
	DeleteDC(PlayerBufferDC);
	DeleteDC(EnemyBufferDC);
	DeleteDC(enemyDC);

	DeleteObject(BufferBit);
	DeleteObject(PlayerBufferBit);
	DeleteObject(EnemyBufferBit);
}

void GDI_ExtendedView(HWND hWnd)
{
	RECT rt;
	GetWindowRect(hWnd, &rt);
	HDC memDC = CreateCompatibleDC(MainDC);
	BitBlt(memDC, 0, 0, 1920, 1080, BufferDC, 0, 0, SRCCOPY);
	StretchBlt(BufferDC, 0, 0, 2560, 1080, memDC, 0, 0, 1920, 1080, SRCCOPY);
	DeleteDC(memDC);
}

void GDI_PrintBitMap()
{
	//BITMAP bmp;
	//GetObject(backgroundBitmap, sizeof(BITMAP), &bmp);

	//SelectObject(BufferDC, backgroundBitmap);

	BitBlt(MainDC, 0, 0, Rect.right, Rect.bottom, BufferDC, 0, 0, SRCCOPY);
	//StretchBlt(MainDC, 0, 0, bmp.bmWidth, bmp.bmHeight, BufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}

#pragma region Ending
void GDI_SurviveEnding(double deltaTime)
{
	static double speed = 800.0;

	static double rightSix = 704;
	static double leftSix = 704;

	if (IsSceneJustChanged())
	{
		speed = 800;
		rightSix = 704;
		leftSix = 704;
	}


	SelectObject(BufferDC, blackBrush);

	HDC gameClearBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP gameClearBufferBitmap = CreateCompatibleBitmap(MainDC, Rect.right, Rect.bottom);
	SelectObject(gameClearBufferDC, gameClearBufferBitmap);
	FillRect(gameClearBufferBitmap, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC gameClearDC = CreateCompatibleDC(MainDC);

	BITMAP bmp;
	GetObject(gameClearBitmap[1], sizeof(BITMAP), &bmp);

	if (rightSix >= 1256)
	{
		rightSix = 1256;
	}

	if (leftSix <= 152)
	{
		leftSix = 152;
	}	
	
	SelectObject(gameClearDC, gameClearBitmap[2]);
	TransparentBlt(gameClearBufferDC, rightSix, 200, bmp.bmWidth, bmp.bmHeight, gameClearDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(gameClearDC, gameClearBitmap[0]);
	TransparentBlt(gameClearBufferDC, leftSix, 200, bmp.bmWidth, bmp.bmHeight, gameClearDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(gameClearDC, gameClearBitmap[1]);
	TransparentBlt(gameClearBufferDC, 704, 200, bmp.bmWidth, bmp.bmHeight, gameClearDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	BitBlt(BufferDC, 0, 0, 1920, 1080, gameClearBufferDC, 0, 0, SRCCOPY);

	speed = speed - (speed * 1.400 * (deltaTime / 1000.0));

	rightSix = rightSix + (speed * (deltaTime / 1000.0));
	leftSix = leftSix - (speed * (deltaTime / 1000.0));

	DeleteDC(gameClearDC);
	DeleteObject(gameClearBufferBitmap);
	DeleteDC(gameClearBufferDC);
}

void GDI_DeleteSix(double deltaTime)
{
	static double frameDelta = 0;
	static int frame;

	if (IsSceneJustChanged())
	{
		frameDelta = 0;
		frame = 0;
	}

	if (frameDelta < 2600)
	{
		frameDelta += deltaTime;
		return;
	}

	HDC sixDC = CreateCompatibleDC(MainDC);
	SelectObject(sixDC, sixBitmap);

	if (frameDelta >= 2660)
	{
		++frame;
		frameDelta = 2600;
		if (frame / 15 >= 1)
		{
			frame %= 12;
			frame += 3;
		}
	}

	StretchBlt(BufferDC, 704, 830, 512, 128, sixDC, 0, 128 * frame, 512, 128, SRCCOPY);

	frameDelta += deltaTime;

	DeleteDC(sixDC);
}

void GDI_EndingCreadit()
{
	HDC endingCreaditDC = CreateCompatibleDC(MainDC);
	SelectObject(endingCreaditDC, endingCreatditBitmap);

	BitBlt(BufferDC, 0, 0, 1920, 1080, endingCreaditDC, 0, 0, SRCCOPY);
	DeleteDC(endingCreaditDC);
}

void GDI_DeadEnding(double deltaTime)
{
	static double frameDelta;
	static int frame;

	if (IsSceneJustChanged())
	{
		frameDelta = 0;
		frame = 0;
	}

	HDC GameOverDC = CreateCompatibleDC(MainDC);
	SelectObject(GameOverDC, gameOverBitmap);

	if (frameDelta >= 65)
	{
		++frame;
		frameDelta = 0;
		if (frame / 17 >= 1)
		{
			frame %= 13;
			frame += 4;
		}
	}

	StretchBlt(BufferDC, 0, 0, 1920, 1080, GameOverDC, 0, 1080 * frame, 1920, 1080, SRCCOPY);

	frameDelta += deltaTime;

	DeleteDC(GameOverDC);
}

void GDI_MousePress(double deltaTime)
{
	static double frameDelta = 0;
	static int frame;

	if (IsSceneJustChanged())
	{
		frameDelta = 0;
		frame = 0;
	}

	if (frameDelta < 1000)
	{
		frameDelta += deltaTime;
		return;
	}

	HDC mousePressDC = CreateCompatibleDC(MainDC);
	SelectObject(mousePressDC, coninueBitmap);

	if (frameDelta >= 1350)
	{
		++frame;
		frameDelta = 1000;
		frame %= 2;
	}

	StretchBlt(BufferDC, 0, 830, 1920, 120, mousePressDC, 0, 120 * frame, 1920, 120, SRCCOPY);

	frameDelta += deltaTime;

	DeleteDC(mousePressDC);
}

#pragma endregion

#pragma region Main
void GDI_MainBackGround()
{
	HDC mainBackGroundDC = CreateCompatibleDC(MainDC);
	SelectObject(mainBackGroundDC, mainBackgroundBitmap);

	BITMAP bmp;
	GetObject(mainBackgroundBitmap, sizeof(BITMAP), &bmp);

	StretchBlt(BufferDC, 0, 0, 1920, 1080, mainBackGroundDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	//Rectangle(BufferDC, 1105, 640, 1740, 820); //CreditButton
	//Rectangle(BufferDC, 1105, 450, 1740, 630); //StartButton
	//Rectangle(BufferDC, 1105, 835, 1740, 1015); //QuitButton
	//Rectangle(BufferDC, 1820, 10, 1910, 98); //OptionButton

	DeleteDC(mainBackGroundDC);
}

void GDI_Creadit(BOOL isUp)
{
	if (!isUp)
	{
		return;
	}

	HDC CreaditDC = CreateCompatibleDC(MainDC);
	SelectObject(CreaditDC, creaditBitmap);

	BITMAP bmp;
	GetObject(creaditBitmap, sizeof(BITMAP), &bmp);
	TransparentBlt(BufferDC, 80, 200, bmp.bmWidth, bmp.bmHeight, CreaditDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	
	DeleteDC(CreaditDC);
}

void GDI_Popup(float backgroundVolume, float effectVolume, BOOL isUp)
{
	if (!isUp)
	{
		return;
	}

	BITMAP bmp;
	HDC PopUpBufferDC = CreateCompatibleDC(MainDC);
	SelectObject(PopUpBufferDC, optionFrameBitmap);

	HDC SoundDC = CreateCompatibleDC(MainDC);
	GetObject(backgroundSoundBitmap, sizeof(BITMAP), &bmp);

	SelectObject(SoundDC, backgroundSoundBitmap);
	TransparentBlt(PopUpBufferDC, 400, 110, bmp.bmWidth, bmp.bmHeight, SoundDC, 0, 0, bmp.bmWidth, bmp.bmHeight, BLACKCOLOR);

	SelectObject(SoundDC, effectSoundBitmap);
	TransparentBlt(PopUpBufferDC, 400, 320, bmp.bmWidth, bmp.bmHeight, SoundDC, 0, 0, bmp.bmWidth, bmp.bmHeight, BLACKCOLOR);

	HDC DirectionButtonDC = CreateCompatibleDC(MainDC);
	GetObject(leftButtonBitmap, sizeof(BITMAP), &bmp);

	SelectObject(DirectionButtonDC, leftButtonBitmap);
	BitBlt(PopUpBufferDC, 220, 180, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);
	BitBlt(PopUpBufferDC, 220, 380, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);

	SelectObject(DirectionButtonDC, rightButtonBitmap);
	BitBlt(PopUpBufferDC, 635, 180, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);
	BitBlt(PopUpBufferDC, 635, 380, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);

	HDC NumberDC = CreateCompatibleDC(MainDC);
	GetObject(NumberBitmap[0], sizeof(BITMAP), &bmp);

	int bgVolume = backgroundVolume * 10;
	SelectObject(NumberDC, NumberBitmap[0]);
	TransparentBlt(PopUpBufferDC, 453, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(NumberDC, NumberBitmap[bgVolume]);
	TransparentBlt(PopUpBufferDC, 493, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	if (bgVolume - 10 == 0)
	{
		SelectObject(NumberDC, NumberBitmap[1]);
		TransparentBlt(PopUpBufferDC, 453, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

		SelectObject(NumberDC, NumberBitmap[0]);
		TransparentBlt(PopUpBufferDC, 493, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}

	int effVolume = effectVolume * 10;
	SelectObject(NumberDC, NumberBitmap[0]);
	TransparentBlt(PopUpBufferDC, 453, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(NumberDC, NumberBitmap[effVolume]);
	TransparentBlt(PopUpBufferDC, 493, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	if (effVolume - 10 == 0)
	{
		SelectObject(NumberDC, NumberBitmap[1]);
		TransparentBlt(PopUpBufferDC, 453, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

		SelectObject(NumberDC, NumberBitmap[0]);
		TransparentBlt(PopUpBufferDC, 493, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}

	GetObject(optionFrameBitmap, sizeof(BITMAP), &bmp);
	TransparentBlt(BufferDC, 80, 200, bmp.bmWidth, bmp.bmHeight, PopUpBufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	
	SelectObject(BufferDC, blackBrush);
	Rectangle(BufferDC, 180, 750, 950, 860);

	DeleteDC(SoundDC);
	DeleteDC(DirectionButtonDC);
	DeleteDC(NumberDC);
	DeleteDC(PopUpBufferDC);
}
void GDI_MouseHover(int buttonHover)
{
	HDC mouseHoverDC = CreateCompatibleDC(MainDC);
	SelectObject(mouseHoverDC, mouseHoverBitmap);

	switch (buttonHover)
	{
	case 1:
		TransparentBlt(BufferDC, 0, 0, 1920, 1080, mouseHoverDC, 0, 0, 1280, 720, GREENCOLOR);
		break;

	case 2:
		TransparentBlt(BufferDC, 0, 0, 1920, 1080, mouseHoverDC, 0, 720, 1280, 720, GREENCOLOR);
		break;

	case 3:
		TransparentBlt(BufferDC, 0, 0, 1920, 1080, mouseHoverDC, 0, 720 * 2, 1280, 720, GREENCOLOR);
		break;

	case 4:
		TransparentBlt(BufferDC, 0, 0, 1920, 1080, mouseHoverDC, 0, 720 * 3, 1280, 720, GREENCOLOR);
		break;
	default:
		break;
	}

	DeleteDC(mouseHoverDC);
}
#pragma endregion

#pragma region GameUI
void GDI_GameUI(int playerHP, int remainTime, int playerDash, double deltaTime)
{
	SelectObject(BufferDC, blackBrush);

	if (GameManager_GetOverHeatTime(0) + GameManager_GetOverHeatTime(1) > 0)
	{
		GDI_OverHeatUI(deltaTime);
	}
	GDI_PlayerHPUI(playerHP);
	GDI_TimerUI(remainTime);
	GDI_PlayerDashUI(playerDash);
	GDI_WeaponUI(deltaTime);

}

void GDI_PlayerHPUI(int playerHP)
{
	HDC playerHeartDC = CreateCompatibleDC(MainDC);
	SelectObject(playerHeartDC, playerHeartBitmap);

	BITMAP bmp;
	GetObject(playerHeartBitmap, sizeof(BITMAP), &bmp);

	for (int i = 0; i < playerHP; i++)
	{
		TransparentBlt(BufferDC, (110 * i) + 10, 10, 96, 96, playerHeartDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
		//StretchBlt(BufferDC, (110 * i) + 10, 10, 96, 96, playerHeartDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}

	DeleteDC(playerHeartDC);
}

void GDI_TimerUI(int remainTime)
{
	int minute = remainTime / 60;
	int second = remainTime % 60;

	BITMAP bmp;
	GetObject(NumberBitmap[0], sizeof(BITMAP), &bmp);

	HDC TimeBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP TimeBufferBit = CreateCompatibleBitmap(MainDC, Rect.right, Rect.bottom);
	SelectObject(TimeBufferDC, TimeBufferBit);
	FillRect(TimeBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC TimeDC = CreateCompatibleDC(MainDC);
	SelectObject(TimeDC, NumberBitmap[0]);
	TransparentBlt(TimeBufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	//BitBlt(TimeBufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, SRCCOPY);

	SelectObject(TimeDC, NumberBitmap[minute]);
	TransparentBlt(TimeBufferDC, 40, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	//BitBlt(TimeBufferDC, 40, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, SRCCOPY);

	SelectObject(TimeDC, NumberBitmap[10]);
	TransparentBlt(TimeBufferDC, 80, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	//BitBlt(TimeBufferDC, 80, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, SRCCOPY);

	SelectObject(TimeDC, NumberBitmap[second / 10]);
	TransparentBlt(TimeBufferDC, 120, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	//BitBlt(TimeBufferDC, 120, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, SRCCOPY);
	second %= 10;

	SelectObject(TimeDC, NumberBitmap[second]);
	TransparentBlt(TimeBufferDC, 160, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	//BitBlt(TimeBufferDC, 160, 0, bmp.bmWidth, bmp.bmHeight, TimeDC, 0, 0, SRCCOPY);

	TransparentBlt(BufferDC, 760, 10, 400, 96, TimeBufferDC, 0, 0, 200, 50, WHITECOLOR);
	//StretchBlt(BufferDC, 760, 10, 400, 96, TimeBufferDC, 0, 0, 200, 50, SRCCOPY);

	DeleteDC(TimeDC);
	DeleteObject(TimeBufferBit);
	DeleteDC(TimeBufferDC);
}

void GDI_PlayerDashUI(int playerDash)
{
	HDC playerDashDC = CreateCompatibleDC(MainDC);
	SelectObject(playerDashDC, playerDashBitmap);

	BITMAP bmp;
	GetObject(playerDashBitmap, sizeof(BITMAP), &bmp);

	for (int i = 0; i < playerDash; i++)
	{
		TransparentBlt(BufferDC, 485 + (i * 75), 10, 90, 90, playerDashDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}

	DeleteDC(playerDashDC);
}

void GDI_WeaponUI(double deltaTime)
{
	static double slotPosition = 1920;
	static double slotDelta = SLIDESPEED;
	static double SlotAcceleration;
	static double SlotSpeed = 0;

	if (GameManager_GetJustChanged())
	{
		slotPosition = 1920;
		slotDelta = SLIDESPEED;
		SlotSpeed = 0;
	}

	BITMAP bmp;
	GetObject(weaponSlotBitmap, sizeof(BITMAP), &bmp);

	HDC weaponSoltDC = CreateCompatibleDC(MainDC);
	SelectObject(weaponSoltDC, weaponSlotBitmap);

	TransparentBlt(BufferDC, 0, 950, bmp.bmWidth, bmp.bmHeight / 2, weaponSoltDC, 0, 0, bmp.bmWidth, bmp.bmHeight / 2, GREENCOLOR);
	TransparentBlt(BufferDC, 0, 950, bmp.bmWidth, bmp.bmHeight / 2, weaponSoltDC, 0, 120, bmp.bmWidth, bmp.bmHeight / 2, GREENCOLOR);
	//BitBlt(BufferDC, 0, 950, bmp.bmWidth, 950 + bmp.bmHeight, weaponSoltDC, 0, 0, SRCCOPY);

	HDC weaponUIDC = CreateCompatibleDC(MainDC);		// 무기 UI를 그릴 DC
	SelectObject(weaponUIDC, weaponUIBitmap);

	int leftWeapon = GameManager_GetWeaponIndex(0);
	if (leftWeapon != -1)
	{
		StretchBlt(BufferDC, 80, 950+38, 240, 56, weaponUIDC, 0, (2 * leftWeapon) * 56, 240, 56, SRCCOPY);
	}

	int rightWeapon = GameManager_GetWeaponIndex(1);
	if (rightWeapon != -1)
	{
		StretchBlt(BufferDC, 520, 950+38, 240, 56, weaponUIDC, 0, (2 * rightWeapon + 1) * 56, 240, 56, SRCCOPY);
	}

	int nextWeapon = GameManager_GetWeaponIndex(2);
	if (nextWeapon == -1)
	{
		DeleteDC(weaponSoltDC);
		DeleteDC(weaponUIDC);
		return;
	}

	SlotAcceleration = 0.0003 * deltaTime;
	SlotSpeed += SlotAcceleration;
	slotDelta -= SlotSpeed;

	if (slotDelta < 1)
	{
		slotDelta = 1;
	}


	slotPosition = slotPosition - slotDelta;

	if (slotPosition < 1100)
	{
		slotPosition = 1100;
	}

	GetObject(addSlotBitmap, sizeof(BITMAP), &bmp);
	SelectObject(weaponSoltDC, addSlotBitmap);
	TransparentBlt(BufferDC, slotPosition, 965, bmp.bmWidth, bmp.bmHeight, weaponSoltDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	StretchBlt(BufferDC, slotPosition + 80, 965 + 18, 240, 56, weaponUIDC, 0, (2 * nextWeapon) * 56, 240, 56, SRCCOPY);

	DeleteDC(weaponUIDC);
	DeleteDC(weaponSoltDC);
}

void GDI_OverHeatUI(double deltaTime)
{
	static int overHeatBlink = 0;
	int alpha = 100;

	if (overHeatBlink > 20)
	{
		overHeatBlink++;
		overHeatBlink %= 40;
		alpha = 50;
	}
	else
	{
		overHeatBlink++;
		overHeatBlink %= 40;
		alpha = 100;
	}

	HDC overHeatDC = CreateCompatibleDC(MainDC);
	SelectObject(overHeatDC, overHeatBitmap);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;

	AlphaBlend(BufferDC, 0, 0, 1920, 1080, overHeatDC, 0, 0, 1920, 1080, bf);

	DeleteDC(overHeatDC);
}

void GDI_BackGround(int cameraPosX, int cameraPosY)
{
	HDC backgroundDC = CreateCompatibleDC(MainDC);
	SelectObject(backgroundDC, backgroundBitmap);

	//BitBlt(BufferDC, 0, 0, 1920, 1080, backgroundDC, cameraPosX, cameraPosY, SRCCOPY);
	StretchBlt(BufferDC, 0, 0, 1920, 1080, backgroundDC, cameraPosX, cameraPosY, 1920, 1080, SRCCOPY);

	//DeleteObject(backgroundBitmap);
	DeleteDC(backgroundDC);
}

void GDI_OccupiedArea(occupiedArea* area, Camera* mainCamera, Player* player, double deltaTime)
{
	static double frameDelta;
	static int frame;

	if (area == NULL)
	{
		return;
	}

	HDC occupationDC = CreateCompatibleDC(MainDC);
	
	SelectObject(occupationDC, OccupationBitmap);
	BITMAP bmp;
	GetObject(OccupationBitmap, sizeof(BITMAP), &bmp);

	// area->area.right = area->area.left + bmp.bmWidth;
	// area->area.bottom = area->area.top + bmp.bmHeight;

	POINT areaLocal = { area->area.left, area->area.top };
	areaLocal = Camera_WorldToCameraPOINT(mainCamera, &areaLocal);

	if (frameDelta >= 200)
	{
		++frame;
		frameDelta = 0;
		frame %= 4;
	}

	if (CollisionCheck_RtoP(area->area, player->centerPosition))
	{
		StretchBlt(BufferDC, areaLocal.x, areaLocal.y, 450, 300, occupationDC, 0, 300 * frame, 450, 300, SRCCOPY);
	}
	else
	{
		BitBlt(BufferDC, areaLocal.x, areaLocal.y, 450, 300, occupationDC, 0, 900, SRCCOPY);
	}

	frameDelta += deltaTime;
	DeleteDC(occupationDC);
}

void GDI_Building(RECT* buildingRect, Camera* mainCamera)
{
	HDC buildingDC = CreateCompatibleDC(MainDC);

	for (int i = 0; i < 16; ++i)
	{
		SelectObject(buildingDC, ObjectBitmap[i]);
		BITMAP bmp;
		GetObject(ObjectBitmap[i], sizeof(BITMAP), &bmp);

		if (i == 4)
		{
			buildingRect[i].right = buildingRect[i].left - 200 + bmp.bmWidth;
			buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

			POINT buildingWorld = { buildingRect[i].left - 200, buildingRect[i].top };
			POINT buildingLocal = Camera_WorldToCameraPOINT(mainCamera, &buildingWorld);

			TransparentBlt(BufferDC, buildingLocal.x, buildingLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
		}
		else
		{
			buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
			buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

			POINT buildingWorld = { buildingRect[i].left, buildingRect[i].top };
			POINT buildingLocal = Camera_WorldToCameraPOINT(mainCamera, &buildingWorld);

			TransparentBlt(BufferDC, buildingLocal.x, buildingLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
		}

		if (i == 3)
		{
			buildingRect[i].right = 590;
		}
	}

	// 배경에 오브젝트 배치
	for (int i = 22; i < 31; i++)
	{
		SelectObject(buildingDC, CarBitmap[0]);
		BITMAP bmp;
		GetObject(CarBitmap[0], sizeof(BITMAP), &bmp);

		buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
		buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

		POINT carLocal = { buildingRect[i].left, buildingRect[i].top };
		carLocal = Camera_WorldToCameraPOINT(mainCamera, &carLocal);
			
		TransparentBlt(BufferDC, carLocal.x, carLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}
	for (int i = 31; i < 34; i++)
	{
		SelectObject(buildingDC, CarBitmap[1]);
		BITMAP bmp;
		GetObject(CarBitmap[1], sizeof(BITMAP), &bmp);

		buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
		buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

		POINT carLocal = { buildingRect[i].left, buildingRect[i].top };
		carLocal = Camera_WorldToCameraPOINT(mainCamera, &carLocal);

		TransparentBlt(BufferDC, carLocal.x, carLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}
	for (int i = 34; i < 37; i++)
	{
		SelectObject(buildingDC, CarBitmap[2]);
		BITMAP bmp;
		GetObject(CarBitmap[2], sizeof(BITMAP), &bmp);

		buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
		buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

		POINT carLocal = { buildingRect[i].left, buildingRect[i].top };
		carLocal = Camera_WorldToCameraPOINT(mainCamera, &carLocal);

		TransparentBlt(BufferDC, carLocal.x, carLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}
	for (int i = 37; i < 41; i++)
	{
		SelectObject(buildingDC, CarBitmap[3]);
		BITMAP bmp;
		GetObject(CarBitmap[3], sizeof(BITMAP), &bmp);

		buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
		buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

		POINT carLocal = { buildingRect[i].left, buildingRect[i].top };
		carLocal = Camera_WorldToCameraPOINT(mainCamera, &carLocal);

		TransparentBlt(BufferDC, carLocal.x, carLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}
	for (int i = 41; i < 44; i++)
	{
		SelectObject(buildingDC, CarBitmap[4]);
		BITMAP bmp;
		GetObject(CarBitmap[4], sizeof(BITMAP), &bmp);

		buildingRect[i].right = buildingRect[i].left + bmp.bmWidth;
		buildingRect[i].bottom = buildingRect[i].top + bmp.bmHeight;

		POINT carLocal = { buildingRect[i].left, buildingRect[i].top };
		carLocal = Camera_WorldToCameraPOINT(mainCamera, &carLocal);

		TransparentBlt(BufferDC, carLocal.x, carLocal.y, bmp.bmWidth, bmp.bmHeight, buildingDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}

	DeleteDC(buildingDC);
}

void GDI_GamePopupUI(float backgroundVolume, float effectVolume, BOOL isUp)
{
	if (!isUp)
	{
		return;
	}

	BITMAP bmp;
	HDC PopUpBufferDC = CreateCompatibleDC(MainDC);
	SelectObject(PopUpBufferDC, optionFrameBitmap);

	HDC SoundDC = CreateCompatibleDC(MainDC);
	GetObject(backgroundSoundBitmap, sizeof(BITMAP), &bmp);

	SelectObject(SoundDC, backgroundSoundBitmap);
	TransparentBlt(PopUpBufferDC, 400, 110, bmp.bmWidth, bmp.bmHeight, SoundDC, 0, 0, bmp.bmWidth, bmp.bmHeight, BLACKCOLOR);

	SelectObject(SoundDC, effectSoundBitmap);
	TransparentBlt(PopUpBufferDC, 400, 320, bmp.bmWidth, bmp.bmHeight, SoundDC, 0, 0, bmp.bmWidth, bmp.bmHeight, BLACKCOLOR);

	HDC DirectionButtonDC = CreateCompatibleDC(MainDC);
	GetObject(leftButtonBitmap, sizeof(BITMAP), &bmp);

	SelectObject(DirectionButtonDC, leftButtonBitmap);
	BitBlt(PopUpBufferDC, 220, 180, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);
	BitBlt(PopUpBufferDC, 220, 380, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);

	SelectObject(DirectionButtonDC, rightButtonBitmap);
	BitBlt(PopUpBufferDC, 635, 180, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);
	BitBlt(PopUpBufferDC, 635, 380, bmp.bmWidth, bmp.bmHeight, DirectionButtonDC, 0, 0, SRCCOPY);

	HDC NumberDC = CreateCompatibleDC(MainDC);
	GetObject(NumberBitmap[0], sizeof(BITMAP), &bmp);

	int bgVolume = backgroundVolume * 10;
	SelectObject(NumberDC, NumberBitmap[0]);
	TransparentBlt(PopUpBufferDC, 453, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(NumberDC, NumberBitmap[bgVolume]);
	TransparentBlt(PopUpBufferDC, 493, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	if (bgVolume - 10 == 0)
	{
		SelectObject(NumberDC, NumberBitmap[1]);
		TransparentBlt(PopUpBufferDC, 453, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

		SelectObject(NumberDC, NumberBitmap[0]);
		TransparentBlt(PopUpBufferDC, 493, 213, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}


	int effVolume = effectVolume * 10;
	SelectObject(NumberDC, NumberBitmap[0]);
	TransparentBlt(PopUpBufferDC, 453, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	SelectObject(NumberDC, NumberBitmap[effVolume]);
	TransparentBlt(PopUpBufferDC, 493, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	if (effVolume - 10 == 0)
	{
		SelectObject(NumberDC, NumberBitmap[1]);
		TransparentBlt(PopUpBufferDC, 453, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

		SelectObject(NumberDC, NumberBitmap[0]);
		TransparentBlt(PopUpBufferDC, 493, 420, bmp.bmWidth, bmp.bmHeight, NumberDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);
	}

	HDC ButtonDC = CreateCompatibleDC(MainDC);
	GetObject(mainButtonBitmap, sizeof(BITMAP), &bmp);

	SelectObject(ButtonDC, mainButtonBitmap);
	BitBlt(PopUpBufferDC, 80, 550, bmp.bmWidth, bmp.bmHeight, ButtonDC, 0, 0, SRCCOPY);

	SelectObject(ButtonDC, continueButtonBitmap);
	BitBlt(PopUpBufferDC, 500, 550, bmp.bmWidth, bmp.bmHeight, ButtonDC, 0, 0, SRCCOPY);


	GetObject(optionFrameBitmap, sizeof(BITMAP), &bmp);
	TransparentBlt(BufferDC, 450, 170, bmp.bmWidth, bmp.bmHeight, PopUpBufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	//Rectangle(BufferDC, 550, 720, 900, 820); //MainMenuButton
	//Rectangle(BufferDC, 960, 720, 1310, 820); //ContinueButton

	DeleteDC(SoundDC);
	DeleteDC(DirectionButtonDC);
	DeleteDC(NumberDC);
	DeleteDC(ButtonDC);
	DeleteDC(PopUpBufferDC);
}

void GDI_OverHeatExplode(Player* player, Camera* mainCamera, double deltaTime)
{
	HDC explodeDC = CreateCompatibleDC(MainDC);
	SelectObject(explodeDC, explodeBitmap);

	int test = player->selectedWeapon[0]->frame;

	if (player->selectedWeapon[0]->frameDelta >= FRAMEINTERVAL)
	{
		++(player->selectedWeapon[0]->frame);
		player->selectedWeapon[0]->frameDelta = 0;

		if (player->selectedWeapon[0]->frame >= 13)
		{
			player->selectedWeapon[0]->frame = 13;
		}
	}

	test = player->selectedWeapon[0]->frame;

	if (player->selectedWeapon[1]->frameDelta >= FRAMEINTERVAL/2)
	{
		++(player->selectedWeapon[1]->frame);
		player->selectedWeapon[1]->frameDelta = 0;

		if (player->selectedWeapon[1]->frame >= 13)
		{
			player->selectedWeapon[1]->frame = 13;
		}
	}

	POINT playerPOINT;

	playerPOINT.x = (int)player->centerPosition.x;
	playerPOINT.y = (int)player->centerPosition.y;

	POINT playerLocal = Camera_WorldToCameraPOINT(mainCamera, &playerPOINT);


	if (player->selectedWeapon[0]->frame < 13)
	{
		BOOL test = TransparentBlt(BufferDC,
			playerLocal.x - player->radius * 4, playerLocal.y - player->radius * 4,
			1024, 1024,
			explodeDC,
			256 * player->selectedWeapon[0]->frame, 0,
			256, 256,
			GREENCOLOR
		);
	}

	if (player->selectedWeapon[1]->frame < 13)
	{
		BOOL test = TransparentBlt(BufferDC,
			playerLocal.x - player->radius * 4, playerLocal.y - player->radius * 4,
			1024, 1024,
			explodeDC,
			256 * player->selectedWeapon[1]->frame, 0,
			256, 256,
			GREENCOLOR
		);
	}

	player->selectedWeapon[0]->frameDelta += deltaTime;
	player->selectedWeapon[1]->frameDelta += deltaTime;

	DeleteDC(explodeDC);
}

#pragma endregion

#pragma region Player

void GDI_FindArea(Player* player, occupiedArea* area, Camera* mainCamera)
{
	if (area == NULL)
	{
		return;
	}
	if (CollisionCheck_RtoP(area->area, player->centerPosition) == TRUE)
	{
		return;
	}

	HDC findArrowDC = CreateCompatibleDC(MainDC);
	SelectObject(findArrowDC, ArrowBitmap);
	BITMAP bmp;
	GetObject(ArrowBitmap, sizeof(BITMAP), &bmp);

	POINT arrowPosition = { player->centerPosition.x - player->radius / 4, player->centerPosition.y - player->radius * 1.2 };
	POINT arrowLocal = Camera_WorldToCameraPOINT(mainCamera, &arrowPosition);


	Vector2D areaPosition = { (area->area.right + area->area.left) / 2.0, (area->area.bottom + area->area.top) / 2.0 };

	Vector2D up = { 0, -1 };

	int frame = 0;

	double radian = Vector2D_Dot(Vector2D_Normalization(Vector2D_Sub(areaPosition, player->centerPosition)), up);

	double ccw = Vector2D_Cross(Vector2D_Normalization(Vector2D_Sub(areaPosition, player->centerPosition)), up);

	if (ccw > 0)
	{
		if (0.9238795255 <= radian && radian < 1.0)
		{
			frame = 0;
		}
		else if (0.3826836588 <= radian && radian < 0.9238795255)
		{
			frame = 7;
		}
		else if (-0.38268342451 <= radian && radian < 0.3826836588)
		{
			frame = 6;
		}
		else if (-0.92387954326 <= radian && radian < -0.38268342451)
		{
			frame = 5;
		}
		else if (-1.0 < radian && radian < -0.92387954326)
		{
			frame = 4;
		}
	}
	else if (ccw < 0)
	{
		if (0.9238795255 <= radian && radian < 1.0)
		{
			frame = 0;
		}
		else if (0.3826836588 <= radian && radian < 0.9238795255)
		{
			frame = 1;
		}
		else if (-0.38268342451 <= radian && radian < 0.3826836588)
		{
			frame = 2;
		}
		else if (-0.92387954326 <= radian && radian < -0.38268342451)
		{
			frame = 3;
		}
		else if (-1.0 < radian && radian < -0.92387954326)
		{
			frame = 4;
		}
	}
	else
	{
		frame = 4;
	}

	TransparentBlt(BufferDC, arrowLocal.x, arrowLocal.y, 64, 64, findArrowDC, 128 * frame, 0, 128, 128, GREENCOLOR);

	DeleteDC(findArrowDC);
}

void GDI_PlayerObejct(Player* player, Camera* mainCamera, double deltaTime)
{
	static int graceBlink = 0;

	if (graceBlink > 5 && player->gracePeriod > 0)
	{
		graceBlink++;
		graceBlink %= 10;
		return;
	}
	else
	{
		graceBlink++;
		graceBlink %= 10;
	}

	PlayerBufferDC = CreateCompatibleDC(MainDC);
	PlayerBufferBit = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(PlayerBufferDC, PlayerBufferBit);
	FillRect(PlayerBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	GDI_Player(player, deltaTime);
	if (!(player->nowStatus == Anim_DeadLeft || player->nowStatus == Anim_DeadRight))
	{
		GDI_PlayerWeapon(player, deltaTime);
	}

	POINT playerPOINT;
	playerPOINT.x = (int)player->centerPosition.x;
	playerPOINT.y = (int)player->centerPosition.y;

	POINT playerLocal = Camera_WorldToCameraPOINT(mainCamera, &playerPOINT);

	TransparentBlt(BufferDC,
		playerLocal.x - player->radius, playerLocal.y - player->radius,
		PRINTBMPSIZE, PRINTBMPSIZE, PlayerBufferDC, 0, 0,
		PRINTBMPSIZE, PRINTBMPSIZE, WHITECOLOR);
}
 
void GDI_Player(Player* player, double deltaTime)
{
	static int frame;
	static double frameDelta;

	if (player->statusChanged)
	{
		frame = 0;
	}

	HDC playerDC = CreateCompatibleDC(MainDC);
	SelectObject(playerDC, playerBitmap);

	BITMAP bmp;
	GetObject(playerBitmap, sizeof(BITMAP), &bmp);

	int frameInterval = FRAMEINTERVAL;
	if (player->nowStatus == Anim_DeadLeft || player->nowStatus == Anim_DeadRight)
	{
		frameInterval = FRAMEINTERVAL * 2;
	}

	if (frameDelta >= frameInterval)
	{
		++frame;
		frameDelta = 0;
	}

	switch (player->nowStatus)
	{
	case Anim_IdleLeft:
	case Anim_IdleRight:
		frame %= PLAYERIDLECOUNT;
		break;
	case Anim_DashLeft:
	case Anim_DashRight:
		frame %= PLAYERDASHCOUNT;
		break;
	case Anim_MoveLeft:
	case Anim_MoveRight:
		frame %= PLAYERMOVECOUNT;
		break;
	case Anim_DeadLeft:
	case Anim_DeadRight:
		if (frame >= PLAYERDEADCOUNT)
		{
			--frame;
		}
		break;
	default:
		break;
	}

	TransparentBlt(PlayerBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE,
		playerDC, BMPSIZE * frame, BMPSIZE * (player->nowStatus), BMPSIZE, BMPSIZE, GREENCOLOR);

	frameDelta += deltaTime;
	DeleteDC(playerDC);
}

void GDI_PlayerWeapon(Player* player, double deltaTime)
{
	static int frame;
	static double frameDelta;

	HDC sliceBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP sliceBufferBlt = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(sliceBufferDC, sliceBufferBlt);
	FillRect(sliceBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC plgBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP plgBufferBlt = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(plgBufferDC, plgBufferBlt);
	FillRect(plgBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC weaponDC = CreateCompatibleDC(MainDC);
	SelectObject(weaponDC, weaponBitmap);

	BITMAP bmp;
	GetObject(weaponBitmap, sizeof(BITMAP), &bmp);

	if (player->shootingTime > 0)
	{
		if (frameDelta >= FRAMEINTERVAL)
		{
			++frame;
			frameDelta = 0;
		}

		if (player->nowWeapon == Weapon_Shotgun)
		{
			frame %= WEAPONSHOTGUNCOUNT;
		}
		else
		{
			frame %= WEAPONSHEETCOUNT;
		}
		player->shootingTime -= deltaTime;
	}
	else
	{
		frame = 0;
	}

	StretchBlt(sliceBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, weaponDC, BMPSIZE * frame, BMPSIZE * (2 * player->nowWeapon + player->isRight), BMPSIZE, BMPSIZE, SRCCOPY);
	PlgBlt(plgBufferDC, player->plgPOINT, sliceBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, 0, 0, 0);
	TransparentBlt(PlayerBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, plgBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, GREENCOLOR);


	frameDelta += deltaTime;

	DeleteDC(weaponDC);
	DeleteObject(sliceBufferBlt);
	DeleteDC(sliceBufferDC);
	DeleteObject(plgBufferBlt);
	DeleteDC(plgBufferDC);
}
#pragma endregion

#pragma region Enemy
void GDI_EnemyObject(Enemy (*enemyPool)[ENEMYPOOL_MAX], Camera* mainCamera, double deltaTime)
{
	EnemyBufferDC = CreateCompatibleDC(MainDC);
	EnemyBufferBit = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(EnemyBufferDC, EnemyBufferBit);

	enemyDC = CreateCompatibleDC(MainDC);

	for (int enemyType = 0; enemyType < 2; ++enemyType)
	{
		if (enemyType == EnemyType_Range)
		{
			SelectObject(enemyDC, enemyRangeBitmap);
		}
		else if (enemyType == EnemyType_Melee)
		{
			SelectObject(enemyDC, enemyMeleeBitmap);
		}

		for (int index = 0; index < ENEMYPOOL_MAX; ++index)
		{
			if (!enemyPool[enemyType][index].isActive)
			{
				continue;
			}

			FillRect(EnemyBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

			GDI_Enemy(&enemyPool[enemyType][index], deltaTime);
			if (enemyType == EnemyType_Range && !enemyPool[enemyType][index].isDead)
			{
				GDI_EnemyWeapon(&enemyPool[enemyType][index], deltaTime);
			}

			POINT enemyPOINT;

			enemyPOINT.x = (int)enemyPool[enemyType][index].centerPosition.x;
			enemyPOINT.y = (int)enemyPool[enemyType][index].centerPosition.y;

			POINT enemyLocal = Camera_WorldToCameraPOINT(mainCamera, &enemyPOINT);

			TransparentBlt(BufferDC,
				enemyLocal.x - enemyPool[enemyType][index].radius, enemyLocal.y - enemyPool[enemyType][index].radius,
				PRINTBMPSIZE, PRINTBMPSIZE,
				EnemyBufferDC, 0, 0,
				PRINTBMPSIZE, PRINTBMPSIZE, WHITECOLOR);
		}
	}

}

void GDI_Enemy(Enemy* enemy, double deltaTime)
{
	if (enemy->statusChanged)
	{
		enemy->frame = 0;
	}

	//BITMAP bmp;
	//GetObject(enemyRangeBitmap, sizeof(BITMAP), &bmp);

	if (enemy->frameDelta >= FRAMEINTERVAL)
	{
		++(enemy->frame);
		enemy->frameDelta = 0;
	}

	int temp = enemy->nowStatus;
	switch (enemy->nowStatus)
	{

	case EAnim_IdleLeft:
	case EAnim_IdleRight:
		enemy->frame %= ENEMYIDLECOUNT;
		break;
	case EAnim_MoveLeft:
	case EAnim_MoveRight:
		enemy->frame %= ENEMYMOVECOUNT;
		break;
	case EAnim_DeadLeft:
	case EAnim_DeadRight:
		if (enemy->frame >= ENEMYDEADCOUNT)
		{
			--(enemy->frame);
		}
		break;
	case EAnim_AttackLeft:
	case EAnim_AttackRight:
		enemy->frame %= ENEMYATTACKCOUNT;
		break;
	default:
		break;
	}

	if (enemy->nowType == EnemyType_Range)
	{
		TransparentBlt(EnemyBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE,
			enemyDC, BMPSIZE * enemy->frame, BMPSIZE * (enemy->nowStatus), BMPSIZE, BMPSIZE, GREENCOLOR);
	}
	else if (enemy->nowType == EnemyType_Melee)
	{
		TransparentBlt(EnemyBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE,
			enemyDC, (2 * BMPSIZE) * enemy->frame, (2 * BMPSIZE) * (enemy->nowStatus), 2 * BMPSIZE, 2 * BMPSIZE, GREENCOLOR);
	}

	enemy->frameDelta += deltaTime;
}

void GDI_EnemyWeapon(Enemy* enemy, double deltaTime)
{

	HDC sliceBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP sliceBufferBlt = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(sliceBufferDC, sliceBufferBlt);
	FillRect(sliceBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC plgBufferDC = CreateCompatibleDC(MainDC);
	HBITMAP plgBufferBlt = CreateCompatibleBitmap(MainDC, 256, 256);
	SelectObject(plgBufferDC, plgBufferBlt);
	FillRect(plgBufferDC, &Rect, (HBRUSH)GetStockObject(GREENCOLOR));

	HDC weaponDC = CreateCompatibleDC(MainDC);
	SelectObject(weaponDC, weaponBitmap);

	BITMAP bmp;
	GetObject(weaponBitmap, sizeof(BITMAP), &bmp);

	if (enemy->isShooting)
	{
		if (enemy->weaponFrameDelta >= FRAMEINTERVAL)
		{
			++(enemy->weaponFrame);
			enemy->weaponFrameDelta = 0;
		}

		enemy->weaponFrame %= WEAPONSHEETCOUNT;
	}
	else
	{
		enemy->weaponFrame = 0;
	}

	if (enemy->nowType == EnemyType_Range)
	{
		StretchBlt(sliceBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, weaponDC, BMPSIZE * enemy->weaponFrame, BMPSIZE * (2 * enemy->nowWeapon + enemy->isRight), BMPSIZE, BMPSIZE, SRCCOPY);
		PlgBlt(plgBufferDC, enemy->plgPOINT, sliceBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, 0, 0, 0);
		TransparentBlt(EnemyBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, plgBufferDC, 0, 0, PRINTBMPSIZE, PRINTBMPSIZE, GREENCOLOR);
	}

	enemy->weaponFrameDelta += deltaTime;

	DeleteDC(weaponDC);
	DeleteObject(sliceBufferBlt);
	DeleteDC(sliceBufferDC);
	DeleteObject(plgBufferBlt);
	DeleteDC(plgBufferDC);
}
#pragma endregion

void GDI_Bullet(Bullet(*bulletPool)[BULLETPOOL_MAX], Camera* mainCamera)
{
	HDC bulletDC = CreateCompatibleDC(MainDC);
	SelectObject(bulletDC, bulletBitmap);

	BITMAP bmp;
	GetObject(bulletBitmap, sizeof(BITMAP), &bmp);

	Vector2D up;
	up.x = 0;
	up.y = -1;

	for (int bulletType = 0; bulletType < WEAPONTYPECOUNT; bulletType++)
	{
		for (int index = 0; index < BULLETPOOL_MAX; index++)
		{
			if (!bulletPool[bulletType][index].isActive)
			{
				continue;
			}

			POINT bulletPOINT;

			bulletPOINT.x = (int)bulletPool[bulletType][index].centerPosition.x;
			bulletPOINT.y = (int)bulletPool[bulletType][index].centerPosition.y;

			POINT bulletLocal = Camera_WorldToCameraPOINT(mainCamera, &bulletPOINT);

			int frame = 0;

			double radian = Vector2D_Dot(bulletPool[bulletType][index].headVector, up);

			if (Vector2D_Cross(bulletPool[bulletType][index].headVector, up) < 0)
			{
				if (0.98075 < radian && radian <= 1)
				{
					frame = 0;
				}
				else if (0.83145 < radian && radian <= 0.98075)
				{
					frame = 1;
				}
				else if (0.55555 < radian && radian <= 0.83145)
				{
					frame = 2;
				}
				else if (0.1951 <= radian && radian <= 0.55555)
				{
					frame = 3;
				}
				else if (-0.1951 < radian && radian < 0.1951)
				{
					frame = 4;
				}
				else if (-0.55555 <= radian && radian <= -0.1951)
				{
					frame = 5;
				}
				else if (-0.83145 <= radian && radian < -0.55555)
				{
					frame = 6;
				}
				else if (-0.98075 <= radian && radian < -0.83145)
				{
					frame = 7;
				}
				else if (-1 <= radian && radian < -0.98075)
				{
					frame = 8;
				}
			}
			else if (Vector2D_Cross(bulletPool[bulletType][index].headVector, up) > 0)
			{
				if (-1 <= radian && radian < -0.98075)
				{
					frame = 8;
				}
				else if (-0.98075 <= radian && radian < -0.83145)
				{
					frame = 9;
				}
				else if (-0.83145 <= radian && radian < -0.55555)
				{
					frame = 10;
				}
				else if (-0.55555 <= radian && radian <= -0.1951)
				{
					frame = 11;
				}
				else if (-0.1951 < radian && radian < 0.1951)
				{
					frame = 12;
				}
				else if (0.1951 <= radian && radian <= 0.55555)
				{
					frame = 13;
				}
				else if (0.55555 < radian && radian <= 0.83145)
				{
					frame = 14;
				}
				else if (0.83145 < radian && radian <= 0.98075)
				{
					frame = 15;
				}
				else if (0.98075 < radian && radian <= 1)
				{
					frame = 0;
				}
			}
			else
			{
				frame = 0;
			}

			if (bulletType == Weapon_Shotgun || bulletType == Weapon_EnemyRifle)
			{
				TransparentBlt(BufferDC, bulletLocal.x - 64, bulletLocal.y - 64, 128, 128, bulletDC, 0, 128 * bulletType, 128, 128, GREENCOLOR);
			}
			else if (bulletType == Weapon_Railgun)
			{
				TransparentBlt(BufferDC, bulletLocal.x - 64, bulletLocal.y - 64, 128, 128, bulletDC, 128 * 6, 128 * bulletType, 128, 128, GREENCOLOR);
			}
			else
			{
				TransparentBlt(BufferDC, bulletLocal.x - 64, bulletLocal.y - 64, 128, 128, bulletDC, 128 * frame, 128 * bulletType, 128, 128, GREENCOLOR);
			}
		}
	}

	DeleteDC(bulletDC);
}

void GDI_HitMouseCursor(int mousePosX, int mousePosY)
{
	static double* leftHeat = NULL;
	static double* rightHeat = NULL;

	if (leftHeat == NULL)
	{
		leftHeat = GameManager_GetWeaponHeatGuage(0);
	}

	if (rightHeat == NULL)
	{
		rightHeat = GameManager_GetWeaponHeatGuage(1);
	}

	HDC mouseDC = CreateCompatibleDC(MainDC);
	SelectObject(mouseDC, mouseBitmap);

	BITMAP bmp;
	GetObject(mouseBitmap, sizeof(BITMAP), &bmp);

	HDC leftFillDC = CreateCompatibleDC(MainDC);
	SelectObject(leftFillDC, mouseLeftFillBitmap);

	HDC rightFillDC = CreateCompatibleDC(MainDC);
	SelectObject(rightFillDC, mouseRightFillBitmap);

	double leftRatio = *leftHeat / 100;
	double rightRatio = *rightHeat / 100;

	if (leftRatio >= 1)
	{
		leftRatio = 1;
	}

	if (rightRatio >= 1)
	{
		rightRatio = 1;
	}

	TransparentBlt(BufferDC, mousePosX - (bmp.bmWidth / 2), mousePosY - (bmp.bmHeight*leftRatio) + bmp.bmHeight/2, bmp.bmWidth, bmp.bmHeight * leftRatio,
		leftFillDC, 0, bmp.bmHeight - bmp.bmHeight * leftRatio, bmp.bmWidth, bmp.bmHeight * leftRatio, GREENCOLOR);

	TransparentBlt(BufferDC, mousePosX - (bmp.bmWidth / 2), mousePosY - (bmp.bmHeight * rightRatio) + bmp.bmHeight / 2, bmp.bmWidth, bmp.bmHeight * rightRatio,
		rightFillDC, 0, bmp.bmHeight - bmp.bmHeight * rightRatio, bmp.bmWidth, bmp.bmHeight * rightRatio, GREENCOLOR);

	TransparentBlt(BufferDC, mousePosX - (bmp.bmWidth / 2), mousePosY - (bmp.bmHeight / 2), bmp.bmWidth, bmp.bmHeight,
		mouseDC, 0, 0, bmp.bmWidth, bmp.bmHeight, GREENCOLOR);

	DeleteDC(leftFillDC);
	DeleteDC(rightFillDC);
	DeleteDC(mouseDC);
}



//BOOL GDI_FadeIn(HWND hWnd, double deltaTime)
//{
//	GDI_StartDC(hWnd);
//
//	HDC fdaeDC = CreateCompatibleDC(MainDC);
//
//	static int alpha;
//	static int frame;
//	static double frameDelta;
//
//	if (alpha >= 255)
//	{
//		return TRUE;
//	}
//
//	if (frameDelta >= 0)
//	{
//		++frame;
//		frameDelta = 0;
//	}
//
//	BITMAP bmp;
//	GetObject(fadeBitmap, sizeof(BITMAP), &bmp);
//	SelectObject(fdaeDC, fadeBitmap);
//
//	alpha = frame * 100;
//
//	BLENDFUNCTION bf;
//	bf.BlendOp = 0;
//	bf.BlendFlags = 0;
//	bf.SourceConstantAlpha = alpha; // alpha 0 ~ 255
//	bf.AlphaFormat = AC_SRC_OVER;
//	
//	AlphaBlend(BufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight, fdaeDC, 0, 0, bmp.bmWidth, bmp.bmHeight, bf);
//	BitBlt(MainDC, 0, 0, 1920, 1080, BufferDC, 0, 0, SRCCOPY);
//
//	return FALSE;
//}
//
//BOOL GDI_FadeOut(int deltaTime)
//{
//}

RECT GDI_Rect(int left, int top, int right, int bottom)
{
	RECT rec = { left, top, right, bottom };
	HBITMAP RectBit = CreateCompatibleBitmap(MainDC, rec.right, rec.bottom);
	SelectObject(BufferDC, RectBit);

	return rec;
}

HDC GDI_GetDC()
{
	return MainDC;
}
