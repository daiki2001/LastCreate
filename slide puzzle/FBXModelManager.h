#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Model.h"

class FBXModelManager final
{
public: // サブクラス
    struct ModelData
    {
        std::string fName; // ファイル名
        std::unique_ptr<Model> fbxModel; // FBXモデル
    };

public:
    // ルートパス
    static const std::string BASEDIRECTORY;

private:
    FBXModelManager() = default;
    ~FBXModelManager() = default;

public:
    FBXModelManager(const FBXModelManager& r) = delete;
    FBXModelManager& operator=(const FBXModelManager& r) = delete;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();
    /// <summary>
    /// 終了
    /// </summary>
    void Finalize();
    /// <summary>
    /// モデルのロード
    /// </summary>
    /// <param name="fName">ファイル名</param>
    void LoadModel(const std::string& fName);
    /// <summary>
    /// モデルの取得
    /// </summary>
    /// <param name="fName">ファイル名</param>
    /// <returns>検索したモデル</returns>
    Model* GetModel(const std::string& fName);
    /// <summary>
    /// シングルトンインスタンスの取得
    /// </summary>
    /// <returns>シングルトンインスタンス</returns>
    static FBXModelManager* GetInstance();

private:
    // モデルデータ
    std::vector<ModelData> modelData_;
};

