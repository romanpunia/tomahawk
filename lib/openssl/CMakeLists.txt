#Resolve headers, refs and linkage
set(TH_USE_OPENSSL ON CACHE BOOL "Enable OpenSSL library")
if (TH_USE_OPENSSL)
	find_path(OpenSSL_LOCATION openssl/aes.h)
	if (OpenSSL_LOCATION)
		target_include_directories(tomahawk PRIVATE ${OpenSSL_LOCATION})
		find_package(OpenSSL QUIET)
		if (NOT OpenSSL_FOUND)
			find_library(OpenSSL_FOUND "ssl")
			find_library(OpenCRYPTO_FOUND "crypto")
			if (OpenSSL_FOUND AND OpenCRYPTO_FOUND)
				set(OPENSSL_LIBRARIES "${OpenSSL_FOUND};${OpenCRYPTO_FOUND}")
				target_compile_definitions(tomahawk PRIVATE -DTH_HAS_OPENSSL)
			endif()
		else()
			target_compile_definitions(tomahawk PRIVATE -DTH_HAS_OPENSSL)
		endif()
		if (OpenSSL_FOUND OR OPENSSL_LIBRARIES)
			if (DEFINED OPENSSL_LIBRARIES)
				string(STRIP "${OPENSSL_LIBRARIES}" OPENSSL_LIBRARIES)
				target_link_libraries(tomahawk PRIVATE ${OPENSSL_LIBRARIES})
				message(STATUS "OpenSSL library found")
			else()
				target_link_libraries(tomahawk PRIVATE OpenSSL::SSL OpenSSL::Crypto)
				message(STATUS "OpenSSL target library found")
			endif()
		else()
			message("OpenSSL library was not found")
		endif()
	else()
		message("OpenSSL library was not found")
	endif()
endif()

#Cleanup
unset(OpenSSL_LOCATION CACHE)
unset(OpenSSL_FOUND CACHE)
unset(OPENSSL_LIBRARIES CACHE)
unset(OpenCRYPTO_FOUND CACHE)
unset(LIB_EAY_DEBUG CACHE)
unset(LIB_EAY_RELEASE CACHE)
unset(SSL_EAY_DEBUG CACHE)
unset(SSL_EAY_RELEASE CACHE)