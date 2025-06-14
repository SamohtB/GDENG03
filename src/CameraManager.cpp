#include "CameraManager.h"
#include "InputSystem.h"
#include "SceneCamera.h"
#include "Camera.h"
#include "Debug.h"

std::unique_ptr<CameraManager> CameraManager::sharedInstance = nullptr;

CameraManager* CameraManager::GetInstance()
{
	return sharedInstance.get();
}

void CameraManager::Initialize(UINT width, UINT height)
{
	try 
	{
		sharedInstance.reset();
		sharedInstance = std::make_unique<CameraManager>(width, height);
	}
	catch (...)
	{
		Debug::LogError("Camera Manager creation failed!");
	}
}

void CameraManager::Destroy()
{
	sharedInstance.reset();
}

CameraManager::CameraManager(UINT width, UINT height)
{
	this->m_sceneCamera = std::make_shared<SceneCamera>(width, height);
	this->m_sceneCamera->SetPosition(0.0f, 0.0f, -10.0f);
	this->AddCamera(this->m_sceneCamera);
	this->PossessCamera(this->m_sceneCamera);

	std::shared_ptr<InputListener> listenerRef = std::dynamic_pointer_cast<InputListener>(this->m_sceneCamera);
	InputSystem::GetInstance()->AddListener(listenerRef);
}

int CameraManager::AddCamera(const CameraPtr& reference, bool setMain)
{
	this->m_cameraList.push_back(reference);
	this->m_cameraMap[this->m_cameraIndex] = reference;
	int index = static_cast<int>(this->m_cameraIndex);
	this->m_cameraIndex++;

	if (setMain || this->m_activeCamera == nullptr)
	{
		this->PossessCamera(reference);
	}

	return index;
}

void CameraManager::Update(float deltaTime)
{
	Debug::Assert(m_activeCamera != nullptr, "No Active Camera Set!");
	this->m_activeCamera->Update(deltaTime);
}

Matrix CameraManager::GetActiveCameraViewMatrix()
{
	Debug::Assert(m_activeCamera != nullptr, "No Active Camera Set!");
	return this->m_activeCamera->GetViewMatrix();
}

Matrix CameraManager::GetActiveCameraProjMatrix()
{
	Debug::Assert(m_activeCamera != nullptr, "No Active Camera Set!");
	return this->m_activeCamera->GetProjectionMatrix();
}

void CameraManager::UpdateViewportSize(UINT width, UINT height)
{
	for (const auto& camera : this->m_cameraList)
	{
		camera->SetViewportSize(width, height);
	}
}

void CameraManager::PossessCamera(int index)
{
	auto it = m_cameraMap.find(index);
	if (it != m_cameraMap.end())
	{
		m_previousCamera = m_activeCamera;
		m_activeCamera = it->second;
	}
	else
	{
		Debug::LogWarning("CameraManager::SetActiveCamera: Invalid index " + std::to_string(index));
	}
}

void CameraManager::PossessCamera(const CameraPtr& reference)
{
	bool found = false;
	for (const auto& [id, cam] : m_cameraMap)
	{
		if (cam == reference)
		{
			m_previousCamera = m_activeCamera;
			m_activeCamera = reference;
			found = true;
			break;
		}
	}

	if (!found)
	{
		Debug::LogWarning("CameraManager::SetActiveCamera: Provided reference not found in camera map.");
	}
}

void CameraManager::UnpossessCamera()
{
	if (m_sceneCamera)
	{
		m_previousCamera = m_activeCamera;
		m_activeCamera = m_sceneCamera;
	}
	else
	{
		Debug::LogWarning("CameraManager::Unpossess: Scene camera not initialized.");
	}
}

const std::shared_ptr<Camera>& CameraManager::GetActiveCamera() const
{
	return this->m_activeCamera;
}

const std::shared_ptr<Camera>& CameraManager::GetSceneCamera() const
{
	return this->m_sceneCamera;
}


