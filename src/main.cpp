

#include "event_loop.hpp"
#include "cairo_xlib_window.hpp"
#include "cairo_image_surface.hpp"
#include "rendering_context.hpp"
#include "scene/scene.hpp"

#define CAPTURE_WIDTH  (720)
#define CAPTURE_HEIGHT  (405)

#define REPLAY_WIDTH (3840)
#define REPLAY_HEIGHT (2160)

int main(int argc, char* argv[])
{
    int width = CAPTURE_WIDTH;
    int height = CAPTURE_HEIGHT;
    int ref_width = CAPTURE_WIDTH;
    int ref_height = CAPTURE_HEIGHT;
    int x_pos = 200;
    int y_pos = 200;
    int target_framerate = 60;
    int input_device_poll_rate = 65;

    auto window = std::shared_ptr<cairo_xlib_window>(new cairo_xlib_window(width, height, x_pos, y_pos, "Water Mill Animation", false));

    auto rc = std::shared_ptr<rendering_context>(new rendering_context(window->get_surface(), window->get_context(), width, height, ref_width, ref_height, anti_aliasing::fast));

    auto wma_capture_scene = std::shared_ptr<scene>(new scene(width, height, rc, 100));

    auto eloop = event_loop(target_framerate, input_device_poll_rate, window, rc, wma_capture_scene);
    auto capture_vector = eloop.run();
    window->close();

    if (!capture_vector.empty()) {
        width = REPLAY_WIDTH;
        height = REPLAY_HEIGHT;
        auto image = cairo_image_surface(width, height);
        auto rc2 = std::shared_ptr<rendering_context>(new rendering_context(image.get_surface(), image.get_context(), width, height, ref_width, ref_height, anti_aliasing::best));
        auto wma_replay_scene = std::shared_ptr<scene>(new scene(CAPTURE_WIDTH, CAPTURE_HEIGHT, rc2, 500000));
        int i = 0;
        for (auto&& frame : capture_vector) {
            char path[1024];
            snprintf(path, sizeof(path), "/home/png/out_%06d.png", i++);
            printf("Generating %s\n", path);
            wma_replay_scene->draw(frame);
            image.write_png(std::string(path));

        }
    }

    return 0;
}
