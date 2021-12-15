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
#include <array>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// グラフィック関連
constexpr DWORD FRAME_RATE = 30;				// フレームレート
constexpr DOUBLE FRAME_TIME = (1 / FRAME_RATE) * 1000;
LPDIRECT3D9 direct3d9 = nullptr;
IDirect3DDevice9* pD3ddev9 = nullptr;			// グラフィックデバイス管理インターフェース
D3DPRESENT_PARAMETERS d3dPram;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOLEAN				initD3DP(D3DPRESENT_PARAMETERS*);
BOOLEAN				createDirect3DDevice9(HWND, D3DDEVTYPE, DWORD);

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


	DWORD current	= 0;
	DWORD prev		= 0;
	timeBeginPeriod(1);
	prev	= timeGetTime();	// ゲームループ用 過去の時間

	// IDirect3D9の生成
	::direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d9 == nullptr) { /*作成失敗*/ }

	initD3DP(&::d3dPram);//D3DPRESENT_PARAMETERSを初期化

	if (createDirect3DDevice9(hWnd, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING) == 0) {
		std::array<D3DDEVTYPE, 3> typeArr = { D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_SW };
		std::array<DWORD, 3> d3dcArr = { D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DCREATE_SOFTWARE_VERTEXPROCESSING };
		for (size_t i = 0; i < 3; ++i) {
			if (createDirect3DDevice9(hWnd, typeArr[i], d3dcArr[i]) == 0) break;
		}
	}


	//MessageBox(NULL, L"Hello DxD9!!", L"Windows Programming", MB_OK);

	MSG msg;
	// メイン ループ
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
		} else {
			pD3ddev9->BeginScene();
			// 描画
			pD3ddev9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			pD3ddev9->EndScene();

			pD3ddev9->Present(NULL, NULL, NULL, NULL); // バッグバッファから画面へ転送
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

	::pD3ddev9->Release();
	::direct3d9->Release();

	return 0;

}

// ==================================================================
// IDirect3DDevice9コンポーネントを取得します。
// ==================================================================
BOOLEAN createDirect3DDevice9(HWND hWnd, D3DDEVTYPE DeviceType, DWORD BehaviorFlags) {
	HRESULT hr = direct3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		DeviceType,
		hWnd,
		BehaviorFlags,
		&::d3dPram,
		&::pD3ddev9 // LPDirect3DDevice9
	);
	if (hr != D3D_OK) return FALSE;
	return TRUE;
}
// ==================================================================
// D3DPRESENT_PARAMETER 構造体を初期化します。
// IDirect3DDevice9コンポーネントの生成に必要な初期化をします。
// ==================================================================
BOOLEAN initD3DP(D3DPRESENT_PARAMETERS *d3dPram) {
	if (d3dPram == nullptr) return FALSE;
	// バックバッファの数 => 一つ
	d3dPram->BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	d3dPram->BackBufferFormat = D3DFMT_UNKNOWN;
	/*
		ウィンドウモード設定 => 定数で切り替え

		true(ウィンドウ)、false(フルスクリーン)
	*/
	d3dPram->Windowed = true;
	/*
		スワップエフェクト => D3DSWAPEFFECT_DISCARD(自動設定)

		スワップエフェクトとは：
			バックバッファとフロントバッファへの切り替え方法
	*/
	d3dPram->SwapEffect = D3DSWAPEFFECT_DISCARD;
	return TRUE;
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
