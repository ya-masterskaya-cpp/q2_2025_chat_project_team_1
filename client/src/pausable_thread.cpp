#include "pausable_thread.h"

namespace domain {

void PausableThread::Start() {
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&PausableThread::Run, this);
}

void PausableThread::SetTask(Task task) {
    task_ = task;
}

// Пауза
void PausableThread::Pause() {
    paused_ = true;
}

// Продолжение
void PausableThread::Resume() {
    paused_ = false;
    cv_.notify_one();
}

// Остановка
void PausableThread::Stop() {
    running_ = false;
    Resume();  // пробуждаем, если в паузе
    if (thread_.joinable())
        thread_.join();
}

void PausableThread::Run() {
    while (running_) {
        std::unique_lock<std::mutex> lock(mtx_);
        //pause
        cv_.wait(lock, [&]() { return !paused_ || !running_; });

        if (!running_) break;
        lock.unlock();

        if (task_) task_();
    }
}

} //namespace domain
