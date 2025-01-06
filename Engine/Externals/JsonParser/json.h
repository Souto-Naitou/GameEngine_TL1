#pragma once

#include <string>
#include <variant>
#include <memory>
#include <map>
#include <vector>
#include <filesystem>

namespace Json
{
    /// 前方宣言
    class Value;


    // 別名定義
    using Array = std::vector<std::shared_ptr<Json::Value>>;
    using Object = std::map<std::string, std::shared_ptr<Json::Value>>;


    // JSONデータ型の定義
    using Type = std::variant<
        std::monostate,   // 未初期化状態を表現
        int,
        float,
        std::string,
        bool,
        Json::Array,
        Json::Object
    >;


    /// <summary>
    /// データクラス
    /// </summary>
    class Value
    {
    public:

        /// <summary>
        /// JSONデータの種類
        /// </summary>
        enum class Type
        {
            Null,
            Int,
            Float,
            String,
            Bool,
            Array,
            Object
        };

        Json::Value::Type type = Json::Value::Type::Null;
        Json::Type value;

        Value operator[](const char* _key) const;
        Value operator[](const std::string& _key) const;

        operator int() const;
        operator float() const;
        operator std::string() const;
        operator bool() const;
        operator Json::Array() const;
        operator Json::Object() const;


    private:
        Value recursive_search(const std::string& _key, const Json::Object& _obj) const;
    };


    /// <summary>
    /// トークン分割クラス
    /// </summary>
    class Tokenizer
    {
    public:
        enum class Type
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

        struct Token
        {
            Json::Tokenizer::Type type;
            std::string value;
        };

        explicit Tokenizer(const std::string& _jsonStr) : jsonStr_(_jsonStr) {}

        Json::Tokenizer::Token NextToken();

    private:
        std::string jsonStr_;
        uint32_t pos_ = 0;


    private:
        void SkipWhiteSpace();
        Json::Tokenizer::Token ParseString();
        Json::Tokenizer::Token ParseNumber();
        bool IsMatchLiteral(const std::string& literal);
    };


    /// <summary>
    /// パースクラス
    /// </summary>
    class Parser
    {
    public:
        explicit Parser(const std::string& _jsonStr) : tokenizer_(_jsonStr) {};

        void Run();

        const Json::Value& GetData() { return data_; }

    private:
        Json::Tokenizer tokenizer_;
        Json::Tokenizer::Token currentToken_;

        Json::Value data_;

    private:
        void Consume(Json::Tokenizer::Type type);

        Json::Object ParseObject();

        Json::Array ParseArray();

        void ParseNumber(Json::Value& _value);

        bool IsDouble(const std::string& _str);
    };


    /// <summary>
    /// 保存クラス (JSON形式)
    /// </summary>
    class Saver
    {
    public:
        Saver(const Saver&) = delete;
        Saver& operator=(const Saver&) = delete;
        Saver(const Saver&&) = delete;
        Saver& operator=(const Saver&&) = delete;

        static Saver* GetInstance()
        {
            static Saver instance;
            return &instance;
        }

        /// <summary>
        /// JSONファイルを保存する
        /// </summary>
        /// <param name="_path">保存先ファイルパス</param>
        /// <param name="_value">JSONデータ</param>
        void Run(std::filesystem::path _path, Json::Value _value);


    private:
        Saver() = default;
        ~Saver() = default;

        std::string GetString(const Json::Value& _value, int _indent, bool _isSameLine);

        uint32_t indentSize_ = 4u;
    };
};