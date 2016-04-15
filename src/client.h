// -*-c++-*-
//******************************************************************
//
// Copyright 2014 Intel Corporation.
// Copyright 2015 Eurogiciel <philippe.coval@eurogiciel.fr>
// Copyright 2016 Samsung <philippe.coval@osg.samsung.com>
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef CLIENT_H_
#define CLIENT_H_

#include <memory>
#include <iostream>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>


class Resource
{
        std::shared_ptr<OC::OCResource> m_resourceHandle;
        OC::OCRepresentation m_Representation;
        OC::GetCallback m_GETCallback;
        OC::PutCallback m_PUTCallback;
        void onGet(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPut(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
    public:
        void get();
        void put(std::string);
        Resource(std::shared_ptr<OC::OCResource> resource);
        virtual ~Resource();
};


class IoTClient
{
        std::shared_ptr<Resource> m_platformResource;
        std::shared_ptr<OC::PlatformConfig> m_platformConfig;
        OC::FindCallback m_resourceDiscoveryCallback;
        void initializePlatform();
        void discoveredResource(std::shared_ptr<OC::OCResource>);
    public:
        std::shared_ptr<Resource> getPlatformResource();
        void findResource();
        IoTClient();
        virtual ~IoTClient();
        static void DisplayMenu();

        static IoTClient *getInstance();
        static IoTClient *mInstance;
};

#endif /* CLIENT_H_ */
