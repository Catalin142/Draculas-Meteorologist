project "olcCodeJam2022"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"%{wks.location}/olcCodeJam2022/src",
		"%{wks.location}/olcCodeJam2022/vendor",
	}

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/**.hpp",
		"vendor/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"AL_LIBTYPE_STATIC"
	}

	includedirs
	{
		"vendor;",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"vendor/stb_image",
		"vendor/FreeType/include",
		"src;",
		"vendor/glm",
	}

	links 
	{
		"GLFW",
		"GLEW",
		"FreeType",
		"opengl32.lib",
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG=1"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE=1"
		runtime "Release"
		optimize "on"