#include "StageSelect.h"
#include"SceneManager.h"
#include"GameScene.h"
StageSelect::StageSelect()
{}
StageSelect::~StageSelect()
{}

void StageSelect::Init()
{
	//���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//�J�����ݒ�
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());


	back = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");
	//�I�u�W�F�N�g����
	object = Shape::CreateOBJ("sphere");
	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();
}


void StageSelect::Update()
{
	//���C�g�X�V
	lightGroup->Update();

	if ((Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA)) && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
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
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	Object::Draw(object, Vec3(), Vec3(1.0f, 1.0f, 1.0f), Vec3());

	DebugText::Get()->Print(10, 20, 3, "Select");

	sceneChange_->Draw();
}

void StageSelect::ShadowDraw()
{



}

void StageSelect::Finalize()
{
	Texture::Get()->Delete();
}