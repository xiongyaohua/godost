#include <godot_cpp/classes/editor_interface.hpp>
#include "typst_view_editor_plugin.h"
#include "godot_cpp/classes/editor_plugin.hpp"
#include "godot_cpp/classes/theme.hpp"
#include "godot_cpp/core/memory.hpp"
//#include "typst_view.h"

TypstViewEditor::TypstViewEditor() {
    // Create widgets in editor GUI.
    bake_button = memnew(Button);
	bake_button->set_text("Bake Typst"); // TODO: How to translate text?
	bake_button->show();
	add_child(bake_button);
	//create_curve_button->connect(SceneStringName(pressed), callable_mp(this, &Path2DEditor::_create_curve));
}

void TypstViewEditor::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_THEME_CHANGED: {
            bake_button->set_button_icon(get_theme_icon("TypstBake", "EditorIcons")); // TODO: use a customized typst icon.
        } break;
    }
}

void TypstViewEditor::_bind_methods() {

}

void TypstViewEditorPlugin::_enter_tree() {
    Ref<Theme> editor_theme = EditorInterface::get_singleton()->get_editor_theme();

    Ref<Theme> theme;
    theme.instantiate();

    // TODO: Create our own icon.
    Ref<Texture2D> icon_bake = editor_theme->get_icon("Clear", "EditorIcons");
    theme->set_icon("TypstBake", "EditorIcons", icon_bake);
    editor_theme->merge_with(theme);
}

void TypstViewEditorPlugin::_exit_tree() {
    // TODO: Restore editor theme?
}

void TypstViewEditorPlugin::_bind_methods() {

}

void TypstViewEditor::edit(Node* p_view) {
    if (p_view) {
        // TODO: Prepare edit progress
    } else {
        // TODO: Cancel edit progress
    }
    //_update_toolbar()
}

void TypstViewEditorPlugin::_edit(Object *p_object) {
	typst_view_editor->edit(Object::cast_to<Node>(p_object));
}

bool TypstViewEditorPlugin::_handles(Object *p_object) const {
	return p_object->is_class("TypstView");
}

void TypstViewEditorPlugin::_make_visible(bool p_visible) {
	if (p_visible) {
		typst_view_editor->show();
	} else {
		typst_view_editor->hide();
		typst_view_editor->edit(nullptr);
	}
}

TypstViewEditorPlugin::TypstViewEditorPlugin() {
	typst_view_editor = memnew(TypstViewEditor());
	add_control_to_container(CONTAINER_CANVAS_EDITOR_MENU, typst_view_editor);

	typst_view_editor->hide();
}

TypstViewEditorPlugin::~TypstViewEditorPlugin() {
    remove_control_from_container(CONTAINER_CANVAS_EDITOR_MENU, typst_view_editor);
    memdelete(typst_view_editor);
}
