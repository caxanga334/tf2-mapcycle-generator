--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local todir = "build/" .. action

workspace "tf2-mapcycle-generator"
    configurations { 
        "Debug",
        "Release"
    }

    platforms { 
        "Win64",
        "Linux64"
    }

    location (todir)

    symbols "On"
	exceptionhandling "On"
	rtti "On"
    stringpooling "On"
    omitframepointer "Off"
    vectorextensions "AVX2"
    pic "On"

    filter { "system:Windows" }
        defines { "_CRT_SECURE_NO_DEPRECATE", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "_WINDOWS", "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING" }
        flags { "MultiProcessorCompile" }
        characterset "MBCS"
        staticruntime "on"
        editandcontinue "off"
        -- Disable macro redefinition warnings
        disablewarnings { "4005"}
        links {
            "kernel32.lib",
            "user32.lib",
            "legacy_stdio_definitions.lib",
        }

    filter { "system:Linux" }
        defines {  "_LINUX" }
        defines { "stricmp=strcasecmp", "_stricmp=strcasecmp", "_snprintf=snprintf", "_vsnprintf=vsnprintf", "HAVE_STDINT_H", "GNUC" }

        -- disable prefixes for Linux
        targetprefix ""

    filter { "configurations:Debug" }
        defines { "DEBUG", "_DEBUG", "_ITERATOR_DEBUG_LEVEL=2" }
        targetdir "build/bin/%{cfg.architecture}/debug"
        optimize "Off"
		runtime "Debug"

    filter { "configurations:Release" }
        defines { "NDEBUG", "_RELEASE", "_ITERATOR_DEBUG_LEVEL=0" }
        targetdir "build/bin/%{cfg.architecture}/release"
        optimize "Full"
        flags { "LinkTimeOptimization" }
		visibility "Hidden"
		runtime "Release"

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"
        defines { "WIN64", "COMPILER_MSVC", "COMPILER_MSVC64", "X64BITS", "PLATFORM_64BITS" }

    filter { "platforms:Linux64" }
        system "Linux"
        architecture "x86_64"
        defines { "X64BITS", "PLATFORM_64BITS" }

project "tf2-mapcycle-generator"
    language "C++"
    kind "ConsoleApp"
    cppdialect "C++20"
    targetname "tf2-mapcycle-generator"

	includedirs { 
		"./includes",
        "./libs/ValveFileVDF/include",
	}
	files {
		"./libs/ValveFileVDF/include/**.hpp",
        "./includes/**.h",
        "./sources/**.cpp",
	}

