#include "StageSelect.h"
#include"SceneManager.h"
#include"GameScene.h"
#include "../../GameInputManager.h"
StageSelect::StageSelect()
{}
StageSelect::~StageSelect()
{}

void StageSelect::Init()
{
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	manipulate = Sprite::Get()->SpriteCreate(L"Resources/sprite/manipulate.png");
	back = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");
	//オブジェクト生成
	object = Shape::CreateOBJ("sphere");
	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}


void StageSelect::Update()
{
	//ライト更新
	lightGroup->Update();

	if (GameInputManager::Get()->IsDecide() && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
	}

	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}

	sceneChange_->Update();
}

void StageSelect::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	Vec2 size = { 700,500 };
	float posX = static_cast<float>(window_width) / 2 - size.x / 2;
	float posY = static_cast<float>(window_height) / 2 - size.y / 2;
	Sprite::Get()->Draw(manipulate, Vec2(posX, posY), size.x, size.y);

	Object::Draw(object, Vec3(), Vec3(1.0f, 1.0f, 1.0f), Vec3());

	DebugText::Get()->Print(10, 20, 3, "Select");

	sceneChange_->Draw();
}

void StageSelect::ShadowDraw()
{



}

void StageSelect::Finalize()
{
	Texture::Get()->Delete();
}