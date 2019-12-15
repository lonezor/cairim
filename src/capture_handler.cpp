#include "capture_handler.hpp"

#include "event_loop.hpp"
#include "scene.hpp"
#include "common.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <endian.h>
#include <fstream>

capture_handler::capture_handler(std::shared_ptr<scene> scene,
                                 std::shared_ptr<cairo_xlib_window> window,
                                 std::shared_ptr<rendering_context> rc)
 : scene_(scene)
 , window_(window)
 , rc_(rc)
{

}

std::vector<frame_context> capture_handler::run()
{
 
    // Connect everything together in an event loop
    auto eloop = event_loop(target_frame_rate,
                            input_dev_poll_rate,
                            window_,
                            rc_,
                            scene_);

    // Event loop (blocking)
    auto capture_vector = eloop.run();

    // Close window after user has requested to exit
    window_->close();

    return capture_vector;
}

void capture_handler::write_capture_file(std::vector<frame_context>& frame_ctx_vector, std::string path)
{
    auto file = std::fstream(path, std::ios::out | std::ios::binary);

    // Magic
    uint64_t magic = htobe64(capture_file_magic);
    file.write(reinterpret_cast<char*>(&magic), sizeof(magic));

    // Number of entries
    uint64_t nr_entries = frame_ctx_vector.size();
    nr_entries = htobe64(nr_entries);
    file.write(reinterpret_cast<char*>(&nr_entries), sizeof(nr_entries));

    for (auto&& frame_ctx : frame_ctx_vector) {
        // Timestamp
        uint64_t timestamp = static_cast<uint64_t>(frame_ctx.timestamp);
        timestamp = htobe64(timestamp);
        file.write(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));

        // Frame rate
        uint16_t frame_rate = static_cast<uint16_t>(frame_ctx.frame_rate);
        frame_rate = htobe16(frame_rate);
        file.write(reinterpret_cast<char*>(&frame_rate), sizeof(frame_rate));

        // Scene width
        uint16_t scene_width = static_cast<uint16_t>(frame_ctx.scene_width);
        scene_width = htobe16(scene_width);
        file.write(reinterpret_cast<char*>(&scene_width), sizeof(scene_width));

        // Scene height 
        uint16_t scene_height = static_cast<uint16_t>(frame_ctx.scene_height);
        scene_height = htobe16(scene_height);
        file.write(reinterpret_cast<char*>(&scene_height), sizeof(scene_height));

        // Cursor x
        uint16_t cursor_x = static_cast<uint16_t>(frame_ctx.cursor_x);
        cursor_x = htobe16(cursor_x);
        file.write(reinterpret_cast<char*>(&cursor_x), sizeof(cursor_x));

        // Cursory y
        uint16_t cursor_y = static_cast<uint16_t>(frame_ctx.cursor_y);
        cursor_y = htobe16(cursor_y);
        file.write(reinterpret_cast<char*>(&cursor_y), sizeof(cursor_y));

        // Button state
        uint16_t button_state = static_cast<uint16_t>(frame_ctx.button_state);
        button_state = htobe16(button_state);
        file.write(reinterpret_cast<char*>(&button_state), sizeof(button_state));

        // Cursor visible
        uint8_t cursor_visible = frame_ctx.cursor_visible;
        file.write(reinterpret_cast<char*>(&cursor_visible), sizeof(cursor_visible));

        // OSD visible
        uint8_t osd_visible = frame_ctx.osd_visible;
        file.write(reinterpret_cast<char*>(&osd_visible), sizeof(osd_visible));

        // Screen border
        uint8_t screen_border = frame_ctx.screen_border;
        file.write(reinterpret_cast<char*>(&screen_border), sizeof(screen_border));
    }

    file.close();
}

std::vector<frame_context> capture_handler::read_capture_file(std::string path)
{
    std::vector<frame_context> frame_ctx_vector;

    auto file = std::fstream(path, std::ios::in | std::ios::binary);

    // Magic
    uint64_t magic = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    magic = be64toh(magic);
    if (magic != capture_file_magic) {
        std::cerr << "Bad file input: " << path << std::endl;
        return frame_ctx_vector;
    }

    // Number of entries
    uint64_t nr_entries = 0;
    file.read(reinterpret_cast<char*>(&nr_entries), sizeof(nr_entries));
    nr_entries = be64toh(nr_entries);

    for (uint64_t i=0; i < nr_entries; i++) {
        frame_context frame;
        memset(&frame, 0, sizeof(frame));

        // Timestamp
        uint64_t timestamp = 0;
        file.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
        timestamp = be64toh(timestamp);
        frame.timestamp = static_cast<int64_t>(timestamp);

        // Frame rate
        uint16_t frame_rate = 0;
        file.read(reinterpret_cast<char*>(&frame_rate), sizeof(frame_rate));
        frame_rate = be16toh(frame_rate);
        frame.frame_rate = static_cast<double>(frame_rate);

        // Scene width
        uint16_t scene_width = 0;
        file.read(reinterpret_cast<char*>(&scene_width), sizeof(scene_width));
        scene_width = be16toh(scene_width);
        frame.scene_width = static_cast<double>(scene_width);

        // Scene height 
        uint16_t scene_height = 0;
        file.read(reinterpret_cast<char*>(&scene_height), sizeof(scene_height));
        scene_height = be16toh(scene_height);
        frame.scene_height = static_cast<double>(scene_height);

        // Cursor x
        uint16_t cursor_x = 0;
        file.read(reinterpret_cast<char*>(&cursor_x), sizeof(cursor_x));
        cursor_x = be16toh(cursor_x);
        frame.cursor_x = static_cast<double>(cursor_x);

        // Cursory y
        uint16_t cursor_y = 0;
        file.read(reinterpret_cast<char*>(&cursor_y), sizeof(cursor_y));
        cursor_y = be16toh(cursor_y);
        frame.cursor_y = static_cast<double>(cursor_y);

        // Button state
        uint16_t button_state = 0;
        file.read(reinterpret_cast<char*>(&button_state), sizeof(button_state));
        button_state = be16toh(button_state);
        frame.button_state = static_cast<button>(button_state);

        // Cursor visible
        uint8_t cursor_visible = 0;
        file.read(reinterpret_cast<char*>(&cursor_visible), sizeof(cursor_visible));
        frame.cursor_visible = static_cast<bool>(cursor_visible);

        // OSD visible
        uint8_t osd_visible = 0;
        file.read(reinterpret_cast<char*>(&osd_visible), sizeof(osd_visible));
        frame.osd_visible = static_cast<bool>(osd_visible);

        // Screen border
        uint8_t screen_border = 0;
        file.read(reinterpret_cast<char*>(&screen_border), sizeof(screen_border));
        frame.screen_border = static_cast<bool>(screen_border);

        frame_ctx_vector.emplace_back(frame);
    }

    file.close();
}
