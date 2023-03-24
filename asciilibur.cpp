#include "asciilibur.hpp"

#include <windows.h>
#include <iostream>


asciilibur::FrameBuffer::FrameBuffer(uint8_t width, uint8_t height)
: width(width), height(height), buffer(nullptr), camera()
{
    buffer = reinterpret_cast<uint8_t*>(malloc(width * height));

    clear_buffer();
    hide_cursor();
}

asciilibur::FrameBuffer::~FrameBuffer() {
    free(buffer);
}

void asciilibur::FrameBuffer::draw(uint8_t chr, Position pos) {
    pos.x = pos.x - camera.x;
    pos.y = pos.y - camera.y;

    if (pos.x >= width || pos.y >= height) {
        return;
    }

    buffer[pos.y * width + pos.x] = chr;
}

void asciilibur::FrameBuffer::draw(Char chr, Position pos) {
    draw(static_cast<uint8_t>(chr), pos);
}

void asciilibur::FrameBuffer::draw(uint8_t chr, uint16_t x, uint16_t y) {
    draw(chr, Position(x, y));
}

void asciilibur::FrameBuffer::draw(Char chr, uint16_t x, uint16_t y) {
    draw(static_cast<uint8_t>(chr), Position(x, y));
}

void asciilibur::FrameBuffer::set_camera_pos(uint16_t x, uint16_t y) {
    camera.x = x;
    camera.y = y;
}

void asciilibur::FrameBuffer::set_camera_pos(Position pos) {
    camera = std::move(pos);
}

void asciilibur::FrameBuffer::clear_buffer() {
    memset(buffer, Char::WHITESPACE, width * height);
}

void asciilibur::FrameBuffer::render_buffer() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD console_pos;
    console_pos.X = 0;
    console_pos.Y = 0;

    for (uint8_t y = 0; y < height; ++y) {
        for (uint8_t x = 0; x < width; ++x) {
            std::cout << buffer[y * width + x];
        }
        console_pos.X = 0;
        console_pos.Y = y;
        SetConsoleCursorPosition(stdout_handle, console_pos);
    }
}

void asciilibur::FrameBuffer::hide_cursor() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(stdout_handle, &info);
    info.bVisible = false;
    SetConsoleCursorInfo(stdout_handle, &info);
}
