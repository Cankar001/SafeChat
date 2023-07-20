include "./vendor/premake5/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

includedir = {}
includedir["safechat_core"]    = "%{wks.location}/SafeChat-Core/src"
includedir["steam_networking"] = "%{wks.location}/vendor/Steam/GameNetworkingSockets/include"

librarydir = {}
librarydir["steam_networking_windows_debug"] = "%{wks.location}/vendor/Steam/GameNetworkingSockets/bin/Windows/Debug/GameNetworkingSockets.lib"
librarydir["steam_networking_windows"] = "%{wks.location}/vendor/Steam/GameNetworkingSockets/bin/Windows/Release/GameNetworkingSockets.lib"

workspace "SafeChat"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
		
	solution_items
	{
		".config"
	}

	group "Dependencies"
	include "SafeChat-Core"
	group ""
	
	--group "examples"
	--include "examples/"
	--group ""
	
	include "SafeChat-Server"
	include "SafeChat-Client"
	
	