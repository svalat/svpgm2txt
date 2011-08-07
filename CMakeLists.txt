######################################################
#            PROJECT  : svPgm2Txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################


################## BASE DECLARATIONS #################
cmake_minimum_required(VERSION 2.6)
project(svpgm2txt)

##################### SETUP PARAMS ###################
add_definitions(-DVERSION="1.0")

###################### CONTENT #######################
add_subdirectory(src)