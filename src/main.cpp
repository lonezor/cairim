#include "common.hpp"

#include "capture_handler.hpp"
#include "replay_handler.hpp"

int main(int argc, char* argv[])
{
    auto cap_handler = capture_handler();
    auto rep_handler = replay_handler();

    // Capture user input until 'q' key is pressed
    auto frame_ctx_vector = cap_handler.run();
    
    // Replay user input and render high resolution output
    rep_handler.run(frame_ctx_vector, replay_width_2k, replay_height_2k, "/home/png");

    return 0;
}
