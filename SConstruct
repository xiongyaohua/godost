#!/usr/bin/env python

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# Incoporate doc
if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# Rust part
rustlib = env.Command("target/debug/libstub.a", [Glob("/cargos/**/*.rs"), Glob("cargos/**/*.toml")], "cargo build")
#AlwaysBuild(rustlib)

# 追加而不是替代
env.Append(LIBS=rustlib)

lib_name = "godost"
# C++ part
if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData(
        "src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
    sources.append(doc_data)

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "project/bin/lib{}.{}.{}.framework/libgdexample.{}.{}".format(
            lib_name, env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "project/bin/lib{}.{}.{}.simulator.a".format(
                lib_name, env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "project/bin/lib{}.{}.{}.a".format(
                lib_name, env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "project/bin/lib{}{}{}".format(
            lib_name, env["suffix"], env["SHLIBSUFFIX"]),
        source=sources
    )

env.NoCache(library)
Default(library)
