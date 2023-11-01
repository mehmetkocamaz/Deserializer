-- Build.lua
workspace "EraTools"
    architecture "x64"
    configurations {"Debug","Release","Dist"}
    startproject "EraSandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
   include "EraSandbox/vendor/imgui"
   include "EraSandbox/vendor/glfw"
group ""

include "EraSandbox/EraSandbox.lua"
include "DeSerializer/DeSerializer.lua"