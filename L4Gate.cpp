#include "L4Gate.h"

std::string L4Gate::type()
{
	return "L4Gate";
}

std::string L4Gate::description()
{
	std::stringstream ss;

	ss << std::hex << std::showbase << type() << " badge=" << _badge << " name='" << _name << "' thread=" << _thread_badge
	   << ' ' << _additional_info;

	return ss.str();
}

void L4Gate::mark_as_thread_state( const std::string& thread_gate_cap )
{
	_additional_info += "<Thread_state for " + thread_gate_cap + '>';
}
