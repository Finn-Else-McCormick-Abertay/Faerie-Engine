use anyhow::Result;
use component::ResourceTable;
use wasmtime::*;
use wasmtime_wasi::{WasiCtx, WasiCtxBuilder, WasiView};

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
}

#[cxx::bridge]
mod other_ffi {
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

		let result = other_ffi::TestFunction();
		println!("TestFunction Result: {result}");

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
		let opt = self.instance.get_func(&mut engine.store, "hello-world").and_then(|func|{
			let result = func.typed::<(),(String,)>(&engine.store);
			match result {
				Ok(typed_func) => Some(typed_func),
				Err(_error) => None
			}
		});
		match opt {
			Some(func) => {
				match func.call(&mut engine.store, ()) {
					Ok(result) => { println!("Result: {:?}", result.0); }
					Err(_error) => ()
				}
			}
			None => ()
		}
	}
}

pub fn create_script_engine() -> Box<ScriptEngine> {
	Box::new(ScriptEngine::default())
}