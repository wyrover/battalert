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
// Base code for nCore users
//---------------------------------------------------------------------------
// user of this library can define its own exception codes by using the
// following value as a base code. for example :
//   #define MY_FIRST_EXCODE   (::nCore::EXCODE_USER_BASE + 1)
//   #define MY_SECOND_EXCODE  (::nCore::EXCODE_USER_BASE + 2)
EXCODE_DEFINE(5000, EXCODE_USER_BASE, "-")


//---------------------------------------------------------------------------
// Standard exception codes
//---------------------------------------------------------------------------
// global codes
EXCODE_DEFINE(   1, EXCODE_NONE,          "No given exception code")
EXCODE_DEFINE(   2, EXCODE_UNKNOWN,       "Unknown error")
EXCODE_DEFINE(   3, EXCODE_STD_EXCEPTION, "Standard exception") // std::exception
EXCODE_DEFINE(   4, EXCODE_ASSERT,        "Assertion")
EXCODE_DEFINE(   5, EXCODE_SYNTAX,        "Invalid syntax")
EXCODE_DEFINE(   6, EXCODE_PARAM,         "Invalid parameter(s)")
EXCODE_DEFINE(   7, EXCODE_MEMORY,        "Out of memory")
EXCODE_DEFINE(   8, EXCODE_OPERATION,     "Invalid/unsupported operation")

// generic exceptions
EXCODE_DEFINE( 100, EXCODE_SYSTEM_GENERIC, "Standard/System API call failed")

// multithreading
EXCODE_DEFINE( 200, EXCODE_THREAD_CREATE,     "Unable to create thread")
EXCODE_DEFINE( 201, EXCODE_THREAD_LAUNCH,     "Thread was properly created but it's impossible to launch it")
EXCODE_DEFINE( 202, EXCODE_THREAD_NOTJOINED,  "You are about to destroy a running thread")
EXCODE_DEFINE( 203, EXCODE_THREAD_DEADLOCK,   "Deadlock context")
EXCODE_DEFINE( 204, EXCODE_MUTEX_ACCESS,      "You do not have rights anymore to access to this entity")
EXCODE_DEFINE( 205, EXCODE_MUTEX_CREATE,      "Could not create mutex")
EXCODE_DEFINE( 206, EXCODE_MUTEX_NOTACQUIRED, "Could not acquire mutex")
EXCODE_DEFINE( 207, EXCODE_CONDITION_CREATE,  "Could not create condition")

// physical filesystem
EXCODE_DEFINE( 300, EXCODE_FILE_DOESNTEXIST, "File does not exist")
EXCODE_DEFINE( 301, EXCODE_FILE_OPEN,        "Unable to open file")
//EXCODE_DEFINE( 302, EXCODE_FILE_CREATE,      "Unable to create file")
//EXCODE_DEFINE( 303, EXCODE_FILE_NOTREADABLE, "File is not readable")
//EXCODE_DEFINE( 304, EXCODE_FILE_NOTWRITABLE, "File is not writable")
EXCODE_DEFINE( 305, EXCODE_FILE_READING,     "Error while reading file")
EXCODE_DEFINE( 306, EXCODE_FILE_WRITING,     "Error while writing to file")
EXCODE_DEFINE( 307, EXCODE_FILE_SEEKING,     "Error while seeking into file")
EXCODE_DEFINE( 308, EXCODE_FILE_DISKFULL,    "Error while writing to file (maybe disk is full)")
EXCODE_DEFINE( 309, EXCODE_FILE_MAPPING,     "Error while mapping file into memory")
EXCODE_DEFINE( 310, EXCODE_FILE_EOF,         "End of file")

// dog tags
EXCODE_DEFINE( 400, EXCODE_DOGTAG_INVALIDFORMAT, "Invalid DogTag format")


//---------------------------------------------------------------------------
// Undefine Macros
//---------------------------------------------------------------------------
#undef EXCODE_DEFINE
