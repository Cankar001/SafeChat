project "SafeChat-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	files { "src/**.h", "src/**.cpp" }

	includedirs
	{
	  "src",
	  
	  -- Steam networking library
	  "%{includedir.steam_networking}",
	}

	filter "system:windows"
	  systemversion "latest"
	  defines { "SAFE_PLATFORM_WINDOWS" }
	  links { "Ws2_32.lib" }

	filter "system:linux"
	  defines { "SAFE_PLATFORM_LINUX" }

	filter { "system:windows", "configurations:Debug" }	
		links
		{
			"%{librarydir.steam_networking_windows_debug}",
		}

	filter { "system:windows", "configurations:Release or configurations:Dist" }	
		links
		{
			"%{librarydir.steam_networking_windows}",
		}

	filter "configurations:Debug"
	  defines { "SAFE_DEBUG" }
	  runtime "Debug"
	  symbols "On"

	filter "configurations:Release"
	  defines { "SAFE_RELEASE" }
	  runtime "Release"
	  optimize "On"
	  symbols "On"
	  
	  