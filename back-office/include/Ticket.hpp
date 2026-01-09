#pragma once
#include <cstdint>

struct Ticket
{
    int64_t ticket_id;
    int64_t creation_date; // epoch
    int validity_in_days;
    int line_number;
};


enum class TicketErrorCode : uint8_t
{
    NoErr = 1,
    InvalidBase64,
    InvalidPayload,
    Expired,
    NotYetValid,
    InvalidLine,
    NotFound,
    FromFuture
};
