#pragma once
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include"ShaderManager.h"
using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum PipelineType
{
	PipelineNormal,
	PipelineNoShadow,
	PipelineFBX,
};

//構造体
struct PipelineSet
{
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState>pipelinestate;
	//ルートシグネチャ
	ComPtr <ID3D12RootSignature>rootsignature;
};
/// <summary>
/// パイプラインクラス
/// </summary>
class Pipeline
{
public://スプライト
	//通常スプライト
	static PipelineSet  SpriteCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://OBJ
	//影ありOBJ読み込み
	static PipelineSet  OBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//影画像生成用
	static PipelineSet  ShadowCreateGraphicsPipeline(ID3D12Device* dev, const Shader& shader);
	//影無しOBJ
	static PipelineSet  NoShadowOBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://パーティクル
	//通常パーティクル
	static PipelineSet ParticleCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://ポストエフェクト
	//ポストエフェクト(通常)
	static PipelineSet PostNormalCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//被写界深度
	static PipelineSet DepthOfFieldPipelineCreateGraphicesPipeline(ID3D12Device* dev, const Shader &shader);
public:
	//オブジェクト描画
	static PipelineSet OBJPipeline;
	//影無しオブジェクト
	static PipelineSet NoShadowOBJPipeline;
	//２Dスプライト描画
	static PipelineSet SpritePipeline;
	//パーティクル
	static PipelineSet ParticlePiepline;
	//ポストエフェクト
	static PipelineSet PostPipeline;
	//深度値保存用
	static PipelineSet ShadowMapPipeline;
	//被写界深度
	static PipelineSet DepthOfFieldPipeline;





	//パイプライン作成
	static void CreatePipeline(ID3D12Device* dev);

	static int pipelineType;

	static bool SetPipeline(int type);
};