name = "lwss"
workspace (name)
    language "C++"
    location "build"
    warnings "Extra"
    cppdialect "C++14"
    configurations {"Debug", "Release"}

    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "Off"
        symbols "On"

    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "Speed"
        symbols "Off"

project (name)
    targetdir "bin"
    kind "WindowedApp"
    files {"src/main.cc"}
    files {"foreign/src/**.c"}
    files {"foreign/src/**.cpp"}
    files {"src/"..name.."/**.cc"}
    includedirs {"foreign/include"}
    includedirs {"include"}

    filter {"system:windows"}
        links {"glew3dll", "opengl32"}
        buildoptions {"-static -static-libgcc -static-libstdc++",
                      "-mwindows", "-mconsole"}
    filter {"system:linux or bsd or macosx"}
        links {"glfw", "GL", "dl"}
