#include "demoapp.h"
#include <vector>
#include "shared_buffer.h"
using namespace std;
shared_ptr<DemoApp> DemoApp::_inst;
int DemoApp::logicFps = 10;
int DemoApp::renderFps = 30;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

SharedBuffer<CUSTOMVERTEX> gVertex;

DemoApp& DemoApp::inst()
{
	if (!_inst)
	{
		_inst.reset(new DemoApp);
	}
	return *_inst;
}

bool DemoApp::init_device()
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
	m_device.init(m_MainWnd);


	return true;
}

bool DemoApp::init_render_data()
{
	gVertex.reset(new vector<CUSTOMVERTEX>(3));
	gVertex->at(0) = CUSTOMVERTEX{ 100,100,1, 1, 0xffff0000 };
	gVertex->at(1) = CUSTOMVERTEX{ 100,200,1, 1, 0xffff0000 };
	gVertex->at(2) = CUSTOMVERTEX{ 200,100,1, 1, 0xffff0000 };
	auto d = m_device.get_device();
	IDirect3DVertexBuffer9* tmp;
	auto size = 3 * sizeof(CUSTOMVERTEX);
	d->CreateVertexBuffer(size, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &tmp, nullptr);
	void* gpuBuf;
	tmp->Lock(0, sizeof(CUSTOMVERTEX) * 3, &gpuBuf, 0);
	memcpy_s(gpuBuf, size, gVertex->data(), size);
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
				render(lastRenderCnt * 1.0 / cpuFreq);
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
	m_device.render();
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
