#pragma once

#include "jsontypes.h"
#include "JsonTokenizer.h"

class JsonParser
{
public:
    explicit JsonParser(const std::string& _jsonStr) : tokenizer_(_jsonStr) {};

    void Run();

    const JsonValue& GetData() { return data_; }

private:
    JsonTokenizer tokenizer_;
    JsonToken currentToken_;

    JsonValue data_;

private:
    void Consume(JsonTokenType type);

    JsonObject ParseObject();

    JsonArray ParseArray();

    void ParseNumber(JsonValue& _value);

    bool IsDouble(const std::string& _str);
};