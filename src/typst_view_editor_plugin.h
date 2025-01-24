#ifndef TYPST_VIEW_EDITOR_PLUGIN_H
#define TYPST_VIEW_EDITOR_PLUGIN_H
/// Editor Plugin for TypstView.

#include "typst_view.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>

using namespace godot;

class TypstViewEditor : public HBoxContainer {
	GDCLASS(TypstViewEditor, HBoxContainer);

	friend class TypstViewEditorPlugin;

	Button* bake_button;

protected:
    void _notification(int p_what); // For theme update.
    static void _bind_methods();

public:
	void edit(Node *p_view);
	TypstViewEditor();
};

class TypstViewEditorPlugin : public EditorPlugin {
	GDCLASS(TypstViewEditorPlugin, EditorPlugin);

	TypstViewEditor* typst_view_editor;

protected:
    static void _bind_methods();

public:
    TypstViewEditorPlugin();
    ~TypstViewEditorPlugin();

    void _enter_tree() override;
    void _exit_tree() override;

    virtual String _get_plugin_name() const override { return "TypstView"; }
	bool _has_main_screen() const override { return false; }
	virtual void _edit(Object *p_object) override;
	virtual bool _handles(Object *p_object) const override;
	virtual void _make_visible(bool p_visible) override;
};

#endif // TYPST_VIEW_EDITOR_PLUGIN_H
