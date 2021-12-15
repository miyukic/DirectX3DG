// WindowsProject1.cpp : アプリケーションのエントリ ポイントを定義します。
//
#pragma comment(lib, "winmm.lib")
// ======DirectXライプラリ=====
#pragma comment(lib, "./lib/x86/d3d9.lib")
#pragma comment(lib, "./lib/x86/d3dx9.lib")

#include "framework.h"
#include "WindowsProject1.h"
#include "Windows.h"
#include "timeapi.h"
#include <d3d9.h>
#include <d3dx9.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	HWND hWnd;
	if (!InitInstance(hInstance, nCmdShow, &hWnd)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// フレームレート
	const static DWORD FRAME_RATE = 30;
	const static FLOAT FRAME_TIME = (1 / FRAME_RATE) * 1000;

	DWORD current	= 0;
	DWORD prev		= 0;
	timeBeginPeriod(1);
	prev	= timeGetTime();	// ゲームループ用 過去の時間

	// IDirect3D9の生成
	LPDIRECT3D9 direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d9 == nullptr) { /*作成失敗*/ }

	// IDirect3DDevice9コンポーネントの生成に必要な初期化をします。
	D3DPRESENT_PARAMETERS d3dPram;
	// バックバッファの数 => 一つ
	d3dPram.BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	d3dPram.BackBufferFormat = D3DFMT_UNKNOWN;
	/*
		ウィンドウモード設定 => 定数で切り替え

		true(ウィンドウ)、false(フルスクリーン)
	*/
	d3dPram.Windowed = true;
	/*
		スワップエフェクト => D3DSWAPEFFECT_DISCARD(自動設定)

		スワップエフェクトとは：
			バックバッファとフロントバッファへの切り替え方法
	*/
	d3dPram.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// IDirect3DDevice9コンポーネントを取得します。
	IDirect3DDevice9* pD3ddev9 = nullptr;
	direct3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dPram,
		&pD3ddev9
	);



	//IDirect3D9* pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	MessageBox(NULL, L"Hello DxD9!!", L"Windows Programming", MB_OK);

	// メイン ゲーム ループ
	while (TRUE) {
		current = timeGetTime();

		// Message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	
		if (current - prev >= FRAME_TIME) {
			Sleep(1);
			current = timeGetTime();
		}
		prev = timeGetTime();
	}

	timeEndPeriod(1);

	// メイン メッセージ ループ:
//	while (GetMessage(&msg, nullptr, 0, 0)) {
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {割り込み
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}

//	return (int) msg.wParam;
	return 0;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* hWnd) {
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	*hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!*hWnd) {
		return FALSE;
	}

	ShowWindow(*hWnd, nCmdShow);
	UpdateWindow(*hWnd);

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC を使用する描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR) TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR) TRUE;
		}
		break;
	}
	return (INT_PTR) FALSE;
}
