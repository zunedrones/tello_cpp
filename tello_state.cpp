#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "tello.h"

using tello::Tello;

void ShowStatus(const std::string& state) {
    int ret = system("clear");
    if (ret == -1) {
        std::cerr << "Error clearing the screen" << "\n";
        return;
    }

    int begin{0};
    std::cout << "+-----------+-----------+" << "\n";
    const int padding = 10;
    while (begin < state.size()) {
        const auto split{state.find(':', begin)};
        const auto name{state.substr(begin, split - begin)};
        const auto end{state.find(';', split)};
        const auto value{state.substr(split + 1, end - split - 1)};
        begin = end + 1;

        std::cout << "|  " << name;
        std::cout << std::setw(padding - name.size()) << "|";
        std::cout << "  " << value;
        std::cout << std::setw(padding - value.size()) << "|";
        std::cout << "\n";
    }
    std::cout << "+-----------+-----------+" << "\n";
}

int main() {
    Tello tello{};
    if (!tello.Bind()) {
        return 0;
    }

    while (true) {
        if (const auto state = tello.GetState()) {
            ShowStatus(*state);
        }
    }
}
