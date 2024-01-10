#pragma once

#define NOMINMAX
#define BOOST_THREAD_VERSION 5

#include "boost/thread/future.hpp"
#include "boost/thread/executors/executor.hpp"
#include "winsock2.h"
#include "MSWSock.h"

#include "BufferAllocator.h"
#include "Util.h"
#include "ExpandedOverlapped.h"
#include "../Shared/Shared.h"
#include "../Shared/Constant.h"

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <unordered_map>
#include <limits>

namespace flatbuffers
{
	class Table;
}

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	class Server
	{
	public:
		Server() = default;
		Server(uint16_t portNum) : m_port(portNum) {}
		Server(const Server&) = default;

		std::string Start();

		virtual ~Server();

		bool GetKeepLooping() const
		{
			return m_keepLooping;
		}

		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			assert(messageID < ENGINE_RESERVED_PROTOCOL_NUMBER_RANGE_START);
			assert(ENGINE_RESERVED_PROTOCOL_NUMBER_RANGE_END < messageID);

			return m_CoreShared.AddStubInternal(messageID, Stub);
		}

		template<typename CALLABLE>
		bool Send(ExpandedOverlapped* pEol, const int32_t messageID,
			CALLABLE&& callable)
		{
			if (!pEol)
			{
				// #ToDo
				m_CoreShared.ErrorLog("");
				return false;
			}

			::flatbuffers::FlatBufferBuilder fbb((size_t)BUFFER_SIZE, &g_bufferAllocator);

			// Do not use FinishSizePrefixed because it makes buffer invalid.
			fbb.Finish(std::forward<CALLABLE>(callable)(fbb));

			size_t size, offset;
			char* pFlatRawBuffer = reinterpret_cast<char*>(fbb.ReleaseRaw(size, offset));

			int32_t tableSize = size - offset;

			char* buffer = pEol->m_outputData.m_buffer;

			memcpy_s(buffer, BUFFER_SIZE, &messageID, sizeof(messageID));
			buffer += sizeof(messageID);
			memcpy_s(buffer, BUFFER_SIZE, &tableSize, sizeof(tableSize));
			buffer += sizeof(tableSize);
			memcpy_s(buffer, BUFFER_SIZE, pFlatRawBuffer + offset, tableSize);
			buffer += tableSize;

			pEol->m_outputData.m_offset = tableSize + sizeof(messageID) + sizeof(tableSize);

			__super::Send(pEol);

			return true;
		};

		void SendPing(ExpandedOverlapped* pEol);

		void SetConnectedTask(const std::function<void(ExpandedOverlapped* pEol)>& task);
	protected:
		void OnConnected(ExpandedOverlapped* pEol);

		void Send(ExpandedOverlapped* pEol);

		//// \return true - all completed, false - not yet gathering completed.
		//virtual bool OnReceived(ExpandedOverlapped* pEol, DWORD receivedBytes) = 0;
		//virtual bool OnSent(ExpandedOverlapped* pEol, DWORD sentBytes) = 0;
		int IssueRecv(ExpandedOverlapped* pEol, ULONG lengthToReceive);
		int IssueSend(ExpandedOverlapped* pEol, const unsigned long throttling = 30);//(std::numeric_limits<unsigned long>::max)());
	private:
		std::pair<bool, std::string> SetListeningSocket();
		void SendThreadFunction();

		void ThreadFunction();
	private:
		CoreShared m_CoreShared;

		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		boost::future<void> m_sendTask;

		// #ToDo lock free circular queue
		std::mutex m_sendLock;
		std::unordered_map<SOCKET, std::queue<ExpandedOverlapped*>> m_sendQueues;
		std::vector<ExpandedOverlapped> m_sessions;

		WSADATA m_wsaData;
		sockaddr_in m_addr, m_client;

		SOCKET m_listeningSocket;

		// AcceptEx 함수 포인터
		LPFN_ACCEPTEX m_lpfnAcceptEx = NULL;
		uint16_t m_port = 0;
		int m_addrSize = sizeof(sockaddr_in);

		// 주의 : 실제 사용은 안하지만 있어야 제대로 동작한다.
		char m_listenBuffer[1024] = { 0, };

		std::atomic_bool m_keepLooping = true;

		HANDLE m_IOCP = INVALID_HANDLE_VALUE;

		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::function<void(ExpandedOverlapped* pEol)> m_connectedTask;

		static ThreadSafeBufferAllocator g_bufferAllocator;
	};
}