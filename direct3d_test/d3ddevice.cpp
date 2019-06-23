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

	auto ret = m_IDevice->CreateDevice(0, D3DDEVTYPE_HAL, m_Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3dpp,
	                                   &m_Device);
	if (FAILED(ret))
	{
		printf("create d3d failed\n");
		return false;
	}
	return true;
}

void Direct3dDevice::render()
{
	HRESULT ret;
	ret = m_Device->Clear(D3DADAPTER_DEFAULT, nullptr, D3DCLEAR_TARGET ,
	                      D3DCOLOR_XRGB(0x22, 0x44, 0x88),
	                      1.0, 0);
	if(FAILED(ret))
	{
		return;
	}
	ret = m_Device->BeginScene();
	ret = m_Device->EndScene();
	ret = m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}
