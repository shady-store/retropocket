#ifndef SYSTEMHANDLER_HPP
#define SYSTEMHANDLER_HPP
#include "System.hpp"
#include <crow.h>
class SystemHandler
{
  public:
    static const crow::response handleShutdown(const crow::request &req)
    {
        crow::response resp = {"ok"};
        auto reboot = req.url_params.get("reboot");

        if (reboot != nullptr && std::string(reboot) == "true")
        {
            System::exec("reboot");
        }
        else
        {
            System::exec("shutdown -h now");
        }
        return resp;
    }
};
#endif
