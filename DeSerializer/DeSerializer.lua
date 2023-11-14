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
      "vendor/json",
      "vendor/zlib-1.2.11/include",
      "vendor/cryptopp890"
   }

   libdirs{
      "vendor/zlib-1.2.11/lib",
      "vendor/cryptopp890/lib/Debug"
	}

   links
   {
      "cryptlib.lib",
      "zlib.lib"
   }

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      characterset "MBCS"
      defines { "ERA_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "ERA_DEBUG" }
      runtime "Debug"
      symbols "On"
      staticruntime "On"

   filter "configurations:Release"
      defines { "ERA_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"
      staticruntime "On"

   filter "configurations:Dist"
      defines { "ERA_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      staticruntime "On"