#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP
#include "BluetoothDevice.hpp"
#include "System.hpp"
#include "Tools.hpp"
#include "crow/logging.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>
class Bluetooth
{
  public:
    static std::vector<BluetoothDevice> getKnownDevices()
    {
        auto knownDevices = getDevices(false);
        std::vector<BluetoothDevice> returnValue;
        returnValue.reserve(knownDevices.size());
        std::transform(knownDevices.begin(), knownDevices.end(),                             // plage d'entrée
                       std::back_inserter(returnValue),                                      // où mettre les résultats
                       [](const std::pair<std::string, std::string> &a) -> BluetoothDevice { // fonction de conversion
                           return {std::get<1>(a), std::get<0>(a), true};
                       });
        return returnValue;
    }

    static std::vector<BluetoothDevice> getAllDevices()
    {
        std::vector<BluetoothDevice> returnValue;

        auto knownDevices = getDevices(false);
        auto connectedDevices = getDevices(false, "Connected");

        auto allDevices = getDevices(true);
        returnValue.reserve(allDevices.size());

        std::transform(allDevices.begin(), allDevices.end(), // plage d'entrée
                       std::back_inserter(returnValue),      // où mettre les résultats
                       [&knownDevices, &connectedDevices](
                           const std::pair<std::string, std::string> &a) -> BluetoothDevice { // fonction de conversion
                           return {std::get<1>(a), std::get<0>(a), Tools::vectorContains(knownDevices, a),
                                   Tools::vectorContains(connectedDevices, a)};
                       });
        return returnValue;
    }
    static bool addDevice(const std::string &mac)
    {
        bool returnValue = false;
        std::string command = R"-( 
                      (
                      echo power on
                      echo agent on
                      echo default-agent
                      echo pair )-" +
                              mac + R"-(
                      sleep 2
                      echo trust )-" +
                              mac + R"-(
                      sleep 1
                      echo quit
                      ) | bluetoothctl)-";
        std::string output = System::exec(command.c_str());
        if (output.find("Failed to pair") != std::string::npos)
        {
            CROW_LOG_DEBUG << "❌ Appairage échoué.";
        }
        else if (output.find("Pairing successful") != std::string::npos)
        {
            CROW_LOG_DEBUG << "✅ Appairage réussi.";
            returnValue = true;
        }

        return returnValue;
    }
    static bool disconnectDevice(const std::string &mac)
    {
        bool returnValue = false;
        std::string command = R"-( 
                    (
                    echo disconnect )-" +
                              mac + R"-(
                    sleep 1
                    echo quit
                    ) | bluetoothctl)-";
        std::string output = System::exec(command.c_str());
        if (output.find("Disconnection successful") != std::string::npos)
        {
            CROW_LOG_DEBUG << "✅ Déconnexion réussie.";
            returnValue = true;
        }
        else if (output.find("not available") != std::string::npos)
        {
            CROW_LOG_DEBUG << "⚠️ Le périphérique semble indisponible ou déjà déconnecté.";
        }
        else
        {
            CROW_LOG_DEBUG << "❓ Résultat incertain, voici l'output :\n" << output;
        }
        return returnValue;
    }
    static bool removeDevice(const std::string &mac)
    {
        bool returnValue = false;
        std::string command = R"-( 
                    (
                    echo remove )-" +
                              mac + R"-(
                    sleep 1
                    echo quit
                    ) | bluetoothctl)-";
        std::string output = System::exec(command.c_str());
        if (output.find("Device has been removed") != std::string::npos)
        {
            CROW_LOG_DEBUG << "✅ Déconnexion réussie.";
            returnValue = true;
        }
        else if (output.find("not available") != std::string::npos)
        {
            CROW_LOG_DEBUG << "⚠️ Le périphérique semble indisponible ou déjà déconnecté.";
        }
        else
        {
            CROW_LOG_DEBUG << "❓ Résultat incertain, voici l'output :\n" << output;
        }
        return returnValue;
    }
    static bool connectDevice(const std::string &mac)
    {
        bool returnValue = false;
        std::string command = R"-( 
                      (
                      echo connect )-" +
                              mac + R"-(
                      sleep 2
                      echo quit
                      ) | bluetoothctl)-";
        std::string output = System::exec(command.c_str());

        if (output.find("Connection successful") != std::string::npos)
        {
            CROW_LOG_DEBUG << "✅ Connexion réussie.";
            returnValue = true;
        }
        else if (output.find("Failed to connect") != std::string::npos)
        {
            CROW_LOG_DEBUG << "❌ Connexion échouée.";
        }

        if (output.find("AuthenticationFailed") != std::string::npos)
        {
            CROW_LOG_DEBUG << "⚠️ Authentification échouée (peut-être un mauvais PIN ?).";
        }
        return returnValue;
    }
    static bool disconnectAndRemoveDevice(const std::string &mac)
    {
        bool returnValue = false;
        std::string command = R"-( 
                    (
                    echo disconnect )-" +
                              mac + R"-(
                    sleep 1
                    echo remove )-" +
                              mac + R"-(
                    sleep 1
                    echo quit
                    ) | bluetoothctl)-";
        std::string output = System::exec(command.c_str());
        if (output.find("Successful disconnected") != std::string::npos)
        {
            CROW_LOG_DEBUG << "✅ Déconnexion réussie.";
            returnValue = true;
        }
        else if (output.find("not available") != std::string::npos)
        {
            CROW_LOG_DEBUG << "⚠️ Le périphérique semble indisponible ou déjà déconnecté.";
        }
        else
        {
            CROW_LOG_DEBUG << "❓ Résultat incertain, voici l'output :\n" << output;
        }
        return returnValue;
    }

  private:
    static bool isMacLikeName(const std::string &name, const std::string &mac)
    {
        std::string formatted_mac = std::regex_replace(mac, std::regex(":"), "-");
        return name == formatted_mac;
    }
    static std::vector<std::pair<std::string, std::string>> getDevices(bool scan, const std::string &status = "")
    {
        std::string cmd;
        if (scan)
        {
            cmd = R"-((
                              echo scan on
                              sleep 10
                              echo devices
                              echo quit
                              ) | bluetoothctl)-";
        }
        else
        {
            cmd = R"-((
                              echo devices )-" +
                  status +
                  R"-(
                              echo quit
                              ) | bluetoothctl)-";
        }

        CROW_LOG_DEBUG << "Get Device with Command : " << cmd;

        std::string devices = System::exec(cmd.c_str());

        std::vector<std::pair<std::string, std::string>> returnValue;

        std::regex ansi_regex("\x1B\\[[0-9;]*[A-Za-z]");
        std::string cleaned = std::regex_replace(devices, ansi_regex, "");
        std::regex device_regex(R"(^Device\s+([0-9A-F:]{17})\s+([^\n]+))", std::regex::icase);
        std::smatch match;

        std::istringstream iss(cleaned);
        std::string line;

        while (std::getline(iss, line))
        {
            // On ignore les lignes de type [CHG], [DEL], etc.
            if (std::regex_search(line, std::regex(R"(\[(CHG|DEL|NEW)\])")))
                continue;
            if (line.find("RSSI") != std::string::npos || line.find("ManufacturerData") != std::string::npos)
                continue;

            if (std::regex_match(line, match, device_regex))
            {
                std::string mac = match[1];
                std::string name = match[2];

                if (isMacLikeName(name, mac))
                    continue;

                CROW_LOG_DEBUG << "MAC: " << mac << " | Nom: " << name;
                returnValue.push_back({mac, name});
            }
        }
        return returnValue;
    }
};

#endif // !DEBUG
