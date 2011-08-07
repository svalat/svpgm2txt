######################################################
#            PROJECT  : svPgm2Txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

##################### TARGETS ########################
set(SVPGM2TXT_SOUCRES main.cpp svOCROptions.cpp svOCR.cpp svOCRLine.cpp svOCRHeuristic.cpp 
	svExtractedChar.cpp svCharDb.cpp svChar.cpp image.cpp chaine.cpp pgmHeader.cpp)
add_executable(svpgm2txt ${SVPGM2TXT_SOUCRES})