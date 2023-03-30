#include "asciilibur.hpp"

#include <iostream>

#include <windows.h>
#include <sys/timeb.h>


asciilibur::FrameBuffer::FrameBuffer(uint8_t width, uint8_t height)
: width(width), height(height), buffer(nullptr), camera()
{
    buffer = reinterpret_cast<uint8_t*>(malloc(static_cast<size_t>(width) * height));

    clear_buffer();
    hide_cursor();
}

asciilibur::FrameBuffer::~FrameBuffer() {
    free(buffer);
}

void asciilibur::FrameBuffer::draw(uint8_t chr, Position pos) {
    pos.x = pos.x - camera.x;
    pos.y = pos.y - camera.y;

    if (pos.x >= width || pos.y >= height || pos.x < 0 || pos.y < 0) {
        return;
    }

    buffer[pos.y * width + pos.x] = chr;
}

void asciilibur::FrameBuffer::draw(Char chr, Position pos) {
    draw(static_cast<uint8_t>(chr), pos);
}

void asciilibur::FrameBuffer::draw(uint8_t chr, int16_t x, int16_t y) {
    draw(chr, Position(x, y));
}

void asciilibur::FrameBuffer::draw(Char chr, int16_t x, int16_t y) {
    draw(static_cast<uint8_t>(chr), Position(x, y));
}

void asciilibur::FrameBuffer::draw_sprite(const char* sprite, Position pos) {
    const uint8_t* sprite_bytes = reinterpret_cast<const uint8_t*>(sprite);
    size_t size = strlen(sprite);
    draw_sprite(sprite_bytes, size, pos);
}

void asciilibur::FrameBuffer::draw_sprite(const char* sprite, int16_t x, int16_t y) {
    draw_sprite(sprite, Position(x, y));
}

void asciilibur::FrameBuffer::draw_sprite(const uint8_t* sprite, size_t size, Position pos) {
    uint8_t x = 0;
    uint8_t y = 0;

    for (size_t idx = 0; idx < size; ++idx) {
        uint8_t chr = sprite[idx];
        if (chr == '\n') {
            ++y;
            x = 0;
            continue;
        }
        draw(chr, Position(pos.x + x, pos.y + y));
        ++x;
    }
}

void asciilibur::FrameBuffer::draw_sprite(const uint8_t* sprite, size_t size, int16_t x, int16_t y) {
    draw_sprite(sprite, size, Position(x, y));
}

void asciilibur::FrameBuffer::set_camera_pos(int16_t x, int16_t y) {
    camera.x = x;
    camera.y = y;
}

void asciilibur::FrameBuffer::set_camera_pos(Position pos) {
    camera = std::move(pos);
}

void asciilibur::FrameBuffer::clear_buffer() {
    memset(buffer, Char::WHITESPACE, static_cast<size_t>(width) * height);
}

void asciilibur::FrameBuffer::render_buffer() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD console_pos;
    console_pos.X = 0;
    console_pos.Y = 0;
    SetConsoleCursorPosition(stdout_handle, console_pos);

    uint8_t last_line = height - 1;
    for (uint8_t y = 0; y < height; ++y) {
        printf("%.*s%s", width, (buffer + y * width), (y == last_line ? "" : "\n"));
    }
}

void asciilibur::FrameBuffer::hide_cursor() {
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(stdout_handle, &info);
    info.bVisible = false;
    SetConsoleCursorInfo(stdout_handle, &info);
}

uint64_t asciilibur::time::get_time() {
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return static_cast<uint64_t>((timebuffer.time * 1000) + timebuffer.millitm);
}

asciilibur::input::KeyState asciilibur::input::get_key_state(int key_code) {
    return GetAsyncKeyState(key_code) ? KeyState::Down : KeyState::Up;
}
