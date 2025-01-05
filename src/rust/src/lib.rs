use anyhow::Result;
use wasmtime::*;
use wasmtime::component::{types::ComponentItem, ResourceTable, ComponentNamedList, Lower, Lift};
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

		fn init(self: &mut Script);
		fn process(self: &mut Script, delta: f64);
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

		println!("{}", debug::component_item_to_string(ComponentItem::Component(component.component_type()), &engine.engine));

		Ok(Script{ instance, store })
	}

	fn call<Params, Return>(&mut self, interface_name: &str, func_name: &str, params: Params) -> Option<Return>
	where Params: ComponentNamedList + Lower, Return: ComponentNamedList + Lift {
		let func = self.instance.get_export(&mut self.store, None, interface_name)
			.and_then(|interface_index| self.instance.get_export(&mut self.store, Some(&interface_index), func_name)
			.and_then(|func_index| self.instance.get_func(&mut self.store, func_index)
			.and_then(|func| func.typed::<Params, Return>(&self.store)
				.inspect_err(|err|eprintln!("Error while getting typed func: {err}"))
				.ok()
			)));
		
		func.and_then(|func|{
			let result = func.call(&mut self.store, params)
				.inspect_err(|err|eprintln!("Error during call: {err}"))
				.ok();
			let _ = func.post_return(&mut self.store).inspect_err(|err|eprintln!("Error during post_return: {err}"));
			result
		})
	}

	pub fn init(&mut self) {
		self.call::<(),()>("faerie:script-api/script-lifecycle", "init", ());
	}

	pub fn process(&mut self, delta: f64) {
		self.call::<(f64,),()>("faerie:script-api/process-hook", "process", (delta,));
	}

}

pub fn create_script_engine() -> Box<ScriptEngine> {
	Box::new(ScriptEngine::default())
}