#ifndef QUORUMCONTEXT_H
#define QUORUMCONTEXT_H

#include "System/Events/Callable.h"
#include "Quorum.h"

#include "Framework/Replication/Quorums/QuorumDatabase.h"
#include "Framework/Replication/Quorums/QuorumTransport.h"

/*
===============================================================================

 QuorumContext
 
 This should be called QuorumContext.

===============================================================================
*/

class QuorumContext
{
public:
	virtual ~QuorumContext() {};
	
	virtual bool					IsLeader()					= 0;
	virtual bool					IsLeaderKnown()				= 0;
	virtual uint64_t				GetLeader()					= 0;
	virtual uint64_t				GetEpoch() const			= 0;
	virtual uint64_t				GetLogID() const			= 0;
	virtual uint64_t				GetPaxosID() const			= 0;

	virtual Quorum*					GetQuorum() const			= 0;
	virtual QuorumDatabase*			GetDatabase() const			= 0;
	virtual QuorumTransport*		GetTransport() const		= 0;
	
	virtual void					OnMessage()					= 0;
};

#endif
