if (WIN32)
	#Resolve source targets
	file(GLOB_RECURSE SOURCE_WEPOLL
			${CMAKE_CURRENT_LIST_DIR}/*.inl*
			${CMAKE_CURRENT_LIST_DIR}/*.h*
			${CMAKE_CURRENT_LIST_DIR}/*.c*
			${CMAKE_CURRENT_LIST_DIR}/*.cc*
			${CMAKE_CURRENT_LIST_DIR}/*.hpp*
			${CMAKE_CURRENT_LIST_DIR}/*.cpp*
			${CMAKE_CURRENT_LIST_DIR}/*.hxx*
			${CMAKE_CURRENT_LIST_DIR}/*.cxx*)
	list (APPEND SOURCE ${SOURCE_WEPOLL})
endif()