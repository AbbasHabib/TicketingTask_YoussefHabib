#pragma once
#include <crow.h>
#include "ITicketsService.hpp"

class TicketsController {
public:
    TicketsController(crow::SimpleApp& app, ITicketsService& service);
    
public:
    crow::response validate_ticket(const crow::request& req);
    crow::response create_ticket(const crow::request& req);
private:    
    ITicketsService& m_ticket_service;
};
