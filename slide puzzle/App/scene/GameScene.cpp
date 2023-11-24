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

	ball = std::make_unique<Ball>();
	ball->Init();


	stage = std::make_unique<Stage>();
	stage->Init();
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

	BallHave();

	player->Update(stage->GetStageSize());
	CameraMove();
	ball->Update(player->GetPosition(), player->GetRotation(), Vec3{ 0.0f, 0.0f, 0.0f }, stage->GetStageSize());
	sceneChange_->Update();
}

void GameScene::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	player->Draw();


	ball->Draw();

	stage->Draw();

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

void GameScene::BallHave()
{
	if (ball->HaveHit(player->GetPosition()) && !ball->GetHaveFlag())
	{
		player->SetBall(ball.get());
	}
}

void GameScene::CameraMove()
{
	//半径は-10
	XMVECTOR v0 = { 0, 0, -10, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(30.0f));
	rotM *= XMMatrixRotationY(XMConvertToRadians(player->GetRotation().y + 90));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR target = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR v3 = target + v;
	Vec3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	Vec3 center = { target.m128_f32[0], target.m128_f32[1], target.m128_f32[2] };
	Vec3 pos = f;

	//カメラ位置をセット
	Camera::Get()->SetCamera(pos, center, Vec3{ 0, 1, 0 });
}

