use anyhow::{Result, Ok};

#[cxx::bridge(namespace = "faerie_rust")]
mod ffi {
    extern "Rust" {
        type ScriptEngine;
        type Script;

        fn create_script_engine() -> Box<ScriptEngine>;

        fn load_module(self: &ScriptEngine, bytes: &[u8]) -> Result<Box<Script>>;
    }
}

pub struct ScriptEngine {
    engine: wasmtime::Engine
}

pub struct Script {
    module: wasmtime::Module
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

    pub fn load_module(&self, bytes: &[u8]) -> Result<Box<Script>> {
        let module = wasmtime::Module::new(&self.engine, bytes)?;
        Ok(Box::new(Script{ module }))
    }
}

pub fn create_script_engine() -> Box<ScriptEngine> {
    Box::new(ScriptEngine::default())
}