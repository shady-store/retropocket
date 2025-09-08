#include "handlers/BluetoothHandler.hpp"
#include "handlers/SystemHandler.hpp"
#include "handlers/UpdateHandler.hpp"
#include "handlers/WifiHandler.hpp"
#include <crow/common.h>
#include <crow/logging.h>
int main(int argc, char *argv[])
{
    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Debug);

    CROW_ROUTE(app, "/bluetooth")(BluetoothHandler::handleGetAllDevices);
    CROW_ROUTE(app, "/bluetooth").methods(crow::HTTPMethod::Put)(BluetoothHandler::handleConnectDisconnect);
    CROW_ROUTE(app, "/bluetooth").methods(crow::HTTPMethod::Post)(BluetoothHandler::handleAddDevice);
    CROW_ROUTE(app, "/bluetooth").methods(crow::HTTPMethod::Delete)(BluetoothHandler::handleRemoveDevice);

    CROW_ROUTE(app, "/wifi").methods(crow::HTTPMethod::Post)(WifiHandler::handleWifi);
    CROW_ROUTE(app, "/update").methods(crow::HTTPMethod::Get)(UpdateHandler::handleUpdate);
    CROW_ROUTE(app, "/update").methods(crow::HTTPMethod::Post)(UpdateHandler::handleDoUpdate);

    CROW_ROUTE(app, "/shutdown").methods(crow::HTTPMethod::Post)(SystemHandler::handleShutdown);

    app.port(18080).multithreaded().run();

    return 0;
}
