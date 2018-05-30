#include "L4IRQ.h"

L4IRQ::L4IRQ(int badge) : L4Object( badge ) {}

std::string L4IRQ::type()
{
	return "L4IRQ";
}

std::string L4IRQ::description()
{
	std::stringstream ss;

	ss << std::hex << std::showbase << type() << " badge=" << _badge << " name='" << _name << "' thread(s)=";

	for( auto& thread : _threads )
	{
		ss << thread->badge() << '|';
	}

	ss << ' ' << _additional_info;

	return ss.str();
}

void L4IRQ::attach( std::shared_ptr<L4Thread> thread )
{
	_threads.push_back( thread );
}

void L4IRQ::detach()
{
	_threads.clear();
}

void L4IRQ::mark_as_semaphore_irq( const std::string& thread_badge )
{
	_additional_info += "<semaphore IRQ for " + thread_badge + '>';
}
