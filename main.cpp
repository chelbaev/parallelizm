#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <functional>
#include <future>
#include <cmath>
#include <random>
#include <fstream>
#include <atomic>

template<typename TaskType, typename ResultType>
class Server {
public:
    Server() : running(false), next_id(0) {}

    void start() {
        running = true;
        server_t = std::thread(process_tasks, this);
    }

    void stop() {
        running = false;
        cv.notify_all();
        if (server_t.joinable()) {
            server_t.join();
        }
    }

    size_t add_task(TaskType task) {
        std::unique_lock<std::mutex> lock(mtx);
        size_t id = next_id++;
        task_queue.push({id, task});
        cv.notify_one();
        return id;
    }

    ResultType request_result(size_t id) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_result.wait(lock, [this, id] { return results.count(id) > 0; });
        return results[id];
    }

private:
    void process_tasks() {
        while (running) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !task_queue.empty() || !running; });
            
            if (!running) break;

            auto [id, task] = task_queue.front();
            task_queue.pop();
            lock.unlock();

            ResultType result = task();

            lock.lock();
            results[id] = result;
            lock.unlock();
            cv_result.notify_all();
        }
    }

    std::thread server_t;
    std::atomic<bool> running;
    std::queue<std::pair<size_t, std::function<ResultType()>>> task_queue;
    std::unordered_map<size_t, ResultType> results;
    std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable cv_result;
    size_t next_id;
};

double compute_sin(double arg) {
    return std::sin(arg);
}

double compute_sqrt(double arg) {
    return std::sqrt(arg);
}

double compute_pow(double base, double exp) {
    return std::pow(base, exp);
}

// Генератор случайных чисел
double random_double(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

void fun_sin(Server<std::function<double()>, double>& server, const std::string& filename, int N) {
    std::ofstream file(filename);
    for (int i = 0; i < N; ++i) {
        double arg = random_double(0, 2 * M_PI);
        size_t id = server.add_task([arg] { return compute_sin(arg); });
        double result = server.request_result(id);
        file << "ID " << id << " - sin(" << arg << ") = " << result << std::endl;
    }
}

void fun_sqrt(Server<std::function<double()>, double>& server, const std::string& filename, int N) {
    std::ofstream file(filename);
    for (int i = 0; i < N; ++i) {
        double arg = random_double(0, 1000);
        size_t id = server.add_task([arg] { return compute_sqrt(arg); });
        double result = server.request_result(id);
        file << "ID " << id << " - sqrt(" << arg << ") = " << result << std::endl;
    }
}

void fun_pow(Server<std::function<double()>, double>& server, const std::string& filename, int N) {
    std::ofstream file(filename);
    for (int i = 0; i < N; ++i) {
        double base = random_double(1, 10);
        double exp = random_double(1, 5);
        size_t id = server.add_task([base, exp] { return compute_pow(base, exp); });
        double result = server.request_result(id);
        file << "ID " << id << " - pow(" << base << ", " << exp << ") = " << result << std::endl;
    }
}

int main() {
    Server<std::function<double()>, double> server;
    server.start();

    const int N = 1000;

    std::thread sin_t(fun_sin, std::ref(server), "sin_results.txt", N);
    std::thread sqrt_t(fun_sqrt, std::ref(server), "sqrt_results.txt", N);
    std::thread pow_t(fun_pow, std::ref(server), "pow_results.txt", N);

    sin_t.join();
    sqrt_t.join();
    pow_t.join();

    server.stop();

    std::cout << "Сервер закончил работу" << std::endl;
    return 0;
}
