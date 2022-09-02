workspace "olcCodeJam2022"
	architecture "x86"
	configurations { "Debug", "Release" }
	startproject "olcCodeJam2022"
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLEW"] = "%{wks.location}/olcCodeJam2022/vendor/GLEW/include"
	IncludeDir["GLFW"] = "%{wks.location}/olcCodeJam2022/vendor/GLFW/include"
	IncludeDir["stb_image"] = "%{wks.location}/olcCodeJam2022/vendor/stb_image"
	IncludeDir["glm"] = "%{wks.location}/olcCodeJam2022/vendor/glm"
	IncludeDir["FreeType"] = "%{wks.location}/olcCodeJam2022/vendor/FreeType/include"
	
	group "Dependencies"
	include "olcCodeJam2022/vendor/GLEW"
	include "olcCodeJam2022/vendor/GLFW"
	include "olcCodeJam2022/vendor/FreeType"
	group ""

	include "olcCodeJam2022"
