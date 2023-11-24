#include "ResultScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include "../../GameInputManager.h"
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{}
void ResultScene::Init()
{
	// ���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
	//�J�����ݒ�
	Camera::Get()->SetCamera(Vec3{ 0,10,-10 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();

	back = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");

	//�I�u�W�F�N�g����
	object = Shape::CreateOBJ("sphere");
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
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	Object::Draw(object, Vec3(), Vec3(1.0f, 1.0f, 1.0f), Vec3());

	DebugText::Get()->Print(10, 20, 3, "Result");
	sceneChange_->Draw();
}

void ResultScene::ShadowDraw()
{

}


void ResultScene::Finalize()
{
	Texture::Get()->Delete();
}
