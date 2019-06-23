#include "d3ddevice.h"
#include <cstdio>

#pragma comment(lib, "d3d9.lib")

Direct3dDevice::Direct3dDevice()
{
}

bool Direct3dDevice::init(HWND hwnd)
{
	m_Hwnd = hwnd;
	m_IDevice = Direct3DCreate9(D3D_SDK_VERSION);
	ZeroMemory(&m_D3dpp, sizeof(m_D3dpp));
	m_D3dpp.Windowed = true;
	m_D3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	auto ret = m_IDevice->CreateDevice(0, D3DDEVTYPE_HAL, m_Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3dpp, &m_Device);
	if (FAILED(ret))
	{
		printf("create d3d failed\n");
		return false;
	}
	return true;
}

void Direct3dDevice::render()
{
	m_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xff, 0x44, 0x88),
	                1.0, 0);
	m_Device->BeginScene();
	m_Device->EndScene();
	m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}
