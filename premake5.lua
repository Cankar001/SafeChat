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
	
	include "SafeChatServer"
	
	group "Dependencies"
	group ""
	
	group "tests"
	include "tests/"
	group ""
	
	