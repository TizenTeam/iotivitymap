Name:           iotivitymap
Version:        1.0.0
Release:        0
License:        Apache-2.0
Summary:        Very minimalist example of IoTivity resource
Url:            https://gitorious.org/tizen/iotivitity-example
Group:          System/Libraries
#X-Vc-Url:      git@gitorious.org:tizen/iotivitity-example.git
Group:          Contrib
Source:         %{name}-%{version}.tar.gz

BuildRequires:  make
BuildRequires:  fdupes
BuildRequires:  iotivity-devel
BuildRequires:  boost-devel


%description
Mimimal client/server application
that share a single gpio output as IoTivity resource.

%prep
%setup -q

%build
%__make %{?_smp_mflags}


%install
%__make install DEST_LIB_DIR=%{buildroot}%{_libdir}/%{name}/ 


%fdupes %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root)
%{_libdir}/%{name}/*

