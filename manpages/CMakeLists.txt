######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.1                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

################### CHECK GZIP #######################
FIND_PROGRAM(GZIP_TOOL
	NAMES gzip
	PATHS /bin
	/usr/bin
	/usr/local/bin)

IF(GZIP_TOOL)
	MESSAGE(STATUS "Find gzip : ${GZIP_TOOL}" )
ELSE(GZIP_TOOL)
	MESSAGE(FATAL_ERROR "Unable to find 'gzip' program to compress manpages. You can disable it with -DDISABLE_MANPAGES")
ENDIF(GZIP_TOOL)

################## LIST MAN FILES ####################
set(svgm2txt_man_src svpgm2txt.1)

################ COMPRESS MAN FILES ##################
# Compression of the manuals
FOREACH(man ${svgm2txt_man_src})
	ADD_CUSTOM_COMMAND(OUTPUT ${man}.gz
		COMMAND ${GZIP_TOOL} -c ${CMAKE_CURRENT_SOURCE_DIR}/${man} > ${CMAKE_CURRENT_BINARY_DIR}/${man}.gz
		DEPENDS ${man}
		COMMENT "Building ${man}.gz")
ENDFOREACH(man)
add_custom_target(manpages ALL DEPENDS svpgm2txt.1.gz) 

################ INSTALL MANUALS #####################
# Installation of the manuals
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/svpgm2txt.1.gz
        DESTINATION "share/man/man1"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME svpgm2txt.1.gz)
