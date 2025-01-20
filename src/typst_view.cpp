#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "typst_view.h"


using namespace godot;

TypstView::TypstView() {
    // TODO make some palceholder image
}

TypstView::~TypstView() {
    cancel_compilation();
}

void TypstView::_notification(int p_what) {

};

void TypstView::_bind_methods() {
}
