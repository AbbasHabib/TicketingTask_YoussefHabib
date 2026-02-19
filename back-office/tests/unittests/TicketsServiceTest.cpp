#include <crow/utility.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <optional>
#include "TicketsService.hpp"
#include "Ticket.hpp"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;


class MockTicketsRepository : public ITicketsRepository
{
public:
    MOCK_METHOD(void, save, (const Ticket& ticket), (override));

    MOCK_METHOD(std::optional<Ticket>, find_by_id, (int64_t ticket_id), (override));

    MOCK_METHOD(std::vector<Ticket>, find_all, (), (override));

    MOCK_METHOD(int64_t, get_last_ticket_id, (), (override));
};


class TicketsServiceTest : public ::testing::Test
{
protected:
    NiceMock<MockTicketsRepository> mock_repo;
    TicketsService service{mock_repo};
};


std::string base64encode(const std::string& in)
{
    return crow::utility::base64encode(in.c_str(), in.size());
}


std::string base64decode(const std::string& in)
{
    return crow::utility::base64decode(in.c_str(), in.size());
}


TEST_F(TicketsServiceTest, CreateTicket_Success)
{
    int validity = 2;   // 2 days
    int line = 5;
    int64_t now = std::time(nullptr);

    // Mock repository behavior
    EXPECT_CALL(mock_repo, get_last_ticket_id())
        .WillOnce(Return(42));  // Last ticket ID = 42

    EXPECT_CALL(mock_repo, save(_))
        .Times(1);

    auto [err, encoded] = service.create_ticket_base64(validity, line, now);

    EXPECT_EQ(err, TicketErrorCode::NoErr);
    EXPECT_FALSE(encoded.empty());

    // Decode to verify content
    auto decoded_json = nlohmann::json::parse(base64decode(encoded));
    EXPECT_EQ(decoded_json["ticket_id"], 43);  // next ID
    EXPECT_EQ(decoded_json["line_number"], line);
    EXPECT_EQ(decoded_json["validity_in_days"], validity);
    EXPECT_EQ(decoded_json["creation_date"], now);
}


TEST_F(TicketsServiceTest, CreateTicket_Expired)
{
    int validity = 1;
    int line = 1;
    int64_t request_date = std::time(nullptr) - 86400 * 2;  // 2 days ago

    auto [err, msg] = service.create_ticket_base64(validity, line, request_date);

    EXPECT_EQ(err, TicketErrorCode::Expired);
}


TEST_F(TicketsServiceTest, ValidateTicket_Success)
{
    Ticket t{1, std::time(nullptr) - 1000, 2, 3}; // Not expired

    EXPECT_CALL(mock_repo, find_by_id(1))
        .WillOnce(Return(t));

    nlohmann::json j = {
        {"ticket_id", 1},
        {"creation_date", t.creation_date},
        {"validity_in_days", t.validity_in_days},
        {"line_number", t.line_number}
    };

    std::string encoded = base64encode(j.dump());

    auto [err, result] = service.validate_ticket_base64(encoded);

    EXPECT_EQ(err, TicketErrorCode::NoErr);
    EXPECT_TRUE(result["valid"]);
    EXPECT_EQ(result["ticket_id"], 1);
}


TEST_F(TicketsServiceTest, ValidateTicket_NotFound)
{
    Ticket t{1, std::time(nullptr) - 1000, 2, 3}; // Not expired

    EXPECT_CALL(mock_repo, find_by_id(1))
        .WillOnce(Return(std::nullopt));

    nlohmann::json j = {
        {"ticket_id", 1},
        {"creation_date", t.creation_date},
        {"validity_in_days", t.validity_in_days},
        {"line_number", t.line_number}
    };

    std::string encoded = base64encode(j.dump());

    auto [err, result] = service.validate_ticket_base64(encoded);

    EXPECT_EQ(err, TicketErrorCode::NotFound);
    EXPECT_FALSE(result["valid"]);
    EXPECT_EQ(result["reason"], "NOT_FOUND");
}


TEST_F(TicketsServiceTest, ValidateTicket_Expired)
{
    Ticket t{1, std::time(nullptr) - 86400 * 5, 2, 3}; // expired

    EXPECT_CALL(mock_repo, find_by_id(1))
        .WillOnce(Return(t));

    nlohmann::json j ={
        {"ticket_id", 1},
        {"creation_date", t.creation_date},
        {"validity_in_days", t.validity_in_days},
        {"line_number", t.line_number}
    };
    std::string encoded = base64encode(j.dump());

    auto [err, result] = service.validate_ticket_base64(encoded);

    EXPECT_EQ(err, TicketErrorCode::Expired);
    EXPECT_FALSE(result["valid"]);
    EXPECT_EQ(result["reason"], "EXPIRED");
}
