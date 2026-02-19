#include "TransactionTrackingClient.hpp"


TransactionTrackingClient::TransactionTrackingClient()
    : m_stub(ticketing::TransactionTrackingService::NewStub(
            grpc::CreateChannel(GRPC_SERVER_URI, grpc::InsecureChannelCredentials())))
{}

// Submit a report to the server. Returns grpc::Status so caller can inspect errors.
grpc::Status TransactionTrackingClient::SubmitReport(const ticketing::GateTicketsValidationReport& report)
{
    grpc::ClientContext ctx;
    google::protobuf::Empty resp;
    return m_stub->SubmitReport(&ctx, report, &resp);
}
