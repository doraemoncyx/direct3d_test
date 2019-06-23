#pragma once
#include <d3d9.h>

class Direct3dDevice
{
public:
	Direct3dDevice();
	bool init(HWND hwnd);
	void render();

private:
	IDirect3D9 *m_IDevice;
	D3DPRESENT_PARAMETERS m_D3dpp;
	IDirect3DDevice9* m_Device;
	HWND m_Hwnd;
};