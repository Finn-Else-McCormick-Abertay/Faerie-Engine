use anyhow::{Result, Ok};
use component::ResourceTable;
use ffi::TestFunction;
use wasmtime::*;
use wasmtime_wasi::{bindings::*, WasiCtx, WasiCtxBuilder, WasiView};

#[cxx::bridge]
mod ffi {
    #[namespace = "faerie_rust"]
    extern "Rust" {
        type ScriptEngine;
        type Script;

        fn create_script_engine() -> Box<ScriptEngine>;

        fn load_script(self: &mut ScriptEngine, bytes: &[u8]) -> Result<Box<Script>>;

        fn test(self: &Script, engine: &mut ScriptEngine);
    }

    unsafe extern "C++" {
        include!("src/client/test.h");

        fn TestFunction() -> u32;

    }
}

struct ScriptState {
    ctx: WasiCtx,
    resource_table: component::ResourceTable
}

impl WasiView for ScriptState {
    fn ctx(&mut self) -> &mut WasiCtx {
        &mut self.ctx
    }

    fn table(&mut self) -> &mut component::ResourceTable {
        & mut self.resource_table
    }
}

pub struct ScriptEngine {
    engine: Engine,
    store: Store::<ScriptState>,
    linker: component::Linker::<ScriptState>,
}

pub struct Script {
    instance: component::Instance
}

impl Default for ScriptEngine {
    fn default() -> Self {
        ScriptEngine::new(
            Config::new()
                .wasm_component_model(true)
                .debug_info(true)
        ).unwrap()
    }
}

impl ScriptEngine {
    pub fn new(config: &Config) -> Result<Self> {
        let engine = Engine::new(config)?;
        let wasi = WasiCtxBuilder::new()
            .inherit_stdio()
            .build();
        let store = Store::<ScriptState>::new(&engine, ScriptState{ ctx: wasi, resource_table: ResourceTable::new() });
        let mut linker = component::Linker::<ScriptState>::new(&engine);
        wasmtime_wasi::add_to_linker_sync(&mut linker)?;

        let result = TestFunction();
        println!("TestFunction result: {result}");

        Ok(ScriptEngine{engine, store, linker})
    }

    pub fn load_script(&mut self, bytes: &[u8]) -> Result<Box<Script>> {
        let component = component::Component::new(&self.engine, bytes)?;

        let instance = self.linker.instantiate(&mut self.store, &component)?;
        Ok(Box::new(Script{ instance }))
    }
}

impl Script {
    pub fn test(&self, engine: &mut ScriptEngine) {
        let export_opt = self.instance.get_export(&mut engine.store, None, "hello-world");
        match export_opt {
            Some(export_name) => {
                let func_opt = self.instance.get_func(&mut engine.store, export_name);
                match func_opt {
                    Some(func) => {
                        let mut result = [component::Val::String("".into())];
                        match func.call(&mut engine.store, &[], &mut result) {
                            Result::Ok(_) => {
                                println!("Result: {:?}", result);
                            }
                            Err(error) => {
                                println!("{error}");
                            }
                        }
                    }
                    None => ()
                }
            }
            None => ()
        }
    }
}

pub fn create_script_engine() -> Box<ScriptEngine> {
    Box::new(ScriptEngine::default())
}