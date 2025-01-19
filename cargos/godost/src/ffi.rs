use std::ffi::{CStr, c_char, c_void};

#[repr(C)]
pub struct Bytes {
    pub pointer: *const u8,
    pub size: usize
}

// Ffi functions import from C++ side.
extern "C" {
    // Get the buffer for a file at "path". null pointer means error.
    pub fn ffi_cpp_file_open(path: *const c_char) -> *const c_void;
    pub fn ffi_cpp_file_borrow_buffer(file: *const c_void) -> Bytes;
    // Release the file after use.
    pub fn ffi_cpp_file_release(file: *const c_void) -> bool;
    // Download typse package at `url`, then extract it into `path`. Return `false` if something goes wrong.
    pub fn ffi_cpp_package_download_and_extract(url: *const c_char, path: *const c_char) -> bool;
}

// Ffi functions export to C++ side.
//
// The API is a single-threaded and synchronouse. Threaded multiplexing should be handled by C++ caller.

// Finishe compiling a Typst file at `main_path`. Return a handle to access the results.
#[no_mangle]
pub extern "C" fn ffi_rust_task_create(main_path: *const c_char) -> *const c_void {
    let main_path = unsafe { CStr::from_ptr(main_path) };
    let _ = main_path;
    1 as *const c_void
}

// Get the count of resulting pages. 0 means something is wrong.
#[no_mangle]
pub extern "C" fn ffi_rust_task_get_page_count(task: *const c_void) -> usize {
    let _ = task;
    0
}

// Borrow a pointer to the error message. The pointer is null if succeeded( page count > 0).
#[no_mangle]
pub extern "C" fn ffi_rust_task_borrow_error_message(task: *const c_void) -> *const c_char {
    let _ = task;
    0 as *const c_char
}

// Borrow a buffer, including the pointer and the size, for the `page`.
#[no_mangle]
pub extern "C" fn ffi_rust_task_borrow_page_buffer(task: *const c_void, page: usize) -> Bytes {
    let _ = task;
    let _ = page;
    Bytes {
        pointer: 0 as *const u8,
        size: 0
    }
}

// Destroy the task instance, releasing all buffers it holds.
#[no_mangle]
pub extern "C" fn ffi_rust_task_destroy(task: *const c_void) -> bool {
    let _ = task;
    false
}
