/// A view of Typst pages in scene tree.

#ifndef TYPST_VIEW_CLASS_H
#define TYPST_VIEW_CLASS_H

#include <cstddef>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/variant_internal.hpp>

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image.hpp>
#include "godot_cpp/classes/image_texture.hpp"
#include "godot_cpp/classes/texture.hpp"
#include "godot_cpp/classes/texture2d.hpp"
//#include <godot_cpp/core/binder_common.hpp>
//#include <godot_cpp/core/gdvirtual.gen.inc>

#include <ffi.h>

using namespace godot;

class TypstView : public TextureRect {
	GDCLASS(TypstView, TextureRect);

private:
    Vector<Ref<Image>> pages;
    Ref<Texture2D> placeholder; // A placeholder image shown when there is no actual typst pages.
    TypstTask *running_task = nullptr; // Handle to a running compilation task.
    String main_file = ""; // The main ".typ" for for compilation session.
    size_t preferred_page = 0;
    size_t actual_page = 0;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	TypstView();
	~TypstView();

	void set_placeholder(const Ref<Texture2D> &p_image) { placeholder = p_image; }
	Ref<Texture2D> get_placeholder() { return placeholder; }

	void set_main_file(const String &p_path) { main_file = p_path; } // TODO: validate its a ".typ" file.
	String get_main_file() { return main_file; }

	// IMPORTANT: Here pages are 1-based. Page 0 is the placeholder.
	Ref<Image> get_page(const size_t p_page) { return placeholder; }
	size_t get_page_count() { return pages.size(); }
	void show_page(size_t p_page) {}

	bool is_ready() { return true; }
	String invoke_compilation(bool p_async = false) { return "Do Nothing!"; }
	void await_compilation() {}
};

#endif // TYPST_VIEW_CLASS_H
