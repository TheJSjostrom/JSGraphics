workspace "JSGraphics"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "JSGraphics/vendor/GLFW/include"
IncludeDir["Glad"] = "JSGraphics/vendor/Glad/include"
IncludeDir["ImGui"] = "JSGraphics/vendor/imgui"

include "JSGraphics/vendor/GLFW"
include "JSGraphics/vendor/Glad"
include "JSGraphics/vendor/imgui"

project "JSGraphics"
	location "JSGraphics"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}
		
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	filter "system:window"
		cppdialect "C++latest"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG" 
		symbols "On"

	filter "configurations:Release" 
		defines "NDEBUG" 
		optimize "On"