#pragma once

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <google/protobuf/empty.pb.h>

#include "transaction_tracking.grpc.pb.h"

class TransactionTrackingClient {
public:
    explicit TransactionTrackingClient(const std::string& server_address)
        : stub_(ticketing::TransactionTrackingService::NewStub(
              grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())))
    {}

    // Submit a report to the server. Returns grpc::Status so caller can inspect errors.
    grpc::Status SubmitReport(const ticketing::GateTicketsValidationReport& report) {
        grpc::ClientContext ctx;
        google::protobuf::Empty resp;
        return stub_->SubmitReport(&ctx, report, &resp);
    }

private:
    std::unique_ptr<ticketing::TransactionTrackingService::Stub> stub_;
};