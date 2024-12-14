#pragma once

#include <string>
#include <variant>
#include <memory>
#include <map>
#include <vector>

// 前方宣言
class JsonValue;

// 別名定義
using JsonArray = std::vector<std::shared_ptr<JsonValue>>;
using JsonObject = std::map<std::string, std::shared_ptr<JsonValue>>;

// JSONデータ型の定義
using JsonValueType = std::variant<
    std::monostate,   // 未初期化状態を表現
    int,
    float,
    std::string,
    bool,
    JsonArray,
    JsonObject
>;

enum class JsonValueTypes
{
    Null,
    Int,
    Float,
    String,
    Bool,
    Array,
    Object
};

class JsonValue
{
public:
    JsonValueTypes type;
    JsonValueType value;

    JsonValue operator[](const char* _key) const;
    JsonValue operator[](const std::string& _key) const;

    operator int() const;
    operator float() const;
    operator std::string() const;
    operator bool() const;
    operator JsonArray() const;
    operator JsonObject() const;


private:
    JsonValue recursive_search(const std::string& _key, const JsonObject& _obj) const;
};

enum class JsonTokenType
{
    LeftBrace,          // {
    RightBrace,         // }
    LeftBracket,        // [
    RightBracket,       // ]
    Colon,              // :
    Comma,              // ,
    String,             // 文字列
    Number,             // 数値
    True,               // 真
    False,              // 偽
    Null,               // null
    EndOfFile,          // ファイルの終わり
};

struct JsonToken
{
    JsonTokenType type;
    std::string value;
};