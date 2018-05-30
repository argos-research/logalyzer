#include "Stage.h"

void Stage::add_event(const Event& event)
{
	_events.push_back( event );
}

void Stage::add_event(const std::string& log_line)
{

}
