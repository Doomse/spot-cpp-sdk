/**
 * Copyright (c) 2023 Boston Dynamics, Inc.  All rights reserved.
 *
 * Downloading, reproducing, distributing or otherwise using the SDK Software
 * is subject to the terms and conditions of the Boston Dynamics Software
 * Development Kit License (20191101-BDSDK-SL).
 */


#include "bosdyn/client/ir_enable_disable/ir_enable_disable_client.h"

using namespace std::placeholders;

namespace bosdyn {

namespace client {

const char* IREnableDisableClient::s_default_service_name = "ir-enable-disable-service";

const char* IREnableDisableClient::s_default_service_authority =
    "ir-enable-disable-service.spot.robot";

const char* IREnableDisableClient::s_service_type = "bosdyn.api.IREnableDisableService";

std::shared_future<IREnableDisableResultType> IREnableDisableClient::IREnableDisableAsync(
    ::bosdyn::api::IREnableDisableRequest& request, const RPCParameters& parameters) {
    std::promise<IREnableDisableResultType> response;
    std::shared_future<IREnableDisableResultType> future = response.get_future();
    BOSDYN_ASSERT_PRECONDITION(m_stub != nullptr, "Stub for service is unset!");

    MessagePumpCallBase* one_time = InitiateAsyncCall<::bosdyn::api::IREnableDisableRequest,
                                                      ::bosdyn::api::IREnableDisableResponse,
                                                      ::bosdyn::api::IREnableDisableResponse>(
        request,
        std::bind(&::bosdyn::api::IREnableDisableService::StubInterface::AsyncIREnableDisable,
                  m_stub.get(), _1, _2, _3),
        std::bind(&IREnableDisableClient::OnIREnableDisableComplete, this, _1, _2, _3, _4, _5),
        std::move(response), parameters);

    return future;
}

IREnableDisableResultType IREnableDisableClient::IREnableDisable(
    ::bosdyn::api::IREnableDisableRequest& request, const RPCParameters& parameters) {
    return IREnableDisableAsync(request, parameters).get();
}

void IREnableDisableClient::OnIREnableDisableComplete(
    MessagePumpCallBase* call, const ::bosdyn::api::IREnableDisableRequest& request,
    ::bosdyn::api::IREnableDisableResponse&& response, const grpc::Status& status,
    std::promise<IREnableDisableResultType> promise) {
    ::bosdyn::common::Status ret_status =
        ProcessResponseAndGetFinalStatus<::bosdyn::api::IREnableDisableResponse>(
            status, response, SDKErrorCode::Success);
    promise.set_value({ret_status, std::move(response)});
}

ServiceClient::QualityOfService IREnableDisableClient::GetQualityOfService() const {
    return QualityOfService::NORMAL;
}

void IREnableDisableClient::SetComms(const std::shared_ptr<grpc::ChannelInterface>& channel) {
    m_stub.reset(new ::bosdyn::api::IREnableDisableService::Stub(channel));
}

}  // namespace client

}  // namespace bosdyn
