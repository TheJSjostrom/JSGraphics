workspace "JSGraphics"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/src"
	}
		
	links
	{
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