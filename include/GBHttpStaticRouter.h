#pragma once

#define COMBINE_INTERNAL(X,Y) X##Y
#define COMBINE(X,Y) COMBINE_INTERNAL(X,Y)

#define PUT(PATH,CALLABLE) static const char COMBINE(PUT,__LINE__)[]{ PATH };Put<COMBINE(PUT,__LINE__)>(CALLABLE)
#define HEAD(PATH,CALLABLE) static const char COMBINE(HEAD,__LINE__)[]{ PATH };Head<COMBINE(HEAD,__LINE__)>(CALLABLE)
#define GET(PATH,CALLABLE) static const char COMBINE(GET,__LINE__)[]{ PATH };Get<COMBINE(GET,__LINE__)>(CALLABLE)
#define POST(PATH,CALLABLE) static const char COMBINE(POST,__LINE__)[]{ PATH };Post<COMBINE(POST,__LINE__)>(CALLABLE)

namespace Proud
{
	template<const char* PATH, typename CALLABLE>
	void Put(CALLABLE callable)
	{
		// 사전 작업을 여기서 한다.
		static const char[] putStr = "Put";
		Before<putStr, PATH>();

		callable();

		// 사후 작업을 여기서 한다.
		After<putStr, PATH>();
	}

	template<const char* PATH, typename CALLABLE>
	void Head(CALLABLE callable)
	{
		// 사전 작업을 여기서 한다.
		static const char[] putStr = "Head";
		Before<putStr, PATH>();

		callable();

		// 사후 작업을 여기서 한다.
		After<putStr, PATH>();
	}

	template<const char* PATH, typename CALLABLE>
	void Get(CALLABLE callable)
	{
		// 사전 작업을 여기서 한다.
		static const char[] putStr = "Get";
		Before<putStr, PATH>();

		callable();

		// 사후 작업을 여기서 한다.
		After<putStr, PATH>();
	}

	template<const char* PATH, typename CALLABLE>
	void Post(CALLABLE callable)
	{
		// 사전 작업을 여기서 한다.
		static const char[] putStr = "Post";
		Before<putStr, PATH>();

		callable();

		// 사후 작업을 여기서 한다.
		After<putStr, PATH>();
	}

	template<const char* METHOD_NAME, const char* PATH>
	void Before()
	{
		
	}

	template<const char* METHOD_NAME, const char* PATH>
	void After()
	{

	}
}