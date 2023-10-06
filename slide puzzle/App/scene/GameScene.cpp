#include "GameScene.h"
#include"StageSelect.h"
#include "ResultScene.h"
#include"SceneManager.h"
GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//Audioクラス作成
	audio = std::make_unique<Audio>();
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//音データ読み込み
	
	//カメラ位置をセット
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	//スプライト画像読み込み

	back = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");

	//オブジェクト生成
	player = std::make_unique<Player>();
	player->Init();

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//ライト更新
	lightGroup->Update();
	if (sceneChange_->GetinEndFlag())
	{
		/*if (Input::Get()->KeybordTrigger(DIK_SPACE) && sceneChange_->GetinEndFlag())
		{
			sceneChange_->SceneChangeStart("");
		}
		if (sceneChange_->GetOutEndFlag())
		{
			BaseScene* scene = new ResultScene();
			sceneManager_->SetNextScene(scene);
		}*/
	}

	player->Update();
	sceneChange_->Update();
}

void GameScene::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	player->Draw();

	DebugText::Get()->Print(10, 20, 3, "GameScene");
	sceneChange_->Draw();
}

void GameScene::ShadowDraw()
{

}

void GameScene::Finalize()
{
	Texture::Get()->Delete();
}

