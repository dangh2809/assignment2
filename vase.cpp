#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>

#define N_GUESTS 20

enum Status {
    AVAILABLE,
    BUSY
};

// Keeps track of what guests have already seen the vase
std::unordered_set<std::thread::id> visited{};
std::mutex mutex;
Status roomStatus = Status::AVAILABLE;

// Note: the bounds for min and max are both inclusive
unsigned int generateRandomNumber(int min, int max) {
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

void admireVase(int threadIndex) {
    std::thread::id threadId = std::this_thread::get_id();

    while (visited.size() < N_GUESTS) {
        mutex.lock();

        if (roomStatus == Status::AVAILABLE && visited.find(threadId) == visited.end()) {
            roomStatus = Status::BUSY;
            std::cout << "Guest #" << threadIndex << " is admiring the vase" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomNumber(10, 500)));
            roomStatus = Status::AVAILABLE;

            visited.insert(threadId);
        }

        mutex.unlock();
    }
}
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::array<std::thread, N_GUESTS> threads{};

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i] = std::thread(admireVase, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

   
    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}
