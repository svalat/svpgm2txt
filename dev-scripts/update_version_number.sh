#!/bin/bash
######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.1                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

########################## VARIABLES ###########################
old_version="$1"
new_version="$2"

######################### CHECK ARGS ###########################
#check parameter
if [ -z "$old_version" ] || [ -z "$new_version" ]; then
	echo "Usage : $0 {old_version} {new_version}"
	echo
	echo "Version format : major.minor.release[-dev|-rc1]"
	echo
	exit 1
fi

########################## FUNCTION ############################
#check format
#params :
#  - $1 : version value to check
function check_version_format()
{
	if [ -z "`echo \"$1\" | egrep '^[0-9]+\.[0-9]+(-dev|-rc[0-9]+)?$'`" ]
	#if [ -z "`echo \"$1\" | egrep '^[0-9]+\.[0-9]+\.[0-9]+(-dev|-rc[0-9]+)?$'`" ]
	then
		echo "Error : Invalid version format : $1"
		echo "        Must be major.minor[-dev|-rc1]"
		exit 1
	fi
}

########################## FUNCTION ############################
#check exec dir to force execution from project root dir
function check_exec_dir()
{
	if [ ! -f dev-scripts/update_version_number.sh ] || [ ! -f ChangeLog.txt ] || [ ! -f COPYING-FR.txt ] ; then
		echo "Error: this script must be executed from project root dir as ./dev-scripts/update_version_number.sh"
		exit 1
	fi
}

########################## FUNCTION ############################
#get list of source files in which to update version
#params :
function get_file_list()
{
	git ls-files
}

########################## FUNCTION ############################
#update version in cpp headers
#params :
#   -$1 : old version
#   -$2 : new version
function update_version_cpp_headers()
{
	for file in `get_file_list`
	do
		sed -i "s/^             VERSION  : $1\$/             VERSION  : $2/g" "$file"
	done
}

########################## FUNCTION ############################
#update date in cpp headers
#params :
function update_date_cpp_headers()
{
	new_date=`date +%m/%Y`
	for file in `get_file_list`
	do
		sed -i -r "s@^             DATE     : ([0-9]{2}/)?[0-9]{4}\$@             DATE     : ${new_date}@g" "$file"
	done
}

########################## FUNCTION ############################
#update version in script headers
#params :
#   -$1 : old version
#   -$2 : new version
function update_version_script_headers()
{
	orig=`printf "#            VERSION  : %-20s         #" "$1"`
	dest=`printf "#            VERSION  : %-20s         #" "$2"`
	for file in `get_file_list`
	do
		sed -i "s/^${orig}\$/${dest}/g" "$file"
	done
}

########################## FUNCTION ############################
#update version in man headers
#params :
#   -$1 : old version
#   -$2 : new version
function update_version_man_headers()
{
	orig=`printf "#            VERSION  : %-20s         #" "$1"`
	dest=`printf "#            VERSION  : %-20s         #" "$2"`
	for file in `get_file_list`
	do
		sed -i "s/^\.\"${orig}\$/.\"${dest}/g" "$file"
	done
}

########################## FUNCTION ############################
#update date in scripts headers
#params :
function update_date_script_headers()
{
	new_date=`date +%m/%Y`
	for file in `get_file_list`
	do
		sed -i -r "s@^#            DATE     : [0-9]{2}/[0-9]{4}                      #\$@#            DATE     : ${new_date}                      #@g" "$file"
		sed -i -r "s@^#            DATE     : [0-9]{4}                         #\$@#            DATE     : ${new_date}                      #@g" "$file"
	done
}

########################## FUNCTION ############################
#update date in man pages
#params :
function update_date_man_headers()
{
	new_date=`date +%m/%Y`
	for file in `get_file_list`
	do
		sed -i -r "s@^\.\"#            DATE     : [0-9]{2}/[0-9]{4}                      #\$@.\"#            DATE     : ${new_date}                      #@g" "$file"
		sed -i -r "s@^\.\"#            DATE     : [0-9]{4}                         #\$@.\"#            DATE     : ${new_date}                      #@g" "$file"
	done	
}

########################## FUNCTION ############################
#get_major {version}
function get_major()
{
	echo "${1}" | sed -e 's/[.-]/ /g' | awk '{print $1}'
}

########################## FUNCTION ############################
#get_minor {version}
function get_minor()
{
	echo "${1}" | sed -e 's/[.-]/ /g' | awk '{print $2}'
}

########################## FUNCTION ############################
function get_date_formated_for_man()
{
	 LC_ALL="en_US" date "+%B %d, %Y"
}

########################## FUNCTION ############################
#update_packaging_files {old_version} {new_version}
function update_packaging_files()
{
	#ubuntu
	if [ -z "$(grep "svpgm2txt ($2-1) unstable; urgency=low" packaging/ubuntu/changelog)" ]; then
		tmp="$(cat packaging/ubuntu/changelog)"
		echo "svpgm2txt ($2-1) unstable; urgency=low

  * PUT THE CHANGELOG HERE

 -- Sébastien Valat <sebastien.valat.dev@orange.fr>  $(date -R)

$tmp" > packaging/ubuntu/changelog
	fi
	#fedora
	sed -i "s/^Version:        $1$/Version:        $2/g" packaging/fedora/svpgm2txt.spec
	#Gentoo
	if [ -f "packaging/gentoo/app-text/svpgm2txt/svpgm2txt-$1.ebuild" ]; then
		git mv "packaging/gentoo/app-text/svpgm2txt/svpgm2txt-$1.ebuild" "packaging/gentoo/app-text/svpgm2txt/svpgm2txt-$2.ebuild"
	fi
}

########################## FUNCTION ############################
#update version in root CMakeLists.txt
#params :
#   -$1 : old version
#   -$2 : new version
function update_version_exotic_files()
{
# 	#root CMakeLists.txt (to define version of project)
	sed -i "s/^set(svpgm2txt_VERSION \"$1\")\$/set(svpgm2txt_VERSION \"$2\")/g" CMakeLists.txt
	sed -i "s/^add_definitions\(-DVERSION=\"$1\"\)$/add_definitions(-DVERSION=\"$2\")/g" CMakeLists.txt
	sed -i "s/^SET(CPACK_PACKAGE_VERSION \"$1\")/SET(CPACK_PACKAGE_VERSION \"$2\")/g" CMakeLists.txt
	sed -i "s/^SET(CPACK_PACKAGE_VERSION_MAJOR \"$(get_major $1)\")/SET(CPACK_PACKAGE_VERSION_MAJOR \"$(get_major $2)\")/g" CMakeLists.txt
	sed -i "s/^SET(CPACK_PACKAGE_VERSION_MINOR \"$(get_minor $1)\")/SET(CPACK_PACKAGE_VERSION_MINOR \"$(get_minor $2)\")/g" CMakeLists.txt
	#manpage
	sed -i "s/^\.TH svpgm2txt 1  \"[a-zA-Z]* [0-9]*, [0-9]*\" \"version $1\" \"USER COMMANDS\"/.TH svpgm2txt 1  \"$(get_date_formated_for_man)\" \"version $2\" \"USER COMMANDS\"/g" manpages/svpgm2txt.1
# 	#Doxyfile
# 	sed -i "s/^PROJECT_NUMBER         = $1\$/PROJECT_NUMBER         = $2/g" Doxyfile
# 	#packaging README
 	sed -i "s/svpgm2txt-$1/svpgm2txt-$2/g" packaging/README.txt
 	sed -i "s/svpgm2txt_$1/svpgm2txt_$2/g" packaging/README.txt
 	sed -i "s/v$1/v$2/g" packaging/README.txt
}

########################## FUNCTION ############################
#update date in exotic files
#params:
function update_date_exotic_files()
{
	new_date=`date +%m/%Y`
	##.....
}

########################## FUNCTION ############################
function exclude_from_final_check()
{
	sed -e 's#^test/data-db.txt$##g' -e 's#^test/test-data-ascii.pgm.txt.md5sum$##g'\
		-e 's#^test/test-data-bin.pgm.txt.md5sum$##g' -e 's#^test/test-data-ascii.pgm$##g'
}

########################## FUNCTION ############################
#Display version not update in case of new files not supported by the script
#params :
#  - $1 : old version
function display_version_not_updated()
{
	echo "--------------------- Not updated : -------------------------"
	grep --color -R -I "$(echo $1 | sed -e 's/\./\\./g')" `git ls-files | exclude_from_final_check`
	echo "-------------------------------------------------------------"
}

############################ MAIN ##############################
#gen_changelog_entry {old_version} {new_version}
function gen_changelog_entry()
{
	echo "Version $2 ($(get_date_formated_for_man))"
	echo ""
	git log "v${1}.." | grep '*'
	echo ""
}

############################ MAIN ##############################
#update_changelog {old_version} {new_version}
function update_changelog()
{
	if [ -z "$(egrep "^Version $2 \([A-Za-z]+ [0-9]+, [0-9]+\)" ChangeLog.txt)" ]; then
		gen_changelog_entry $1 $2 > ChangeLog.txt.new
		sed -i "8 r ChangeLog.txt.new" ChangeLog.txt
		rm -f ChangeLog.txt.new
	fi
}

############################ MAIN ##############################
function display_todo()
{
	echo "TODO : "
	echo " * Update packaging/ubuntu/changelog"
	echo " * Update ChangeLog.txt"
}

############################ MAIN ##############################
#check
check_version_format "$old_version"
check_version_format "$new_version"
check_exec_dir
#update headers
update_version_cpp_headers "$old_version" "$new_version"
update_version_script_headers "$old_version" "$new_version"
update_version_man_headers "$old_version" "$new_version"
update_version_exotic_files "$old_version" "$new_version"
update_date_cpp_headers
update_date_script_headers
update_date_man_headers
update_date_exotic_files
update_packaging_files "$old_version" "$new_version"
update_changelog  "$old_version" "$new_version"
display_version_not_updated "$old_version"
display_todo