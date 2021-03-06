######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.2                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################


################## BASE DECLARATIONS #################
cmake_minimum_required(VERSION 2.6)
project(svpgm2txt)
set(svpgm2txt_VERSION "1.2")

##################### SETUP PARAMS ###################
add_definitions(-DVERSION="${svpgm2txt_VERSION}")

################### ENABLE TESTS #####################
enable_testing()

###################### CPACK #########################
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OCR to convert PGM file to txt files.")
SET(CPACK_PACKAGE_VENDOR "Sébastien Valat")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING-EN.txt")
SET(CPACK_PACKAGE_VERSION "1.2")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_GENERATOR "TGZ;TBZ2;DEB")
SET(CPACK_PACKAGE_CONTACT "Sébastien Valat <sebastien.valat.dev@orange.fr>")
INCLUDE(CPack)

###################### CONTENT #######################
add_subdirectory(src)
add_subdirectory(test)
if (NOT DISABLE_MANPAGES)
	add_subdirectory(manpages)
endif(NOT DISABLE_MANPAGES)
