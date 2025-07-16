#pragma once

#include <string>
#include <Features/Model/IModel.h>

struct ModelData;
struct MaterialData;

namespace Helper::Model
{
    ModelData LoadObjFile(const std::string& _directoryPath, const std::string& _filename, const std::string& _texturePath = {});
    MaterialData LoadMaterialTemplateFile(const std::string& _directoryPath, const std::string& _filename, const std::string& _texturePath = {});
    void DispatchModel(IModel* _pModel);
}