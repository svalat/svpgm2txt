######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################


TABLE OF CONTENT :
__________________

   - I.   SOURCE ARCHIVE
   - II.  UBUNTU
   - III. DEBIAN
   - IV.  GENTOO WITHOUT LAYMAN
   - V.   FEDORA


INTRODUCTION :
______________



This directory contain the required files to build the project on distributions, please read this 
file before using it.



I. SOURCE ARCHIVE :
___________________



To generate a source archive, as we use git for repository, you just need to execute :

--------------------------------------------------------------------
$user> git archive --prefix=svpgm2txt-1.0/ v1.0 | gzip > svpgm2txt-1.0.tar.gz
--------------------------------------------------------------------




II. UBUNTU :
____________





This project support the ubuntu packing to build .deb files. To build such package you may use the 
files in packaging/ubuntu. First create a directory in your home dir, place the project archive in
it and rename it to svpgm2txt_1.0.orig.tar.gz.

To setup your working environment, you will found documentation on 
https://wiki.ubuntu.com/PbuilderHowto. We mostly need :

--------------------------------------------------------------------
sudo apt-get install pbuilder debootstrap devscripts cdbs debhelper
sudo pbuilder create
--------------------------------------------------------------------

If you work with git version you can build the archive with :

--------------------------------------------------------------------
$user> git archive --prefix=svpgm2txt-1.0/ v1.0 | gzip > YOUR_LOCATION/svpgm2txt_1.0.orig.tar.gz
--------------------------------------------------------------------

Go to your directory and extract svpgm2txt_1.0.orig.tar.gz with and go in the directory.

--------------------------------------------------------------------
$user> tar -xvzf svpgm2txt_1.0.orig.tar.gz
$user> cd svpgm2txt-1.0
--------------------------------------------------------------------

Now go to the  svpgm2txt-1.0 directory and move the packaging/ubuntu into debian :

--------------------------------------------------------------------
$user> mv packaging/ubuntu debian
--------------------------------------------------------------------

You can quickly build the package for the running ubuntu with (remove -uc and -us if you have some 
GPG keys to sign the package) :

--------------------------------------------------------------------
$user> debuild -uc -us
--------------------------------------------------------------------

You may get a .deb archive in the parent directory.

You can build a source package with :

--------------------------------------------------------------------
$user> debuild -S -uc -us
--------------------------------------------------------------------

So you can build package with your source package by running :

--------------------------------------------------------------------
sudo pbuilder build ../*.dsc
--------------------------------------------------------------------

Some extra links about ubuntu packaging : 

 * https://wiki.ubuntu.com/PackagingGuide/Basic
 * https://wiki.ubuntu.com/PackagingGuide/Complete
 * https://wiki.ubuntu.com/PbuilderHowto




III. DEBIAN :
_____________



The process was not tested but it may be the same way than ubuntu.




IV. GENTOO WITHOUT LAYMAN :
___________________________




For gentoo packaging, you will found ebuild files in the packaging/gentoo subdirectory. To use this, 
simply place the app-text/svpgm2txt directory in your own portage overlay.

--------------------------------------------------------------------
#root> mkdir /usr/local/portage/myoverlay
#root> cp -r app-text /usr/local/portage/myoverlay/media-sound
--------------------------------------------------------------------

Add "/usr/local/portage/myoverlay" in your PORTDIR_OVERLAY variable in /etc/make.conf.

Now get the project archive and place it in /usr/portage/distfiles (or corresponding dir if you 
change it in make.conf). If you work on the git version, you can build the project archive with :

--------------------------------------------------------------------
$user> git archive --prefix=svpgm2txt-1.0/ v1.0 | gzip > svpgm2txt-1.0.tar.gz
--------------------------------------------------------------------

Now got to the /usr/local/portage/myoverlay/app-text/svpgm2txt and build the Manifest file :

--------------------------------------------------------------------
#root> ebuild svpgm2txt-1.0.ebuild digest
--------------------------------------------------------------------

Now you can test the build steps one by one :

--------------------------------------------------------------------
#root> ebuild svpgm2txt-1.0.ebuild clean
#root> ebuild svpgm2txt-1.0.ebuild fetch
#root> ebuild svpgm2txt-1.0.ebuild unpack
#root> ebuild svpgm2txt-1.0.ebuild prepare
#root> ebuild svpgm2txt-1.0.ebuild configure
#root> ebuild svpgm2txt-1.0.ebuild compile
#root> ebuild svpgm2txt-1.0.ebuild test
--------------------------------------------------------------------

... I don't do install here bu you also can do preinst, install, postinst, qmerge 
(maybe qmerge is not a goot idea for the fist testings steps).

Or you can simply run :

--------------------------------------------------------------------
#root> echo "=app-text/svpgm2txt-1.0" >> /etc/portage/package.keywords
#root> emerge -a svpgm2txt
--------------------------------------------------------------------

Now you can export your OVERLAY with svn or git to share it with the world.




V. FEDORA :
___________



To build packages for Fedora (tested on Fedora 12), you must install the rpm dev tools, please read 
the fedora doc to do it. You may install : 
 * rpmdevtools
 * cmake
 * gcc-c++
 * git support if you ar using the git tree

Some URL which can help : 
 * http://doc.fedora-fr.org/wiki/La_cr%C3%A9ation_de_RPM_pour_les_nuls_:_Cr%C3%A9ation_du_fichier_SPEC_et_du_Paquetage
 * http://fedoraproject.org/wiki/PackageMaintainers
 * http://fedoraproject.org/wiki/How_to_create_an_RPM_package
 * http://fedoraproject.org/wiki/Docs/Drafts/BuildingPackagesGuide
 * https://fedoraproject.org/wiki/A_Short_RPM_Tutorial
 * http://fedoraproject.org/wiki/How_to_create_a_GNU_Hello_RPM_package

Start as for other packaging by building your tarball :

--------------------------------------------------------------------
$user> git archive --prefix=svpgm2txt-1.0/ v1.0 | gzip > YOUR_LOCATION/svpgm2txt-1.0.tar.gz
--------------------------------------------------------------------

Check if you already hale the "rpmbuild" dir in your home directory, if not, please use 

--------------------------------------------------------------------
$user> rpmdev-setuptree
--------------------------------------------------------------------

Now copy the tarball to rpmbuild sources dir and .spec file to spec dir :

--------------------------------------------------------------------
$user> cp YOUR_LOCATION/svpgm2txt-1.0.tar.gz ~/rpmbuild/SOURCES
$user> cp packaging/fedora/svpgm2txt.spec ~/rpmbuild/SPECS
--------------------------------------------------------------------

Now you can go into SPECS dir and run the build command :

--------------------------------------------------------------------
$user> cd ~/rpmbuild/SPECS
$user> rpmbuild -ba svpgm2txt.spec
--------------------------------------------------------------------

Now you may get *.rpm files in RPMS and SRPMS dirs.


