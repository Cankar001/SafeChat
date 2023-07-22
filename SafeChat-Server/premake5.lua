project "SafeChat-Server"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	entrypoint "mainCRTStartup"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files { "src/**.h", "src/**.cpp" }

	includedirs
	{
	  "src",
	  
	  -- Core library
	  "%{includedir.safechat_core}",
	  
	  -- Steam networking library
	  "%{includedir.steam_networking}",
	}

	links
	{
		"SafeChat-Core"
	}

	filter "system:windows"
	  systemversion "latest"
	  defines { "SAFE_PLATFORM_WINDOWS" }

	filter "system:linux"
	  defines { "SAFE_PLATFORM_LINUX" }

	filter "configurations:Debug"
	  defines { "SAFE_DEBUG" }
	  runtime "Debug"
	  symbols "On"

	filter "configurations:Release"
	  defines { "SAFE_RELEASE" }
	  runtime "Release"
	  optimize "On"
	  symbols "On"
	  
	  