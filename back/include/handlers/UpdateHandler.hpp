#ifndef UPDATE_HANDLER_HPP
#define UPDATE_HANDLER_HPP
#include "Update.hpp"
#include <crow.h>
#include <crow/http_request.h>
#include <crow/mustache.h>
class UpdateHandler
{

  public:
    static const crow::response handleUpdate(const crow::request &req)
    {
        crow::mustache::context ctx;
        auto updateTpl = crow::mustache::load("update.mustache.html");
        ctx["needUpdate"] = Update::needUpdate();
        return updateTpl.render(ctx);
    }
    static const crow::response handleDoUpdate(const crow::request &req)
    {
        crow::response resp = {"Un problème est survenu, veuillez réessayer plus tard"};
        // check if update is necessary
        if (Update::needUpdate())
        {
            // if update needed
            // download package
            if (Update::downloadUpdateAndExtract())
            {
                resp = {"Mise à jour téléchargée, veuillez redémarrer l'appareil"};
            }
            // reboot
        }
        return resp;
    }
};

#endif
