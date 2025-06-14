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
	
	Matrix GetActiveCameraViewMatrix();
	Matrix GetActiveCameraProjMatrix();

	void Update(float deltaTime);
	void UpdateViewportSize(UINT width, UINT height);

	void PossessCamera(int index);
	void PossessCamera(const CameraPtr& reference);
	void UnpossessCamera();

	const CameraPtr& GetActiveCamera() const;
	const CameraPtr& GetSceneCamera() const;

	CameraManager(UINT width, UINT height);
	~CameraManager() = default;
	CameraManager(CameraManager const&) = delete;
	CameraManager& operator=(CameraManager const&) = delete;

private:
	static std::unique_ptr<CameraManager> sharedInstance;

	CameraPtr m_sceneCamera = nullptr;
	CameraPtr m_activeCamera = nullptr;
	CameraPtr m_previousCamera = nullptr;

	CameraMap m_cameraMap;
	CameraList m_cameraList;

	UINT m_cameraIndex = 0;
};

