#ifndef LOGALYZER_H
#define LOGALYZER_H

#include <map>
#include <list>
#include <vector>
#include <memory>
#include <unordered_map>

#include "CapabilityInfo.h"
#include "Stage.h"
#include "CapMap.h"
#include "L4Object.h"

class Logalyzer
{
private:
	CapMap _capmap_core;
	std::list<std::shared_ptr<Stage>> _stages;
	std::unordered_map<std::string, CapMap> _capmaps;
	std::map<int, std::shared_ptr<L4Object>> _l4objects;

	std::unordered_map<std::string, std::string> _capmap_to_taskname;

	std::string _current_task_name{ TASK_NAME_CORE };

public:
	static constexpr const char DELIMITER = '|';
	static constexpr const char* TASK_NAME_CORE = "core";
	const std::string DEFAULT_STAGE_NAME = "default_stage"; // TODO: make static

	Logalyzer( const std::vector<std::string>& log_lines );

	void print_summary();
};

#endif // LOGALYZER_H
