Owner: Thomas Bryle Banatin

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

* Due to my limitations, the render system only supports 128 objects with constant buffers.
This is due the program using only 1 centralized constant buffer for all per-object constant buffers
for logic only game objects (no need for constant buffers/ no need to be rendered), a boolean can be added when adding the object to the GameObjectManager
	ex. GameObjectManager::GetInstance()->AddGameObject(object, false);
this would not allocate a constant buffer for the added game object


Recreating Test Cases:

* Comment/Remove unrelated logic in GameWindow::OnCreate/Cube::Cube/Cube::OnUpdate

Test Case 1
	uncomment Load Single Cube in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Rainbow Shader Cube (1 & 3) in Cube::Cube inside Cube.cpp

Test Case 2
	uncomment Load Single Cube in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Single Color (Default = white) (2 & 6) in Cube::Cube inside Cube.cpp
	uncomment Rotate On All Axis Behaviour (2 & 4) in Cube::OnUpdate inside Cube.cpp

Test Case 3
	uncomment Load Single Cube in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Rainbow Shader Cube (1 & 3) in Cube::Cube inside Cube.cpp
	uncomment Move Along X and Y axis + Lerp Scale (3) in Cube::OnUpdate inside Cube.cpp

Test Case 4:
	uncomment Load_4 in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Rainbow Shader Cube (1 & 3) in Cube::Cube inside Cube.cpp
	uncomment Rotate On All Axis Behaviour (2 & 4) in Cube::OnUpdate inside Cube.cpp

Test Case 5:
	uncomment Load Single Cube in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Rainbow Shader Cube (1 & 3) in Cube::Cube inside Cube.cpp
	uncomment Warping Animation (5) behaviour in Cube::OnUpdate inside Cube.cpp

Test Case 6:
	uncomment Load_6 in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Single Color (Default = white) (2 & 6) in Cube::Cube inside Cube.cpp

Test Case 7: (Loaded by Default)
	uncomment Load_7 in GameWindow::OnCreate inside GameWindow.cpp
	uncomment Card Blue and White Colors (7) in Cube::Cube inside Cube.cpp


	
