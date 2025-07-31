#pragma once

#include <Network/TCP_IP.h>
#include <string>
#include <Features/ThreadPool/ThreadPool.h>
#include <DebugTools/Logger/Logger.h>
#include <cstdint>

class BlenderConnect
{
public:
    enum class OrderType
    {
        None = 0,
        RequestSendModelList,
        RequestReloadScene,
    };

public:
    void Initialize();
    void Finalize();

    void OpenConnection(const std::string& _port = "2121");

private:
    void ListenAndAccept();

    // 通信のメインループ
    void Communicate();

    // Orderを受信してキューに格納する
    std::string ReceiveOrder();
    // OrderをOrderTypeに変換してキューに格納する
    OrderType DeserializeOrder(const std::string& _order);
    // OrderTypeからResultを生成してキューに格納する
    std::string GenerateResult(const OrderType& _orderType);
    // Orderに返答する
    void SendResult(const std::string& _result);

    // 接続状態を確認する
    const bool IsConnected() const;

    TCP::Host host_;
    uint32_t handle_ = 0;

    bool isConnected_ = false; // 接続状態

    // Pointers
    ThreadPool* pThreadPool_ = nullptr; // スレッドプール
    Logger* pLogger_ = nullptr; // ロガー
};