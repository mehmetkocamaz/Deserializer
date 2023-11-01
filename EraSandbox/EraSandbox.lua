project "EraSandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "off"

    files { "src/**.h", "src/**.cpp" }

    VULKAN_SDK = os.getenv("VULKAN_SDK")

    IncludeDir = {}
    IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
    IncludeDir["glm"] = "vendor/glm"
    
    LibraryDir = {}
    LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
    
    Library = {}
    Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
    
    includedirs
    {
      "src",
      "../DeSerializer/src",
      "../DeSerializer/vendor/json",
      "vendor/imgui",
      "vendor/glfw/include",
      "vendor/stb_image",
      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}"
    }

    links
    {
      "DeSerializer",
      "ImGui",
      "GLFW",
      "%{Library.Vulkan}"
    }

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
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
       kind "WindowedApp"
       defines { "ERA_DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       staticruntime "On"

