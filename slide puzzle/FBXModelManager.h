#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Model.h"

class FBXModelManager final
{
public: // �T�u�N���X
    struct ModelData
    {
        std::string fName; // �t�@�C����
        std::unique_ptr<Model> fbxModel; // FBX���f��
    };

public:
    // ���[�g�p�X
    static const std::string BASEDIRECTORY;

private:
    FBXModelManager() = default;
    ~FBXModelManager() = default;

public:
    FBXModelManager(const FBXModelManager& r) = delete;
    FBXModelManager& operator=(const FBXModelManager& r) = delete;

    /// <summary>
    /// ������
    /// </summary>
    void Initialize();
    /// <summary>
    /// �I��
    /// </summary>
    void Finalize();
    /// <summary>
    /// ���f���̃��[�h
    /// </summary>
    /// <param name="fName">�t�@�C����</param>
    void LoadModel(const std::string& fName);
    /// <summary>
    /// ���f���̎擾
    /// </summary>
    /// <param name="fName">�t�@�C����</param>
    /// <returns>�����������f��</returns>
    Model* GetModel(const std::string& fName);
    /// <summary>
    /// �V���O���g���C���X�^���X�̎擾
    /// </summary>
    /// <returns>�V���O���g���C���X�^���X</returns>
    static FBXModelManager* GetInstance();

private:
    // ���f���f�[�^
    std::vector<ModelData> modelData_;
};

