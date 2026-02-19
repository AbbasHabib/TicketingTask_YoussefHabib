#pragma once

#include "ITransactionTrackingClient.hpp"


class TransactionTrackingClient : public ITransactionTrackingClient 
{
public:
    explicit TransactionTrackingClient();

    // Submit a report to the server. Returns grpc::Status so caller can inspect errors.
    grpc::Status SubmitReport(const ticketing::GateTicketsValidationReport& report) override;

private:
    std::unique_ptr<ticketing::TransactionTrackingService::Stub> m_stub;
};