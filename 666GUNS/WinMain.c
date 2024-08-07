// 666GUNS.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#pragma comment (lib, "Msimg32.lib")

#include "framework.h"
#include "WinMain.h"
#include "GameDefine.h"
#include "Core.h"
#include "InputManager.h"
#include "SoundManager.h"

#define MAX_LOADSTRING 100

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
extern HWND hWnd;                                      // 현재 윈도우 핸들입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void ShowTaskBar(BOOL show);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY666GUNS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	/// 비트맵을 파일에서 로드한다.
	//HDC _bitmapMemDC = LoadBitmapFromFile(L"scienceVessel.bmp");   
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY666GUNS));

    MSG msg;


	RECT fullrect = { 0 };
	//SetRect(&fullrect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

    ShowTaskBar(FALSE);
    ShowCursor(FALSE);

	// 엔진,메인메뉴,게임을 초기화 시킨다.
    Engine_Init();

    // 키보드 입력을 초기화한다.
    KeyManager_Init();
    LeftMouseManager_Init();
    RightMouseManager_Init();

    // 사운드 시스템과 사운드 파일을 생성하여 입력한다.
    SoundSystemInput();
    LoadBGMSound();
    LoadEffectSound();

    // 메인 브금을 재생한다.
    //BGMSound(0);

    // 기본 메시지 루프입니다:
    while (1)
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			DispatchMessage(&msg);
		}
		else
		{
			int nowScene = Engine_GetSceneNumber();
            
            switch (nowScene)
            {
                case Scene_Menu:
                {
					if (IsSceneJustChanged())
					{
						Engine_MenuInit();
					}
					Engine_MenuLoop();
                }
                break;

                case Scene_Game:
                {
					if (IsSceneJustChanged())
					{
						Engine_GameInit();
					}
					// BGM을 종료한다.
					Engine_GameLoop();
                }
                break;

                case Scene_Ending:
                {
					if (IsSceneJustChanged())
					{
						Engine_EndingInit();
					}
					// BGM을 종료한다.
					Engine_EndingLoop();
                }
                break;

				case Scene_Credit:
				{
					if (IsSceneJustChanged())
					{
						Engine_CreditInit();
					}
					// BGM을 종료한다.
					Engine_CreditLoop();
				}
				break;

                default:
                    break;
            }
            
        }
        // 사운드를 재생할 때 마다 갱신시켜 준다.
        //SoundUpdate();
    }
    // 동작이 끝나면 사운드를 해제한다.
    SoundDestroy();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY666GUNS));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; // MAKEINTRESOURCEW(IDC_MY666GUNS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, L"666GUNS", WS_POPUP | WS_VISIBLE,
	   CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
   //hWnd = CreateWindowW(szWindowClass, L"666GUNS", WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

extern HDC mainDC;

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT rt;

    switch (message)
    {
        /*
    case WM_SIZE:
		SetMapMode(mainDC, MM_ANISOTROPIC); //표준화면으로
		SetWindowExtEx(mainDC, 160, 100, NULL); //화면에 맞춰주고 내가 조절해준다.
		GetClientRect(hWnd, &rt);
		SetViewportExtEx(mainDC, rt.right, rt.bottom, NULL);
	    return 0;
        */
        

    case WM_DESTROY:
        PostQuitMessage(0);
        ShowTaskBar(TRUE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


/// 작업표시줄을 가리고 다시 표시하는 함수
/// 폭군강석원
void ShowTaskBar(BOOL show)
{
	HWND taskbar = FindWindow(_T("Shell_TrayWnd"), NULL);
	HWND start = FindWindow(_T("Button"), NULL);

	if (taskbar != NULL) {
		ShowWindow(taskbar, show ? SW_SHOW : SW_HIDE);
		UpdateWindow(taskbar);
	}
	if (start != NULL) {
		// Vista
		ShowWindow(start, show ? SW_SHOW : SW_HIDE);
		UpdateWindow(start);
	}
}
