#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>

class Event
{
protected:
	std::vector<std::string> _data;

public:
	Event();
	explicit Event( const std::string& log_line );

	size_t add_data(const std::string& token );
	std::string get_data( size_t index );

	virtual std::string event_string();
};

#endif // EVENT_H
