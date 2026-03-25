#pragma once

#include "../exec/exec.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdio>

namespace sombrero::backends::wpctl {

static const std::string DEFAULT = "@DEFAULT_AUDIO_SINK@";

inline int parseVolume(const std::string& out) {
    std::istringstream iss(out);
    std::string label;
    float volume = 0.0f;
    iss >> label >> volume;
    return static_cast<int>(volume * 100);
}

inline int volume() {
    return parseVolume(exec("wpctl get-volume " + DEFAULT));
}

inline int volume(int sink) {
    return parseVolume(exec("wpctl get-volume " + std::to_string(sink)));
}

inline void setVolume(int v) {
    v = std::clamp(v, 0, 100);
    exec("wpctl set-volume " + DEFAULT + " " + std::to_string(v) + "%");
}

inline void setVolume(int sink, int v) {
    v = std::clamp(v, 0, 100);
    exec("wpctl set-volume " + std::to_string(sink) + " " + std::to_string(v) + "%");
}

inline void setMute(bool state) {
    exec(std::string("wpctl set-mute ") + DEFAULT + (state ? " 1" : " 0"));
}

inline bool muted() {
    return exec("wpctl get-volume " + DEFAULT).find("MUTED") != std::string::npos;
}

inline void toggleMute() {
    exec("wpctl set-mute " + DEFAULT + " toggle");
}

inline void increase(int step) {
    exec("wpctl set-volume " + DEFAULT + " " + std::to_string(step) + "%+");
}

inline void decrease(int step) {
    exec("wpctl set-volume " + DEFAULT + " " + std::to_string(step) + "%-");
}

struct Sink {
    int id;
    std::string name;
    bool is_default;
};

inline std::vector<Sink> sinks() {
    std::string out = exec("wpctl status");

    std::vector<Sink> result;
    std::istringstream iss(out);
    std::string line;

    bool inSinks = false;

    while (std::getline(iss, line)) {
        if (line.find("Sinks:") != std::string::npos) {
            inSinks = true;
            continue;
        }

        if (!inSinks) continue;

        if (line.find_first_not_of(" \t") == std::string::npos)
            break;

        Sink s{};
        s.is_default = (line.find('*') != std::string::npos);

        if (sscanf(line.c_str(), " %d.", &s.id) != 1)
            continue;

        auto pos = line.find('.');
        if (pos != std::string::npos) {
            s.name = line.substr(pos + 1);
            s.name.erase(0, s.name.find_first_not_of(" \t"));
        }

        result.push_back(s);
    }

    return result;
}

inline int defaultSink() {
    auto list = sinks();
    for (const auto& s : list) {
        if (s.is_default)
            return s.id;
    }
    return -1;
}

inline void setDefaultSink(int sink) {
    exec("wpctl set-default " + std::to_string(sink));
}

}