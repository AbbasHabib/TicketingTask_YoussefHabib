#include <crow.h>
#include <thread>
#include "TicketsRepository.hpp"
#include "TicketsService.hpp"
#include "TicketsController.hpp"
#include "TransactionTrackingService.hpp"

using grpc::Server;
using grpc::ServerBuilder;


int main() {
    crow::SimpleApp app;

    // TICKETS_FILE_PATH defined in the build sys
    TicketsRepository repo(TICKETS_FILE_PATH);
    // inject the repo dependency
    TicketsService service(repo);
    // inject the service and app dependency
    TicketsController controller(app, service);

    // TODO: 55556 such info should be fetched from a config manager
    std::thread http_server([&]{app.port(55556).run();});

    // TODO: 55559 such info should be fetched from a config manager
    const std::string server_address(GRPC_SERVER_URI);

    ticketing::TransactionTrackingServiceImpl transactionTrackingRPCservice;

    ServerBuilder builder;
    builder.AddListeningPort(
        server_address,
        grpc::InsecureServerCredentials());

    builder.RegisterService(&transactionTrackingRPCservice);

    std::unique_ptr<Server> grpc_server(builder.BuildAndStart());
    if (!grpc_server)
    {
        std::cerr << "Failed to start gRPC server\n";
    }
    else
    {
        std::cout << "Transaction Tracking gRPC Server listening on "
                    << server_address << std::endl;
        
        grpc_server->Wait();
    }

    http_server.join();
}
