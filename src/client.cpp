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

#include "config.h"
#include <unistd.h>
#include <ctime>
#include "client.h"
#include "observer.h"

using namespace std;
using namespace OC;


Resource::Resource(shared_ptr<OCResource> resource)
{
	cout<<__PRETTY_FUNCTION__<<std::endl;
    m_resourceHandle = resource;
    m_GETCallback = bind(&Resource::onGet, this, placeholders::_1, placeholders::_2, placeholders::_3);
}

Resource::~Resource()
{
}

shared_ptr<Resource> IoTClient::getPlatformResource()
{
    return m_platformResource;
}


void Resource::get()
{
	dlog_print(DLOG_ERROR, LOG_TAG, __PRETTY_FUNCTION__);

    QueryParamsMap params;
    assert(m_resourceHandle); //TODO display alert if not ready
    m_resourceHandle->get(params, m_GETCallback);
}


void Resource::onGet(const HeaderOptions &headerOptions, const OCRepresentation &representation,
                     int errCode)
{
	dlog_print(DLOG_ERROR, LOG_TAG,"%s {", __PRETTY_FUNCTION__ );

    if (errCode == OC_STACK_OK)
    {
      IoTObserver::handleObserve(headerOptions, representation, errCode, 0);
    }
    else
    {
        cerr << endl << endl << "Error in GET response from Resource resource" << endl;
    }

	dlog_print(DLOG_ERROR, LOG_TAG,"%s }", __PRETTY_FUNCTION__ );
}


IoTClient::IoTClient()
{
    cerr << "Running IoTClient constructor" << endl;
    initializePlatform();
}

IoTClient::~IoTClient()
{
    cout << "Running IoTClient destructor" << endl;
}


IoTClient *IoTClient::mInstance = nullptr;

IoTClient *IoTClient::getInstance()
{
    if ( IoTClient::mInstance == 0 )
    {
        mInstance = new IoTClient;
    }
    return mInstance;
}

void IoTClient::initializePlatform()
{
    m_platformConfig = make_shared<PlatformConfig>(ServiceType::InProc, ModeType::Client, "0.0.0.0",
                       0, OC::QualityOfService::HighQos);
    OCPlatform::Configure(*m_platformConfig);
    m_resourceDiscoveryCallback = bind(&IoTClient::discoveredResource, this, placeholders::_1);
}

void IoTClient::findResource()
{
	dlog_print(DLOG_ERROR, LOG_TAG,__PRETTY_FUNCTION__);

    string coap_multicast_discovery = string(OC_RSRVD_WELL_KNOWN_URI);
//  coap_multicast_discovery += "?rt=";
//  coap_multicast_discovery += Config::m_interface;
    OCConnectivityType connectivityType(CT_ADAPTER_IP);
    OCPlatform::findResource("", coap_multicast_discovery.c_str(),
                             connectivityType,
                             m_resourceDiscoveryCallback,
                             OC::QualityOfService::LowQos);
}

void IoTClient::discoveredResource(shared_ptr<OCResource> resource)
{
	dlog_print(DLOG_ERROR, LOG_TAG, __PRETTY_FUNCTION__);

    try
    {
        if (resource)
        {
            string resourceUri = resource->uri();
            string hostAddress = resource->host();

            cout << "\nFound Resource" << endl << "Resource Types:" << endl;
            for (auto & resourceTypes : resource->getResourceTypes())
            {
                cout << "\t" << resourceTypes << endl;
            }

            cout << "Resource Interfaces: " << endl;
            for (auto & resourceInterfaces : resource->getResourceInterfaces())
            {
                cout << "\t" << resourceInterfaces << endl;
            }
            cout << "Resource uri: " << resourceUri << endl;
            cout << "host: " << hostAddress << endl;

            if (resourceUri == Config::m_endpoint)
            {
                m_platformResource = make_shared<Resource>(resource);
            }
        }
    }
    catch (OCException &ex)
    {
        cerr << "Caught exception in discoveredResource: " << ex.reason() << endl;
    }
}


