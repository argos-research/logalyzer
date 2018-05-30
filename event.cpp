#include "event.h"

#include <sstream>

Event::Event()
{

}

size_t Event::add_data( const std::string& token )
{
	auto size = _data.size();
	_data.push_back( token );

	return size;
}

std::string Event::get_data( size_t index )
{
	if( index >= _data.size() )
		return {}; // empty string

	return _data[ index ];
}

std::string Event::event_string()
{
	std::ostringstream ss;

	for( auto& element : _data )
		ss << element << '|';

	return ss.str();
}
