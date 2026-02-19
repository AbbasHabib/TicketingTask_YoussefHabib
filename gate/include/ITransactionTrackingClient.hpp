#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include <google/protobuf/empty.pb.h>
#include "transaction_tracking.grpc.pb.h"



class ITransactionTrackingClient
{
public:
    virtual ~ITransactionTrackingClient() = default;
    virtual grpc::Status SubmitReport(const ticketing::GateTicketsValidationReport&) = 0;
};
