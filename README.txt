Owner: Thomas Bryle Banatin

Special Thanks To:
Jose Romulo Guzman (GDENG03 Scene Editor)

How To Run Program:
- Dependencies:
  - DirectXShaderCompiler (directx-dxc)
  - d3dx12.h (part of D3D12 or DirectXHeaders)
  - DirectXToolkit (directxtk12)
  *Currently dependencies are managed by nuget packages

Entry Class File: main.cpp

Scene Camera Controls:
  -FlyCam (FPS-Flying camera)
	- Hold Right Click to Activate
	- While Holding Right Click
		- W/S buttons to move forward/backward
		- A/D buttons to strafe camera
		- Q/E buttons to move up/down
		- Move mouse to pan camera
		- LShift to move faster
  -While not holding right click
	- scroll mouse wheel to move forward/backward

Notes:
* Currently, all loading of objects/resources must be done between
	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();
	and
	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();

This is due to vertex buffers, index buffers, and texture buffers are passed to a BatchResourceUploader that uses a different command queue than the one for drawing

GDrive Link to Test Cases and Asset Folder: https://drive.google.com/drive/folders/1qlJDlBZe4FXhACQ4wmM7yOEjfh9H1t2H?usp=drive_link 


	
