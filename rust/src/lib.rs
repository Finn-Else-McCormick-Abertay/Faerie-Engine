use anyhow::{Result, Ok};

#[cxx::bridge(namespace = "faerie_rust")]
mod ffi {
    extern "Rust" {
        type ScriptEngine;
        fn create_script_engine() -> Box<ScriptEngine>;

        fn load_module_from_binary(&self, binary: &[u8]);
    }
}

pub struct ScriptEngine {
    engine: wasmtime::Engine
}

impl Default for ScriptEngine {
    fn default() -> Self {
        let config = wasmtime::Config::new();
        ScriptEngine::new(&config).unwrap()
    }
}

impl ScriptEngine {
    pub fn new(config: &wasmtime::Config) -> Result<Self> {
        let engine = wasmtime::Engine::new(config)?;
        Ok(ScriptEngine{engine})
    }

    pub fn load_module_from_binary(&self, binary: &[u8]) {
        wasmtime::Module::from_binary(&self.engine, binary);
    }
}

pub fn create_script_engine() -> Box<ScriptEngine> {
    Box::new(ScriptEngine::default())
}