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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <dlog.h>

#if !defined(PACKAGE)
#define PACKAGE "$(packageName)"
#endif

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "IotivityMap"


/** Pseudo singleton class to store common configuration variables **/
class Config
{
    public:
        /** public interface (used both sides) **/
        static std::string  m_interface;
        /** type of resource (used both sides) **/
        static std::string  m_type;
        /** url's path (used both sides) **/
        static std::string  m_endpoint;
        /** key (used both sides) **/
        static std::string  m_key;
        /** network interface**/
        static std::string m_link;
        /** polling period**/
        static const int m_period = 3;
};

#endif /* CONFIG_H_ */
