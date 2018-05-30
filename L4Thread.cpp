#include "L4Thread.h"

#include <iostream>

std::string L4Thread::type()
{
	return "L4Thread";
}

void L4Thread::add_gate(std::shared_ptr<L4Gate> gate)
{
	_gates.push_back( gate );
}

std::string L4Thread::description()
{
	std::stringstream ss;

	ss << std::hex << std::showbase << type() << " badge=" << _badge << " name='" << _name << "' gate(s)=";

	if( _gates.size() > 0 )
	{
		ss << _gates.front()->badge();

		if( _gates.size() > 1 )
			ss << std::dec << '(' << _gates.size() << ')';
	}
	else
	{
		ss << "<none>";
	}

	return ss.str();
}
