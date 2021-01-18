include "./vendor/premake5/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

includedir                = {}
librarydir                = {}

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
	group ""
	
	group "Servers"
	include "SafeChatServer"
	group ""
	
	group "Clients"
	include "SafeChatDesktopClient"
	group ""
	
	group "tests"
	include "tests/"
	group ""
	
	