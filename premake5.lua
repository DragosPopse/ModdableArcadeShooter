workspace "ArcadeAirship"
    configurations
    {
        "Debug32",
        "Release32",
        "Debug64",
        "Release64"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ArcadeAirship"
    kind "ConsoleApp"
    language "C++"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/include/**.h",
        "%{prj.location}/include/**.hpp"
    }
    includedirs
    {
        "%{prj.location}/include"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "_USE_MATH_DEFINES"
        }

    filter "configurations:Debug32"
        architecture "x86"
        symbols "On"
        links 
        {
            "sfml-system-d",
            "sfml-window-d",
            "sfml-graphics-d",
            "sfml-audio-d",
            "lua535_x86",
            "thor-d",
            "tgui-d"
        }
        libdirs
        {
            "lib/x86"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x86"
        }

    filter "configurations:Release32"
        architecture "x86"
        optimize "Full"
        links 
        {
            "sfml-system",
            "sfml-window",
            "sfml-graphics",
            "sfml-audio",
            "lua535_x86",
            "thor",
            "tgui"
        }
        libdirs
        {
            "lib/x86"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x86"
        }

        filter "configurations:Debug64"
        architecture "x86_64"
        symbols "On"
        links 
        {
            "sfml-system-d",
            "sfml-window-d",
            "sfml-graphics-d",
            "sfml-audio-d",
            "lua535_x64",
            "thor-d",
            "tgui-d"
        }
        libdirs
        {
            "lib/x64"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x64"
        }
        buildoptions { "/bigobj" }

    filter "configurations:Release64"
        architecture "x86_64"
        optimize "Full"
        links 
        {
            "sfml-system",
            "sfml-window",
            "sfml-graphics",
            "sfml-audio",
            "lua535_x64",
            "thor",
            "tgui"
        }
        libdirs
        {
            "lib/x64"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x64"
        }