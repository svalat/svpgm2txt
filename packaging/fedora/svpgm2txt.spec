######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.0                          #
#            DATE     : 2009                         #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

Name:           svpgm2txt
Version:        1.0
Release:        1%{?dist}
Summary:        A simple OCR replacement for pgm2txt.

Group:          Applications/Text
License:        CeCILL-C
URL:            file:///svpgm2txt/%{name}-%{version}.tar.gz
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  cmake, gcc-c++, gzip
Requires:       

%description
Quick OCR to extract perfect formed text from PGM files instead of use gocr to get less error when working on perfect files. This is to fix issues of default pgm2txt command.

%prep
%setup -q


%build
%cmake
make %{?_smp_mflags}
make test

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_datadir}/*
%doc



%changelog