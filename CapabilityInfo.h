#ifndef CAPABILITYINFO_H
#define CAPABILITYINFO_H

#include <cstddef>
#include <memory>
#include <sstream>
#include <map>

#include "Stage.h"

class CapabilityInfo
{
public:

	CapabilityInfo( int badge, size_t kcap, std::shared_ptr<Stage> stage )
		: badge( badge ), kcap( kcap ), stage( stage )
	{}

	std::string description()
	{
		std::stringstream ss;

		ss << std::hex << std::showbase << "cap badge=" << badge << " kcap=" << kcap;

		auto it = const_caps.find( kcap );
		if( it != const_caps.end() )
			ss << '(' << it->second << ')';

		ss << std::dec << " removed=" << removed << " stage=" << stage->name();

		return ss.str();
	}

	int badge;
	size_t kcap;
	std::shared_ptr<Stage> stage;
	bool removed = false;

	std::map<size_t, std::string> const_caps // TODO: make static const and move to .cpp file
	{
		{ 0x1000, "L4_BASE_TASK_CAP" },
		{ 0x2000, "L4_BASE_FACTORY_CAP" },
		{ 0x3000, "L4_BASE_THREAD_CAP" },
		{ 0x4000, "L4_BASE_PAGER_CAP" },
		{ 0x5000, "L4_BASE_LOG_CAP" },
		{ 0x6000, "L4_BASE_ICU_CAP" },
		{ 0x7000, "L4_BASE_SCHEDULER_CAP" },
		{ 0x8000, "PARENT_CAP" },
		{ 0x9000, "MAIN_THREAD_CAP" },
	};
};

#endif // CAPABILITYINFO_H
