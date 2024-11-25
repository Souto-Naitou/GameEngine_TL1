#include "ModelManager.h"


void ModelManager::Initialize()
{
}

void ModelManager::LoadModel(const std::string& _filePath)
{
    if (models_.contains(_filePath))
    {
        return;
    }

    std::unique_ptr<Model> model = std::make_unique<Model>();
    model->Initialize(_filePath);
    models_.insert(std::make_pair(_filePath, std::move(model)));
}
