#include "CapMap.h"

#include <iostream>

CapMap::CapMap()
{

}

void CapMap::add_cap_info( int badge, size_t kcap, std::shared_ptr<Stage> stage )
{
	_capabilities.emplace_back( std::make_shared<CapabilityInfo>( badge, kcap, stage ) );
}

void CapMap::remove_cap_info( int badge, size_t kcap )
{
/*	for( auto it = _capabilities.begin(); it != _capabilities.end(); ++it )
	{
		if( it->badge == badge && it->kcap == kcap && !it->removed )
		{
			it->removed = true;
			break;
		}
	}
*/
	for( auto& cap_info : _capabilities )
	{
		if( cap_info->badge == badge && cap_info->kcap == kcap && !cap_info->removed )
		{
			cap_info->removed = true;
			break;
		}
	}
}

std::shared_ptr<CapabilityInfo> CapMap::get_cap_info( int badge )
{
	for( auto& cap_info : _capabilities )
	{
		if( cap_info->badge == badge && !cap_info->removed )
			return cap_info;
	}

	return {};
}

const std::list<std::shared_ptr<CapabilityInfo>>& CapMap::get_cap_infos()
{
	return _capabilities;
}

int CapMap::kcap_to_badge( size_t kcap )
{
	for( auto& cap_info : _capabilities )
	{
		if( cap_info->kcap == kcap && !cap_info->removed )
			return cap_info->badge;
	}

	return 0;
}

const std::string&CapMap::task_name()
{
	return _task_name;
}

size_t CapMap::valid_caps()
{
	size_t count = 0;

	for( auto& cap_info : _capabilities )
	{
		if( !cap_info->removed )
			++count;
	}

	return count;
}

void CapMap::print_info()
{
	std::cout << "CapMap for task '" << _task_name << "'" << std::endl;

	for( auto& cap_info : _capabilities )
	{
		std::cout << cap_info->description() << std::endl;
	}

	std::cout << std::endl;
}
