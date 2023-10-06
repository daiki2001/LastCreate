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
	object = Shape::CreateOBJ("sphere");

	//FBX関連
	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("player");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->SetScale(Vec3(0.005f, 0.005f, 0.005f));
	m_fbx->SetPosition(Vec3(0, 0, 0));

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//ライト更新
	lightGroup->Update();
	if (sceneChange_->GetinEndFlag())
	{
		if (Input::Get()->KeybordTrigger(DIK_SPACE) && sceneChange_->GetinEndFlag())
		{
			sceneChange_->SceneChangeStart("");
		}
		if (sceneChange_->GetOutEndFlag())
		{
			BaseScene* scene = new ResultScene();
			sceneManager_->SetNextScene(scene);
		}
	}
	m_fbx->PlayAnimation(true);
	m_fbx->Update();
	sceneChange_->Update();
}

void GameScene::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	Object::Draw(object, Vec3(), Vec3(1.0f, 1.0f, 1.0f), Vec3());

	DebugText::Get()->Print(10, 20, 3, "GameScene");
	sceneChange_->Draw();
	m_fbx->Draw(true);
}

void GameScene::ShadowDraw()
{

}

void GameScene::Finalize()
{
	Texture::Get()->Delete();
}

