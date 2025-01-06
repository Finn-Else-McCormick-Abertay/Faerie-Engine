use anyhow::Result;
use component::{ComponentExportIndex, Resource, ResourceType, Val};
use wasmtime::*;
use wasmtime::component::{types::ComponentItem, ResourceTable, ComponentNamedList, Lower, Lift};
use wasmtime_wasi::{WasiCtx, WasiCtxBuilder, WasiView};

mod debug;

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
		 fn on_input_press(self: &mut Script, action: &str, pressed: bool);
		 fn on_input_motion(self: &mut Script, action: &str, x: f32, y:f32);
	}

	#[namespace = "bridge"]
	unsafe extern "C++" {
		include!("bridge.h");

		fn CreateEntity() -> u32;
		fn AddTransform(id: u32);
    	fn AddHierarchy(id: u32);
    	fn AddCamera(id: u32);
		
		fn GetPositionX(id: u32) -> f32;
		fn GetPositionY(id: u32) -> f32;
		fn GetPositionZ(id: u32) -> f32;
		fn SetPosition(id: u32, x: f32, y: f32, z: f32);

		fn HierarchyAddChild(id: u32, child_id: u32);
		fn HierarchyGetChildren(id: u32) -> Vec<u32>;
		fn HierarchyGetDescendants(id: u32) -> Vec<u32>;
		
		fn CameraSetActive(id: u32);
		
		fn InstantiateModel(id: u32, model_path: &str);
		fn AddMaterial(id: u32, material_name: &str);
		
		fn CreateMaterial(material_name: &str, vert_path: &str, frag_path: &str);
		fn LoadModel(model_path: &str);
	}
}

mod bindings {
	use wasmtime::component::bindgen;

	bindgen!({
		path: "../wit/script-api.wit",
		world: "script-all",
		with: {
		    "faerie:script-api/ecs/entity": Entity,
		    "faerie:script-api/ecs/transform": Transform,
		    "faerie:script-api/ecs/hierarchy": Hierarchy,
		    "faerie:script-api/ecs/camera": Camera,
		}
	});

	pub struct Entity {
		pub id: u32,
	}

	pub struct Transform {
		pub entity_id: u32,
	}

	pub struct Hierarchy {
		pub entity_id: u32,
	}

	pub struct Camera {
		pub entity_id: u32,
	}

}

struct ScriptState {
	ctx: WasiCtx,
	resource_table: component::ResourceTable,
	namespace: String
}

impl WasiView for ScriptState {
	fn ctx(&mut self) -> &mut WasiCtx {
		&mut self.ctx
	}

	fn table(&mut self) -> &mut component::ResourceTable {
		& mut self.resource_table
	}
}

impl bindings::faerie::script_api::ecs::Host for ScriptState {
	fn load_model(&mut self, model_path: String) {
		ffi::LoadModel(model_path.as_str());
	}
	fn load_material(&mut self, vert_path: String, frag_path: String, material_name: String) {
		ffi::CreateMaterial(material_name.as_str(), vert_path.as_str(), frag_path.as_str());
	}
}
impl bindings::faerie::script_api::ecs::HostEntity for ScriptState {
	fn new(&mut self, id:u32,) -> Resource<bindings::Entity> {
		self.resource_table
			.push(bindings::Entity{ id })
			.unwrap()
	}

	fn create(&mut self) -> Resource<bindings::Entity> {
		let id = ffi::CreateEntity();
		self.resource_table
			.push(bindings::Entity{ id })
			.unwrap()
	}

	fn drop(&mut self, resource:Resource<bindings::Entity>) -> Result<()> {
		self.resource_table.delete(resource)?;
		Ok(())
	}

	fn add_transform(&mut self,self_:Resource<bindings::Entity>,) -> Resource<bindings::Transform> {
		let id = self.resource_table.get(&self_).unwrap().id;
		ffi::AddTransform(id);
		self.resource_table
			.push(bindings::Transform{ entity_id: id })
			.unwrap()
	}

	fn add_hierarchy(&mut self,self_:Resource<bindings::Entity>,) -> Resource<bindings::Hierarchy> {
		let id = self.resource_table.get(&self_).unwrap().id;
		println!("add_hierarchy called in rust");
		ffi::AddHierarchy(id);
		self.resource_table
			.push(bindings::Hierarchy{ entity_id: id })
			.unwrap()
	}

	fn add_camera(&mut self,self_:Resource<bindings::Entity>,) -> Resource<bindings::Camera> {
		let id = self.resource_table.get(&self_).unwrap().id;
		ffi::AddCamera(id);
		self.resource_table
			.push(bindings::Camera{ entity_id: id })
			.unwrap()
	}

	fn add_material(&mut self,self_:Resource<bindings::Entity>, material_name: String) {
		let id = self.resource_table.get(&self_).unwrap().id;
		ffi::AddMaterial(id, material_name.as_str());
	}

	fn instantiate_model(&mut self,self_:Resource<bindings::Entity>, model_path: String) {
		let id = self.resource_table.get(&self_).unwrap().id;
		ffi::InstantiateModel(id, model_path.as_str());
	}
}

impl bindings::faerie::script_api::ecs::HostTransform for ScriptState {
	fn get_position(&mut self,self_:Resource<bindings::Transform>,) -> bindings::faerie::script_api::ecs::Vec3 {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		bindings::faerie::script_api::ecs::Vec3{ x: ffi::GetPositionX(id), y: ffi::GetPositionY(id), z: ffi::GetPositionZ(id) }
	}

	fn set_position(&mut self,self_:Resource<bindings::Transform>,pos:bindings::faerie::script_api::ecs::Vec3) {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		ffi::SetPosition(id, pos.x, pos.y, pos.z);
	}
	
	fn move_(&mut self,self_:Resource<bindings::Transform>,vec:bindings::faerie::script_api::ecs::Vec3) {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		let x = ffi::GetPositionX(id);
		let y = ffi::GetPositionY(id);
		let z = ffi::GetPositionZ(id);

		ffi::SetPosition(id, x + vec.x, y + vec.y, z + vec.z);
	}

	fn drop(&mut self,rep:Resource<bindings::Transform>) -> Result<()> {
		self.resource_table.delete(rep)?;
		Ok(())
	}
}

impl bindings::faerie::script_api::ecs::HostHierarchy for ScriptState {
	fn add_child(&mut self,self_:Resource<bindings::Hierarchy>, entity:Resource<bindings::Entity>,) -> () {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		let child_id = self.resource_table.get(&entity).unwrap().id;
		ffi::HierarchyAddChild(id, child_id);
	}

	fn get_children(&mut self,self_:Resource<bindings::Hierarchy>,) -> Vec<Resource<bindings::Entity>> {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		let children = ffi::HierarchyGetChildren(id);
		children.into_iter().map(|id| {
			self.resource_table
				.push(bindings::Entity{ id })
				.unwrap()
		}).collect()
	}

	fn get_descendants(&mut self,self_:Resource<bindings::Hierarchy>,) -> Vec<Resource<bindings::Entity>> {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		let descendants = ffi::HierarchyGetDescendants(id);
		descendants.into_iter().map(|id| {
			self.resource_table
				.push(bindings::Entity{ id })
				.unwrap()
		}).collect()
	}

	fn drop(&mut self,rep:Resource<bindings::Hierarchy>) -> Result<()> {
		self.resource_table.delete(rep)?;
		Ok(())
	}
}

impl bindings::faerie::script_api::ecs::HostCamera for ScriptState {
	fn set_active(&mut self, self_:Resource<bindings::Camera>) {
		let id = self.resource_table.get(&self_).unwrap().entity_id;
		ffi::CameraSetActive(id);
	}

	fn drop(&mut self,rep:Resource<bindings::Camera>) -> Result<()> {
		self.resource_table.delete(rep)?;
		Ok(())
	}
}

// Unfinished
impl bindings::faerie::script_api::input::Host for ScriptState {
	fn bind_physical_scancode(&mut self, action: String, scancode: bindings::faerie::script_api::input::Scancode) {
		
	}
	fn bind_logical_keycode(&mut self, action: String, keycode: char) {

	}
	fn bind_controller_button(&mut self, action: String, button: bindings::faerie::script_api::input::ControllerButton) {
		
	}
	fn bind_controller_stick(&mut self, action: String, tick: bindings::faerie::script_api::input::ControllerStick) {
		
	}
	fn bind_controller_trigger(&mut self, action: String, stick: bindings::faerie::script_api::input::ControllerTrigger) {
		
	}
	fn bind_mouse_button(&mut self, action: String, button: bindings::faerie::script_api::input::MouseButton) {
		
	}
	fn bind_mouse_motion(&mut self, action: String, source: bindings::faerie::script_api::input::MouseMotionSource) {
		
	}

	fn unbind_physical_scancode(&mut self, action: String, scancode: bindings::faerie::script_api::input::Scancode) {
		
	}
	fn unbind_logical_keycode(&mut self, action: String, keycode: char) {

	}
	fn unbind_controller_button(&mut self, action: String, button: bindings::faerie::script_api::input::ControllerButton) {
		
	}
	fn unbind_controller_stick(&mut self, action: String, tick: bindings::faerie::script_api::input::ControllerStick) {
		
	}
	fn unbind_controller_trigger(&mut self, action: String, stick: bindings::faerie::script_api::input::ControllerTrigger) {
		
	}
	fn unbind_mouse_button(&mut self, action: String, button: bindings::faerie::script_api::input::MouseButton) {
		
	}
	fn unbind_mouse_motion(&mut self, action: String, source: bindings::faerie::script_api::input::MouseMotionSource) {
		
	}

	fn has_physical_scancode(&mut self, action: String, scancode: bindings::faerie::script_api::input::Scancode) -> bool {
		false
	}
	fn has_logical_keycode(&mut self, action: String, keycode: char) -> bool {
		false
	}
	fn has_controller_button(&mut self, action: String, button: bindings::faerie::script_api::input::ControllerButton) -> bool {
		false
	}
	fn has_controller_stick(&mut self, action: String, tick: bindings::faerie::script_api::input::ControllerStick) -> bool {
		false
	}
	fn has_controller_trigger(&mut self, action: String, stick: bindings::faerie::script_api::input::ControllerTrigger) -> bool {
		false
	}
	fn has_mouse_button(&mut self, action: String, button: bindings::faerie::script_api::input::MouseButton) -> bool {
		false
	}
	fn has_mouse_motion(&mut self, action: String, source: bindings::faerie::script_api::input::MouseMotionSource) -> bool {
		false
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

		bindings::faerie::script_api::ecs::add_to_linker(&mut linker, |state: &mut ScriptState| state)?;

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
		let mut store = Store::<ScriptState>::new(&engine.engine, ScriptState{
			ctx: wasi, resource_table: ResourceTable::new(), namespace: String::new()
		});
		bindings::ScriptAll::instantiate(&mut store, &component, &engine.linker)?;

		let instance = engine.linker.instantiate(&mut store, &component)?;

		let mut script = Script{ instance, store };
		script.get_interface_export("faerie:script-api/script-lifecycle", "namespace")
			.and_then(|func| script.call::<(),(String,)>(func, ()))
			.inspect(|ret| script.store.data_mut().namespace = String::from(ret.0.as_str()) );

		//println!("{}", debug::component_item_to_string(ComponentItem::Component(component.component_type()), &engine.engine));

		Ok(script)
	}

	fn get_export(&mut self, root: Option<ComponentExportIndex>, name: &str) -> Option<ComponentExportIndex> {
		self.instance.get_export(&mut self.store, root.as_ref(), name)
	}

	fn get_interface_export(&mut self, interface_name: &str, name: &str) -> Option<ComponentExportIndex> {
		let interface = self.get_export(None, interface_name);
		self.get_export(interface, name)
	}

	fn call<Params, Return>(&mut self, func_export: ComponentExportIndex, params: Params) -> Option<Return>
	where Params: ComponentNamedList + Lower, Return: ComponentNamedList + Lift {
		let func_untyped = self.instance.get_func(&mut self.store, func_export);
		match func_untyped { None => eprintln!("Export is not valid function"), _ => () }

		let func = func_untyped.and_then(|func| func.typed::<Params, Return>(&self.store)
			.inspect_err(|err|eprintln!("Error while getting typed func: {err}"))
			.ok());
		
		func.and_then(|func|{
			let result = func.call(&mut self.store, params)
				.inspect_err(|err|eprintln!("Error during call: {err}"))
				.ok();
			let _ = func.post_return(&mut self.store).inspect_err(|err|eprintln!("Error during post_return: {err}"));
			result
		})
	}

	pub fn init(&mut self) {
		self.get_interface_export("faerie:script-api/script-lifecycle", "init")
			.and_then(|func| self.call::<(),()>(func, ()));
	}

	pub fn process(&mut self, delta: f64) {
		self.get_interface_export("faerie:script-api/process-hook", "process")
			.and_then(|func| self.call::<(f64,),()>(func, (delta,)));
	}

	pub fn on_input_press(&mut self, action: &str, pressed: bool) {
		let func = self.get_interface_export("faerie:script-api/input-listen", "on-input");
		if func.is_none() {
			println!("faerie:script-api/input-listen::on-input not found");
		}
		func.inspect(|func| {
			use bindings::exports::faerie::script_api::input_listen;
			let input_event = input_listen::InputEvent::Press(pressed);
			self.call::<(String,input_listen::InputEvent,),()>(*func, (String::from(action), input_event));
		});
	}

	pub fn on_input_motion(&mut self, action: &str, x: f32, y: f32) {
		let func = self.get_interface_export("faerie:script-api/input-listen", "on-input");
		if func.is_none() {
			println!("faerie:script-api/input-listen::on-input not found");
		}
		func.inspect(|func| {
			use bindings::exports::faerie::script_api::input_listen;
			let input_event = input_listen::InputEvent::Motion(input_listen::Vec2{x,y});
			self.call::<(String,input_listen::InputEvent,),()>(*func, (String::from(action), input_event));
		});
	}

}

pub fn create_script_engine() -> Box<ScriptEngine> {
	Box::new(ScriptEngine::default())
}