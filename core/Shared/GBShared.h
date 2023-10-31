#pragma once

#include <string_view>
#include <format>
#include <cassert>

namespace GenericBoson
{
	typedef size_t BUFFER_SIZE_TYPE;
	constexpr BUFFER_SIZE_TYPE BUFFER_SIZE = 8 * 1024;

	enum class IO_TYPE : int32_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	class VectoredIO
	{
	public:
		enum STATE : char
		{
			ID = 0,
			LENGTH,
			PAYLOAD,
			MAX_STATE
		};
	public:
		void AdvanceState()
		{
			m_state = STATE{ (char{ m_state } + 1) % STATE::MAX_STATE };
		}

		STATE GetState() const
		{
			return m_state;
		}

	public:
		char m_buffer[BUFFER_SIZE];
		BUFFER_SIZE_TYPE m_length = 0, m_offset = 0;

	private:
		STATE m_state = STATE::ID;
	};

	class GBShared
	{
	public:
		virtual bool ErrorLog(const std::string_view msg);
		virtual bool WarningLog(const std::string_view msg);
		virtual bool InfoLog(const std::string_view msg);

		virtual bool OnGatheringCompleted(VectoredIO& inputData, const BUFFER_SIZE_TYPE messageID) {};
	
		bool OnReceived(VectoredIO& inputData, const unsigned long transferredBytes);
		bool OnSent(VectoredIO& outputData, const unsigned long transferredBytes);
	private:
		bool Gather(VectoredIO& vectoredIO, const unsigned long transferredBytes);
	};
}