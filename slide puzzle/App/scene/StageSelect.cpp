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
	Camera::Get()->SetCamera(Vec3{ 0,1,-1 }, Vec3{ -1.6f, 1.7f, 0 }, Vec3{ 0, 1, 0 });


	manipulate = Sprite::Get()->SpriteCreate(L"Resources/sprite/manipulate.png");
	
	//オブジェクト生成
	object = Shape::CreateOBJ("titleStage",true);
	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();

	decideSound = Audio::Get()->SoundLoadWave("Resources/Sound/decide.wav");
}


void StageSelect::Update()
{
	//ライト更新
	lightGroup->Update();

	if (GameInputManager::Get()->IsDecide() && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
		if (soundFlag == false)
		{
			Audio::Get()->SoundSEPlayWave(decideSound);
			soundFlag = true;
		}
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
	
	//Object::Draw(object, Vec3(), Vec3(0.5f, 0.5f, 0.5f), Vec3());
	//Vec2 size = { 700,500 };
	//float posX = static_cast<float>(window_width) / 2 - size.x / 2;9ko
	//float posY = static_cast<float>(window_height) / 2 - size.y / 2;
	//Sprite::Get()->Draw(manipulate, Vec2(posX, posY), size.x, size.y);

	//

	//

	//sceneChange_->Draw();
}

void StageSelect::ShadowDraw()
{

	Object::Draw(object, Vec3(), Vec3(0.5f, 0.5f, 0.5f), Vec3());
	Vec2 size = { 700,500 };
	float posX = static_cast<float>(window_width) / 2 - size.x / 2;
	float posY = static_cast<float>(window_height) / 2 - size.y / 2;
	Sprite::Get()->Draw(manipulate, Vec2(posX, posY), size.x, size.y);





	sceneChange_->Draw();

}

void StageSelect::Finalize()
{
	Texture::Get()->Delete();
}