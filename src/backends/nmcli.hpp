#pragma once

#include "../../exec/exec.hpp"

#include <string>
#include <sstream>
#include <vector>

namespace sombrero::backends::nmcli {

inline bool connected() {
    std::string out = exec("nmcli -t -f STATE networking");
    return out.find("connected") != std::string::npos;
}

inline std::string type() {
    std::string out = exec("nmcli -t -f TYPE,STATE device status");

    std::istringstream iss(out);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.find(":connected") != std::string::npos) {
            return line.substr(0, line.find(':'));
        }
    }

    return "unknown";
}

inline std::string ssid() {
    std::string out = exec("nmcli -t -f ACTIVE,SSID dev wifi");

    std::istringstream iss(out);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.rfind("yes:", 0) == 0) {
            return line.substr(4);
        }
    }

    return "";
}

inline int strength() {
    std::string out = exec("nmcli -t -f ACTIVE,SIGNAL dev wifi");

    std::istringstream iss(out);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.rfind("yes:", 0) == 0) {
            return std::stoi(line.substr(4));
        }
    }

    return -1;
}

inline bool wifiEnabled() {
    std::string out = exec("nmcli radio wifi");
    return out.find("enabled") != std::string::npos;
}

inline void setWifi(bool state) {
    exec(std::string("nmcli radio wifi ") + (state ? "on" : "off"));
}

inline void connect(const std::string& ssid, const std::string& password = "") {
    if (password.empty())
        exec("nmcli dev wifi connect \"" + ssid + "\"");
    else
        exec("nmcli dev wifi connect \"" + ssid + "\" password \"" + password + "\"");
}

inline void disconnect() {
    exec("nmcli networking off");
    exec("nmcli networking on");
}

struct Network {
    std::string ssid;
    int signal;
    bool secure;
};

inline std::vector<Network> scan() {
    std::string out = exec("nmcli -t -f SSID,SIGNAL,SECURITY dev wifi list");

    std::vector<Network> result;
    std::istringstream iss(out);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream l(line);

        std::string ssid, signal, security;

        std::getline(l, ssid, ':');
        std::getline(l, signal, ':');
        std::getline(l, security, ':');

        if (ssid.empty()) continue;

        Network n;
        n.ssid = ssid;
        n.signal = std::stoi(signal);
        n.secure = !security.empty();

        result.push_back(n);
    }

    return result;
}

}