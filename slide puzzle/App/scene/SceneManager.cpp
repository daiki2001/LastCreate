#include "SceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"PostEffect.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"StageSelect.h"
#include"ResultScene.h"
#include "../../GameInputManager.h"
#include"../slide puzzle/Score.h"
#include "../../FBXModelManager.h"
SceneManager::SceneManager()
{}
SceneManager::~SceneManager()
{
	//XAudio2解放
	Audio::Get()->xAudio2.Reset();

	//音データ解放
	scene_->Finalize();
	delete scene_;
}
void SceneManager::Initialize()
{
	//汎用機能
	//ゲームウィンドウの作成
	Window::Get()->CreateGameWindow();
	// DirectX初期化処理
	DirectXBase::Get()->Initilize();
	//入力の初期化
	Input::Get()->Initialize();
	GameInputManager::Get()->Init();
	//シェーダーの読み込み
	ShaderManager::LoadShaders();
	//ライト静的初期化
	LightGroup::StaticInitialize(DirectXBase::Get()->GetDevice());
	//スプライト静的初期化
	Sprite::StaticInit(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	//テキストクラス初期化
	Texture::Get()->Init(DirectXBase::Get()->GetDevice());
	Pipeline::CreatePipeline(DirectXBase::Get()->GetDevice());
	//デバックテキスト初期化
	DebugText::Get()->Initialize();
	//スプライトクラス作成
	Sprite::Get()->Init();
	//FBX初期化
	FbxLoader::GetInstance()->Initialize(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetDevice(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetCmdList(DirectXBase::Get()->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateShadowPipeline();

	FBXModelManager::GetInstance()->Initialize();
	//図形モデル初期化
	Shape::Init(DirectXBase::Get()->GetDevice());
	//パーティクル初期化
	ParticleManager::StaticInitialize(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList(), window_width, window_height);
	//3Dオブジェクト初期化
	Object::Init(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());

	PostEffect::Get()->Initialize(DirectXBase::Get()->GetDevice());
	////影
	m_shadowMapFar.Init();

	Texture::Get()->LoadShadowTexture(m_shadowMapFar.GetTexbuff());
	Texture::Get()->LoadCameraTexture(PostEffect::Get()->GetDepth());

	//音作成
	Audio::Get()->Init();
	Score::Get()->Init();
	Texture::Get()->KeepTexture();
	//シーンをタイトルに設定
	BaseScene* scene = new TitleScene();
	SetNextScene(scene);
	sound = Audio::SoundLoadWave("Resources/Sound/BGM.wav");
	Audio::Get()->SoundBGMPlayLoopWave(sound);
	Audio::Get()->SetVolume(0.05f);

}

void SceneManager::Update()
{
	Input::Get()->Update();
	GameInputManager::Get()->Update();

	//シーンチェンジ
	SceneChange();

	//次のシーンの予約があるなら
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		
		scene_->SetSceneManager(this);
		//次のシーンを初期化する
		scene_->Init();
	}
	//更新
	scene_->Update();

	if (Input::Get()->KeybordTrigger(DIK_T) == true)
	{
		BaseScene* scene = new TitleScene();
		SetNextScene(scene);
	}
	if (Input::Get()->KeybordTrigger(DIK_0) == true) {
		BaseScene* scene = new ResultScene();
		SetNextScene(scene);
	}
}

void SceneManager::Draw()
{
	//影深度値取得
	m_shadowMapFar.PreDraw(DirectXBase::Get()->GetCmandList());
	Object::InitShadow(), Sprite::Get()->PreShadowDraw(),FBXObject3d::InitShadow();
	//影描画
	scene_->Draw();
	m_shadowMapFar.PostDraw(DirectXBase::Get()->GetCmandList());


	PostEffect::Get()->PreDrawScene(DirectXBase::Get()->GetCmandList());
	Object::InitDraw(), Sprite::Get()->PreDraw(), FBXObject3d::InitDraw();
	//カメラ目線の描画
	scene_->Draw();
	scene_->ShadowDraw();
	DebugText::Get()->DrawAll();
	PostEffect::Get()->PostDrawScene(DirectXBase::Get()->GetCmandList());

	DirectXBase::Get()->PreDraw();
	////ポストエフェクトの描画
	PostEffect::Get()->Draw(DirectXBase::Get()->GetCmandList());
	DirectXBase::Get()->ResourceBarrier();
}

void SceneManager::Delete()
{
	Object::Delete();
	FBXModelManager::GetInstance()->Finalize();
}

void SceneManager::SceneChange()
{

}