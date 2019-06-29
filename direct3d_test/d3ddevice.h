#pragma once
#include <d3d9.h>
#include "comptr.h"

class Direct3dDevice
{
public:
	Direct3dDevice();
	bool init(HWND hwnd);
	void render();
	IDirect3DDevice9* get_device();
private:
	ComPtr<IDirect3D9> m_d3d9;
	ComPtr<IDirect3DDevice9> m_device;
	// IDirect3D9 *m_IDevice;
	D3DPRESENT_PARAMETERS m_D3dpp;
	// IDirect3DDevice9* m_Device;
	HWND m_Hwnd;
};