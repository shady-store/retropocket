#ifndef TOOLS_HPP
#define TOOLS_HPP
#include "BluetoothDevice.hpp"
#include <crow/json.h>
struct BluetoothDevice;
class Tools
{
  public:
    static crow::json::wvalue convertToWValue(const BluetoothDevice &btd)
    {
        crow::json::wvalue outValue;
        outValue["mac"] = btd.mac;
        outValue["name"] = btd.name;
        outValue["known"] = btd.knownDevice;
        outValue["connected"] = btd.connectedDevice;
        std::string status = "Inconnue";
        if (btd.connectedDevice)
        {
            outValue["status"] = "Connecte";
            outValue["delete-class"] = "hidden";
            outValue["add-class"] = "hidden";
            outValue["connect-class"] = "hidden";
            outValue["disconnect-class"] = "";
        }
        else if (btd.knownDevice)
        {
            outValue["status"] = "Ajoute";
            outValue["delete-class"] = "";
            outValue["add-class"] = "hidden";
            outValue["connect-class"] = "";
            outValue["disconnect-class"] = "hidden";
        }
        else
        {
            outValue["status"] = "Inconnu";
            outValue["delete-class"] = "hidden";
            outValue["add-class"] = "";
            outValue["connect-class"] = "hidden";
            outValue["disconnect-class"] = "hidden";
        }
        return outValue;
    }
    static crow::json::wvalue convertToWValue(const std::pair<std::string, std::string> &pairOfString)
    {
        crow::json::wvalue outValue;
        outValue["mac"] = std::get<0>(pairOfString);
        outValue["name"] = std::get<1>(pairOfString);
        return outValue;
    }
    template <typename T> static crow::json::wvalue convertListToWValue(const std::vector<T> &list)
    {
        std::vector<crow::json::wvalue> listValue;
        std::for_each(list.begin(), list.end(),
                      [&listValue](const T &value) { listValue.push_back(convertToWValue(value)); });
        return crow::json::wvalue(crow::json::wvalue::list(listValue));
    }
    bool static vectorContains(std::vector<std::pair<std::string, std::string>> v,
                               std::pair<std::string, std::string> elt)
    {
        auto it = std::find_if(v.begin(), v.end(), [&elt](const std::pair<std::string, std::string> &vElt) {
            return std::get<0>(elt) == std::get<0>(vElt);
        });

        return it != v.end();
    }
};
#endif
