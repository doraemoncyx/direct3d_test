#include "demoapp.h"
shared_ptr<DemoApp> DemoApp::_inst;
int DemoApp::logicFps = 10;
int DemoApp::renderFps = 30;

DemoApp& DemoApp::inst()
{
	if (!_inst)
	{
		_inst.reset(new DemoApp);
	}
	return *_inst;
}

bool DemoApp::init()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	WNDCLASSEXW tmp{
		sizeof(tmp), CS_CLASSDC, wnd_proc, 0, 0, GetModuleHandleW(nullptr), nullptr, nullptr, nullptr, nullptr,
		windowClsName, nullptr
	};
	m_WndCls = tmp;
	if (!RegisterClassExW(&m_WndCls))
	{
		return false;
	}
	m_MainWnd = CreateWindowExW(0, windowClsName, L"name", WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, nullptr, nullptr,
	                            tmp.hInstance, nullptr);
	ShowWindow(m_MainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_MainWnd);
	return true;
}

void DemoApp::run()
{
	message_loop();
}

DemoApp::DemoApp(): m_WndCls()
{
}

void DemoApp::message_loop()
{
	MSG msg;
	auto pMsg = &msg;
	ZeroMemory(pMsg, sizeof(msg));
	static int64_t cpuFreq = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)& cpuFreq);
	int64_t curCpuCnt;
	int64_t lastLogicCnt = 0, lastRenderCnt = 0;
	const int64_t perLogicCnt = cpuFreq / logicFps;
	const int64_t perRenderCnt = cpuFreq / renderFps;
	QueryPerformanceCounter((LARGE_INTEGER*)& lastLogicCnt);
	lastRenderCnt = lastLogicCnt;
	while (pMsg->message != WM_QUIT)
	{
		if (PeekMessageW(pMsg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(pMsg);
			DispatchMessageW(pMsg);
		}
		else
		{
			bool doingWork = false;;
			QueryPerformanceCounter((LARGE_INTEGER*)& curCpuCnt);
			while (lastLogicCnt < curCpuCnt)
			{
				doingWork = true;
				lastLogicCnt += perLogicCnt;
				logic(lastLogicCnt * 1.0 / cpuFreq);
			}
			while (lastRenderCnt < curCpuCnt)
			{
				doingWork = true;
				lastRenderCnt += perRenderCnt;
			}
			if (!doingWork)
			{
				Sleep(10);
			}
		}
	}
}

void DemoApp::render(double time)
{
}

void DemoApp::logic(double time)
{
}


LRESULT DemoApp::wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}
