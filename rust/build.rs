
fn main() {
    cxx_build::bridge("src/lib.rs")
        .compile("faerie-rust");

    println!("cargo:rerun-if-changed=src/lib.rs");
}