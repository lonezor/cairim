#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <sys/epoll.h>

constexpr int epoll_max_events = 200;

class timer_event
{
  public:
    timer_event(uint64_t id)
     : id_(id)
     {}

    uint64_t get_id()
    {
        return id_;
    }

  private:
    uint64_t id_;
};

class timer
{
  public:
    timer();

    uint64_t register_one_shot_timer(std::chrono::milliseconds duration);

    std::vector<std::shared_ptr<timer_event>> wait_for_events();

  private:
    void epoll_setup();
    void epoll_add(int fd);
    void epoll_teardown();
    void timerfd_set_timer();
    void timerfd_teardown();

    std::shared_ptr<timer_event> timerfd_handle_event(int fd);

    int epoll_fd_;
    struct epoll_event epoll_events_[epoll_max_events];
    std::unordered_map<int, uint64_t> timer_map_; // key: timer fd --> value: timer id
    uint64_t timer_id_{0};
};

