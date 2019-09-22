workspace "ArcadeAirship"
    architecture "x86"

    configurations
    {
        "Debug",
        "Release"
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

    links 
    {
        "sfml-system-d",
        "sfml-window-d",
        "sfml-graphics-d",
        "sfml-audio-d"
    }
    libdirs
    {
        "lib"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "_USE_MATH_DEFINES"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"