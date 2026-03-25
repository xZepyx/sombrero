#pragma once

#include "../backends/wpctl.hpp"

namespace sombrero::audio {

inline int volume() {
    return backends::wpctl::volume();
}

inline int volume(int sink) {
    return backends::wpctl::volume(sink);
}

inline void setVolume(int v) {
    backends::wpctl::setVolume(v);
}

inline void setVolume(int sink, int v) {
    backends::wpctl::setVolume(sink, v);
}

inline void setMute(bool state) {
    backends::wpctl::setMute(state);
}

inline bool muted() {
    return backends::wpctl::muted();
}

inline void toggleMute() {
    backends::wpctl::toggleMute();
}

inline void increase(int step = 5) {
    backends::wpctl::increase(step);
}

inline void decrease(int step = 5) {
    backends::wpctl::decrease(step);
}

using Sink = backends::wpctl::Sink;

inline std::vector<Sink> sinks() {
    return backends::wpctl::sinks();
}

inline int defaultSink() {
    return backends::wpctl::defaultSink();
}

inline void setDefaultSink(int sink) {
    backends::wpctl::setDefaultSink(sink);
}

}