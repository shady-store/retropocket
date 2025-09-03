#ifndef BLUETOOTHHANDLER_HPP
#define BLUETOOTHHANDLER_HPP
#include "Bluetooth.hpp"
#include <crow.h>
#include <crow/http_request.h>
#include <crow/mustache.h>
class BluetoothHandler
{
  public:
    static const crow::response handleGetAllDevices(const crow::request &req)
    {
        crow::mustache::context ctx;
        auto bluetoothTableTpl = crow::mustache::load("bluetoothTableTpl.mustache.html");
        ctx["devices"] = Tools::convertListToWValue(Bluetooth::getAllDevices());
        return bluetoothTableTpl.render(ctx);
    }
    static const char *handleRemoveDevice(const crow::request &req)
    {
        std::string macAddr = req.url_params.get("mac");
        if (Bluetooth::removeDevice(macAddr))
        {
            return "ok";
        }
        else
        {
            return "error";
        }
    }
    static const char *handleAddDevice(const crow::request &req)
    {
        std::string macAddr = req.url_params.get("mac");
        if (Bluetooth::addDevice(macAddr))
        {
            return "ok";
        }
        else
        {
            return "error";
        }
    }
    static const char *handleConnectDisconnect(const crow::request &req)
    {
        std::string macAddr = req.url_params.get("mac");
        std::string action = req.url_params.get("action");
        if (action == "connect")
        {
            if (Bluetooth::connectDevice(macAddr))
            {
                return "ok";
            }
            else
            {
                return "error";
            }
        }
        else if (action == "disconnect")
        {
            if (Bluetooth::disconnectDevice(macAddr))
            {
                return "ok";
            }
            else
            {
                return "error";
            }
        }
        else
        {
            return "error";
        }
    }
};
#endif
