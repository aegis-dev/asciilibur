#include "asciilibur.hpp"

#include <windows.h>
#include <iostream>


asciilibur::FrameBuffer::FrameBuffer(uint8_t width, uint8_t height)
: width(width), height(height), buffer(nullptr)
{
    buffer = reinterpret_cast<uint8_t*>(malloc(width * height));

    clear_buffer();
    hide_cursor();
}

asciilibur::FrameBuffer::~FrameBuffer() {
    free(buffer);
}

void asciilibur::FrameBuffer::draw(uint8_t chr, uint8_t x, uint8_t y) {
    if (x >= width || y >= height) {
        return;
    }

    buffer[y * width + x] = chr;
}

void asciilibur::FrameBuffer::draw(Char chr, uint8_t x, uint8_t y) {
    draw(static_cast<uint8_t>(chr), x, y);
}

void asciilibur::FrameBuffer::clear_buffer() {
    memset(buffer, Char::WHITESPACE, width * height);
}

void asciilibur::FrameBuffer::render_buffer() {
    reset_cursor();

    for (uint8_t y = 0; y < height; ++y) {
        for (uint8_t x = 0; x < width; ++x) {
            std::cout << buffer[y * width + x];
        }
        std::cout << std::endl;
    }
}

void asciilibur::FrameBuffer::reset_cursor() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(stdout_handle, pos);
}

void asciilibur::FrameBuffer::hide_cursor() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(stdout_handle, &info);
    info.bVisible = false;
    SetConsoleCursorInfo(stdout_handle, &info);
}
