#pragma once
#include "pch.h"
#include "Math.h"

class Camera;

class CameraManager
{
public:
	using CameraPtr = std::shared_ptr<Camera>;
	using CameraList = std::vector<CameraPtr>;
	using CameraMap = std::unordered_map<UINT, CameraPtr>;

	static CameraManager* GetInstance();
	static void Initialize(UINT width, UINT height);
	static void Destroy();

	int AddCamera(const CameraPtr& reference, bool setMain = false);
	void Update(float deltaTime);
	Matrix GetActiveCameraViewMatrix();
	Matrix GetActiveCameraProjMatrix();

	void UpdateViewportSize(UINT width, UINT height);
	void SetActiveCamera(int index);
	void SetActiveCamera(const CameraPtr& reference);

	CameraManager(UINT width, UINT height);
	~CameraManager() = default;
	CameraManager(CameraManager const&) = delete;
	CameraManager& operator=(CameraManager const&) = delete;

private:
	static std::unique_ptr<CameraManager> sharedInstance;

	CameraPtr m_sceneCamera = nullptr;
	CameraPtr m_activeCamera = nullptr;
	CameraMap m_cameraMap;
	CameraList m_cameraList;

	UINT m_cameraIndex = 0;
};

