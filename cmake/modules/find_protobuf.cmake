SET(PROTOBUF_INCLUDE_SEARCH_DIRS
    /opt/protobuf/current/src
	${GLOBAL_EXT_DIR}/inc/protobuf
)

SET(PROTOBUF_LIBRARY_SEARCH_DIRS
    /opt/protobuf/current/src/.libs
	${GLOBAL_EXT_DIR}/lib
)

message("-- checking for protobuf")

IF (NOT PROTOBUF_INCLUDE_DIRS)

    SET(_PROTOBUF_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${PROTOBUF_INCLUDE_SEARCH_DIRS})
	MESSAGE(${_SEARCH_DIR})
        FIND_PATH(_CUR_SEARCH
                NAMES google/protobuf/stubs/common.h
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _PROTOBUF_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${PROTOBUF_INCLUDE_SEARCH_DIRS})

    IF (NOT _PROTOBUF_FOUND_INC_DIRS)
        MESSAGE(FATAL_ERROR "find_protobuf.cmake: unable to find protobuf headers")
    ENDIF (NOT _PROTOBUF_FOUND_INC_DIRS)
    
    FOREACH(_INC_DIR ${_PROTOBUF_FOUND_INC_DIRS})
        LIST(APPEND PROTOBUF_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_BOOST_FOUND_INC_DIRS})

ENDIF(NOT PROTOBUF_INCLUDE_DIRS)

IF (        PROTOBUF_INCLUDE_DIRS
    AND NOT PROTOBUF_LIBRARIES)

    SET(_PROTOBUF_FOUND_LIB_DIR "")
    SET(_PROTOBUF_POSTFIX "")

	IF (UNIX)
		SET(_PROTOBUF_LIB_FILENAME libprotobuf.so)
	elseif (WIN32)
		SET(_PROTOBUF_LIB_FILENAME libprotobuf.lib)
	endif(UNIX)
	
	
    FOREACH(_SEARCH_DIR ${PROTOBUF_LIBRARY_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES ${_PROTOBUF_LIB_FILENAME}
                PATHS ${_SEARCH_DIR}
				PATH_SUFFIXES debug release 
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _PROTOBUF_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${PROTOBUF_LIBRARY_SEARCH_DIRS})

    IF (NOT _PROTOBUF_FOUND_LIB_DIR)
        MESSAGE(FATAL_ERROR "find_protobuf.cmake: unable to find protobuf library")
    ELSE (NOT _PROTOBUF_FOUND_LIB_DIR)
        message("--  found matching version")
    ENDIF (NOT _PROTOBUF_FOUND_LIB_DIR)
    
    FOREACH(_LIB_DIR ${_PROTOBUF_FOUND_LIB_DIR})
	IF (UNIX)
          LIST(APPEND PROTOBUF_LIBRARIES ${_LIB_DIR}/libprotobuf.so)
	elseif (WIN32)
          LIST(APPEND PROTOBUF_LIBRARIES ${_PROTOBUF_LIB_FILENAME})
	endif(UNIX)
    ENDFOREACH(_LIB_DIR ${_PROTOBUF_FOUND_INC_DIRS})
    

ENDIF(        PROTOBUF_INCLUDE_DIRS
      AND NOT PROTOBUF_LIBRARIES)



