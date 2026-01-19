#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TicketsController.hpp"
#include <nlohmann/json.hpp>
#include "ITicketsService.hpp"

class MockTicketsService : public ITicketsService {
public:
    // wrap return type with parentheses because it contains commas
    MOCK_METHOD((std::pair<TicketErrorCode, std::string>), 
                create_ticket_base64,
                (int validity_in_days, int line_number, int64_t request_date),
                (override));

    MOCK_METHOD((std::pair<TicketErrorCode, nlohmann::json>),
                validate_ticket_base64,
                (const std::string& base64_ticket),
                (override));

    MOCK_METHOD(bool, 
                is_expired, 
                (int64_t creation_date, int64_t validity_in_days), 
                (override));

    MOCK_METHOD(bool, 
                is_expired, 
                (const Ticket& ticket), 
                (override));
};

TEST(TicketsControllerTest, CreateTicket_Success) {
    MockTicketsService mockService;

    // Arrange
    std::string jsonReq = R"({
        "validity_in_days": 999,
        "line_number": 2,
        "request_date": 1768071041
        })";
    EXPECT_CALL(mockService, create_ticket_base64(999, 2, 1768071041))
        .WillOnce(testing::Return(std::make_pair(TicketErrorCode::NoErr, "encoded_ticket_here")));

    crow::SimpleApp app;  // dummy app, won't run server
    TicketsController controller(app, mockService);

    crow::request req;
    req.body = jsonReq;

    // Act
    crow::response res = controller.create_ticket(req);

    // Assert
    EXPECT_EQ(res.code, 201);
    EXPECT_EQ(res.body, "encoded_ticket_here");
}



TEST(TicketsControllerTest, CreateMalformatedTicket_Fail) {
    MockTicketsService mockService;

    // Arrange
    std::string invalidJsonReq = R"({{{)";

    crow::SimpleApp app;  // dummy app, won't run server
    TicketsController controller(app, mockService);

    crow::request req;
    req.body = invalidJsonReq;

    // Act
    crow::response res = controller.create_ticket(req);

    // Assert
    EXPECT_EQ(res.code, 400);
    EXPECT_EQ(res.body, "Invalid JSON");
}



TEST(TicketsControllerTest, ValidateTicket_NotFound) {
    MockTicketsService mockService;
    // Arrange
    std::string ticket = "eyJjcmVhdGlvbl9kYXRlIjoxNzY4MDcxMDQxLCJsaW5lX251bWJlciI6MiwidGlja2V0X2lkIjoxLCJ2YWxpZGl0eV9pbl9kYXlzIjo5OTl9"; 
    EXPECT_CALL(mockService, validate_ticket_base64(ticket))
        .WillOnce(testing::Return(std::make_pair(TicketErrorCode::NotFound, nlohmann::json{{"error", "Ticket not found"}})));

    crow::SimpleApp app;
    TicketsController controller(app, mockService);

    crow::request req;
    req.body = ticket;

    crow::response res = controller.validate_ticket(req);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, R"({"error":"Ticket not found"})");
}




