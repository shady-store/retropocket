#ifndef SYSTEMHANDLER_HPP
#define SYSTEMHANDLER_HPP
#include "System.hpp"
#include <crow.h>
#include <crow/logging.h>
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
    static const crow::response handleOptions(const crow::request &req)
    {
        crow::response resp = {"Please restart"};
        crow::mustache::context ctx;
        crow::mustache::context ctxSway;
        // --- Capture Vidéo ---
        std::string videoCapture = req.get_body_params().get("video-capture-activation");
        ctx["videoCapture"] = videoCapture == "on" ? "true" : "false";

        std::string resolution = req.get_body_params().get("resolution-capture-select");
        ctxSway["resolution"] = resolution;

        std::string fps = req.get_body_params().get("fps-capture-select");
        ctx["fps"] = fps;

        ctx["videoBitrate"] = req.get_body_params().get("bitrate-capture-select");
        ctx["buffer"] = req.get_body_params().get("buf-capture-select");
        ctx["gop"] = req.get_body_params().get("gop-capture-select");

        // --- Capture Audio ---
        std::string audioCapture = req.get_body_params().get("audio-capture-activation");
        ctx["audioCapture"] = audioCapture == "on" ? "true" : "false";
        ctx["audioLatency"] = req.get_body_params().get("audio-latency-select");
        ctx["audioBitrate"] = req.get_body_params().get("audiobitrate-latency-select");
        ctx["audioSampleRate"] = req.get_body_params().get("audiosamplerate-latency-select");
        ctx["audioFrameDur"] = req.get_body_params().get("audio-frame-dur-select");

        // --- Système ---
        ctx["governor"] = req.get_body_params().get("governor-select");
        ctx["readAhead"] = req.get_body_params().get("read-ahead-select");
        std::string janus = req.get_body_params().get("janus-select");
        std::string samba = req.get_body_params().get("samba-select");
        ctx["interface"] = req.get_body_params().get("iface-select");

        auto confTpl = crow::mustache::load("retropocket.mustache.conf");
        auto swayTpl = crow::mustache::load("sway.mustache.conf");

        std::string confRendered = confTpl.render_string(ctx);
        std::string swayRendered = swayTpl.render_string(ctxSway);
        CROW_LOG_DEBUG << confRendered.c_str();
        CROW_LOG_DEBUG << swayRendered.c_str();

        if (!System::writeFile(confRendered, "/home/retropocket/sdcard/retropocket.conf"))
        {
            resp.code = 500;
            resp.body = "Erreur ecriture fichier configuration retropocket";
        }
        else
        {
            CROW_LOG_DEBUG << "Ecriture configuration retropocket OK";
        }

        if (!System::writeFile(swayRendered, "/home/retropocket/sway.conf"))
        {
            resp.code = 500;
            resp.body = "Erreur ecriture fichier configuration sway";
        }
        else
        {
            CROW_LOG_DEBUG << "Ecriture configuration sway OK";
            System::exec(std::string("swaymsg output HDMI-A-1 mode " + resolution + "@" + fps + "Hz").c_str());
            System::exec(std::string("swaymsg output HEADLESS-1 mode " + resolution + "@" + fps + "Hz").c_str());
        }

        if (janus == "on")
        {
            CROW_LOG_DEBUG << "Enabling Janus";
            System::exec("systemctl enable janus");
            System::exec("systemctl restart janus");
        }
        else
        {
            CROW_LOG_DEBUG << "Disabling Janus";
            System::exec("systemctl disable janus");
            System::exec("systemctl stop janus");
        }
        if (samba == "on")
        {
            CROW_LOG_DEBUG << "Enabling Samba";
            System::exec("systemctl enable samba");
            System::exec("systemctl restart samba");
        }
        else
        {
            CROW_LOG_DEBUG << "Disabling Samba";
            System::exec("systemctl disable samba");
            System::exec("systemctl stop samba");
        }
        return resp;
    }
};
#endif
