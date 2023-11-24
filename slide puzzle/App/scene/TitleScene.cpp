#include"TitleScene.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include "../../GameInputManager.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{}

void TitleScene::Init()
{
	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	lightGroup->SetShadowProjection(Vec4(-40.0f, 40.0f, -40.0f, 40.0f), Vec2(-40.0f, 40.0f));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,1,-1 }, Vec3{ -1.6f, 1, 0 }, Vec3{ 0, 1, 0 });

	//スプライト画像読み込み
	button = Sprite::Get()->SpriteCreate(L"Resources/black.png");

	//オブジェクト生成
	object = Shape::CreateOBJ("sphere", true);
	stageObj = Shape::CreateOBJ("stage");

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}

void TitleScene::Update()
{
	lightGroup->Update();

	if (GameInputManager::Get()->IsDecide() && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
	}

	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new StageSelect();
		sceneManager_->SetNextScene(scene);
	}

	sceneChange_->Update();

	TitleDirection();
}

void TitleScene::Draw()
{

	Object::Draw(stageObj, Vec3(), Vec3(10.0f, 10.0f, 10.0f), sAngle);


	Sprite::Get()->Draw(button,Vec2(1000.0f,500.0f),200.0f,80.0f);

	sceneChange_->Draw();
}

void TitleScene::ShadowDraw()
{
}


void TitleScene::Finalize()
{
	Texture::Get()->Delete();
}

void TitleScene::TitleDirection()
{

	sAngle.y += 0.5f;
}
