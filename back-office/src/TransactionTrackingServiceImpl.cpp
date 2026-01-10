#include "TransactionTrackingService.hpp"

using grpc::Status;
using grpc::StatusCode;

namespace ticketing
{

Status TransactionTrackingServiceImpl::SubmitReport(grpc::ServerContext* , const GateTicketsValidationReport* request, ::google::protobuf::Empty* )
{
    if (!request)
    {
        return Status(StatusCode::INVALID_ARGUMENT, "empty request");
    }

    const std::string machine_id = request->machine_id();
    if (machine_id.empty())
    {
        return Status(StatusCode::INVALID_ARGUMENT, "machine_id is required");
    }

    {
        std::lock_guard<std::mutex> lk(m_reports_mtx);
        m_reports[machine_id] = *request;
    }

    return Status::OK;
}

Status TransactionTrackingServiceImpl::GetReport(grpc::ServerContext* /*context*/, const GetReportRequest* request, GetReportResponse* response)
{
    if (!request)
    {
        return Status(StatusCode::INVALID_ARGUMENT, "empty request");
    }

    const std::string machine_id = request->machine_id();
    if (machine_id.empty())
    {
        return Status(StatusCode::INVALID_ARGUMENT, "machine_id is required");
    }

    std::lock_guard<std::mutex> lk(m_reports_mtx);

    auto it = m_reports.find(machine_id);
    if (it == m_reports.end())
    {
        return Status(StatusCode::NOT_FOUND, "report not found for machine_id: " + machine_id);
    }

    response->mutable_report()->CopyFrom(it->second);
    return Status::OK;
}

} // namespace ticketing