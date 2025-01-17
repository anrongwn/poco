//
// LoggingFactory.cpp
//
// Library: Foundation
// Package: Logging
// Module:  LoggingFactory
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/LoggingFactory.h"
#include "Poco/SingletonHolder.h"
#include "Poco/AsyncChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/NullChannel.h"
#include "Poco/EventChannel.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOGCHANNEL)
#include "Poco/SyslogChannel.h"
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/EventLogChannel.h"
#include "Poco/WindowsConsoleChannel.h"
#endif
#include "Poco/PatternFormatter.h"


namespace Poco {


LoggingFactory::LoggingFactory()
{
	registerBuiltins();
}


LoggingFactory::~LoggingFactory()
{
}


void LoggingFactory::registerChannelClass(const std::string& className, ChannelInstantiator* pFactory)
{
	_channelFactory.registerClass(className, pFactory);
}


void LoggingFactory::registerFormatterClass(const std::string& className, FormatterFactory* pFactory)
{
	_formatterFactory.registerClass(className, pFactory);
}


Channel::Ptr LoggingFactory::createChannel(const std::string& className) const
{
	return _channelFactory.createInstance(className);
}


Formatter::Ptr LoggingFactory::createFormatter(const std::string& className) const
{
	return _formatterFactory.createInstance(className);
}


namespace
{
	static SingletonHolder<LoggingFactory> sh;
}


LoggingFactory& LoggingFactory::defaultFactory()
{
	return *sh.get();
}


void LoggingFactory::registerBuiltins()
{
	_channelFactory.registerClass("AsyncChannel", new Instantiator<AsyncChannel, Channel>);
#if defined(POCO_OS_FAMILY_WINDOWS)
	_channelFactory.registerClass("ConsoleChannel", new Instantiator<WindowsConsoleChannel, Channel>);
	_channelFactory.registerClass("ColorConsoleChannel", new Instantiator<WindowsColorConsoleChannel, Channel>);
#else
	_channelFactory.registerClass("ConsoleChannel", new Instantiator<ConsoleChannel, Channel>);
	_channelFactory.registerClass("ColorConsoleChannel", new Instantiator<ColorConsoleChannel, Channel>);
#endif

#ifndef POCO_NO_FILECHANNEL
	_channelFactory.registerClass("FileChannel", new Instantiator<FileChannel, Channel>);
	_channelFactory.registerClass("SimpleFileChannel", new Instantiator<SimpleFileChannel, Channel>);
#endif
	_channelFactory.registerClass("FormattingChannel", new Instantiator<FormattingChannel, Channel>);
#ifndef POCO_NO_SPLITTERCHANNEL
	_channelFactory.registerClass("SplitterChannel", new Instantiator<SplitterChannel, Channel>);
#endif
	_channelFactory.registerClass("NullChannel", new Instantiator<NullChannel, Channel>);
	_channelFactory.registerClass("EventChannel", new Instantiator<EventChannel, Channel>);

#if defined(POCO_OS_FAMILY_UNIX)
#ifndef POCO_NO_SYSLOGCHANNEL
	_channelFactory.registerClass("SyslogChannel", new Instantiator<SyslogChannel, Channel>);
#endif
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
	_channelFactory.registerClass("EventLogChannel", new Instantiator<EventLogChannel, Channel>);
#endif

	_formatterFactory.registerClass("PatternFormatter", new Instantiator<PatternFormatter, Formatter>);
}


} // namespace Poco
