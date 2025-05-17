#pragma once
#include "stdafx.h"

class DeviceManager
{
public:
	DeviceManager(ComPtr<IDXGIFactory6> factory);
	~DeviceManager() = default;

	ComPtr<ID3D12Device> GetD3DDevice() const;

private:
	ComPtr<ID3D12Device> m_d3dDevice;
	ComPtr<IDXGIAdapter1> m_dxgiAdapter;

	ComPtr<IDXGIAdapter1> FindHardwareAdapter(ComPtr<IDXGIFactory6> factory);
	void CreateDeviceFromAdapter();
};

