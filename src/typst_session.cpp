#include "typst_session.h"
#include "ffi.h"
#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/placeholder_mesh.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include <cstdint>

TypstSession::TypstSession(const String& p_main, bool p_async) {
    p_async = false; // TODO: force sync for now

    if (p_async) {
        // TODO: invoke thread.
        state = STATE_RUNNING;
    } else {
        Buffer buffer = string_to_buffer(p_main);
        task = ffi_typst_task_create(buffer);
        if (get_page_count()==0) {
            state = STATE_FAILED;
        } else {
            state = STATE_SUCCEED;
        }
    }
}

TypstSession::~TypstSession() {
    if (state==STATE_RUNNING) {
        // TODO: await thread.
    }
    ffi_typst_task_destroy(task);
}

TypstSession::State TypstSession::get_state() {
    return state;
}

// Get error message if compile failed
String TypstSession::get_error() {
    ERR_FAIL_COND_V(state!=STATE_FAILED, String());

    Buffer buffer  = ffi_typst_task_borrow_error_message(task);
    return buffer_to_string(buffer);
}

// Get number of pages.
uint32_t TypstSession::get_page_count() {
    ERR_FAIL_COND_V(state==STATE_RUNNING, 0);

    return ffi_typst_task_get_page_count(task);
}

// Store page as `Image` instead of `Texture2D` to save video memory.
Ref<Image> TypstSession::get_page(uint32_t p_page) {
    ERR_FAIL_COND_V(state==STATE_RUNNING, Ref<Image>());

    uint32_t count = get_page_count();
    ERR_FAIL_INDEX_V(p_page, count, Ref<Image>());

    PageInfo info = ffi_typst_task_get_page_info(task, p_page);
    Buffer buffer = ffi_typst_task_borrow_page_buffer(task, p_page);
    PackedByteArray data = buffer_to_array(buffer);

    return Image::create_from_data(info.width, info.height, false, godot::Image::FORMAT_RGBA8, data);
}

// Get the meta info of `page`.
//
// width and height are also included in page `Image`.
Dictionary TypstSession::get_page_info(uint32_t p_page) {
    ERR_FAIL_COND_V(state==STATE_RUNNING, Dictionary());

    PageInfo info = ffi_typst_task_get_page_info(task, p_page);
    Dictionary r;
    r["width"] = info.width;
    r["height"] = info.height;

    return r;
}

// Get an list of `Images`, one for each page.
Array TypstSession::get_all_pages() {
    ERR_FAIL_COND_V(state==STATE_RUNNING, Array());

    Array r;

    uint32_t count = get_page_count();
    for(uint32_t i=0; i < count; i++) {
        Ref<Image> page = get_page(i);
        r.append(page);
    }

    return r;
}
