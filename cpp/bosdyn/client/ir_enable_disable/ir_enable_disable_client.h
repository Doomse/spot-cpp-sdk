/**
 * Copyright (c) 2023 Boston Dynamics, Inc.  All rights reserved.
 *
 * Downloading, reproducing, distributing or otherwise using the SDK Software
 * is subject to the terms and conditions of the Boston Dynamics Software
 * Development Kit License (20191101-BDSDK-SL).
 */


#pragma once

#include "bosdyn/client/service_client/service_client.h"

#include <bosdyn/api/ir_enable_disable_service.grpc.pb.h>
#include <bosdyn/api/ir_enable_disable_service.pb.h>
#include <future>

namespace bosdyn {

namespace client {

typedef Result<::bosdyn::api::IREnableDisableResponse> IREnableDisableResultType;

class IREnableDisableClient : public ServiceClient {
 public:
    IREnableDisableClient() = default;

    ~IREnableDisableClient() = default;

    // Asynchronous method to send an IREnableDisable request.
    std::shared_future<IREnableDisableResultType> IREnableDisableAsync(
        ::bosdyn::api::IREnableDisableRequest& request,
        const RPCParameters& parameters = RPCParameters());

    // Synchronous method to send an IREnableDisable request.
    IREnableDisableResultType IREnableDisable(::bosdyn::api::IREnableDisableRequest& request,
                                              const RPCParameters& parameters = RPCParameters());

    // Start of ServiceClient overrides.
    QualityOfService GetQualityOfService() const override;
    void SetComms(const std::shared_ptr<grpc::ChannelInterface>& channel) override;
    // End of ServiceClient overrides.

    // Get the default service name the IREnableDisable service will be registered in the directory
    // with.
    static std::string GetDefaultServiceName() { return s_default_service_name; }

    // Get the default service authority the IREnableDisable service will be registered in the
    // directory with.
    static std::string GetDefaultServiceAuthority() { return s_default_service_authority; }

    // Get the default service type for the IREnableDisable service that will be registered in the
    // directory.
    static std::string GetServiceType() { return s_service_type; }

 private:
    // Callback function registered for the asynchronous calls.
    void OnIREnableDisableComplete(MessagePumpCallBase* call,
                                   const ::bosdyn::api::IREnableDisableRequest& request,
                                   ::bosdyn::api::IREnableDisableResponse&& response,
                                   const grpc::Status& status,
                                   std::promise<IREnableDisableResultType> promise);

    std::unique_ptr<::bosdyn::api::IREnableDisableService::StubInterface> m_stub;

    // Default service name for the IREnableDisable service.
    static const char* s_default_service_name;

    // Default service authority for the IREnableDisable service.
    static const char* s_default_service_authority;

    // Default service type for the IREnableDisable service.
    static const char* s_service_type;
};

}  // namespace client

}  // namespace bosdyn
