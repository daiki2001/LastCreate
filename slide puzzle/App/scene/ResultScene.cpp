#include "ResultScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include "../../GameInputManager.h"
#include"../slide puzzle/Score.h"
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{}
void ResultScene::Init()
{
	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,1,-1 }, Vec3{ -1.6f, 1.7f, 0 }, Vec3{ 0, 1, 0 });

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();

	manipulate = Sprite::Get()->SpriteCreate(L"Resources/sprite/result.png");

	//オブジェクト生成
	object = Shape::CreateOBJ("sky", true);
	stageObj = Shape::CreateOBJ("titleStage", true);
}

void ResultScene::Update()
{

	if (GameInputManager::Get()->IsDecide() && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
	}

	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	lightGroup->Update();
	sceneChange_->Update();
}

void ResultScene::Draw()
{
	
}

void ResultScene::ShadowDraw()
{
	Object::Draw(object, Vec3(), Vec3(0.5f, 0.5f, 0.5f), Vec3());
	Object::Draw(stageObj, Vec3(-3.0f, 1.0f, 0.2f), Vec3(1.6f, 1.6f, 0.0f), Vec3(15, -240, 0));
	Sprite::Get()->Draw(manipulate, Vec2(), (float)window_width, (float)window_height);
	Score::Get()->ResultSceneDraw();

	sceneChange_->Draw();
}


void ResultScene::Finalize()
{
	Texture::Get()->Delete();
}
