######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

EAPI="2"

inherit cmake-utils

DESCRIPTION="A simple OCR replacement for pgm2txt."
HOMEPAGE="https://github.com/svalat/svpgm2txt"
SRC_URI="${P}.tar.gz"

LICENSE="CeCILL-C"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE=""

DEPEND="gzip"
RDEPEND=""
