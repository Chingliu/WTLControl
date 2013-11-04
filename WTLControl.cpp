// WTLControl.cpp : main source file for WTLControl.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"


CAppModule _Module;

CString GetModulePath()
{
	TCHAR s_buf[MAX_PATH];
	s_buf[0] = 0x0;
	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	TCHAR	s_drive[MAX_PATH];
	s_drive[0] = 0x0;
	TCHAR s_dir[MAX_PATH];
	s_dir[0] = 0x0;
	/*errno_t n_err_no =*/ 
	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	//assert(n_err_no == 0);
	_tcscpy_s(s_buf, s_drive);
	_tcscat_s(s_buf, s_dir);
	return CString(s_buf);
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{	
//	CMemLeakCheck memLeakCheck;
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	HMODULE hRichEdit=::LoadLibrary(CRichEditCtrl::GetLibraryName()); 
	CSkinManager::GetInstance()->SetSkinPath(GetModulePath()+_T("Skins\\Default\\"));
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));
			
	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::FreeLibrary(hRichEdit);
	::CoUninitialize();

	return nRet;
}
