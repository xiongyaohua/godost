//! FFI module between the Godot(C++) and Typst(Rust) sides of godost.
//!
//! The general work division is that, C++ side handles all I/O and threading,
//! while Rust side handle typsetting basiclly in a sandbox. The two sides
//! exchange *handles* and *buffers*, where handle is a opaque pointer
//! indicating some "thing" on the other side, buffer is some continuous bytes
//! indicated by a pointer plus a size.
//!
//! The ownership policy is as follows:
//! - that each side *owns* handles(hand things behind them) from the other
//! side, which means it is responsible for releasing them.
//! - Each side could *borrows* buffers from other side using handles, which
//! means it should not hold the borrowed buffers after their source handles
//! released.
//! - Each side could call *lend* buffers it holds to the other side in function
//! calls, trusting the other side will not hold the buffers beyond the function
//! return.
//! - The borrowed buffers could be copied and held.
//!
//! The error handling policy is as following:
//! - Always check if opaque pointer arguments are valid.
//! - Indicate error with null pointer in return values.
//! - When necessary, provide functions that get error details.
//!
//! All texts, including source codes, file paths and names, should be exchanged
//! as well formated utf8 buffer *without* trailing `\0`. As the godot `String`
//! type represent texts using char32, while rust use utf8, the user should take
//! care the encode and decode between two representations.

#![allow(dead_code)]
// Opaque pointers, see explaination from the following link:
// https://stackoverflow.com/questions/38315383/whats-the-rust-idiom-to-define-a-field-pointing-to-a-c-opaque-pointer

/// Opaque pointer to files on the Godot side.
#[repr(C)]
pub struct GodotFile {
    _data: [u8; 0],
    _marker:
        core::marker::PhantomData<(*mut u8, core::marker::PhantomPinned)>,
}

/// Opaque pointer to a compilation task on the Typst side.
pub struct TypstTask {
    _data: [u8; 0],
    _marker:
        core::marker::PhantomData<(*mut u8, core::marker::PhantomPinned)>,
}

/// Readonly buffer exchanged between Godot and Typst.
#[repr(C)]
pub struct Buffer {
    pub pointer: *const u8,
    pub size: usize
}

/// Meta info for page buffers.
#[repr(C)]
pub struct PageInfo {
    pub width: u32,
    pub height: u32
}

/// FFI functions called from Typst side.
mod typst_to_godot {
    use crate::ffi::{Buffer, GodotFile};

    extern "C" {
        /// Open the file at `path`.
        ///
        /// Return an opaque pointer as the file handle. Null pointer indicates
        /// error.
        pub fn ffi_godot_file_open(path: Buffer) -> *const GodotFile;
        /// Release the `file` after use.
        ///
        /// Return `false` if something goes wrong, an invalid pointer most likely.
        pub fn ffi_godot_file_close(file: *const GodotFile) -> bool;
        /// Borrow text buffer from `file`
        ///
        /// Return a well formed utf8 buffer. Null pointer in the buffer indicate
        /// error.
        pub fn ffi_godot_file_borrow_text_buffer(file: *const GodotFile) -> Buffer;
        /// Borrow raw buffer from `file`
        ///
        /// Return a raw buffer. Null pointer in the buffer indicate
        /// error.
        pub fn ffi_godot_file_borrow_raw_buffer(file: *const GodotFile) -> Buffer;
        /// Download typst package at `url`, then extract it into `path` folder.
        ///
        /// Return `false` if something goes wrong.
        pub fn ffi_godot_package_download(url: Buffer, path: Buffer) -> bool;
    }
}

/// FFI functions called from Godot side. All function is a single-threaded and
/// synchronouse. Threaded multiplexing should be handled by C++ caller.
mod godot_to_typst {
    use crate::ffi::{Buffer, TypstTask, PageInfo};

    /// Create a Typst task, compiling main `.typ` file at `main`.
    ///
    /// Return a handle to a finished task. Null pointer indicate failure.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_create(main: Buffer) -> *const TypstTask {
        let _ = main;
        0 as *const TypstTask
    }

    /// Get the count of resulting pages.
    ///
    /// Return 0 for failed compilation. Succeed compilation produce at least
    /// one (empty) page.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_get_page_count(task: *const TypstTask) -> u32 {
        let _ = task;
        0
    }

    /// Borrow a buffer of the error message.
    ///
    /// Only valid to call when the compilation failed. Return null buffer
    /// otherwise.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_borrow_error_message(task: *const TypstTask) -> Buffer {
        let _ = task;
        Buffer {
            pointer: 0 as *const u8,
            size: 0
        }
    }

    /// Borrow a buffer for `page`.
    ///
    /// Return null buffer if the compilation is failed, or the page index is out
    /// of scope.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_borrow_page_buffer(task: *const TypstTask, page: u32) -> Buffer {
        let _ = task;
        let _ = page;
        Buffer {
            pointer: 0 as *const u8,
            size: 0
        }
    }

    /// Get meta info for `page`.
    ///
    /// Return 0x0 size, if the compilation is failed, or the page index is out
    /// of scope.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_get_page_info(task: *const TypstTask, page: u32) -> PageInfo {
        let _ = task;
        let _ = page;
        PageInfo {
            width: 0,
            height: 0,
        }
    }

    /// Release the task instance.
    ///
    /// Return `false` if something goes wrong, an invalid pointer most likely.
    #[no_mangle]
    pub extern "C" fn ffi_typst_task_destroy(task: *const TypstTask) -> bool {
        let _ = task;
        false
    }
}
