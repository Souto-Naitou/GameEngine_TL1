#include <Windows.h>
#include <DirectXCommon.h>
#include <Win32Application.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Win32Application* pWin32App = Win32Application::GetInstance();
	DirectXCommon* pDirectXCommon = DirectXCommon::GetInstance();

	pWin32App->Initialize();
	pWin32App->ShowWnd();

	pDirectXCommon->Initialize();

	while (pWin32App->GetMsg() != WM_QUIT)
	{

	}

	pDirectXCommon->Finalize();
	pWin32App->Finalize();

	return 0;
}