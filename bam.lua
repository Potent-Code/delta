-- important paths
sourcePath = "src/"
implPath = sourcePath .. "impl/"
outputPath = ".build/"
intermediatePath = outputPath .. "intermediates/"
libraryOutputPath = outputPath .. "usr/local/lib/"
executableOutputPath = outputPath .. "usr/local/bin/"
includeOutputPath = outputPath .. "usr/local/include/potent-code/"

-- common settings for all Delta outputs
function NewDeltaSettings()
	local delta_settings = NewSettings()

	-- use clang (TODO: bootstrap this properly with compiler detection)
	delta_settings.cc.exe_c = "clang"
	delta_settings.cc.exe_cxx = "clang++"
	delta_settings.link.exe = "clang"
	delta_settings.dll.exe = "clang"
	
	-- full warning level, treat warnings as errors
	delta_settings.cc.flags:Add("-Wall -Wextra -Werror")
	-- use fPIC for making a shared object
	delta_settings.cc.flags:Add("-fPIC")
	
	-- set up include path
	delta_settings.cc.includes:Add(sourcePath .. "include", "/usr/local/include")
	
	-- version define (TODO: improve via gitflow/semver)
	delta_settings.cc.defines:Add("PACKAGE_VERSION=\\\"0.0.1\\\"")
	
	-- output objects to intermediate directory
	delta_settings.cc.Output = function(delta_settings, input)
		return intermediatePath .. string.gsub(PathBase(input), "src/", "", 1)
	end

	-- output shared libraries to library output path
	delta_settings.dll.Output = function(delta_settings, input)
		return libraryOutputPath .. input
	end

	-- output static libraries to library output path
	delta_settings.lib.Output = function(delta_settings, input)
		return libraryOutputPath .. input
	end

	-- output executables to executable output path
	delta_settings.link.Output = function(delta_settings, input)
		return executableOutputPath .. input
	end

	-- add libpath
	delta_settings.dll.libpath:Add("/usr/local/lib")
	delta_settings.link.libpath:Add("/usr/local/lib")
	
	-- linker flags
	return delta_settings
end

-- collect sources and compile
settings = NewDeltaSettings()

delta_objects = Compile(settings, Collect(implPath .. "*.c"))

-- make shared library libdelta.so
libdelta = SharedLibrary(settings, "libdelta", delta_objects)
-- make static library libdelta.a
libdelta_static = StaticLibrary(settings, "delta", delta_objects)

-- copy library header
AddJob(includeOutputPath .. "delta.h", "h "..includeOutputPath.."delta.h", "cp -f " .. sourcePath .. "include/delta.h " .. includeOutputPath, sourcePath .. "include/delta.h")
