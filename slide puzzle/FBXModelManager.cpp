#include "FBXModelManager.h"
#include "FbxLoader.h"

const std::string FBXModelManager::BASEDIRECTORY = "Resources/";

void FBXModelManager::Initialize()
{
    LoadModel("player");
    LoadModel("titlePlayer");
    LoadModel("enemy");
    LoadModel("endEnemy");
}

void FBXModelManager::Finalize()
{
    for (auto& a : modelData_)
    {
        a.fbxModel.reset();
        a.fName.clear();
    }
}

void FBXModelManager::LoadModel(const std::string& fName)
{
    ModelData tempData;
    tempData.fName = BASEDIRECTORY + fName + ".fbx";
    tempData.fbxModel = FbxLoader::GetInstance()->LoadModelFromFile(fName);

    modelData_.push_back(std::move(tempData));
}

Model* FBXModelManager::GetModel(const std::string& fName)
{
    for (int i = 0; i < modelData_.size(); i++)
    {
        if (modelData_[i].fName == BASEDIRECTORY + fName + ".fbx")
        {
            return modelData_[i].fbxModel.get();
        }
    }

    return nullptr;
}

FBXModelManager* FBXModelManager::GetInstance()
{
    static FBXModelManager instance;
    return &instance;
}
