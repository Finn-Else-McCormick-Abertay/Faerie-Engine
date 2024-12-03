project(
  'cxx',
  'rust',
  version : '1.0.131',
  meson_version : '>= 1.5.0',
  default_options : ['rust_std=2021'],
  license : 'MIT OR Apache-2.0'
)
rust = import('rust')
features = {}
cxxbridge_flags_options = {}
foldhash_options = {}
link_cplusplus_options = {}
required_deps = {}
if get_option('feature-default')
  required_deps += {'foldhash' : true, 'cxxbridge-flags' : true}
  features += {'std' : true, 'alloc' : true, 'default' : true}
  foldhash_options += {'feature-std' : true}
  cxxbridge_flags_options += {'feature-default' : true}
endif
if get_option('feature-cpp14')
  required_deps += {'cxxbridge-flags' : true}
  features += {'cpp14' : true}
  cxxbridge_flags_options += {'feature-cpp14' : true}
endif
if get_option('feature-cpp17')
  required_deps += {'cxxbridge-flags' : true}
  features += {'c++17' : true}
  cxxbridge_flags_options += {'feature-cpp17' : true}
endif
if get_option('feature-cpp20')
  required_deps += {'cxxbridge-flags' : true}
  features += {'c++20' : true}
  cxxbridge_flags_options += {'feature-cpp20' : true}
endif
if get_option('feature-alloc')
  required_deps += {}
  features += {'alloc' : true}
endif
if get_option('feature-std')
  required_deps += {'foldhash' : true}
  features += {'std' : true, 'alloc' : true}
  foldhash_options += {'feature-std' : true}
endif
message(
  'Enabled features:',
  features.keys()
)

cxxbridge_macro_options = {'feature-default' : true}

proc_macro2_dep = dependency(
  'proc-macro2-1-rs',
  version : ['>= 1.0.74', '< 2'],
  #default_options: cxxbridge_macro_options
)

quote_dep = dependency(
  'quote-1-rs',
  version : ['>= 1.0.35', '< 2'],
  #default_options: cxxbridge_macro_options
)

rustversion_dep = dependency(
  'rustversion-1-rs',
  version : ['>= 1', '< 2']
)

syn_dep = dependency(
  'syn-2-rs',
  version : ['>= 2.0.46', '< 3'],
  #default_options: { 'full': true }
)


cxxbridge_macro_lib = rust.proc_macro('cxxbridge_macro',
    sources: [
      'macro/src/lib.rs', 'macro/src/clang.rs', 'macro/src/derive.rs', 'macro/src/expand.rs', 'macro/src/generics.rs', 'macro/src/load.rs', 'macro/src/tokens.rs', 'macro/src/type_id.rs',
      'syntax/atom.rs', 'syntax/attrs.rs', 'syntax/cfg.rs', 'syntax/check.rs', 'syntax/derive.rs', 'syntax/discriminant.rs', 'syntax/doc.rs', 'syntax/error.rs', 'syntax/file.rs', 'syntax/ident.rs',
      'syntax/impls.rs', 'syntax/improper.rs', 'syntax/instantiate.rs', 'syntax/mangle.rs', 'syntax/map.rs', 'syntax/mod.rs', 'syntax/names.rs', 'syntax/namespace.rs', 'syntax/parse.rs', 'syntax/pod.rs',
      'syntax/qualified.rs', 'syntax/report.rs', 'syntax/resolve.rs', 'syntax/report.rs', 'syntax/resolve.rs', 'syntax/set.rs', 'syntax/symbol.rs', 'syntax/tokens.rs', 'syntax/toposort.rs', 'syntax/trivial.rs',
      'syntax/types.rs', 'syntax/visit.rs'
    ],
    dependencies: [ proc_macro2_dep, quote_dep, rustversion_dep, syn_dep ]
)
cxxbridge_macro_dep = declare_dependency(
  link_with: cxxbridge_macro_lib
)

foldhash_options += {'feature-default' : false}
foldhash_dep = dependency(
  'foldhash-0.1-rs',
  version : ['>= 0.1', '< 0.2'],
  #default_options : foldhash_options
)
if foldhash_dep.found()
  actual_features = foldhash_dep.get_variable('features', default_value : '').split(',')
  needed_features = []
  foreach f, enabled : foldhash_options
    if enabled
      needed_features += f.substring(8)
    endif
  endforeach
  foreach f : needed_features
    if f not in actual_features
      error(
        'Dependency',
        'foldhash-0.1-rs',
        'previously configured with features',
        actual_features,
        'but need',
        needed_features
      )
    endif
  endforeach
endif
link_cplusplus_options += {'feature-default' : true}
link_cplusplus_dep = dependency(
  'link-cplusplus-1-rs',
  version : ['>= 1.0.9', '< 2'],
  #default_options : link_cplusplus_options
)
if link_cplusplus_dep.found()
  actual_features = link_cplusplus_dep.get_variable('features', default_value : '').split(',')
  needed_features = []
  foreach f, enabled : link_cplusplus_options
    if enabled
      needed_features += f.substring(8)
    endif
  endforeach
  foreach f : needed_features
    if f not in actual_features
      error(
        'Dependency',
        'link-cplusplus-1-rs',
        'previously configured with features',
        actual_features,
        'but need',
        needed_features
      )
    endif
  endforeach
endif
extra_args = []
extra_deps = []
fs = import('fs')
if fs.is_dir('meson')
  subdir('meson')
endif
features_args = []
foreach f, _ : features
  features_args += [
    '--cfg',
    'feature="' + f + '"'
  ]
endforeach
lib = static_library(
  'cxx',
  'src\\lib.rs',
  dependencies : [cxxbridge_macro_dep, foldhash_dep, link_cplusplus_dep, extra_deps],
  rust_dependency_map : {},
  rust_args : [features_args, extra_args]
)
dep = declare_dependency(
  link_with : lib,
  variables : {
    'features' : ','.join(
      features.keys()
    )
  },
  include_directories: [ 'include' ]
)
meson.override_dependency('cxx-1-rs', dep)
