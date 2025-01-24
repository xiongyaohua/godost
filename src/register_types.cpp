/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>

#include "typst_view.h"
#include "typst_view_editor_plugin.h"

using namespace godot;

void initialize_godost_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        GDREGISTER_INTERNAL_CLASS(TypstViewEditor);
        GDREGISTER_INTERNAL_CLASS(TypstViewEditorPlugin);
        EditorPlugins::add_by_type<TypstViewEditorPlugin>();
    }
    else if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        GDREGISTER_CLASS(TypstView);
	}
}

void uninitialize_godost_module(ModuleInitializationLevel p_level) {
	if (p_level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR) {
	    EditorPlugins::remove_by_type<TypstViewEditorPlugin>();
	}
	else if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT godost_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_godost_module);
	init_obj.register_terminator(uninitialize_godost_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

	return init_obj.init();
}
}
