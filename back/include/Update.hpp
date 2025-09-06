#ifndef UPDATE_HPP
#define UPDATE_HPP
#include <sstream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
#include "miniz-cpp/zip_file.hpp"
#include <chrono>
#include <crow/logging.h>
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
class Update
{
  private:
    constexpr static int VERSION = 1;

    inline static std::string CHECK_UPDATE_ENDPOINT = "https://raw.githubusercontent.com";
    inline static std::string CHECK_UPDATE_ADDR = "/shady-store/retropocket/refs/heads/main/version";
    inline static std::string UPDATE_ENDPOINT = "https://github.com";
    inline static std::string UPDATE_ADDR = "/shady-store/retropocket/releases/download/latest/retropocket.zip";

  public:
    static bool needUpdate()
    {
        CROW_LOG_DEBUG << "Checking for update : " << CHECK_UPDATE_ENDPOINT << CHECK_UPDATE_ADDR;
        httplib::Client client(CHECK_UPDATE_ENDPOINT);

        client.set_connection_timeout(std::chrono::seconds(5));

        auto res = client.Get(CHECK_UPDATE_ADDR);

        if (res != nullptr && res->status == 200)
        {
            CROW_LOG_DEBUG << "Last version : " << res->body;
            int lastver = std::stoi(res->body);
            return lastver > VERSION;
        }
        else
        {
            CROW_LOG_ERROR << "Error http";
            return false;
        }
    }
    static bool downloadUpdateAndExtract()
    {
        try
        {
            CROW_LOG_DEBUG << "Downloading update file from : " << UPDATE_ENDPOINT << UPDATE_ADDR;

            httplib::Client client(UPDATE_ENDPOINT);

            client.set_connection_timeout(std::chrono::seconds(5));

            client.set_default_headers(
                {{"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:142.0) Gecko/20100101 Firefox/142.0"},
                 {"Accept", "application/octet-stream"},
                 {"Accept-Encoding", "gzip, deflate, br, zstd"},
                 {"Connection", "keep-alive"}});

            client.set_ca_cert_path("/etc/ssl/certs/ca-certificates.crt");

            client.set_read_timeout(60);
            client.set_write_timeout(60);
            client.set_follow_location(true);

            std::ostringstream oss;

            auto res = client.Get(UPDATE_ADDR);

            if (res != nullptr && res->status == 200)
            {
                oss << res->body;

                CROW_LOG_DEBUG << "Download completed";

                // Crée un zip_file à partir du contenu mémoire
                std::string zip_data = oss.str();
                std::istringstream iss(zip_data);
                CROW_LOG_DEBUG << "Creating zip file from memory";
                miniz_cpp::zip_file zip(iss);

                const std::string output_dir = "/tmp";

                // Décompresse tous les fichiers
                for (const auto &entry : zip.namelist())
                {
                    fs::path out_path = fs::path(output_dir) / entry;

                    if (entry.back() == '/')
                    {
                        CROW_LOG_DEBUG << "creating directory : " << out_path;
                        fs::create_directories(out_path);
                    }
                    else
                    {
                        CROW_LOG_DEBUG << "creating directory : " << out_path.parent_path();
                        fs::create_directories(out_path.parent_path());
                        auto file_data = zip.read(entry);
                        std::ofstream ofs(out_path, std::ios::binary);
                        ofs.write(file_data.data(), file_data.size());
                        CROW_LOG_DEBUG << "Unzipping file : " << entry;
                    }
                }

                CROW_LOG_DEBUG << "End update, please restart the device";
                return true;
            }
            else
            {
                CROW_LOG_ERROR << "Error downloading file";
                return false;
            }
        }
        catch (std::exception e)
        {
            CROW_LOG_ERROR << "Exception occured while decompressing package : " << e.what();
            return false;
        }
    }
};
#endif
