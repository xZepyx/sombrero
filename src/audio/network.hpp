#pragma once

#include "backends/nmcli.hpp"
#include <vector>
#include <string>

namespace sombrero::network {

using Network = backends::nmcli::Network;

inline bool connected() {
    return nmcli::connected();
}

inline std::string type() {
    return nmcli::type();
}

// wifi info
inline std::string ssid() {
    return nmcli::ssid();
}

inline int strength() {
    return nmcli::strength();
}

inline bool wifiEnabled() {
    return nmcli::wifiEnabled();
}

inline void setWifi(bool state) {
    nmcli::setWifi(state);
}

inline void connect(const std::string& ssid, const std::string& password = "") {
    nmcli::connect(ssid, password);
}

inline void disconnect() {
    nmcli::disconnect();
}

inline std::vector<Network> scan() {
    return nmcli::scan();
}

}