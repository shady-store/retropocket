#ifndef UPDATE_HPP
#define UPDATE_HPP
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

  public:
    static bool needUpdate()
    {
        httplib::Client client("http://localhost:8080");

        client.set_connection_timeout(std::chrono::seconds(5));

        auto res = client.Get("/version/latest");

        if (res->status == 200)
        {
            CROW_LOG_DEBUG << res->body;
            int lastver = std::stoi(res->body);
            return lastver > VERSION;
        }
        else
        {
            CROW_LOG_DEBUG << "Error http";
            return false;
        }
    }
    static bool downloadUpdateAndExtract()
    {
        try
        {
            httplib::Client client("http://localhost:8080");

            client.set_connection_timeout(std::chrono::seconds(5));

            std::ostringstream oss;
            auto res = client.Get("/package/retropocket.zip", [&](const char *data, size_t data_length) {
                oss.write(data, data_length);
                return true;
            });

            if (!res)
            {
                return false;
            }

            // Crée un zip_file à partir du contenu mémoire
            std::string zip_data = oss.str();
            std::istringstream iss(zip_data);
            miniz_cpp::zip_file zip(iss);

            const std::string output_dir = "/tmp";

            // Décompresse tous les fichiers
            for (const auto &entry : zip.namelist())
            {
                fs::path out_path = fs::path(output_dir) / entry;

                if (entry.back() == '/')
                {
                    fs::create_directories(out_path);
                }
                else
                {
                    fs::create_directories(out_path.parent_path());
                    auto file_data = zip.read(entry);
                    std::ofstream ofs(out_path, std::ios::binary);
                    ofs.write(file_data.data(), file_data.size());
                }
            }
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }
};
#endif
