Summary: XawPlus widget set development package
%global AppProgram XawPlus
%global AppVersion 3.1.0
%global AppRelease 20221212
%global ActualProg c%{AppProgram}
# $XTermId: XawPlus.spec,v 1.5 2022/12/13 01:02:07 tom Exp $
Name: %{AppProgram}
Version: %{AppVersion}
Release: %{AppRelease}
License: X11
Group: Applications/Development
URL: ftp://invisible-island.net/%{AppProgram}
Source0: %{AppProgram}-%{AppVersion}-%{AppRelease}.tgz
Packager: Thomas Dickey <dickey@invisible-island.net>

%description
XawPlus is a clone of the original Xaw library, providing a more up to date
look and some extensions to the original Athena widget set. This library is
as compatible as possible to the original. It should  be possible to compile
the source code of an Xaw client without any changes (except the include path
of the XawPlus header files) to get the new look of XawPlus to this client.

XawPlus makes it possible to use XPM pixmaps, tooltips, a truncate mechanism
for too long label strings and UTF8 coded UNICODE labels without changing the
code of your application.
%prep

%define debug_package %{nil}

%setup -q -n %{AppProgram}-%{AppVersion}-%{AppRelease}

%build

INSTALL_PROGRAM='${INSTALL}' \
	./configure \
		--target %{_target_platform} \
		--prefix=%{_prefix} \
		--bindir=%{_bindir} \
		--libdir=%{_libdir} \
		--mandir=%{_mandir} \
		--with-shared \
		--disable-rpath-hack

make

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

make install		   DESTDIR=$RPM_BUILD_ROOT
# libtool --finish %{_libdir} 

chmod 755 $RPM_BUILD_ROOT%{_libdir}/lib%{AppProgram}.so.*.*.*
rm -f $RPM_BUILD_ROOT%{_libdir}/lib%{AppProgram}.la

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_includedir}/X11/%{AppProgram}
%{_libdir}/lib%{AppProgram}.*

%changelog
# each patch should add its ChangeLog entries here

* Fri Jan 02 2015 Thomas Dickey
- initial version
