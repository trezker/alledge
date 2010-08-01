lib_name = "alledge"
dependencies = {  }

solution (lib_name)
	configurations { "Debug", "Release" }

--[[	project (lib_name)
		kind "SharedLib"
		language "C++"
		location "build"
		files { "src/*.cpp" }
		targetdir "build/lib"
		includedirs { "../include" }
]]--
	project (lib_name)
		kind "StaticLib"
		language "C++"
		location "build"
		files { "src/*.cpp" }
		targetdir "build/lib"
		includedirs { "../include" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

	tests = os.matchfiles("tests/*.cpp")
	for index, name in pairs(tests) do
		sname = "test_"..name:sub(7, name:len()-4);
		project (sname)
			kind "ConsoleApp"
			language "C++"
			location "build"
			files { name }
--			includedirs { "../include" }
--			libdirs { "../lib" }
			links (lib_name)
			links (dependencies)
			targetdir "build/tests"
			postbuildcommands { "tests/"..sname }
	end

	ex_dependencies = { "GL","GLU","glut","allegro","allegro_image","allegro_font","allegro_ttf","allegro_primitives","allegro_dialog" }
	examples = os.matchfiles("examples/*.cpp")
	for index, name in pairs(examples) do
		sname = "example_"..name:sub(10, name:len()-4);
		project (sname)
			kind "ConsoleApp"
			language "C++"
			location "build"
			files { name }
--			includedirs { "../include" }
--			libdirs { "../lib" }
			links (lib_name)
			links (ex_dependencies)
			targetdir "build/examples"
--			postbuildcommands { "cd .. && build/examples/"..sname }

			configuration "Debug"
				defines { "DEBUG" }
				flags { "Symbols" }
	 
			configuration "Release"
				defines { "NDEBUG" }
				flags { "Optimize" }
	end

newoption {
   trigger     = "dir",
   value       = "path",
   description = "Choose a path to install dir",
}

newaction {
	trigger     = "install",
	description = "Install the software",
	execute = function ()
		-- copy files, etc. here
		os.mkdir(_OPTIONS["dir"].."lib/");
		files = os.matchfiles("build/lib/*")
		print ("Installing lib files to " .. _OPTIONS["dir"] .."lib/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."lib/")
		end
		os.mkdir(_OPTIONS["dir"].."include/"..lib_name.."/");
		files = os.matchfiles(lib_name.."/*")
		print ("Installing header files to " .. _OPTIONS["dir"] .."include/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."include/"..lib_name.."/")
		end
	end
}

if not _OPTIONS["dir"] then
   _OPTIONS["dir"] = "/usr/local/"
end

if not ("/" == _OPTIONS["dir"]:sub(_OPTIONS["dir"]:len())) then
	_OPTIONS["dir"] = _OPTIONS["dir"] .. "/"
end
