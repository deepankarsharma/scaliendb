#ifndef PAXOSLEASEACCEPTOR_H
#define PAXOSLEASEACCEPTOR_H

#include "System/Common.h"
#include "System/Events/Timer.h"
#include "Framework/Replication/Quorums/QuorumContext.h"
#include "Framework/Replication/Quorums/QuorumTransport.h"
#include "States/PaxosLeaseAcceptorState.h"
#include "PaxosLeaseMessage.h"

/*
===============================================================================

 PaxosLeaseAcceptor

===============================================================================
*/

class PaxosLeaseAcceptor
{
public:
	void						Init(QuorumContext* context);
	void						OnMessage(const PaxosLeaseMessage& msg);

private:
	void						OnLeaseTimeout();
	void						OnPrepareRequest(const PaxosLeaseMessage& msg);
	void						OnProposeRequest(const PaxosLeaseMessage& msg);

	QuorumContext*				context;
	PaxosLeaseAcceptorState		state;
	Timer						leaseTimeout;
};

#endif
