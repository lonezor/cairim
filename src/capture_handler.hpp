#pragma once

#include <vector>
#include "frame_context.hpp"
#include "common.hpp"

class capture_handler
{
public:
    capture_handler();

    std::vector<frame_context> run();

    //void write_capture_file(std::string path);
    //std::vector<frame_context> read_capture_file((std::string path);

};