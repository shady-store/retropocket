#ifndef BLUETOOTHDEVICE_HPP
#define BLUETOOTHDEVICE_HPP
#include <string>
struct BluetoothDevice
{
    std::string name, mac;
    bool knownDevice, connectedDevice;
};
#endif
