#include <string>
#include <array>
#include <cstdio>

std::string exec(const std::string& cmd) {
    std::array<char, 256> buffer{};
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";

    while (fgets(buffer.data(), buffer.size(), pipe)) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}