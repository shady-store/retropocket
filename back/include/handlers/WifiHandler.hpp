#ifndef WIFIHANDLER_HPP
#define WIFIHANDLER_HPP
#include "System.hpp"
#include <crow.h>
#include <crow/http_request.h>
#include <crow/logging.h>
class WifiHandler
{
  public:
    static const crow::response handleWifi(const crow::request &req)
    {
        std::string mode = req.get_body_params().get("mode");
        crow::response retour = {"OK"};

        CROW_LOG_DEBUG << "Mode : " << mode;

        if (mode == "AP")
        {
            crow::mustache::context ctx;
            auto apptpl = crow::mustache::load("ap.yaml");
            ctx["apname"] = "retropocket";
            ctx["appassword"] = "retro1234!";
            std::string rendered = apptpl.render(ctx).dump();
            CROW_LOG_DEBUG << rendered.c_str();
            if (!System::writeFile(rendered, "/etc/netplan/wifi.yaml"))
            {
                retour.code = 500;
                retour.body = "Erreur ecriture du fichier de configuration wifi";
            }
        }
        else if (mode == "Client")
        {
            std::string ssid = req.get_body_params().get("ssid");
            std::string password = req.get_body_params().get("password");
            std::string ipaddr = req.get_body_params().get("ipaddr");

            CROW_LOG_DEBUG << "SSID : " << ssid;
            CROW_LOG_DEBUG << "Password : " << password;
            CROW_LOG_DEBUG << "Address : " << ipaddr;

            crow::mustache::context ctx;
            ctx["apname"] = ssid;
            ctx["appassword"] = password;
            if (ipaddr != "")
            {
                if (isValidIPAddress(ipaddr))
                {
                    ctx["ipaddr"] = ipaddr;
                    retour.code = 406;
                    retour.body = "Erreur adresse ip invalide, le mode automatique sera appliqué";
                }
            }
            auto clienttpl = crow::mustache::load("client.yaml");
            std::string rendered = clienttpl.render(ctx).dump();
            CROW_LOG_DEBUG << rendered.c_str();
            if (!System::writeFile(rendered, "/etc/netplan/wifi.yaml"))
            {
                retour.code = 500;
                retour.body = "Erreur ecriture du fichier de configuration wifi";
            }
        }
        System::exec(APPLYCMD.c_str());
        return retour;
    }

  private:
    inline static const std::string APPLYCMD = "netplan apply";
    static bool isValidIPAddress(const std::string &ip)
    {
        std::stringstream ss(ip);
        std::string segment;
        std::vector<std::string> parts;

        while (std::getline(ss, segment, '.'))
        {
            parts.push_back(segment);
        }

        if (parts.size() != 4)
            return false;

        for (const std::string &part : parts)
        {
            if (part.empty() || part.length() > 3)
                return false;

            // Check all characters are digits
            for (char c : part)
            {
                if (!std::isdigit(c))
                    return false;
            }

            // No leading zeros unless single digit
            if (part.length() > 1 && part[0] == '0')
                return false;

            int value = std::stoi(part);
            if (value < 0 || value > 255)
                return false;
        }

        return true;
    }
};
#endif
