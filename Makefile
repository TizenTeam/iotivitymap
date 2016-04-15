#! /usr/bin/make -f
# //******************************************************************
# //
# // Copyright 2014 Intel Corporation.
# // Copyright 2015 Eurogiciel <philippe.coval@eurogiciel.fr>
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

default: all

package?=iotivity-example-stream


DEST_LIB_DIR?=${DESTDIR}${local_optdir}/${package}/
local_bindir?=bin
local_bindir?=opt
vpath+=src
VPATH+=src

CPPFLAGS=-I. \
 -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/iotivity \
 -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/iotivity/resource/ \
 -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/iotivity/resource/c_common \
 -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/iotivity/resource/oc_logger/ \
 -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/iotivity/resource/stack/

CXXFLAGS+=-std=gnu++0x
LIBS+= -loc -loc_logger -loctbstack
#LIBS+= -luuid -lconnectivity_abstraction

srcs?=config.cpp
objs?=${srcs:.cpp=.o}
client?=${local_bindir}/client
server?=${local_bindir}/server
observer?=${local_bindir}/observer

all?=${client} ${observer}

${local_bindir}/%: %.o ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

all+=${server}

${local_bindir}/server: server.o ${server_objs} ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

all: ${all}

${local_bindir}/%: %.o ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

run: ${client}
	${<D}/${<F}

clean:
	rm -f *.o *~ ${objs} */*.o

distclean: clean
	rm -f ${client} ${server}

install: ${all}
	install -d ${DEST_LIB_DIR}
	install $^ ${DEST_LIB_DIR}

