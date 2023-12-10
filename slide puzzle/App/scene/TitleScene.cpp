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
	lightGroup->SetShadowProjection(Vec4(-15.0f, 15.0f, -15.0f, 15.0f), Vec2(-15.0f, 15.0f));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,1,-1 }, Vec3{ -1.6f, 1.5f, 0 }, Vec3{ 0, 1, 0 });

	//スプライト画像読み込み

	//オブジェクト生成
	object = Shape::CreateOBJ("titleStage", true);

	//スプライト生成
	title = Sprite::Get()->SpriteCreate(L"Resources/sprite/title.png");
	press = Sprite::Get()->SpriteCreate(L"Resources/sprite/press.png");

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
	/*Object::Draw(object, Vec3(), Vec3(0.5f, 0.5f, 0.5f), sAngle);

	sceneChange_->Draw();*/
}

void TitleScene::ShadowDraw()
{
	Object::Draw(object, Vec3(), Vec3(0.5f, 0.5f, 0.5f), sAngle);

	Vec2 size = { 650,650 };
	float posX = static_cast<float>(window_width) / 2 - size.x / 2;
	float posY = static_cast<float>(window_height) / 2 - size.y / 2;
	Sprite::Get()->Draw(title, Vec2(posX, posY), size.x, size.y);

	Vec2 preSize = { 450, 250 };
	float prePosX = static_cast<float>(window_width) - preSize.x;
	float prePosY = static_cast<float>(window_height) - preSize.y;
	Sprite::Get()->Draw(press, Vec2(prePosX, prePosY), preSize.x, preSize.y);

	sceneChange_->Draw();
}


void TitleScene::Finalize()
{
	Texture::Get()->Delete();
}

void TitleScene::TitleDirection()
{
	sAngle.y += 0.05f;
}
