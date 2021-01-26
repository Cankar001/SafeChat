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
		'{COPY} "assets" "%{cfg.targetdir}/assets"'
		}
		
	filter "system:macosx"
		systemversion "latest"
		defines "PLATFORM_MACOS"
				
		filter "configurations:Debug"
			defines "_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "_RELEASE"
			runtime "Release"
			optimize "on"
				
	filter "system:linux"
		systemversion "latest"
		defines "PLATFORM_LINUX"
			
		filter "configurations:Debug"
			defines "_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "_RELEASE"
			runtime "Release"
			optimize "on"
			
	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"
			
		filter "configurations:Debug"
			defines "_DEBUG"
			runtime "Debug"
			symbols "on"
		
		filter "configurations:Release"
			defines "_RELEASE"
			runtime "Release"
			optimize "on"