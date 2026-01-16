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
IncludeDir["glm"] = "JSGraphics/vendor/glm"
IncludeDir["stb_image"] = "JSGraphics/vendor/stb_image"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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