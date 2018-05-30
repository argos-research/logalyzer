#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Logalyzer.h"

int main( int argc, char* argv[] )
{
	if( argc < 2 )
		return -1;

	std::ifstream log_file( argv[1] );

	if( !log_file.is_open() )
		return -1;

	const std::string event_prefix( "cap_cr|" );
	const char delimiter = '|';

	std::string line;
	std::vector<std::string> events;
	std::vector<unsigned int> stage_indices;

	while( std::getline( log_file, line ) )
	{
		auto pos = line.find( event_prefix );
		if( pos == std::string::npos )
			continue;

		line = line.substr( pos + event_prefix.size(), line.find_last_of( delimiter ) );

	/*	std::istringstream ss( line );
		std::string token;
		std::vector<std::string> tokens;

		while( std::getline( ss, token, delimiter ) )
		{
			tokens.push_back( token );
		}

		if( tokens[0] == "STAGE" )
			stage_indices.push_back( events.size() );
	*/
		events.push_back( line );
	}

	log_file.close();

/*	unsigned int index1, index2;
	for( size_t i = 0, j = 1; i != stage_indices.size(); ++i, ++j)
	{
		index1 = stage_indices[i];
		if( j != stage_indices.size() )
			index2 = stage_indices[j];
		else
			index2 = events.size();

		std::cout << ">>> NEW STAGE" << std::endl;

		for( ; index1 < index2; ++index1)
		{
			std::cout << events[index1] << std::endl;
		}

		std::cout << std::endl;

	}
*/
	Logalyzer logalyzer( events );
	logalyzer.print_summary();

    return 0;
}
