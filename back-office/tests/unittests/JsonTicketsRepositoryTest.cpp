#include "JsonTicketsRepository.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class JsonTicketsRepositoryTest : public ::testing::Test
{
protected:
    fs::path test_file;

    void SetUp() override {
        test_file = BINARY_DIR"/tickets_test.json";
        std::cout << "test file path :" << test_file << '\n';
        // empty the file with every test case file
        std::ofstream(test_file) << "{}";
    }

    void TearDown() override {
        fs::remove(test_file);
    }

    void create2tickets()
    {
        std::ofstream(test_file) << R"({
            "tickets": [
                {"ticket_id": 1, "creation_date": 1768786878, "validity_in_days": 2, "line_number": 1},
                {"ticket_id": 2, "creation_date": 1768786879, "validity_in_days": 3, "line_number": 2}
            ]
        })";
    }
};


TEST_F(JsonTicketsRepositoryTest, LoadTicketsFromFile)
{
    create2tickets();

    JsonTicketsRepository repo(test_file.string());

    auto t1 = repo.find_by_id(1);
    auto t2 = repo.find_by_id(2);
    auto t3 = repo.find_by_id(3);

    ASSERT_TRUE(t1.has_value());
    EXPECT_EQ(t1->ticket_id, 1);
    EXPECT_EQ(t1->creation_date, 1768786878);
    EXPECT_EQ(t1->line_number, 1);

    ASSERT_TRUE(t2.has_value());
    EXPECT_EQ(t2->ticket_id, 2);
    EXPECT_EQ(t2->creation_date, 1768786879);
    EXPECT_EQ(t2->line_number, 2);   

    EXPECT_FALSE(t3.has_value());
}


TEST_F(JsonTicketsRepositoryTest, GetLastTicketIdWithPreCreatedTickets)
{
    create2tickets();

    JsonTicketsRepository repo(test_file.string());

    EXPECT_EQ(repo.get_last_ticket_id(), 2);

    repo.save(Ticket{5, 5000, 1, 1});
    EXPECT_EQ(repo.get_last_ticket_id(), 5);
}


TEST_F(JsonTicketsRepositoryTest, SaveAndLoadTicket)
{
    JsonTicketsRepository repo(test_file.string());

    Ticket t{1, 1000, 2, 3};
    repo.save(t);

    auto loaded = repo.find_by_id(1);
    ASSERT_TRUE(loaded.has_value());
    EXPECT_EQ(loaded->ticket_id, 1);
    EXPECT_EQ(loaded->creation_date, 1000);
}

TEST_F(JsonTicketsRepositoryTest, GetLastTicketId)
{
    JsonTicketsRepository repo(test_file.string());

    Ticket t1{1, 1000, 2, 3};
    Ticket t2{5, 2000, 3, 2};
    repo.save(t1);
    repo.save(t2);

    EXPECT_EQ(repo.get_last_ticket_id(), 5);
}

TEST_F(JsonTicketsRepositoryTest, FindAllTickets)
{
    JsonTicketsRepository repo(test_file.string());

    repo.save(Ticket{1, 100, 1, 1});
    repo.save(Ticket{2, 200, 2, 2});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 2);
}

TEST(JsonTicketsRepositoryStandalone, MissingFile)
{
    JsonTicketsRepository repo("/nonexistent/path.json");
    auto tickets = repo.find_all();
    EXPECT_TRUE(tickets.empty());
}
