include "./vendor/premake5/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

includedir = {}
includedir["asio"] = "%{wks.location}/vendor/asio/include"
includedir["safenet_api"] = "%{wks.location}/SafeNetworkAPI/src"

librarydir = {}

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
	include "SafeNetworkAPI"
	group ""
	
	include "SafeChatServer"
	include "SafeChatDesktopClient"
	
	group "tests"
	include "tests/"
	group ""
	
	