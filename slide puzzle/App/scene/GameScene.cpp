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
	audio = std::make_unique<Audio>();
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	player = std::make_unique<Player>();
	player->Init();

	stage = std::make_unique<Stage>();
	stage->Init();
	targetEase_ = std::make_unique<EaseData>(60);
	LoadSpawnStatus();
	gameTime.Init();
	gameTime.Start();
	Score::Get()->ScoreReset();
	sceneChange_ = std::make_unique<SceneChange>();
	respawnObj = Shape::CreateOBJ("stand");
	damegeParticle = std::make_unique<ParticleManager>();
	damegeParticle->Initialize();
	damegeGraph = Texture::Get()->LoadTexture(L"Resources/Paricle/particle.jpg");
	ui = Sprite::Get()->SpriteCreate(L"Resources/sprite/playUI.png");
	damegeSound = Audio::Get()->SoundLoadWave("Resources/Sound/damege.wav");
	LoadRespawn();
	BallRespawn();
}

void GameScene::Update()
{
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
	for (auto& enemy : enemys) {
		enemy->Update();
	}

	for (auto& ball : balls)
	{
		ball->Update(
			player->GetPosition(), player->GetRotation(),
			stage->GetStageSize());
	}
	sceneChange_->Update();

	EnemyDeath();
	BallDelete();
	BallRespawn();

	gameTime.Update();
	damegeParticle->Update();
}

void GameScene::Draw()
{
	player->Draw();

	for (auto& ball : balls)
	{
		ball->Draw();
	}

	for (auto& enemy : enemys)
	{
		enemy->Draw();
	}
	for (int i = 0; i < respawnPos.size(); i++)
	{
		Object::Draw(respawnObj, respawnPos[i]->pos + Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3());
	}
	stage->Draw();
	for (auto& ball : balls)
	{
		ball->AfterDraw();
	}
	player->ParticleDraw();
	damegeParticle->Draw(damegeGraph);
	Sprite::Get()->Draw(ui, Vec2(), (float)window_width, (float)window_height);
	Score::Get()->GameSceneDraw();
	gameTime.Draw();
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
	for (auto& ball : balls)
	{
		if (ball->GetHaveFlag())
		{
			player->SetBall(ball.get());
		}
	}
}

void GameScene::CameraMove()
{
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

	Camera::Get()->SetCamera(pos, center, Vec3{ 0, 1, 0 });
}

void GameScene::NearEnemyCheck() {
	float farLength = 5000.0f;
	int ensmysNumber = 0;
	for (int i = 0; i < enemys.size(); i++) {
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
	int count = 0;
	for (auto& enemy : enemys)
	{
		if (enemy->GetHp() <= 0)
		{
			enemys.erase(enemys.begin() + count);
			targetFlag_ = true;
			Score::Get()->PlasScore();
		}
		else
		{
			count++;
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

	for (auto& ball : balls)
	{
		if (ball->GetThrowFlag() && ball->GetHitFlag())
		{
			Audio::Get()->SoundSEPlayWave(damegeSound);
			enemys[forcusEnemyNum]->DamageHit(ball->GetPosition(), player->GetComboCount(),ball->GetHitFlag());
			damegeParticle->DamegeAdd(ball->GetPosition(), enemys[forcusEnemyNum]->GetPosition(),
				1.0f, 2.0f, 0.0f,
				Vec4(0.5f, 0.0f, 0.0f, 0.5f), Vec4(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}

	TargetReset(enemys[forcusEnemyNum]->GetPosition(), targetFlag_);

	for (auto& ball : balls)
	{
		if (!ball->GetHaveFlag())
		{
			ball->SetTargetPos(enemys[forcusEnemyNum]->GetPosition());
		}
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

void GameScene::BallDelete()
{
	if (balls.size() == 0) { return; }
	int count = 0;

	for (auto& ball : balls)
	{
		if (ball->GetPosition().z >= 1 && !ball->GetThrowFlag()&& ball->GetBallBoundedFlag())
		{
			balls.erase(balls.begin() + count);
		}

		count++;
	}
}

void GameScene::BallCreate(const Vec3& pos)
{
	std::unique_ptr<Ball> ball = std::make_unique<Ball>();
	ball->Init();
	ball->SetPosition(Vec3{ pos.x,1.0f,pos.z });
	balls.push_back(std::move(ball));
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
	if (enemys.size() < enemyMax && spwnCoolTime <= 0)
	{
		spwnCoolTime = spwnCoolTimeMax;
		std::unique_ptr<Enemy> enemy = std::make_unique <BaseEnemy>();

		if (loadStatus.size() == 0) { return; }
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand2(0, (int)loadStatus.size() - 1);
		int spawn = rand2(mt);

		enemy->Init(loadStatus[spawn]->position, loadStatus[spawn]->rotation);
		enemys.push_back(std::move(enemy));
	}

	if (spwnCoolTime > 0)
	{
		spwnCoolTime--;
	}
}

void GameScene::BallRespawn()
{
	//å≈íËóNÇ´Ç∑ÇÈèåè
	if (balls.size() == 0)
	{
		Vec3 pos = {};
		//ÉâÉìÉ_ÉÄÇ≈èoåªà íu
		if (respawnPos.size() == 0) { return; }
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand2(0, (int)respawnPos.size() - 1);
		int spawn = rand2(mt);

		BallCreate(respawnPos[spawn]->pos);
	}
}

void GameScene::LoadRespawn()
{
	LevelData* levelData = nullptr;
	std::string filepath = "ball";
	levelData = LoadJson::Load(filepath);
	for (auto& loadData : levelData->objects)
	{
		RespawnPos* load = new RespawnPos();
		load->pos = loadData.translation;
		respawnPos.push_back(load);
	}
}