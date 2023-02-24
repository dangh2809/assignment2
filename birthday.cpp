#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#define N_GUESTS 10



int curCount = 0;
bool isCakeAvailable = 0;
std::array<bool, N_GUESTS> cakeEatens;
std::mutex mutex; 
int curThread;


unsigned int generateRandomNumber(int min, int max) {
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

void checkCupcake() {
    while (curCount < N_GUESTS) {
        mutex.lock();

        if(curThread == 0)
        {
            if (!isCakeAvailable) {
                curCount++;
                isCakeAvailable = true;
            }

            if (isCakeAvailable && !cakeEatens[0]) {
                curCount++;
                isCakeAvailable = true;
                cakeEatens[0] = true;
            }
        }
        mutex.unlock();
    }
    return;
}
void navigateLabyrinth( int threadIndex) {
    while (curCount < N_GUESTS) {
        mutex.lock();

        if (curThread == threadIndex && isCakeAvailable && !cakeEatens[threadIndex]) {
            isCakeAvailable = false;
            cakeEatens[threadIndex] = true;
            std::cout << "Guest #" << threadIndex << " ate the cupcake!" << std::endl;
        }

        mutex.unlock();
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // int n_guests;
    // cout << "Enter number of guests: ";
    // cin >> n_guests;
    
    std::array<std::thread, N_GUESTS> threads{};  
    
    threads[0] = std::thread(checkCupcake);

    for (size_t i = 1; i < threads.size(); i++) {
        threads[i] = std::thread(navigateLabyrinth,i);
    }

    while (curCount < N_GUESTS) {
        curThread = generateRandomNumber(0, N_GUESTS);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double, std::milli>(stop-start);
    std::cout << "Finished in " << duration.count() << "ms" << std::endl;

}