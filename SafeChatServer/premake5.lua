project "SafeChatServer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "mainCRTStartup"
	
	defines
		{
		"_CRT_SECURE_NO_WARNINGS"
		}
		
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
		{
		"src/**.c",
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp"
		}
		
	includedirs
		{
		"src",
		"%{includedir.safenet_api}",
		"%{includedir.asio}"
		}
		
	postbuildcommands
		{
		}
		
	filter "system:macosx"
		systemversion "latest"
		defines "SAFE_PLATFORM_MACOS"
				
		filter "configurations:Debug"
			defines "SAFE_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "SAFE_RELEASE"
			runtime "Release"
			optimize "on"
				
	filter "system:linux"
		systemversion "latest"
		defines "SAFE_PLATFORM_LINUX"
			
		filter "configurations:Debug"
			defines "SAFE_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "SAFE_RELEASE"
			runtime "Release"
			optimize "on"
			
	filter "system:windows"
		systemversion "latest"
		defines "SAFE_PLATFORM_WINDOWS"
			
		filter "configurations:Debug"
			defines "SAFE_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "SAFE_RELEASE"
			runtime "Release"
			optimize "on"