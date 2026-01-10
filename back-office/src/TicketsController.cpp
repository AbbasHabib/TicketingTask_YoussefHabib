#include "TicketsController.hpp"
#include "Ticket.hpp"

using json = nlohmann::json;

TicketsController::TicketsController(crow::SimpleApp& app, TicketsService& service)
    : m_ticket_service(service)
{
    CROW_ROUTE(app, "/api/v1/tickets")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
            return create_ticket(req);
        });

    CROW_ROUTE(app, "/api/v1/tickets/validations")
        .methods(crow::HTTPMethod::GET)
        ([this](const crow::request& req) {
            return validate_ticket(req);
        });
}

crow::response TicketsController::create_ticket(const crow::request& req)
{
    auto body = json::parse(req.body, nullptr, false);
    if (body.is_discarded())
    {
        return {400, "Invalid JSON"};
    }

    const auto& [err, result] = m_ticket_service.create_ticket_base64(
        body["validity_in_days"],
        body["line_number"],
        body["request_date"]
    );

    if(err != TicketErrorCode::NoErr)
    {
        return crow::response(400, result);
    }
    else
    {
        auto& encodedTicket = result;
        return crow::response(201, encodedTicket);
    }
}

crow::response TicketsController::validate_ticket(const crow::request& req)
{
    const auto& [err, result] = m_ticket_service.validate_ticket_base64(req.body);

    if (err != TicketErrorCode::NoErr)
    {
        if (err == TicketErrorCode::NotFound)
        {
            return crow::response(404, result.dump());
        }
        if (err == TicketErrorCode::Expired)
        {
            return crow::response(410, result.dump());
        }

        return crow::response(400, result.dump());
    }
    else
    {
        return crow::response(200, result.dump());
    }
}
