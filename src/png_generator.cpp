
#include <png.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include "png_generator.hpp"

cairo_surface::cairo_surface(uint8_t* data, size_t buffer_size, int width,
                             int height, int stride, cairo_format_t format)
    : buffer_size_(buffer_size)
    , width_(width)
    , height_(height)
    , stride_(stride)
    , format_(format)
{
    buffer_.resize(buffer_size_);
    memcpy(buffer_.data(), data, buffer_size_);
}


png_generator::png_generator()
{

}

png_generator::png_generator(size_t concurrency, std::string output_dir, png_compression_level level)
    : concurrency_(concurrency)
    , output_dir_(output_dir)
    , level_(level)
{
    for(size_t i=0; i < concurrency; i++) {
        threads_.emplace_back(std::unique_ptr<std::thread>(new std::thread(&png_generator::worker_main, this, i)));
        auto last_idx = threads_.size() - 1;
        threads_[last_idx]->detach();
    }
}

png_generator::~png_generator()
{
    worker_exit_ = true;
}

size_t png_generator::get_queue_size()
{
    mutex_.lock();
    auto size = work_queue_.size();
    mutex_.unlock();
    return size;
}

size_t png_generator::get_concurrency()
{
    return concurrency_;
}

void png_generator::append_work_queue(cairo_surface_t* surface)
{
    uint8_t* buffer = cairo_image_surface_get_data(surface);

    int width = cairo_image_surface_get_width(surface);
    int height = cairo_image_surface_get_height(surface);
    int stride = cairo_image_surface_get_stride(surface);
    cairo_format_t format = cairo_image_surface_get_format(surface);
    size_t buffer_size = height * stride;

    auto s = std::shared_ptr<cairo_surface>(new cairo_surface(buffer,
                                                              buffer_size,
                                                              width,
                                                              height,
                                                              stride,
                                                              format));
    mutex_.lock();
    work_queue_.push(s);
    mutex_.unlock();

    
}

void png_generator::worker_main(int tid)
{
    std::shared_ptr<cairo_surface> surface = nullptr;

    while(!worker_exit_) {
        mutex_.lock();
        if (work_queue_.size() == 0) {
            mutex_.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        
        surface = work_queue_.front();
        work_queue_.pop();
        mutex_.unlock();

        std::stringstream path;
        path << output_dir_ << "/out_";
        path.fill('0');
        path.width(6);
        path << file_idx_++;
        path << ".png";
        std::cout << "Generating " << path.str() << std::endl;
        FILE* png_file = png_file = fopen(path.str().c_str(), "wb");
        if (!png_file) {
            // throw exception since we cannot continue
        }
        // PNG code...
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info = png_create_info_struct(png);

        int depth = 8;
        png_set_IHDR(png,
                     info,
                     surface->width_,
                     surface->height_,
                     depth,
                     PNG_COLOR_TYPE_RGB_ALPHA,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        png_set_compression_level(png, level_);

        png_byte** rows = NULL;
        rows = static_cast<png_byte**>(png_malloc(png, surface->height_ * sizeof(png_byte*)));
        
        uint8_t* row = surface->buffer_.data();
        for (int y = 0; y < surface->height_; y++) {
            rows[y] = row;
            row += surface->stride_;
        }

        png_init_io(png, png_file);
        png_set_rows(png, info, rows);
        png_write_png(png, info, PNG_TRANSFORM_BGR, NULL);
        png_free(png, rows);
        
        png_destroy_write_struct(&png, &info);
        fclose(png_file);
        
    }

#if 0
    while(!worker_exit_) {
        mutex_.lock();
        mutex_.unlock();

        if (!surface) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        continue;

        std::stringstream path;
        path << output_dir_ << "/out_" << file_idx_++ << ".png";

        FILE* png_file = png_file = fopen(path.str().c_str(), "wb");
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info = png_create_info_struct(png);

        //if (setjmp(png_jmpbuf(png))) {
        //    goto exit;
       // }

        // To get max performance we keep alpha channel
        // and let the PNG library deal with Cairo's
        // internal BGRA representation.
        // NOTE: even CAIRO_FORMAT_RGB24 use 4 channels
        int depth = 8;
        png_set_IHDR(png,
                     info,
                     surface->width_,
                     surface->height_,
                     depth,
                     PNG_COLOR_TYPE_RGB_ALPHA,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        png_set_compression_level(png, level_);

        png_byte** rows = NULL;
        rows = static_cast<png_byte**>(png_malloc(png, surface->height_ * sizeof(png_byte*)));

        uint8_t* row = surface->buffer_.data();
        for (int y = 0; y < surface->height_; y++) {
            rows[y] = row;
            row += surface->stride_;
        }

        png_init_io(png, png_file);
        png_set_rows(png, info, rows);
        png_write_png(png, info, PNG_TRANSFORM_BGR, NULL);
        png_free(png, rows);

    //exit:
        png_destroy_write_struct(&png, &info);
        fclose(png_file);
    }
    #endif
}
