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
	//Audio�N���X�쐬
	audio = std::make_unique<Audio>();
	//���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g��Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//���f�[�^�ǂݍ���

	//�J�����ʒu��Z�b�g
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	//�X�v���C�g�摜�ǂݍ���

	

	//�I�u�W�F�N�g����
	player = std::make_unique<Player>();
	player->Init();

	// �{�[���̐���
	BallCreate();

	stage = std::make_unique<Stage>();
	stage->Init();

	targetEase_ = std::make_unique<EaseData>(60);

	LoadSpawnStatus();
	gameTime.Init();
	gameTime.Start();
	Score::Get()->ScoreReset();
	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//���C�g�X�V
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

	for (auto& ball : balls)
	{
		ball->Update(
			player->GetPosition(), player->GetRotation(),
			stage->GetStageSize());
	}
	sceneChange_->Update();

	EnemyDeath();
	BallDelete();

	gameTime.Update();
}

void GameScene::Draw()
{
	

	player->Draw();

	for (auto& ball : balls)
	{
		ball->Draw();
	}

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Draw();
	}

	stage->Draw();
	for (auto& ball : balls)
	{
		ball->AfterDraw();
	}
	ball->AfterDraw();
	player->ParticleDraw();
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
	//���a��-10
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

	//�J�����ʒu��Z�b�g
	Camera::Get()->SetCamera(pos, center, Vec3{ 0, 1, 0 });
}

void GameScene::NearEnemyCheck() {
	// �v���C���[�Ɉ�ԋ߂��G����߂�
	float farLength = 5000.0f;
	int ensmysNumber = 0;
	for (int i = 0; i < enemys.size(); i++) {
		// �v���C���[�ƓG�Ƃ̋���
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

	for (auto& ball : balls)
	{
		if (ball->GetThrowFlag() && ball->GetHitFlag())
		{
			enemys[forcusEnemyNum]->DamageHit(ball->GetPosition(), player->GetComboCount());
		}
		enemys[forcusEnemyNum]->DamageHit(ball->GetPosition(), player->GetComboCount());

	}
	
	TargetReset(enemys[forcusEnemyNum]->GetPosition(), targetFlag_);

		TargetReset(enemys[forcusEnemyNum]->GetPosition(), targetFlag_);

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
		if (ball->GetPosition().z >= 1 && !ball->GetThrowFlag())//�o�E���h���I�������
		{
			balls.erase(balls.begin() + count);
		}

		count++;
	}
}

void GameScene::BallCreate()
{
	std::unique_ptr<Ball> ball = std::make_unique<Ball>();
	ball->Init();
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

	//�ő吔���ǂ����@�N�[���^�C���ő���
	if (enemys.size() < enemyMax && spwnCoolTime <= 0)
	{
		spwnCoolTime = spwnCoolTimeMax;
		//�G�̎�ނɂ���ĕ������
		Enemy* enemy = new BaseEnemy();

		//�����_���ŏo���ʒu
		if (loadStatus.size() == 0) { return; }
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand2(0, (int)loadStatus.size() - 1);
		int spawn = rand2(mt);

		enemy->Init(loadStatus[spawn]->position, loadStatus[spawn]->rotation);
		enemys.push_back(enemy);
	}

	if (spwnCoolTime > 0)
	{
		spwnCoolTime--;
	}
}

