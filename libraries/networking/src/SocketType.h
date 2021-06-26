//
//  SocketType.h
//  libraries/networking/src
//
//  Created by David Rowe on 17 May 2021.
//  Copyright 2021 Vircadia contributors.
//
//  Handles UDP and WebRTC sockets in parallel.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef vircadia_SocketType_h
#define vircadia_SocketType_h

/// @addtogroup Networking
/// @{


/// @brief The network socket type.
enum class SocketType {
    Unknown,    ///< Unknown socket type.
    UDP,        ///< UDP socket.
    WebRTC      ///< WebRTC socket.
};

/// @brief Returns the name of a SocketType value as a string.
/// @param socketType The SocketType value.
/// @return The name of the SocketType value as a string.
static QString socketTypeToString(SocketType socketType) {
    static QStringList SOCKET_TYPE_STRINGS { "Unknown", "UDP", "WebRTC" };
    return SOCKET_TYPE_STRINGS[(int)socketType];
}


/// @}

#endif // vircadia_SocketType_h
