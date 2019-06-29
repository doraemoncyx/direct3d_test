#include "d3ddevice.h"
#include <cstdio>

#pragma comment(lib, "d3d9.lib")

Direct3dDevice::Direct3dDevice()
{
}

bool Direct3dDevice::init(HWND hwnd)
{
	m_Hwnd = hwnd;
	auto p = Direct3DCreate9(D3D_SDK_VERSION);
	m_d3d9.reset(p);
	ZeroMemory(&m_D3dpp, sizeof(m_D3dpp));
	m_D3dpp.Windowed = true;
	m_D3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	IDirect3DDevice9* t1 = nullptr;
	auto ret = m_d3d9->CreateDevice(0, D3DDEVTYPE_HAL, m_Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3dpp,
	                                   &t1);
	if (FAILED(ret))
	{
		printf("create d3d failed\n");
		return false;
	}
	m_device.reset(t1);
	return true;
}

void Direct3dDevice::render()
{
	HRESULT ret;
	ret = m_device->Clear(D3DADAPTER_DEFAULT, nullptr, D3DCLEAR_TARGET ,
	                      D3DCOLOR_XRGB(0x22, 0x44, 0x88),
	                      1.0, 0);
	if(FAILED(ret))
	{
		return;
	}
	ret = m_device->BeginScene();
	ret = m_device->EndScene();
	ret = m_device->Present(nullptr, nullptr, nullptr, nullptr);
}

IDirect3DDevice9* Direct3dDevice::get_device()
{
	return m_device.get();
}
