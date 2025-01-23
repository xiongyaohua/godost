#ifndef TYPST_SESSION
#define  TYPST_SESSION

/* Wraper on Godot side */
#include <cstdint>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/image.hpp>
#include "ffi.h"

extern "C" TypstTask* ffi_typst_task_create(Buffer main);
extern "C" uint32_t ffi_typst_typst_get_page_count(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_error_message(TypstTask* task);
extern "C" Buffer ffi_typst_task_borrow_page_buffer(TypstTask* task, uint32_t page);
extern "C" PageInfo ffi_typst_task_get_page_info(TypstTask* task, uint32_t page);
extern "C" bool ffi_rust_task_destroy(TypstTask* task);

using namespace godot;

class TypstSession {
    TypstTask* task;

public:
    TypstSession(const String& p_main);
    ~TypstSession();

    bool poll();
    Ref<Image> get_page(uint32_t p_page); // Store page as `Image` instead of `Texture2D` to save video memory.
    Array get_all_pages();
};

#endif // TYPST_SESSION
