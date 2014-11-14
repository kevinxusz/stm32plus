/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace usb {

    /*
     * Base class for all USB events
     */

    struct UsbEventDescriptor {

      enum class EventType {
        DEVICE_INIT,
        DEVICE_DEINIT,
        DEVICE_START,
        DEVICE_STOP,
        DEVICE_OPEN_ENDPOINT,
        DEVICE_CLOSE_ENDPOINT,
        DEVICE_FLUSH_ENDPOINT,
        DEVICE_STALL_ENDPOINT,
        DEVICE_CLEAR_STALL_ENDPOINT,
        DEVICE_IS_STALL_ENDPOINT,
        DEVICE_SET_USB_ADDRESS,
        DEVICE_TRANSMIT,
        DEVICE_PREPARE_RECEIVE,
        DEVICE_GET_RX_DATA_SIZE,
        DEVICE_GET_DEVICE_DESCRIPTOR,
        DEVICE_GET_LANGUAGE_DESCRIPTOR,
        DEVICE_GET_STRING_DESCRIPTOR,

        OTG_FS_INTERRUPT,

        USB_ERROR
      };

      EventType eventType;

      UsbEventDescriptor(EventType type)
        : eventType(type) {
      }
    };
  }
}