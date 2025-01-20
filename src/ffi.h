/// FFI functions for C++ side. Keep in-sync with its Rust conterpart.

#ifndef FFI_H
#define FFI_H

#include <cstddef>
#include <cstdint>

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

/* FFI functions called from Typst side. */

extern "C" GodotFile* ffi_godot_file_open(Buffer path);
extern "C" bool ffi_godot_file_close(GodotFile* file);
extern "C" Buffer ffi_godot_file_borrow_text_buffer(GodotFile* file);
extern "C" Buffer ffi_godot_file_borrow_raw_buffer(GodotFile* file);
extern "C" bool ffi_godot_package_download(Buffer url, Buffer path);

/* FFI functions called from Godot side. */

extern "C" TypstTask* ffi_typst_task_create(Buffer main);
extern "C" size_t ffi_typst_typst_get_page_count(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_error_message(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_page_buffer(TypstTask* task);
extern "C" PageInfo ffi_typst_task_get_page_info(TypstTask* task);
extern "C" bool ffi_rust_task_destroy(TypstTask* task);

#endif // FFI_H
