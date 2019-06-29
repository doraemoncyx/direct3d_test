
#pragma comment(lib,"D3dx9.lib")
#pragma comment(lib,"Winmm.lib")
#include "demoapp.h"
#include <vector>
#include "shared_buffer.h"
#include <d3dx9.h>
using namespace std;
shared_ptr<DemoApp> DemoApp::_inst;
int DemoApp::logicFps = 60;
int DemoApp::renderFps = 60;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex
	DWORD color; // The vertex color
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

	init_render_data();
	return true;
}

bool DemoApp::init_render_data()
{
	gVertex.reset(new vector<CUSTOMVERTEX>(3));
	gVertex->at(0) = CUSTOMVERTEX{-1, -1, 0, 0xffff0000};
	gVertex->at(1) = CUSTOMVERTEX{1, -1, 0, 0xff00ff00};
	gVertex->at(2) = CUSTOMVERTEX{0, 1, 0, 0xff0000ff};
	auto d = m_device.get_device();
	IDirect3DVertexBuffer9* tmp;
	auto size = 3 * sizeof(CUSTOMVERTEX);
	d->CreateVertexBuffer(size, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &tmp, nullptr);
	g_vb.reset(tmp);
	void* gpuBuf;
	tmp->Lock(0, sizeof(CUSTOMVERTEX) * 3, &gpuBuf, 0);
	memcpy_s(gpuBuf, size, gVertex->data(), size);

	auto device = d;
	device->SetStreamSource(0, g_vb.get(), 0, sizeof(CUSTOMVERTEX));
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, false);
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
	HRESULT ret;
	auto device = m_device.get_device();
	ret = device->Clear(D3DADAPTER_DEFAULT, nullptr, D3DCLEAR_TARGET,
	                    D3DCOLOR_XRGB(0x22, 0x44, 0x88),
	                    1.0, 0);
	if (FAILED(ret))
	{
		return;
	}
	ret = device->BeginScene();
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	ret = device->EndScene();
	ret = device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DemoApp::logic(double time)
{
	auto device = m_device.get_device();
	D3DXMATRIXA16 mat;
	auto itime = timeGetTime() % 1000;
	auto angle = itime * 2.0 * D3DX_PI / 1000;
	D3DXMatrixRotationY(&mat, angle);
	device->SetTransform(D3DTS_WORLD, &mat);
	D3DXVECTOR3 vEye(0, 3, -5);
	D3DXVECTOR3 vLookat(0, 0, 0);
	D3DXVECTOR3 vUp(1, 1, 1);
	D3DXMatrixLookAtLH(&mat, &vEye, &vLookat, &vUp);
	device->SetTransform(D3DTS_VIEW, &mat);
	D3DXMatrixPerspectiveFovLH(&mat, D3DX_PI / 4, 2.0, 1.0, 100);
	device->SetTransform(D3DTS_PROJECTION, &mat);
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
