project "EraSandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "off"

    files { "src/**.h", "src/**.cpp" }

    includedirs
    {
        "src",
         "../DeSerializer/src",
         "../DeSerializer/vendor/json"

    }
    libdirs{

	}

    links
    {
      "DeSerializer"

        -- "zlib.lib"
    }

    

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
 
    filter "system:windows"
       systemversion "latest"
       characterset "MBCS"
       defines { "NV_PLATFORM_WINDOWS" }
 
    filter "configurations:Debug"
       defines { "NV_DEBUG" }
       runtime "Debug"
       symbols "On"
       staticruntime "On"
 
    filter "configurations:Release"
       defines { "NV_RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
       staticruntime "On"
 
    filter "configurations:Dist"
       kind "WindowedApp"
       defines { "NV_DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       staticruntime "On"