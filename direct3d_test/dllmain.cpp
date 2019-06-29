// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "demoapp.h"
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{

	auto &app = DemoApp::inst();
	auto inited = app.init_device();
	if(inited)
	{
		app.run();
	}
}
