######################################################
#            PROJECT  : svPgm2Txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################



This directory contain the requierd files to build the project on distributions, please read this 
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

To setup your working environnement, you will found documentation on 
https://wiki.ubuntu.com/PbuilderHowto. We mosly need :

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




III. GENTOO WITHOUT LAYMAN :
____________________________




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