#!/bin/sh

  QTDIR="/usr/lib64/qt5" ; export QTDIR ; 
  PATH="/usr/lib64/qt5/bin:$PATH" ; export PATH ; 
  
    
  CFLAGS="${CFLAGS:--O2 -g -pipe -Wformat -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -fPIC}" ; export CFLAGS ; 
  CXXFLAGS="${CXXFLAGS:--O2 -g -pipe -Wformat -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -fPIC}" ; export CXXFLAGS ; 
  FFLAGS="${FFLAGS:--O2 -g -pipe -Wformat -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -fPIC}" ; export FFLAGS ; 
  LDFLAGS="$LDFLAGS -Wl,--as-needed -Wl,--no-undefined -Wl,-z,relro -Wl,-O1 -Wl,--build-id -Wl,--enable-new-dtags"; export LDFLAGS ; 
    mkdir -p build 
    cd build 
         
    /usr/bin/cmake .. \
        -DCMAKE_INSTALL_PREFIX:PATH=/usr \
        -DCMAKE_INSTALL_LIBDIR:PATH=/usr/lib64 \
        -DCMAKE_INSTALL_LIBEXECDIR:PATH=/usr/libexec \
        -DCMAKE_INSTALL_SYSCONFDIR:PATH=/etc \
        -DINCLUDE_INSTALL_DIR:PATH=/usr/include \
        -DLIB_INSTALL_DIR:PATH=/usr/lib64 \
        -DSYSCONF_INSTALL_DIR:PATH=/etc \
        -DSHARE_INSTALL_PREFIX:PATH=/usr/share \
        -DCMAKE_BUILD_TYPE=release \
        -DLIB_SUFFIX=64 \
        -DCMAKE_SKIP_RPATH:BOOL=ON \
        -DBUILD_SHARED_LIBS:BOOL=ON \
        -DBUILD_STATIC_LIBS:BOOL=OFF \
        -DCMAKE_MODULE_LINKER_FLAGS="-Wl,--as-needed  -Wl,-z,relro -Wl,-O1 -Wl,--build-id -Wl,--enable-new-dtags" \
  -DDBUS_SERVICES_DIR=/usr/share/dbus-1/services \
  -DDBUS_INTERFACES_DIR=/usr/share/dbus-1/interfaces
