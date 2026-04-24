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

// const char* TCP20x4StatusName(TCP20x4Status status) {
//   switch (status) {
//     case TCP20x4Status::Ok: return "Ok";
//     case TCP20x4Status::InvalidLine: return "InvalidLine";
//     case TCP20x4Status::LineTooLong: return "LineTooLong";
//     case TCP20x4Status::InvalidArgument: return "InvalidArgument";
//     case TCP20x4Status::NotInitialized: return "NotInitialized";
//     case TCP20x4Status::TransportError: return "TransportError";
//     case TCP20x4Status::NotImplemented: return "NotImplemented";
//     default: return "Unknown";
//   }
// }

#endif
// src/TCP20x4Status.h v2