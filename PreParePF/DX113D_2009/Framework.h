#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define WIN_START_X 100
#define WIN_START_Y 100

#define MAX_BONE 256
#define MAX_FRAME_KEY 600
#define MAX_INSTANCE 300

#define MAX_LIGHT 10

#define LERP(s, e, t) { s + (e - s) * t }

#ifdef NDEBUG
	#define V(hr) hr
#else
	#define V(hr) assert(SUCCEEDED(hr))
#endif

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()
#define FONT DirectWrite::Get()
#define KEY_DOWN(k) Control::Get()->Down(k)
#define KEY_UP(k) Control::Get()->Up(k)
#define KEY_PRESS(k) Control::Get()->Press(k)

#define MOUSEPOS Control::Get()->GetMousePos()

#define CAMERA Environment::Get()->GetMainCamera()
#define GMANAGER GameManager::Get()
#define ITEMMANAGER ItemManager::Get()
#define SCENE SceneManager::Get()

#define AUDIO Audio::Get()

#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//FMOD
#include <../Libraries/inc/fmod.hpp>
#pragma comment(lib, "../Libraries/lib/x64/fmod_vc.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//DirectTex
#include "../DirectXTex/DirectXTex.h"

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

//Dialog
#include <dirent.h>
#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>

//Assimp PATH=$(SolutionDir)\Libraries\DLL;%PATH%
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp/assimp-vc142-mtd.lib")

using namespace DirectX;
using namespace std;
using namespace DirectX::TriangleTests;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> CallBack;
typedef function<void(int)> CallBackParam;

const XMVECTORF32 kRight = { 1, 0, 0 };
const XMVECTORF32 kUp = { 0, 1, 0 };
const XMVECTORF32 kForward = { 0, 0, 1 };

//Framework Header
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"
#include "Framework/Utility/Control.h"
#include "Framework/Utility/DirectWrite.h"
#include "Framework/Utility/Timer.h"
#include "Framework/Utility/Xml.h"
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Audio.h"

using namespace Utility;

#include "Framework/System/Device.h"
#include "Framework/System/DepthStencil.h"
#include "Framework/System/RenderTarget.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/HullShader.h"
#include "Framework/Shader/DomainShader.h"
#include "Framework/Shader/GeometryShader.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/StructuedBuffer.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/CharacterMovement.h"

using namespace GameMath;

#include "Framework/State/RaterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/ModelExport/ModelType.h"
#include "Framework/ModelExport/ModelExporter.h"

#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelReader.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/Models.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelAnimators.h"

#include "Framework/Collider/Collider.h"
#include "Framework/Collider/BoxCollider.h"
#include "Framework/Collider/SphereCollider.h"
#include "Framework/Collider/CapsuleCollider.h"

#include "Environment/Camera.h"
#include "Environment/Environment.h"
#include "Environment/Frustum.h"

#include "Algorithm/Node.h"
#include "Algorithm/Heap.h"
#include "Algorithm/AStar.h"
//Object Header
#include "Objects/GameManager/GameManager.h"

#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Sprite.h"
#include "Objects/Basic/DragBox.h"

#include "Objects/PostEffect/Outline.h"


#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/Sky.h"
#include "Objects/Landscape/SkyBox.h"
#include "Objects/Landscape/TerrainData.h"
#include "Objects/Landscape/QuadTreeTerrain.h"
#include "Objects/Landscape/TerrainLOD.h"
#include "Objects/Landscape/Billboard.h"
#include "Objects/Landscape/Hill.h"
#include "Objects/Landscape/Grass.h"	

#include "Objects/Item/Item.h"
#include "Objects/Item/Meat.h"
#include "Objects/Item/Weapon.h"
#include "Objects/Item/Lightning.h"
#include "Objects/Item/HpPotion.h"
#include "Objects/Item/ChickenItem.h"
#include "Objects/Item/ItemManager.h"

#include "Objects/Bag/Texture2D.h"
#include "Objects/Bag/BagImage.h"
#include "Objects/Bag/Inventory.h"
#include "Objects/Bag/MeatImage.h"
#include "Objects/Bag/ChickenImage.h"
#include "Objects/Bag/HpPotionImage.h"
#include "Objects/Bag/ItemImage.h"


#include "Objects/SelectSceneImage/BackGround.h"

#include "Objects/UI/EzrealUI.h"
#include "Objects/UI/JinUI.h"
#include "Objects/UI/FoxUI.h"
#include "Objects/UI/HpBar.h"
#include "Objects/UI/ExpBar.h"
#include "Objects/UI/BossHpBar.h"
#include "Objects/UI/BossUI.h"
#include "Objects/UI/StartButton.h"
#include "Objects/UI/StartSceneBgUI.h"

#include "Objects/SkillUI/Ez_E.h"
#include "Objects/SkillUI/Jin_E.h"
#include "Objects/SkillUI/SkillImage.h"

#include "Objects/Obstacle/Obstacle.h"
#include "Objects/Obstacle/Wall.h"
#include "Objects/Obstacle/WallManager.h"
#include "Objects/Obstacle/Eve.h"
#include "Objects/Obstacle/Statue.h"
#include "Objects/Obstacle/Egg.h"
#include "Objects/Obstacle/aurora.h"
#include "Objects/Obstacle/ObstacleManager.h"

#include "Objects/Character/Players.h"
#include "Objects/Character/Jin.h"
#include "Objects/Character/Ezreal.h"
#include "Objects/Character/Cyborg.h"
#include "Objects/Character/Fox.h"
#include "Objects/Character/OppentCharacter/Monster.h"
#include "Objects/Character/OppentCharacter/MonsterManager.h"
#include "Objects/Character/OppentCharacter/Chicken.h"
#include "Objects/Character/OppentCharacter/FemailEnemy.h"
#include "Objects/Character/OppentCharacter/Boss.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/SpriteEffect.h"
#include "Objects/Particle/ParticleManager.h"




//Program Header
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Program/Program.h"

extern HWND hWnd;