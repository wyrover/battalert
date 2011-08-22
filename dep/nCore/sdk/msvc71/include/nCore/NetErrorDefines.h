//***************************************************************************
//
// [ nCore ]
//
// Copyright (c) 2001-2008 Jean-Charles Lefebvre <jcl ATNOSPAM jcl DOT name>
//
// This file is part of nCore.
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************


//---------------------------------------------------------------------------
// Basic Codes
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(
  NETERR_OK,
  "OK",
  0
)
NETERROR_DEFINE_CODE(
  NETERR_UNKNOWN,
  "System error code not translated !",
  0xFFFFFFFF
)


//---------------------------------------------------------------------------
// Sub-System
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_SUBSYSTEM__BASE, "-", 100)

NETERROR_DEFINE(
  NETERR_SUBSYSTEMNOTINIT,
  "Network subsystem not initialized !"
)
NETERROR_DEFINE(
  NETERR_INVALIDARG,
  "Invalid argument."
)
NETERROR_DEFINE(
  NETERR_INVALIDPOINTER,
  "Address is not writable."
)
NETERROR_DEFINE(
  NETERR_INPROGRESS,
  "Operation now in progress."
)
NETERROR_DEFINE(
  NETERR_INTERRUPTED,
  "Operation interrupted."
)
NETERROR_DEFINE(
  NETERR_AGAIN,
  "Operation cannot be completed immediately."
)
NETERROR_DEFINE(
  NETERR_NOSPACE,
  "No buffer space available."
)
NETERROR_DEFINE(
  NETERR_NOOP,
  "Operation not supported by this socket."
)
NETERROR_DEFINE(
  NETERR_OPALREADY,
  "Operation already in progress."
)
NETERROR_DEFINE(
  NETERR_ACCESS,
  "Permition denied."
)
NETERROR_DEFINE(
  NETERR_TIMEOUT,
  "Connection/Operation timed out."
)


//---------------------------------------------------------------------------
// Address
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_ADDRESS__BASE, "-", 200)

NETERROR_DEFINE(
  NETERR_ADDRESS_WRONGFAMILY,
  "Address family not supported by protocol family."
)
NETERROR_DEFINE(
  NETERR_ADDRESS_INUSE,
  "Address already in use."
)
//NETERROR_DEFINE(
//  NETERR_ADDRESS_NOTAVAIL,
//  "Cannot assign requested address."
//)


//---------------------------------------------------------------------------
// Network
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_NETWORK__BASE, "-", 300)

NETERROR_DEFINE(
  NETERR_NETWORK_DOWN,
  "Network is down."
)
NETERROR_DEFINE(
  NETERR_NETWORK_UNREACH,
  "Network is unreachable from this host at this time."
)


//---------------------------------------------------------------------------
// Protocol
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_PROTOCOL__BASE, "-", 400)

NETERROR_DEFINE(
  NETERR_PROTOCOL_NOTSUPPORTED,
  "Protocol not supported."
)


//---------------------------------------------------------------------------
// Socket (generic)
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_SOCKET__BASE, "-", 500)

NETERROR_DEFINE(
  NETERR_SOCKET_WRONGTYPE_FAMILY,
  "Socket type not supported by specified address family."
)
NETERROR_DEFINE(
  NETERR_SOCKET_WRONGTYPE_PROTO,
  "Socket type not supported by specified protocol."
)
NETERROR_DEFINE(
  NETERR_SOCKET_NOTCONN,
  "Socket not connected."
)
NETERROR_DEFINE(
  NETERR_SOCKET_ISCONN,
  "Socket already connected."
)
NETERROR_DEFINE(
  NETERR_SOCKET_INVALID,
  "Descriptor is not a socket."
)
NETERROR_DEFINE(
  NETERR_SOCKET_SHUTDOWN,
  "Socket has been shut down."
)


//---------------------------------------------------------------------------
// Connection-Oriented Socket
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_CONN__BASE, "-", 600)

NETERROR_DEFINE(
  NETERR_CONN_RESET,
  "Connection reset by peer."
)
NETERROR_DEFINE(
  NETERR_CONN_REFUSED,
  "Connection refused."
)


//---------------------------------------------------------------------------
// Message-Oriented Socket
//---------------------------------------------------------------------------
NETERROR_DEFINE_CODE(NETERR_MESSAGE__BASE, "-", 700)

NETERROR_DEFINE(
  NETERR_MESSAGE_SIZE,
  "Message size it too big for the underlying transport."
)



#undef NETERROR_DEFINE
#undef NETERROR_DEFINE_CODE
