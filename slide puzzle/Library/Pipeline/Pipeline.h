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

//�\����
struct PipelineSet
{
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState>pipelinestate;
	//���[�g�V�O�l�`��
	ComPtr <ID3D12RootSignature>rootsignature;
};
/// <summary>
/// �p�C�v���C���N���X
/// </summary>
class Pipeline
{
public://�X�v���C�g
	//�ʏ�X�v���C�g
	static PipelineSet  SpriteCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://OBJ
	//�e����OBJ�ǂݍ���
	static PipelineSet  OBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//�e�摜�����p
	static PipelineSet  ShadowCreateGraphicsPipeline(ID3D12Device* dev, const Shader& shader);
	//�e����OBJ
	static PipelineSet  NoShadowOBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://�p�[�e�B�N��
	//�ʏ�p�[�e�B�N��
	static PipelineSet ParticleCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public://�|�X�g�G�t�F�N�g
	//�|�X�g�G�t�F�N�g(�ʏ�)
	static PipelineSet PostNormalCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//��ʊE�[�x
	static PipelineSet DepthOfFieldPipelineCreateGraphicesPipeline(ID3D12Device* dev, const Shader &shader);
public:
	//�I�u�W�F�N�g�`��
	static PipelineSet OBJPipeline;
	//�e�����I�u�W�F�N�g
	static PipelineSet NoShadowOBJPipeline;
	//�QD�X�v���C�g�`��
	static PipelineSet SpritePipeline;
	//�p�[�e�B�N��
	static PipelineSet ParticlePiepline;
	//�|�X�g�G�t�F�N�g
	static PipelineSet PostPipeline;
	//�[�x�l�ۑ��p
	static PipelineSet ShadowMapPipeline;
	//��ʊE�[�x
	static PipelineSet DepthOfFieldPipeline;





	//�p�C�v���C���쐬
	static void CreatePipeline(ID3D12Device* dev);

	static int pipelineType;

	static bool SetPipeline(int type);
};