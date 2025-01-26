/// FFI functions for C++ side. Keep in-sync with its Rust conterpart.

#ifndef FFI_H
#define FFI_H

#include <cstddef>
#include <cstdint>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

struct Buffer {
    const uint8_t *pointer;
    size_t size;
};

struct PageInfo {
    uint32_t width;
    uint32_t height;
};

struct GodotFile;
struct TypstTask;

/* Helper function converting between Buffer and PackedByteArray */
static inline PackedByteArray buffer_to_array(Buffer p_buffer) {
    PackedByteArray array;
    array.resize(p_buffer.size);
    memcpy(array.ptrw(), p_buffer.pointer, p_buffer.size);

    return array;
}

static inline Buffer array_to_buffer(const PackedByteArray& p_array) {
    Buffer buffer;
    buffer.pointer = p_array.ptr();
    buffer.size = p_array.size();

    return buffer;
}

static inline String buffer_to_string(Buffer p_buffer) {
    return buffer_to_array(p_buffer).get_string_from_utf8();
}

static inline Buffer string_to_buffer(const String& p_string) {
    return array_to_buffer(p_string.to_utf8_buffer());
}

/* FFI functions called from Typst side. */

extern "C" GodotFile* ffi_godot_file_open(Buffer path);
extern "C" bool ffi_godot_file_close(GodotFile* file);
extern "C" Buffer ffi_godot_file_borrow_text_buffer(GodotFile* file);
extern "C" Buffer ffi_godot_file_borrow_raw_buffer(GodotFile* file);
extern "C" bool ffi_godot_package_download(Buffer url, Buffer path);

/* FFI functions called from Godot side. */

extern "C" TypstTask* ffi_typst_task_create(Buffer main);
extern "C" uint32_t ffi_typst_task_get_page_count(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_error_message(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_page_buffer(TypstTask* task, uint32_t page);
extern "C" PageInfo ffi_typst_task_get_page_info(TypstTask* task, uint32_t page);
extern "C" bool ffi_typst_task_destroy(TypstTask* task);

#endif // FFI_H
