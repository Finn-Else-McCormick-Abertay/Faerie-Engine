use anyhow::Result;
use wasmtime::*;
use wasmtime::component::{types::ComponentItem, ResourceTable};
use wasmtime_wasi::{WasiCtx, WasiCtxBuilder, WasiView};

mod debug;

//bindgen!("module" in "../wit");

#[cxx::bridge]
mod ffi {
	#[namespace = "faerie_rust"]
	extern "Rust" {
		type ScriptEngine;
		type Script;

		fn create_script_engine() -> Box<ScriptEngine>;

		fn load_script(self: &ScriptEngine, bytes: &[u8]) -> Result<Box<Script>>;

		fn test(self: &mut Script);
	}
}

#[cxx::bridge]
mod other_ffi {
	unsafe extern "C++" {
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
	linker: component::Linker::<ScriptState>,
}

pub struct Script {
	instance: component::Instance,
	store: Store::<ScriptState>,
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
		let mut linker = component::Linker::<ScriptState>::new(&engine);
		wasmtime_wasi::add_to_linker_sync(&mut linker)?;

		Ok(ScriptEngine{engine, linker})
	}

	pub fn load_script(&self, bytes: &[u8]) -> Result<Box<Script>> {
		let component = component::Component::new(&self.engine, bytes)?;

		let script = Script::new(component, &self)?;
		Ok(Box::new(script))
	}
}

impl Script {
	pub fn new(component: component::Component, engine: &ScriptEngine) -> Result<Script> {
		let wasi = WasiCtxBuilder::new()
			.inherit_stdio()
			.build();
		let mut store = Store::<ScriptState>::new(&engine.engine, ScriptState{ ctx: wasi, resource_table: ResourceTable::new() });
		let instance = engine.linker.instantiate(&mut store, &component)?;

		let component_type = component.component_type();
		println!("{}", debug::component_item_to_string(ComponentItem::Component(component_type), &engine.engine));

		Ok(Script{ instance, store })
	}

	pub fn test(&mut self) {
		let opt = self.instance.get_func(&mut self.store, "setup").and_then(|func|{
			let result = func.typed::<(),()>(&self.store);
			match result {
				Ok(typed_func) => Some(typed_func),
				Err(_error) => None
			}
		});
		match opt {
			Some(func) => {
				match func.call(&mut self.store, ()) {
					Ok(_) => { println!("Successfully called setup"); }
					Err(error) => { eprintln!("Error during call: {error}"); }
				}
				match func.post_return(&mut self.store) {
					Ok(_) => {}
					Err(error) => { eprintln!("Error during post_return: {error}"); }
				}
			}
			None => { println!("Function does not exist"); }
		}
	}
}

pub fn create_script_engine() -> Box<ScriptEngine> {
	Box::new(ScriptEngine::default())
}