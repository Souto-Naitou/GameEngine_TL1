#include <Windows.h>
#include "DirectXCommon.h"
#include "WinApp.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* pWinApp = WinApp::GetInstance();
	DirectXCommon* pDirectXCommon = DirectXCommon::GetInstance();

	pWinApp->Initialize();
	pWinApp->ShowWnd();

	pDirectXCommon->Initialize();

	while (pWinApp->GetMsg() != WM_QUIT)
	{

	}

	pDirectXCommon->Finalize();
	pWinApp->Finalize();

	return 0;
}