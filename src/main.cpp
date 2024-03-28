#include "data_enclosed_mutex.hpp"
#include <cstdio>
#include <memory>
#include <ranges>
#include <thread>
#include <vector>

int main() {
    auto counter = std::make_shared<Mutex<int>>(0);
    auto threads = std::vector<std::thread>{};

    for ([[maybe_unused]] int i : std::ranges::iota_view(0, 10)) {
        threads.emplace_back([counter] {
            auto guard = counter->lock();

            (*guard)++;
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    std::printf("Counter: %d\n", *counter->lock());

    {
        Mutex<int> dataMutex(0);

        {
            auto guard = dataMutex.lock();

            (*guard) = 42;
        }

        printf("Data: %d\n", *dataMutex.lock());
    }

    return 0;
}
