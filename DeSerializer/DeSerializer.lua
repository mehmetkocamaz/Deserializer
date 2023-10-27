project "DeSerializer"
kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      "vendor/json"
   }

   libdirs{

	}

   links
   {
   }

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
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
      defines { "NV_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      staticruntime "On"