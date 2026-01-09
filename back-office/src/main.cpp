#include <crow.h>
#include "TicketsRepository.hpp"
#include "TicketsService.hpp"
#include "TicketsController.hpp"

int main() {
    crow::SimpleApp app;

    // TICKETS_FILE_PATH defined in the build sys
    TicketsRepository repo(TICKETS_FILE_PATH);
    // inject the repo dependency
    TicketsService service(repo);
    // inject the service and app dependency
    TicketsController controller(app, service);

    // TODO: 5556 such info should be fetched from a config manager
    app.port(5556).run();
}
