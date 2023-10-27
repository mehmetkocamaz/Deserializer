-- Build.lua
workspace "EraTools"
    architecture "x64"
    configurations {"Debug","Release","Dist"}
    startproject "EraSandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "EraSandbox/EraSandbox.lua"
include "DeSerializer/DeSerializer.lua"