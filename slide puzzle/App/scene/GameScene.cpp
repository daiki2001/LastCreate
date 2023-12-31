#include "GameScene.h"
#include"StageSelect.h"
#include "ResultScene.h"
#include"SceneManager.h"
#include "../../GameInputManager.h"
#include <LoadJson.h>
#include <random>
#include"../slide puzzle/Score.h"
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

	

	//オブジェクト生成
	player = std::make_unique<Player>();
	player->Init();

	ball = std::make_unique<Ball>();
	ball->Init();


	stage = std::make_unique<Stage>();
	stage->Init();

	targetEase_ = std::make_unique<EaseData>(60);

	LoadSpawnStatus();
	gameTime.Init();
	gameTime.Start();
	Score::Get()->ScoreReset();
	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//ライト更新
	lightGroup->Update();
	if (gameTime.GetChangeFlag())
	{
		sceneChange_->SceneChangeStart("");
	}
	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new ResultScene();
		sceneManager_->SetNextScene(scene);
	}

	BallHave();

	SpawnEnemy();

	TargetAct();
	
	player->Update(stage->GetStageSize());
	CameraMove();
	for (int i = 0; i < enemys.size(); i++){
		enemys[i]->Update();
	}
	ball->Update(
		player->GetPosition(), player->GetRotation(),
	    stage->GetStageSize());
	sceneChange_->Update();

	EnemyDeath();

	gameTime.Update();
}

void GameScene::Draw()
{
	

	player->Draw();


	ball->Draw();

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Draw();
	}

	stage->Draw();
	ball->AfterDraw();
	Score::Get()->GameSceneDraw();
	gameTime.Draw();
	sceneChange_->Draw();
}

void GameScene::ShadowDraw()
{

}

void GameScene::Finalize()
{
	enemys.clear();
	Texture::Get()->Delete();
}

void GameScene::BallHave()
{
	if (ball->GetHaveFlag())
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

void GameScene::NearEnemyCheck() {
	// プレイヤーに一番近い敵を求める
	float farLength = 5000.0f;
	int ensmysNumber = 0;
	for (int i = 0; i < enemys.size(); i++) {
		// プレイヤーと敵との距離
		float length = Vec3(player->GetPosition() - enemys[i]->GetPosition()).length();
		if (farLength > length) {
			farLength = length;
			ensmysNumber = i;
		}
	}

	forcusEnemyNum = ensmysNumber;
}

void GameScene::EnemyDeath()
{
	if (enemys.size() == 0) { return; }

	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys[i]->GetHp() <= 0)
		{
			enemys.erase(enemys.begin() + i);
			targetFlag_ = true;
			Score::Get()->PlasScore();
		}
	}
}

void GameScene::TargetAct()
{
	if (enemys.size() == 0) { return; }

	if (enemys[forcusEnemyNum]->GetForcusChangeFlag()) {
		int oldForcusNum = forcusEnemyNum;
		NearEnemyCheck();
		enemys[oldForcusNum]->SetForcusChangeFlag(false);
	}

	if (ball->GetThrowFlag() && ball->GetHitFlag())
	{
		enemys[forcusEnemyNum]->DamageHit(ball->GetPosition(), player->GetComboCount());
	}
	
	TargetReset(enemys[forcusEnemyNum]->GetPosition(), targetFlag_);

	if (!ball->GetHaveFlag())
	{
		ball->SetTargetPos(enemys[forcusEnemyNum]->GetPosition());
	}
}

void GameScene::TargetReset(Vec3 pos, bool flag)
{
	if (!flag)
	{
		player->TargetLockOn(pos);
		targetEase_->Reset();
		return;
	}

	player->TargetLockOn(Easing::easeInOut(player->GetTargetPos(), pos, targetEase_->GetTimeRate()));

	if (targetEase_->GetEndFlag())
	{
		targetFlag_ = false;
	}

	targetEase_->Update();
}

void GameScene::LoadSpawnStatus()
{
	LevelData* levelData = nullptr;
	std::string filepath = "stage";
	levelData = LoadJson::Load(filepath);
	for (auto& loadData : levelData->objects)
	{
		LoadStatus* load = new LoadStatus();
		load->position = loadData.translation;
		load->rotation = loadData.rotation;
		loadStatus.push_back(load);
	}
}

void GameScene::SpawnEnemy()
{

	//最大数かどうか　クールタイムで測る
	if (enemys.size() < enemyMax && spwnCoolTime <= 0)
	{
		spwnCoolTime = spwnCoolTimeMax;
		//敵の種類によって分かれる
		Enemy* enemy = new BaseEnemy();

		//ランダムで出現位置
		if (loadStatus.size() == 0) { return; }
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand2(0, (int)loadStatus.size() - 1);
		int spawn = rand2(mt);

		if (spawn < 0 || spawn>3)
		{
			int i = 0;
		}
		enemy->Init(loadStatus[spawn]->position, loadStatus[spawn]->rotation);
		enemys.push_back(enemy);
	}

	if (spwnCoolTime > 0)
	{
		spwnCoolTime--;
	}
}

