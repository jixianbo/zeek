// Capsulates local and remote event handlers.

#pragma once

#include "BroList.h"
#include "ZeekArgs.h"

#include <unordered_set>
#include <string>

class Func;
class FuncType;

class EventHandler {
public:
	explicit EventHandler(const char* name);
	~EventHandler();

	const char* Name()	{ return name; }
	Func* LocalHandler()	{ return local; }
	FuncType* FType(bool check_export = true);

	void SetLocalHandler(Func* f);

	void AutoPublish(std::string topic)
		{
		auto_publish.insert(std::move(topic));
		}

	void AutoUnpublish(const std::string& topic)
		{
		auto_publish.erase(topic);
		}

	void Call(const zeek::Args& vl, bool no_remote = false);

	// Returns true if there is at least one local or remote handler.
	explicit operator  bool() const;

	void SetUsed()	{ used = true; }
	bool Used()	{ return used; }

	// Handlers marked as error handlers will not be called recursively to
	// avoid infinite loops if they trigger a similar error themselves.
	void SetErrorHandler()	{ error_handler = true; }
	bool ErrorHandler()	{ return error_handler; }

	void SetEnable(bool arg_enable)	{ enabled = arg_enable; }

	// Flags the event as interesting even if there is no body defined. In
	// particular, this will then still pass the event on to plugins.
	void SetGenerateAlways()	{ generate_always = true; }
	bool GenerateAlways()	{ return generate_always; }

private:
	void NewEvent(const zeek::Args& vl);	// Raise new_event() meta event.

	const char* name;
	Func* local;
	FuncType* type;
	bool used;		// this handler is indeed used somewhere
	bool enabled;
	bool error_handler;	// this handler reports error messages.
	bool generate_always;

	std::unordered_set<std::string> auto_publish;
};

// Encapsulates a ptr to an event handler to overload the boolean operator.
class EventHandlerPtr {
public:
	EventHandlerPtr(EventHandler* p = nullptr)		{ handler = p; }
	EventHandlerPtr(const EventHandlerPtr& h)	{ handler = h.handler; }

	const EventHandlerPtr& operator=(EventHandler* p)
		{ handler = p; return *this; }
	const EventHandlerPtr& operator=(const EventHandlerPtr& h)
		{ handler = h.handler; return *this; }

	bool operator==(const EventHandlerPtr& h) const
		{ return handler == h.handler; }

	EventHandler* Ptr()	{ return handler; }

	explicit operator bool() const	{ return handler && *handler; }
	EventHandler* operator->()	{ return handler; }
	const EventHandler* operator->() const	{ return handler; }

private:
	EventHandler* handler;
};
