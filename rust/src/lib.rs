#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn greet_from_rust() -> String;
    }
}

pub fn greet_from_rust() -> String {
    println!("greet_from_rust called (returning string \"returned from rust\")");


    String::from("returned from rust")
}