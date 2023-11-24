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
	//Audio�N���X�쐬
	audio = std::make_unique<Audio>();
	//���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//���f�[�^�ǂݍ���

	//�J�����ʒu���Z�b�g
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	//�X�v���C�g�摜�ǂݍ���

	back = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");

	//�I�u�W�F�N�g����
	player = std::make_unique<Player>();
	player->Init();

	ball = std::make_unique<Ball>();
	ball->Init();


	stage = std::make_unique<Stage>();
	stage->Init();
	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//���C�g�X�V
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

	//�J�����ʒu���Z�b�g
	Camera::Get()->SetCamera(pos, center, Vec3{ 0, 1, 0 });
}

