#include "stdafx.h"

#define NOMINMAX

#include "GBGameServer.h"
#include "../../core/server/GBExpandedOverlapped.h"


namespace GenericBoson
{
	void GBGameServer::OnConnected(GBExpandedOverlapped* pEol)
	{
		if (m_connectedTask)
		{
			m_connectedTask(pEol);
		}
	}

	void GBGameServer::SetConnectedTask(
		const std::function<void(GBExpandedOverlapped* pEol)>& task
	)
	{
		m_connectedTask = task;
	}

	ThreadSafeBufferAllocator GBGameServer::g_bufferAllocator;
}