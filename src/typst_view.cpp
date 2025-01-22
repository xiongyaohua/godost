#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "typst_view.h"


using namespace godot;

TypstView::TypstView() {
}

TypstView::~TypstView() {
    await_compilation();
}

void TypstView::_notification(int p_what) {

};

void TypstView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_placeholder", "p_image"), &TypstView::set_placeholder);
	ClassDB::bind_method(D_METHOD("get_placeholder"), &TypstView::get_placeholder);
	ClassDB::bind_method(D_METHOD("set_main_file", "p_path"), &TypstView::set_main_file);
	ClassDB::bind_method(D_METHOD("get_main_file"), &TypstView::get_main_file);
	ClassDB::bind_method(D_METHOD("get_page_count"), &TypstView::get_page_count);
	ClassDB::bind_method(D_METHOD("get_page"), &TypstView::get_page);
	ClassDB::bind_method(D_METHOD("show_page", "p_page"), &TypstView::show_page);
	ClassDB::bind_method(D_METHOD("is_ready"), &TypstView::is_ready);
	ClassDB::bind_method(D_METHOD("invoke_compilation", "p_sync"), &TypstView::invoke_compilation);
	ClassDB::bind_method(D_METHOD("await_compilation"), &TypstView::await_compilation);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "placeholder", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_placeholder", "get_placeholder");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "main_file", PROPERTY_HINT_FILE, "*.typ"), "set_main_file", "get_main_file");
	ADD_SIGNAL(MethodInfo("ready", PropertyInfo(Variant::BOOL, "is_succeed")));
}
