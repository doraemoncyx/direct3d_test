#pragma once
#include "demoapp.h"
#include <Windows.h>
#include <memory>
#include "d3ddevice.h"
using namespace std;

static const wchar_t windowClsName[] = L"demoapp";

class DemoApp
{
public:
	static int logicFps;
	static int renderFps;

	static DemoApp& inst();


	bool init();
	void run();


private:
	DemoApp();
	void message_loop();
	inline void render(double time);
	void logic(double time);

	WNDCLASSEXW m_WndCls;
	HWND m_MainWnd{};
	Direct3dDevice m_device;

	static shared_ptr<DemoApp> _inst;
	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
