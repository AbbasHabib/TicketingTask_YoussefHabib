#pragma once
#include <cstdint>


struct TicketRequest
{
    // TicketRequest(int validity_in_days, int line_number, int64_t request_date)
    // {

    // }

    int validity_in_days;
    int line_number;
    int64_t request_date; // epoch
};
