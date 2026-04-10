// src/TCP20x4Status.h v2
#ifndef TCP20X4_STATUS_H
#define TCP20X4_STATUS_H

#include <stdint.h>

enum class TCP20x4Status : uint8_t {
  Ok = 0,
  InvalidLine,
  LineTooLong,
  InvalidArgument,
  NotInitialized,
  TransportError,
  NotImplemented
};

#endif
// src/TCP20x4Status.h v2