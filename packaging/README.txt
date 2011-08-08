######################################################
#            PROJECT  : svPgm2Txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

This directory contain the requierd files to build the project on distributions, please read this file before using it.

I. Source archive :
___________________

To generate a source archive, as we use git for repository, you just need to execute :

--------------------------------------------------------------------
$user> git archive --prefix=svpgm2txt-1.0/ v1.0 | gzip > svpgm2txt-1.0.tar.gz
--------------------------------------------------------------------


II. Ubuntu :
___________

This project support the ubuntu packing to build .deb files. To build such package you may use the files in packaging/ubuntu.
First create a directory in your home dir, place the project archive in it and rename it to svpgm2txt_1.0.orig.tar.gz.

To setup your working environnement, you will found documentation on https://wiki.ubuntu.com/PbuilderHowto. We mosly need :

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

You can quickly build the package for the running ubuntu with (remove -uc and -us if you have some GPG keys to sign the package) :

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
