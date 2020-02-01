#include "cairim.hpp"

int cairim_main(int argc, char* argv[], std::shared_ptr<cairo_xlib_window> window, 
    std::shared_ptr<scene> capture_scene, std::shared_ptr<scene> replay_scene,
    bool show_cursor)
{
    // TODO: parse argv[]

    auto cap_handler = capture_handler(capture_scene, window, capture_scene->get_rc());

    // Capture user input until ESC key is pressed
    auto frame_ctx_vector = cap_handler.run();

    // Replay user input and render high resolution output
    auto rep_handler = replay_handler(replay_scene, show_cursor);
    rep_handler.run(frame_ctx_vector);

    return 0;
}