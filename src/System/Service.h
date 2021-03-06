#ifndef SERVICE_H
#define SERVICE_H

// These are the same codes what Windows uses
#define SERVICE_STATUS_RUNNING          0x00000004
#define SERVICE_STATUS_STOP_PENDING     0x00000003

/*
===============================================================================================

 ServiceIdentity - a helper class for holding names and description of a service

===============================================================================================
*/

class ServiceIdentity
{
public:
    const char*     name;
    const char*     displayName;
    const char*     description;
};

/*
===============================================================================================

 Service - utility class for abstracting Windows Services for all platforms

===============================================================================================
*/

class Service
{
public:
    static bool     Main(int argc, char** argv, void (*func)(), ServiceIdentity& ident);
    static void     SetStatus(unsigned status);
};

#endif
