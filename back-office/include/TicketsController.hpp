#pragma once
#include <crow.h>
#include "TicketsService.hpp"

class TicketsController {
public:
    TicketsController(crow::SimpleApp& app, TicketsService& service);

private:
    TicketsService& service;

    crow::response create_ticket(const crow::request& req);
    crow::response validate_ticket(const crow::request& req);
};
