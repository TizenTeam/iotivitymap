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

#ifndef SENSORS_H_
#define SENSORS_H_

#include <mraa.h>

namespace Sensors
{
    extern mraa_gpio_context led_gpio;

    inline void SetupPins(int pin)
    {
        led_gpio = mraa_gpio_init(pin);
        if (led_gpio != NULL) // Set direction to OUTPUT
            mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT);
    }

    inline void ClosePins()
    {
        mraa_gpio_close(led_gpio);
    }

    inline void SetOnboardLed(int on)
    {
        if (led_gpio == NULL)
        {
            led_gpio = mraa_gpio_init(Config::m_gpio);
            if (led_gpio != NULL)  // Set direction to OUTPUT
                mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT);
            else
                exit(-1);
        }
        if (led_gpio != NULL) // Writes into GPIO
            mraa_gpio_write(led_gpio, on);
    }

}
#endif // SENSORS_H_
