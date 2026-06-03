#ifndef BLUETOOTHHANDLER_HPP
#define BLUETOOTHHANDLER_HPP
#include "Bluetooth.hpp"
#include <crow.h>
#include <crow/http_request.h>
#include <crow/mustache.h>
#include <regex>
class BluetoothHandler
{
  private:
    static bool isValidMacAddress(const std::string &mac)
    {
        static const std::regex macRegex("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
        return std::regex_match(mac, macRegex);
    }

  public:
    static const crow::response handleGetAllDevices(const crow::request &req)
    {
        crow::mustache::context ctx;
        auto bluetoothTableTpl = crow::mustache::load("bluetooth.mustache.html");
        ctx["devices"] = Tools::convertListToWValue(Bluetooth::getAllDevices());
        return bluetoothTableTpl.render(ctx);
    }
    static const char *handleRemoveDevice(const crow::request &req)
    {
        std::string macAddr = req.url_params.get("mac");
        if (!isValidMacAddress(macAddr))
        {
            return "error";
        }
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
        if (!isValidMacAddress(macAddr))
        {
            return "error";
        }
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
        if (!isValidMacAddress(macAddr))
        {
            return "error";
        }
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
