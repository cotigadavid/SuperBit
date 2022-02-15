 workspace "SuperBit"
	architecture "x64"
	startproject "SuperBit"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "SuperBit/vendor/GLFW/include"
IncludeDir["Glad"] = "SuperBit/vendor/Glad/include"
IncludeDir["glm"] = "SuperBit/vendor/glm"
IncludeDir["stb_image"] = "SuperBit/vendor/stb_image"

include "SuperBit/vendor/GLFW"
include "SuperBit/vendor/Glad"


project "SuperBit"
	location "SuperBit"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sbpch.h"
	pchsource "SuperBit/src/sbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
		
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"Glad"
		
	}


	filter "system:windows"
	           systemversion "latest"
            	staticruntime "On"

		defines
		{
			"SB_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "PS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PS_DIST"
		runtime "Release"
		optimize "on"