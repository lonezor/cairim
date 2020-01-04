/*
 *  Cairim
 *  Copyright (C) 2020 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#include "timer.hpp"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/timerfd.h>


//---------------------------------------------------------------------------------------------------------------------------

timer::timer()
{
    epoll_setup();
}

//---------------------------------------------------------------------------------------------------------------------------

uint64_t
timer::register_one_shot_timer(std::chrono::milliseconds duration)
{
    int timer_fd;
    struct itimerspec ts;
    uint64_t res;

    timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer_fd < 0) {
    }

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = duration.count() / 1000;
    ts.it_value.tv_nsec = (duration.count() % 1000) * 1000000;

    res = timerfd_settime(timer_fd, 0, &ts, NULL);
    if (res < 0) {
    }

    epoll_add(timer_fd);

    timer_id_++;
    timer_map_[timer_fd] = timer_id_;
    return timer_id_;
}

//---------------------------------------------------------------------------------------------------------------------------

std::shared_ptr<timer_event>
timer::timerfd_handle_event(int fd)
{
    char buffer[4096];
    ssize_t len = read(fd, buffer, sizeof(buffer));

    if (len <= 0) {
        // throw exception
    }

    auto it = timer_map_.find(fd);
    if (it != timer_map_.end()) {
        uint64_t id = timer_map_[fd];
        return std::shared_ptr<timer_event>(new timer_event(id));
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------

std::vector<std::shared_ptr<timer_event>>
timer::wait_for_events()
{
    std::vector<std::shared_ptr<timer_event>> events;
    struct epoll_event epoll_events[100];

    while (events.size() == 0) {
        int res = epoll_wait(epoll_fd_, epoll_events_, 100, -1);
        if (res > 0) {
            // Go through epoll events
            for (int i = 0; i < res; i++) {
                // Classify type of event and call handle function
                int fd = epoll_events_[i].data.fd;
                auto timer_event = timerfd_handle_event(fd);
                events.push_back(timer_event);
                close(fd);
            }
        } else if (res < 0) {
            printf("epoll error\n");
        } else {
            perror("Timed Out");
        }
    }
    return events;
}

//---------------------------------------------------------------------------------------------------------------------------

void
timer::epoll_setup()
{
    memset(&epoll_events_, 0, sizeof(epoll_events_));
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
}

//---------------------------------------------------------------------------------------------------------------------------

void
timer::epoll_add(int fd)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int res = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    if (res < 0) {
    }
}

//---------------------------------------------------------------------------------------------------------------------------
