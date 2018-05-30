#ifndef STAGE_H
#define STAGE_H

#include <list>

#include "event.h"

class Stage
{
private:
	std::string _name;
	std::list<Event> _events;

public:
	Stage() = default;
	explicit Stage( const std::string& name ) : _name( name ) {}

	void add_event( const Event& event );
	void add_event( const std::string& log_line );

	const std::string& name() { return _name; }
};

#endif // STAGE_H
