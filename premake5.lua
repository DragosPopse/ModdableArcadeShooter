workspace "ArcadeAirship"
    configurations
    {
        "Debug",
        "Release"
    }
    platforms
    {
        "Win32",
        "Win64"
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

    filter "platforms:Win32"
        architecture "x86"
        links
        {
            "lua535_x86"
        }
        libdirs
        {
            "lib/x86"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x86"
        }
    
    filter "platforms:Win64"
        architecture "x86_64"
        links
        {
            "lua535_x64"
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

    filter "configurations:Debug"
        symbols "On"
        links 
        {
            "sfml-system-d",
            "sfml-window-d",
            "sfml-graphics-d",
            "sfml-audio-d",
            "thor-d",
            "tgui-d"
        }

    filter "configurations:Release"
        optimize "Full"
        links 
        {
            "sfml-system",
            "sfml-window",
            "sfml-graphics",
            "sfml-audio",
            "thor",
            "tgui"
        }

