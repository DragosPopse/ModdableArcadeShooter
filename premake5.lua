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
        "%{prj.location}/include/**.hpp",
        "%{prj.location}/rsc/resources.rc"
    }
    includedirs
    {
        "%{prj.location}/include"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "off"
        systemversion "latest"

        postbuildcommands 
        {
            "copy /y %{cfg.architecture}\\*.dll bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\ArcadeAirship\\*.dll",
            "Xcopy /E /I /y assets bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\ArcadeAirship\\assets"
        }

        buildoptions { "/MP" }
        warnings "Extra"

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
            "lib/x86_64"
        }
        debugenvs 
        {
            "PATH=%PATH%;$(ProjectDir)/x86_64"
        }
        buildoptions { "/bigobj" }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
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
        runtime "Release"
        links 
        {
            "sfml-system",
            "sfml-window",
            "sfml-graphics",
            "sfml-audio",
            "thor",
            "tgui"
        }

