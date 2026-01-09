#include "TicketsController.hpp"

using json = nlohmann::json;

TicketsController::TicketsController(crow::SimpleApp& app, TicketsService& service)
    : service(service)
{
    CROW_ROUTE(app, "/api/v1/tickets")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
            return create_ticket(req);
        });

    CROW_ROUTE(app, "/api/v1/tickets/validations")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
            return validate_ticket(req);
        });
}

crow::response TicketsController::create_ticket(const crow::request& req)
{
    auto body = json::parse(req.body, nullptr, false);
    if (body.is_discarded()) return {400, "Invalid JSON"};

    auto encoded = service.create_ticket_base64(
        body["validity_days"],
        body["line_number"],
        body["request_date"]
    );

    return crow::response(201, encoded);
}

crow::response TicketsController::validate_ticket(const crow::request& req)
{
    auto result = service.validate_ticket_base64(req.body);

    if (!result["valid"])
    {
        if (result["reason"] == "NOT_FOUND")
        {
            return crow::response(404, result.dump());
        }
        if (result["reason"] == "EXPIRED")
        {
            return crow::response(410, result.dump());
        }
    }

    return crow::response(200, result.dump());
}
