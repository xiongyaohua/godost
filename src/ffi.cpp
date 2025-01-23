#include "ffi.h"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include <godot_cpp/classes/file_access.hpp>

using namespace godot;

// Helper class and functions. Keep them internal to this unit.
namespace {
    PackedByteArray buffer_to_array(Buffer p_buffer) {
        PackedByteArray array;
        array.resize(p_buffer.size);
        memcpy(array.ptrw(), p_buffer.pointer, p_buffer.size);

        return array;
    }

    Buffer array_to_buffer(const PackedByteArray& p_array) {
        Buffer buffer;
        buffer.pointer = p_array.ptr();
        buffer.size = p_array.size();

        return buffer;
    }

    class FileWithCache {
        String path;
        PackedByteArray bytes;
        PackedByteArray text; // Must be empty or well-formed utf8.
        bool decoded; // Already tried to decode `bytes` into `text`.

    public:
        static FileWithCache* open(const String& p_path) {
            PackedByteArray bytes = FileAccess::get_file_as_bytes(p_path);
            Error err = FileAccess::get_open_error();

            if(err == OK) {
                FileWithCache * ptr = memnew(FileWithCache);
                ptr->path = p_path;
                ptr->bytes = bytes;
                ptr->decoded = false;

                return ptr;
            } else {
                return nullptr;
            }
        }

        Buffer get_bytes() {
            return array_to_buffer(bytes);
        }

        Buffer get_text() {
            if(!decoded) {
                text = bytes.get_string_from_utf8().to_utf8_buffer();
                decoded = true;
            }

            return array_to_buffer(text);
        }
    };

}

/* Implement FFI functions called from Typst side. */

GodotFile* ffi_godot_file_open(Buffer path) {
    String path_string = buffer_to_array(path).get_string_from_utf8();
    return (GodotFile*)FileWithCache::open(path_string);
}

bool ffi_godot_file_close(GodotFile* file) {
    if(file==nullptr) {
        return false;
    } else {
        memdelete((FileWithCache*)file);
        return true;
    }
}

Buffer ffi_godot_file_borrow_text_buffer(GodotFile* file) {
    return ((FileWithCache*)file)->get_text();
}

Buffer ffi_godot_file_borrow_raw_buffer(GodotFile* file) {
    return ((FileWithCache*)file)->get_bytes();
}
bool ffi_godot_package_download(Buffer url, Buffer path) {
    return false;
}
