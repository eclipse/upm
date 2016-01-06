install (DIRECTORY . DESTINATION include/upm FILES_MATCHING PATTERN "*.h")
if (BUILDSWIGJAVA AND BUILDSWIG)
	set (libname "interfaces")
	set (libdescription "upm interfaces")
	set (module_src ${libname}.cxx)
#	set (module_h ${libname}.h)
	upm_module_init()
endif()

   
