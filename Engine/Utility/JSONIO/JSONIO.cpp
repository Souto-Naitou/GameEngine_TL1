#include "./JSONIO.h"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <ostream>

using json = nlohmann::json;

#pragma region JSONDataKeeper

    json& JSONStorage::operator[](const std::string& _key)
    {
        return jsonDataMap_[ToAbsPathLower(_key)];
    }

    JSONStorage::Iterator JSONStorage::begin()
    {
        return Iterator(jsonDataMap_.begin());
    }

    JSONStorage::Iterator JSONStorage::end()
    {
        return Iterator(jsonDataMap_.end());
    }

    json& JSONStorage::at(const std::string& _key)
    {
        return jsonDataMap_.at(ToAbsPathLower(_key));
    }

    JSONStorage::Iterator JSONStorage::find(const std::string& _key)
    {
        auto it = jsonDataMap_.find(ToAbsPathLower(_key));
        if (it != jsonDataMap_.end())
        {
            return Iterator(it);
        }
        return end();
    }

    std::string JSONStorage::ToAbsPathLower(const std::string& _filepath) const
    {
        /// 絶対パスに変換してから小文字に変換する
        std::filesystem::path path(_filepath);
        std::string lowerPath = std::filesystem::absolute(path).string();
        std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);
        return lowerPath;
    }

    std::pair<const std::string, json> JSONStorage::Iterator::operator*() const
    {
        return *it_;
    }

    JSONStorage::Iterator& JSONStorage::Iterator::operator++()
    {
        ++it_;
        return *this;
    }

    JSONStorage::Iterator& JSONStorage::Iterator::operator--()
    {
        --it_;
        return *this;
    }

    bool JSONStorage::Iterator::operator==(const Iterator& _other) const
    {
        return it_ == _other.it_;
    }

    bool JSONStorage::Iterator::operator!=(const Iterator& _other) const
    {
        return it_ != _other.it_;
    }

#pragma endregion


#pragma region JSONLoader
    
    json& JSONIO::Load(const std::string& _path)
    {
        if (jsonDataKeeper_.find(_path) != jsonDataKeeper_.end())
        {
            return jsonDataKeeper_[_path];
        }

        std::fstream fs(_path);
        if (!fs.is_open())
        {
            throw std::runtime_error("Failed to open JSON file: " + _path);
        }

        json jsonData;
        fs >> jsonData;
        fs.close();

        jsonDataKeeper_[_path] = jsonData;
        return jsonDataKeeper_[_path];
    }

    void JSONIO::Save(const std::string& _path, const json& _jsonData)
    {
        std::ofstream ofs(_path);
        if (!ofs.is_open())
        {
            throw std::runtime_error("Failed to open JSON file for writing: " + _path);
        }

        ofs << _jsonData << std::endl;
        ofs.close();
    }

#pragma endregion