######################################################
#            PROJECT  : svPgm2Txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

##################### TARGETS ########################
set(SVPGM2TXT_SOUCRES main.cpp svOCROptions.cpp svOCR.cpp svOCRLine.cpp svOCRHeuristic.cpp 
	svOCRExtractedChar.cpp svOCRCharDb.cpp svOCRChar.cpp svOCRImage.cpp svOCRPgmHeader.cpp)
add_executable(svpgm2txt ${SVPGM2TXT_SOUCRES})