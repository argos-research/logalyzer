#include "Logalyzer.h"

#include <sstream>
#include <iostream>

#include "L4Task.h"
#include "L4Thread.h"
#include "L4Gate.h"
#include "L4IRQ.h"

Logalyzer::Logalyzer( const std::vector<std::string>& log_lines )
{
	_stages.emplace_back( std::make_shared<Stage>( DEFAULT_STAGE_NAME ) );

	auto current_stage = _stages.back();

	for( auto& line : log_lines )
	{
		std::istringstream ss( line );
		std::string token;
		std::vector<std::string> tokens;

		while( std::getline( ss, token, DELIMITER ) )
		{
			tokens.push_back( token );
		}

		auto& event_type = tokens[0];

		if( event_type == "STAGE" )
		{
			auto& stage_name = tokens[1];

			// only start changing stage names after encountering "start"
			if( current_stage->name() != DEFAULT_STAGE_NAME || stage_name == "start" )
			{
				_stages.emplace_back( std::make_shared<Stage>( stage_name ) );
				current_stage = _stages.back();
			}

		//	if( stage_name == "sleep_end" )
		//		break;
		}
		else if( event_type == "Capability_map" )
		{
			auto& this_pointer = tokens[1];
			auto& action = tokens[2];

			if( action == "ctor" )
			{
				_capmap_to_taskname.emplace( this_pointer, _current_task_name );

				_capmaps.emplace( _current_task_name, _current_task_name ); // current_task_name is key and property of CapMap
			}
			else if( action == "insert" )
			{
				auto badge = std::stoi( tokens[3], nullptr, 16 );
				auto kcap = std::stoul( tokens[4], nullptr, 16 );

			/* _capmap_core separate ???
				if( _capmap_to_taskname[ this_pointer ] == TASK_NAME_CORE )
					_capmap_core.add_cap_info( badge, kcap, current_stage );
				else
			*/
					_capmaps[ _capmap_to_taskname[ this_pointer ] ].add_cap_info( badge, kcap, current_stage );
			}
			else if( action == "remove" )
			{
				auto badge = std::stoi( tokens[3], nullptr, 16 );
				auto kcap = std::stoul( tokens[4], nullptr, 16 );

			/* _capmap_core separate ???
				if( _capmap_to_taskname[ this_pointer ] == TASK_NAME_CORE )
					_capmap_core.remove_cap_info( badge, kcap );
				else
			*/
					_capmaps[ _capmap_to_taskname[ this_pointer ] ].remove_cap_info( badge, kcap );
			}
		}
	/*	else if( event_type == "Platform_pd" )
		{
		//	cap_cr|Platform_pd|0x700c80ac|ctor|0xb0|0x238000|init|

		//	auto& this_pointer = tokens[1];
			auto& action = tokens[2];

			if( action == "ctor" )
			{
				auto badge = std::stoi( tokens[3], nullptr, 16 );
				current_task_name = tokens[5];

				_l4objects.emplace( badge, std::make_shared<L4Task>( badge, current_task_name ) );
			}
		}
	*/	else if( event_type == "l4_factory_create_task")
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto& label = tokens[3];

			_current_task_name = tokens[1] + label;

			std::cout << "current task name: " << _current_task_name << std::endl;

			_l4objects.emplace( badge, std::make_shared<L4Task>( badge, label ) );
		}
		else if( event_type == "l4_factory_create_thread" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );

			_l4objects.emplace( badge, std::make_shared<L4Thread>( badge ) );
		}
		else if( event_type == "l4_factory_create_gate" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto thread_badge = std::stoi( tokens[2], nullptr, 16 );

			auto gate = std::make_shared<L4Gate>( badge, thread_badge );
			auto result = _l4objects.emplace( badge, gate );

			if( !result.second )
			{
				std::cout << "Warning: L4 object with same badge does already exist: " << tokens[1] << std::endl;
				_l4objects.erase( result.first );
				_l4objects.emplace( badge, gate );
			}

			auto thread_it = _l4objects.find( thread_badge );

			if( thread_it != _l4objects.end() )
			{
				auto thread_obj = static_cast<L4Thread *>( thread_it->second.get() );
				thread_obj->add_gate( gate );
			}
			else
			{
				std::cout << "Warning: gate references non-existent thread: " << tokens[2] << std::endl;
			}
		}
		else if( event_type == "l4_factory_create_irq" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );

			_l4objects.emplace( badge, std::make_shared<L4IRQ>( badge ) );
		}
		else if( event_type == "l4_irq_attach" )
		{
			auto irq_badge = std::stoi( tokens[1], nullptr, 16 );
			auto thread_badge = std::stoi( tokens[2], nullptr, 16 );

			auto irq_it = _l4objects.find( irq_badge );
			auto thread_it = _l4objects.find( thread_badge );

			if( irq_it != _l4objects.end() && thread_it != _l4objects.end() )
			{
				auto irq_obj = std::static_pointer_cast<L4IRQ>( irq_it->second );
				irq_obj->attach( std::static_pointer_cast<L4Thread>( thread_it->second ) );
			}
		}
		else if( event_type == "l4_irq_attach_core_kcap" )
		{
			// special event type for l4_irq_attach inside irq_session_component.cc
			// id() of thread's Cap_index can't be accessed without code change (method-wrapped magic static)
			// id = IRQ's id()
			// kcap = thread's kcap() (from core capmap)

			auto irq_badge = std::stoi( tokens[1], nullptr, 16 );
			auto thread_kcap = std::stoul( tokens[2], nullptr, 16 );

			auto& core_map = _capmaps[ TASK_NAME_CORE ];

			int thread_badge = core_map.kcap_to_badge( thread_kcap ); // search core's capmap for kcap and return badge

			auto irq_it = _l4objects.find( irq_badge );
			auto thread_it = _l4objects.find( thread_badge );

			if( irq_it != _l4objects.end() && thread_it != _l4objects.end() )
			{
				auto irq_obj = std::static_pointer_cast<L4IRQ>( irq_it->second );
				irq_obj->attach( std::static_pointer_cast<L4Thread>( thread_it->second ) );
			}
		}
		else if( event_type == "l4_irq_detach" )
		{
			auto irq_badge = std::stoi( tokens[1], nullptr, 16 );

			auto irq_it = _l4objects.find( irq_badge );
			if( irq_it != _l4objects.end() )
			{
				auto irq_obj = std::static_pointer_cast<L4IRQ>( irq_it->second );
				irq_obj->detach();
			}
		}
		else if( event_type == "l4_debugger_set_object_name" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto& name = tokens[2];

			auto it = _l4objects.find( badge );
			if( it != _l4objects.end() )
				it->second->set_name( name );
		}
		else if( event_type == "l4_task_unmap" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );

			auto it = _l4objects.find( badge );
			if( it != _l4objects.end() )
			{
				it->second->set_unmapped();
			}
		}
		else if( event_type == "Thread_state" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto& thread_gate_badge = tokens[2];

			auto it = _l4objects.find( badge );
			if( it != _l4objects.end() )
			{
				auto gate_obj = std::static_pointer_cast<L4Gate>( it->second );
				gate_obj->mark_as_thread_state( thread_gate_badge );
			}
		}
		else if( event_type == "semaphore_irq" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto& thread_gate_badge = tokens[2];

			auto it = _l4objects.find( badge );
			if( it != _l4objects.end() )
			{
				auto gate_obj = std::static_pointer_cast<L4IRQ>( it->second );
				gate_obj->mark_as_semaphore_irq( thread_gate_badge );
			}
		}
		else if( event_type == "platform_thread_pager" )
		{
			auto badge = std::stoi( tokens[1], nullptr, 16 );
			auto& thread_badge = tokens[2];

			auto it = _l4objects.find( badge );
			if( it != _l4objects.end() )
			{
				it->second->mark_as_pager( thread_badge );
			}
		}
	}
}

void Logalyzer::print_summary()
{
/*	for( auto& capmap : _capmaps )
	{
		std::cout << "Cap map " << std::hex << capmap.first << std::endl;
		std::cout << "Active capabilities: " << std::dec << capmap.second.valid_caps() << std::endl;
		capmap.second.print_info();
	}
*/
	std::cout << std::endl;

	std::cout << "List of capmaps: " << _capmaps.size() << std::endl;
	for( auto& capmap : _capmaps )
	{
		std::cout << capmap.first << std::endl;
	}
	std::cout << std::endl;

	auto& core_map = _capmaps[ TASK_NAME_CORE ];

	std::cout << "Cap map of " << core_map.task_name() << std::endl;
	std::cout << "Active capabilities: " << std::dec << core_map.valid_caps() << std::endl;

	for( auto& cap_info : core_map.get_cap_infos() )
	{
		std::cout << cap_info->description() << std::endl;

		auto it = _l4objects.find( cap_info->badge );
		if( it != _l4objects.end() )
			std::cout << '\t' << it->second->description() << std::endl;
	}

	std::cout << std::endl;

	auto& target_map = _capmaps[ _current_task_name ];

	std::cout << "Cap map of " << target_map.task_name() << std::endl;
	std::cout << "Active capabilities: " << std::dec << target_map.valid_caps() << std::endl;

	for( auto& cap_info : target_map.get_cap_infos() )
	{
		std::cout << cap_info->description() << std::endl;

		auto it = _l4objects.find( cap_info->badge );
		if( it != _l4objects.end() )
			std::cout << '\t' << it->second->description() << std::endl;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for( auto& stage : _stages )
	{
		std::cout << stage->name() << std::endl;
	}

/*	for( auto& object : _l4objects )
	{
		auto badge = object.second->badge();
		auto cap_info = coremap.get_cap_info( badge );

		std::cout << object.second->description();

		if( cap_info )
			std::cout << " stage=" << cap_info->stage->name();

		std::cout << std::endl;
	}
*/
}
