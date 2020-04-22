#---------------------------------------------
# spec file for package afb-binding
#---------------------------------------------
Name:           afb-binding
Version:        1.2
Release:        0
License:        LGPLv3
Summary:        Binding headers for Application Framework Binder
Group:          Development/Libraries/C and C++
Url:            https://github.com/redpesk/afb-binding
Source:         afb-binding-%{version}.tar.gz
BuildRequires:  make
BuildRequires:  cmake
Requires:       %{name}-devel = %{version}
%global debug_package %{nil} 

%description
Development files for creating application framework bindings

#---------------------------------------------
%package devel
Summary:        afb-binding headers
Provides:       pkgconfig(afb-binding)

%description devel
Development files for creating application framework binding

#---------------------------------------------
%package tutorial
Requires:       %{name} = %{version}
Summary:        afb-binding examples and tutorial

%description tutorial
Provides examples for creation application framework binding

#---------------------------------------------
%package doc
Requires:       %{name} = %{version}
Summary:        documentation files

%description doc
Some documentation on how to write application framework bindings

#---------------------------------------------
%prep
%setup -q -n %{name}-%{version}

%build
%cmake .
%__make %{?_smp_mflags}

%install
%make_install

%post


%postun

#---------------------------------------------
%file

#---------------------------------------------
%files devel
%defattr(-,root,root)
%dir %{_includedir}/afb
%{_includedir}/afb/*
%{_libdir}/pkgconfig/*.pc

#---------------------------------------------
%files doc
%defattr(-,root,root)
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/docs

#---------------------------------------------
%files tutorial
%defattr(-,root,root)
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/tutorials

#---------------------------------------------
%changelog
* Tue Apr 21 2020 jobol
- initial creation
