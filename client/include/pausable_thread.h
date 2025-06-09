#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class PausableThread {
public:
    using Task = std::function<void()>;

    PausableThread() = default;
    PausableThread(Task task) : task_(task) {}

    void Start();
    void SetTask(Task task);
    void Pause();
    void Resume();
    void Stop();

    ~PausableThread() {
        Stop();
    }

private:
    std::thread thread_;
    Task task_;
    std::atomic<bool> running_ = false;
    std::atomic<bool> paused_ = false;
    std::mutex mtx_;
    std::condition_variable cv_;

    void Run();
};
