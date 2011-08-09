######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

##################### TESTS ##########################
add_test(test-no-db-asii ${CMAKE_CURRENT_SOURCE_DIR}/test-ocr-no-db.sh ${CMAKE_CURRENT_SOURCE_DIR} ascii)
add_test(test-no-db-bin ${CMAKE_CURRENT_SOURCE_DIR}/test-ocr-no-db.sh ${CMAKE_CURRENT_SOURCE_DIR} bin)
add_test(test-with-db-asii ${CMAKE_CURRENT_SOURCE_DIR}/test-ocr-with-db.sh ${CMAKE_CURRENT_SOURCE_DIR} ascii)
add_test(test-with-db-bin ${CMAKE_CURRENT_SOURCE_DIR}/test-ocr-with-db.sh ${CMAKE_CURRENT_SOURCE_DIR} bin)