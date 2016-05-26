#!/bin/sh
# //******************************************************************
# //
# // Copyright 2016 Samsung <philippe.coval@osg.samsung.com>
# //
# //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# //
# // Licensed under the Apache License, Version 2.0 (the "License");
# // you may not use this file except in compliance with the License.
# // You may obtain a copy of the License at
# //
# //      http://www.apache.org/licenses/LICENSE-2.0
# //
# // Unless required by applicable law or agreed to in writing, software
# // distributed under the License is distributed on an "AS IS" BASIS,
# // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# // See the License for the specific language governing permissions and
# // limitations under the License.
# //
# //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

set -e
set -x

profile="tizen"
version="1.1.0"
arch="armv7l"
gbsdir="${HOME}/GBS-ROOT-OIC"
rootfs="${gbsdir}/local/BUILD-ROOTS/scratch.${arch}.0/"
rpmdir="${gbsdir}/local/repos/${profile}/${arch}/RPMS/"


die_()
{
    local r=$?
    echo "error: $@"
    echo "exit: $r"
    return $r
}


setup_()
{
    which apt-get || die_ "TODO: port to non debian systen"
    sudo apt-get install make git wget curl
}


build_()
{
    package="iotivity"
    branch="1.1.0"
    url="https://github.com/iotivity/iotivity"
    git clone -b ${branch} ${url}
    cd iotivity || die_ "io"
    ls extlibs/tinycbor/tinycbor || \
	git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor

    wget -c -O extlibs/sqlite3/sqlite-amalgamation-3081101.zip \
	http://www.sqlite.org/2015/sqlite-amalgamation-3081101.zip 

    ./auto_build.sh tizen
}


deploy_()
{

    ls .tproject || die_ "TODO"
    
    rm -rf usr lib
    mkdir -p usr lib

    unp ${rpmdir}/iotivity-${version}-*.${arch}.rpm
    unp ${rpmdir}/iotivity-devel-${version}-*${arch}.rpm

    mkdir -p usr/include/iotivity/
    mv usr/include/* usr/include/iotivity/

    ln -fs ${rootfs}/usr/include/boost usr/include/
    ln -fs ${rootfs}/usr/lib/libuuid.so.1.3.0 usr/lib/libuuid1.so # TODO
    cp -av ${rootfs}/usr/lib/libconnectivity_abstraction.so  usr/lib/ ||: #TODO might not be needed

    rm -rf lib
    ln -fs usr/lib lib
}


main_()
{
    cat<<EOF
Check:

https://wiki.iotivity.org/tizen
EOF

    projectdir=$(pwd)
    which git || setup_
    
    mkdir -p ${projectdir}/tmp
    cd ${projectdir}/tmp && build_
    cd ${projectdir} && deploy_
}


[ "" != "$1" ] || main_
$@
