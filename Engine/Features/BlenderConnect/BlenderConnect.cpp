#include "BlenderConnect.h"
#include <Features/ThreadPool/ThreadPool.h>
#include <DebugTools/Logger/Logger.h>

void BlenderConnect::Initialize()
{
    pThreadPool_ = ThreadPool::GetInstance();
    pLogger_ = Logger::GetInstance();

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        GetErrorMessage(iResult);
    }

    host_.Initialize(wsaData);
}

void BlenderConnect::Finalize()
{
    host_.Close();
    WSACleanup();
}

void BlenderConnect::OpenConnection(const std::string& _port)
{
    host_.Open(_port);
    // Start listening for incoming connections
    pThreadPool_->enqueue(std::bind(&BlenderConnect::ListenAndAccept, this));
}

void BlenderConnect::ListenAndAccept()
{
    handle_ = host_.ListenAndAccept();
    if (handle_ != -1)
    {
        // Start a new thread to handle the connection
        pThreadPool_->enqueue(std::bind(&BlenderConnect::ReceiveOrder, this));
    }
    else
    {
        pLogger_->LogError(__FILE__, __FUNCTION__, "Failed to accept connection.");
    }
}

void BlenderConnect::Communicate()
{
    while (true)
    {
        std::string order = {};
        order = this->ReceiveOrder();

        OrderType orderType = {};
        orderType = this->DeserializeOrder(order);

        std::string result = {};
        result = this->GenerateResult(orderType);

        this->SendResult(result);
    }
}

std::string BlenderConnect::ReceiveOrder()
{
    std::string order = host_.Receive(handle_);
    // 空だったら警告
    if (order.empty())
    {
        pLogger_->LogWarning(__FILE__, __FUNCTION__, "Received order is empty.");
        return {};
    }

    return order;
}

BlenderConnect::OrderType BlenderConnect::DeserializeOrder(const std::string& _order)
{
    if (_order == "RequestSendModelList")
    {
        return OrderType::RequestSendModelList;
    }
    else if (_order == "RequestReloadScene")
    {
        return OrderType::RequestReloadScene;
    }
    else
    {
        pLogger_->LogWarning(__FILE__, __FUNCTION__, "Unknown order type received.");
        return OrderType::None; // デフォルトの処理
    }
}

std::string BlenderConnect::GenerateResult(const OrderType& _orderType)
{
    return {};
}

void BlenderConnect::SendResult(const std::string& _result)
{
    if (!_result.empty())
    {
        host_.Send(handle_, _result.c_str(), static_cast<int>(_result.size()));
    }
    else
    {
        pLogger_->LogWarning(__FILE__, __FUNCTION__, "Result is empty.");
    }
}

const bool BlenderConnect::IsConnected() const
{
    return !isConnected_;
}