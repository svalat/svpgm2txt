######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.1                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

################### CHECK DEPS #######################
find_package(ASPELL)

##################### APPLY DEPS #####################
add_definitions(-I${ASPELL_INCLUDE_DIR})

##################### TARGETS ########################
set(SVPGM2TXT_SOUCRES svOCRILSpellFixer.cpp svOCRILUpperCaseFixer.cpp main.cpp svOCROptions.cpp svOCR.cpp svOCRLine.cpp svOCRHeuristic.cpp 
	svOCRExtractedChar.cpp svOCRCharDb.cpp svOCRChar.cpp svOCRImage.cpp svOCRPgmHeader.cpp)
add_executable(svpgm2txt ${SVPGM2TXT_SOUCRES})

##################### LINKING ########################
target_link_libraries(svpgm2txt ${ASPELL_LIBRARIES})

##################### INSTALL ########################
INSTALL(TARGETS svpgm2txt RUNTIME DESTINATION bin)
