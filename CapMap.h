#ifndef CAPMAP_H
#define CAPMAP_H

#include <string>
#include <cstddef>
#include <list>
#include <memory>

#include "CapabilityInfo.h"
#include "Stage.h"

class CapMap
{
private:
	std::string _task_name;
	std::list<std::shared_ptr<CapabilityInfo>> _capabilities;

public:
	CapMap();
	CapMap( const std::string& task_name ) : _task_name( task_name ) {}

	void add_cap_info( std::shared_ptr<CapabilityInfo> cap_info );
	void add_cap_info( int badge, size_t kcap, std::shared_ptr<Stage> stage );
	void remove_cap_info( int badge, size_t kcap );

	std::shared_ptr<CapabilityInfo> get_cap_info( int badge );
	const std::list<std::shared_ptr<CapabilityInfo>>& get_cap_infos();

	int kcap_to_badge( size_t kcap );

	const std::string& task_name();
	size_t valid_caps();
	void print_info();
};

#endif // CAPMAP_H
