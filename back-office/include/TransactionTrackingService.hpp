#pragma once

#include <grpcpp/grpcpp.h>
#include <unordered_map>
#include <mutex>
#include <string>

#include "transaction_tracking.grpc.pb.h"


namespace ticketing
{

class TransactionTrackingServiceImpl final
    : public TransactionTrackingService::Service
{
public:
    TransactionTrackingServiceImpl() = default;

    grpc::Status SubmitReport(
        grpc::ServerContext* context,
        const GateTicketsValidationReport* request,
        ::google::protobuf::Empty* response) override;

    grpc::Status GetReport(
        grpc::ServerContext* context,
        const GetReportRequest* request,
        GetReportResponse* response) override;

private:
    std::unordered_map<std::string, GateTicketsValidationReport> m_reports;
    std::mutex m_reports_mtx;
};

}