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

#include <csignal>
#include <functional>

#include "server.h"

using namespace std;
using namespace OC;

bool IoTServer::m_over = false;

double IoTServer::m_lat = 48.1033;
double IoTServer::m_lon = -1.6725;
double IoTServer::m_offset = 0.001;


double IoTServer::m_latmax = 49;
double IoTServer::m_latmin = 48;

void IoTServer::initializePlatform()
{
    cerr << __PRETTY_FUNCTION__ << endl;

    m_platformConfig = make_shared<PlatformConfig>
                       (ServiceType::InProc,
                        ModeType::Server, "0.0.0.0",
                        0,
                        OC::QualityOfService::HighQos);

    OCPlatform::Configure(*m_platformConfig);
}

IoTServer::IoTServer(string key)
{
    cerr << __PRETTY_FUNCTION__ << endl;
    m_Data = "0,0";
    initializePlatform();
    setupResources();

    m_Representation.setValue(key, m_Data);
}

IoTServer::~IoTServer()
{
    cerr << __PRETTY_FUNCTION__ << endl;
}

void IoTServer::setupResources()
{
    cerr << __PRETTY_FUNCTION__ << endl;
    EntityHandler cb3 = bind(&IoTServer::ResourceEntityHandler, this, placeholders::_1);
    createResource(Config::m_endpoint, Config::m_type, cb3, m_Resource);

    OCStackResult result ;
    result = OCPlatform::bindTypeToResource(m_Resource, Config::m_type);
    if (OC_STACK_OK != result)
    {
        cout << "Binding TypeName to Resource was unsuccessful\n";
    }

    result = OCPlatform::bindInterfaceToResource(m_Resource, Config::m_link);
    if (OC_STACK_OK != result)
    {
        cout << "Binding TypeName to Resource was unsuccessful\n";
    }
}

void IoTServer::createResource(string Uri, string Type, EntityHandler Cb, OCResourceHandle &Handle)
{
    string resourceUri = Uri;
    string resourceType = Type;
    string resourceInterface = Config::m_interface;
    uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;

    OCStackResult result = OCPlatform::registerResource(Handle,
                           resourceUri, resourceType,
                           resourceInterface, Cb, resourceFlag);

    if (result != OC_STACK_OK)
        cerr << "Could not create " << Type << " resource" << endl;
    else
        cout << "Successfully created " << Type << " resource" << endl;
}

void IoTServer::putResourceRepresentation()
{
    m_Representation.getValue(Config::m_key, m_Data);
    OCStackResult result = OCPlatform::notifyAllObservers(m_Resource);
    cerr << "data=" << m_Data << endl;
}

void IoTServer::update()
{
    cerr << __PRETTY_FUNCTION__ << endl;

    {
        string data = "0,0";
        time_t const now = time(0);
        char *dt = ctime(&now);
        data = string(dt);
        m_Representation.setValue(Config::m_key, data);
    }

    {
        m_lat += m_offset;
        m_lon += m_offset;

        if (m_lat > m_latmax)
        {
            if (m_offset > 0) { m_offset = - m_offset; }
        }
        else if (m_lat < m_latmin)
        {
            if ( m_offset < 0 ) m_offset = - m_offset;
        }

        m_Representation.setValue("lat", m_lat);
        m_Representation.setValue("lon", m_lon);

        cerr << "location: " << std::fixed << m_lat << "," << std::fixed << m_lon << std::endl;
    }

    putResourceRepresentation();
}

OCRepresentation IoTServer::getResourceRepresentation()
{
    return m_Representation;
}


OCEntityHandlerResult IoTServer::ResourceEntityHandler(shared_ptr<OCResourceRequest> Request)
{
    cerr << __PRETTY_FUNCTION__ << endl;

    OCEntityHandlerResult result = OC_EH_ERROR;
    if (Request)
    {
        string requestType = Request->getRequestType();
        int requestFlag = Request->getRequestHandlerFlag();
        if (requestFlag & RequestHandlerFlag::RequestFlag)
        {
            auto Response = std::make_shared<OC::OCResourceResponse>();
            Response->setRequestHandle(Request->getRequestHandle());
            Response->setResourceHandle(Request->getResourceHandle());
            if (requestType == "PUT")
            {
                cout << "PUT request for platform Resource" << endl;
                OCRepresentation requestRep = Request->getResourceRepresentation();
                if (requestRep.hasAttribute(Config::m_key))
                {
                    try
                    {
                        requestRep.getValue<string>(Config::m_key);
                    }
                    catch (...)
                    {
                        Response->setResponseResult(OC_EH_ERROR);
                        OCPlatform::sendResponse(Response);
                        cerr << "Client sent invalid resource value type" << endl;
                        return result;
                    }
                }
                else
                {
                    Response->setResponseResult(OC_EH_ERROR);
                    OCPlatform::sendResponse(Response);
                    cerr << "Client sent invalid resource key" << endl;
                    return result;
                }
                m_Representation = requestRep;
                putResourceRepresentation();
                if (Response)
                {
                    Response->setErrorCode(200);
                    Response->setResourceRepresentation(getResourceRepresentation());
                    Response->setResponseResult(OC_EH_OK);
                    if (OCPlatform::sendResponse(Response) == OC_STACK_OK)
                    {
                        result = OC_EH_OK;
                    }
                }
            }
            else if (requestType == "GET")
            {
                cout << "GET request for platform Resource" << endl;
                if (Response)
                {
                    Response->setErrorCode(200);
                    Response->setResponseResult(OC_EH_OK);
                    Response->setResourceRepresentation(getResourceRepresentation());
                    if (OCPlatform::sendResponse(Response) == OC_STACK_OK)
                    {
                        result = OC_EH_OK;
                    }
                }
            }
            else
            {
                Response->setResponseResult(OC_EH_ERROR);
                OCPlatform::sendResponse(Response);
                cerr << "Unsupported request type" << endl;
            }
        }
    }
    return result;
}



void handle_signal(int signal)
{
    IoTServer::m_over = true;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    cerr << "Server: " << endl
         << "Press Ctrl-C to quit...." << endl;

    IoTServer server(Config::m_key);

    int delay = 5;
    if ((argc > 1) && argv[1])
    {
        delay = atoi(argv[1]);
    }
    if ((argc > 2) && argv[2])
    {
        server.m_offset = atof(argv[2]);
    }

    if ((argc > 3) && argv[3])
    {
        server.m_lat = atof(argv[3]);
    }

    if ((argc > 4) && argv[4])
    {
        server.m_lon = atof(argv[4]);
    }

    server.m_latmax = server.m_lat + 1;
    server.m_latmin = server.m_lat - 1;

    do
    {
        server.update();
        sleep(delay);
    }
    while ( !  IoTServer::m_over );
    return 0;
}

