#[no_mangle]
pub extern "C" fn rust_add(left: u64, right: u64) -> u64 {
    left + right
}

#[no_mangle]
pub extern "C" fn rust_half_mul(left: u64, right: u64) -> u64 {
    let r = left * right;
    unsafe { cpp_div(r, 2) }
}

extern "C" {
    fn cpp_div(left: u64, right: u64) -> u64;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = rust_add(2, 2);
        assert_eq!(result, 4);
    }
}
