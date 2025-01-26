#ifndef TYPST_SESSION_H
#define  TYPST_SESSION_H

/* Wraper on Godot side */
#include <godot_cpp/variant/string.hpp>
#include "godot_cpp/variant/dictionary.hpp"
#include <godot_cpp/classes/image.hpp>
#include "ffi.h"

using namespace godot;

/// Manage the session of calling typst.
///
/// Threading is handled here for `TypstView`.
class TypstSession {
public:
    enum State {
        STATE_RUNNING,
        STATE_FAILED,
        STATE_SUCCEED, // Produce at least one page.
    };

private:
    TypstTask* task;
    State state = STATE_FAILED;

public:
    TypstSession(const String& p_main, bool p_async=false);
    ~TypstSession();

    State get_state();

    // Get error message if compile failed
    String get_error();

    // Get number of pages.
    uint32_t get_page_count();

    // Store page as `Image` instead of `Texture2D` to save video memory.
    Ref<Image> get_page(uint32_t p_page);

    // Get the meta info of `page`.
    //
    // width and height are redundant as they are already included in `Image`.
    Dictionary get_page_info(uint32_t page);

    // Get an list of `Images`, one for each page.
    Array get_all_pages();
};

#endif // TYPST_SESSION_H
