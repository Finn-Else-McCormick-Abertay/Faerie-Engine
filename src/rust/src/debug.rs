use wasmtime::{component::{types::ComponentItem, Type}, Engine};

fn component_type_to_string(ty: Type) -> String {
	match ty {
		Type::Bool => "bool".to_string(),
		Type::S8 => "i8".to_string(),
		Type::U8 => "u8".to_string(),
		Type::S16 => "i16".to_string(),
		Type::U16 => "u16".to_string(),
		Type::S32 => "i32".to_string(),
		Type::U32 => "u32".to_string(),
		Type::S64 => "i64".to_string(),
		Type::U64 => "u64".to_string(),
		Type::Float32 => "f32".to_string(),
		Type::Float64 => "f64".to_string(),
		Type::Char => "char".to_string(),
		Type::String => "string".to_string(),
		Type::List(list) => {
			"list<".to_string() + &component_type_to_string(list.ty()) + ">"
		}
		Type::Tuple(tuple) => {
			let mut output_string = "tuple<".to_string();
			for (i, ty) in tuple.types().enumerate() {
				output_string += &component_type_to_string(ty);
				if (i + 1) < tuple.types().len() {
					output_string += ", ";
				}
			}
			output_string
		}
		Type::Record(record) => {
			let mut output_string = "record {\n".to_string();
			for (i, field) in record.fields().enumerate() {
				output_string += "\t";
				output_string += field.name;
				output_string += ": ";
				let field_string = component_type_to_string(field.ty);
				for (i, line) in field_string.lines().enumerate() {
					output_string += line;
					output_string += "\n";
					if (i + 1) < field_string.lines().count() {
						output_string += "\t";
					}
				}
				if (i + 1) < record.fields().len() {
					output_string += ",";
				}
				output_string += "\n";
			}
			output_string += "}";
			output_string
		}
		Type::Variant(variant) => {
			let mut output_string = "variant {\n".to_string();
			for (i, case) in variant.cases().enumerate() {
				output_string += "\t";
				output_string += case.name;
				match case.ty {
					Some(ty) => {
						output_string += "(";
						output_string += &component_type_to_string(ty);
						output_string += ")";
					}
					None => ()
				}
				if (i + 1) < variant.cases().len() {
					output_string += ",";
				}
				output_string += "\n";
			}
			output_string += "}";
			output_string
		}
		Type::Option(option_type) => {
			"option<".to_string() + &component_type_to_string(option_type.ty()) + ">"
		}
		Type::Result(result_type) => {
			let mut output_string = "result<".to_string();
			match result_type.ok() {
				Some(ty) => output_string += &component_type_to_string(ty),
				None => output_string += "_"
			}
			output_string += ", ";
			match result_type.err() {
				Some(ty) => output_string += &component_type_to_string(ty),
				None => output_string += "_"
			}
			output_string += ">";
			output_string
		}
		Type::Enum(en) => {
			let mut output_string = "enum {\n".to_string();
			for (i, name) in en.names().enumerate() {
				output_string += "\t";
				output_string += name;
				if (i + 1) < en.names().len() {
					output_string += ",";
				}
				output_string += "\n";
			}
			output_string += "}";
			output_string
		}
		Type::Flags(flags) => {
			let mut output_string = "flags {\n".to_string();
			for (i, name) in flags.names().enumerate() {
				output_string += "\t";
				output_string += name;
				if (i + 1) < flags.names().len() {
					output_string += ",";
				}
				output_string += "\n";
			}
			output_string += "}";
			output_string
		}
		Type::Own(_resource_type) => {
			"own<?>".to_string()
		}
		Type::Borrow(_resource_type) => {
			"borrow<?>".to_string()
		}
	}
}


pub fn component_item_to_string(item: ComponentItem, engine: &Engine) -> String {
	match item {
		ComponentItem::ComponentFunc(func) => {
			let mut output_string = "func(".to_string();
			for (i, ty) in func.params().enumerate() {
				output_string += &component_type_to_string(ty);
				if (i + 1) < func.params().len() {
					output_string += ", ";
				}				
			}
			output_string += ")";
			if func.results().len() > 0 {
				output_string += " -> (";
				for (i, ty) in func.results().enumerate() {
					output_string += &component_type_to_string(ty);
					if (i + 1) < func.params().len() {
						output_string += ", ";
					}				
				}
				output_string += ")";
			}
			output_string
		}
		ComponentItem::CoreFunc(_func) => {
			"[core]func".to_string()
		}
		ComponentItem::Module(_module) => {
			"module".to_string()
		}
		ComponentItem::Component(component) => {
			let mut output_string = "component {".to_string();
			for (name, item) in component.imports(&engine) {
				output_string += "import ";
				output_string += name;
				output_string += ": ";
				let item_string = component_item_to_string(item, engine);
				for (i, line) in item_string.lines().enumerate() {
					output_string += line;
					if (i + 1) < item_string.lines().count() {
						output_string += "\n\t";
					}
				}
				output_string += ", ";
				output_string += "\n";
			}
			for (name, item) in component.exports(&engine) {
				output_string += "export ";
				output_string += name;
				output_string += ": ";
				let item_string = component_item_to_string(item, engine);
				for (i, line) in item_string.lines().enumerate() {
					output_string += line;
					if (i + 1) < item_string.lines().count() {
						output_string += "\t";
					}
				}
				output_string += ",\n";
			}
			output_string += "}";
			output_string
		}
		ComponentItem::ComponentInstance(instance) => {
			let mut output_string = "interface {\n".to_string();
			for (i, (name, item)) in instance.exports(&engine).enumerate() {
				output_string += "export ";
				output_string += name;
				output_string += ": ";
				let item_string = component_item_to_string(item, engine);
				for (i, line) in item_string.lines().enumerate() {
					output_string += line;
					if (i + 1) < item_string.lines().count() {
						output_string += "\n\t";
					}
				}
				if (i + 1) < instance.exports(&engine).len() {
					output_string += ", ";
				}
				output_string += "\n";
			}
			output_string += "}";
			output_string
		}
		ComponentItem::Type(ty) => {
			component_type_to_string(ty)
		}
		ComponentItem::Resource(_resource) => {
			"resource".to_string()
		}
	}
}