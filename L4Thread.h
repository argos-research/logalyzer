#ifndef L4THREAD_H
#define L4THREAD_H

#include <list>
#include <memory>

#include "L4Object.h"
#include "L4Gate.h"

class L4Thread : public L4Object
{
private:
	std::list<std::shared_ptr<L4Gate>> _gates;

public:
	L4Thread( int badge ) : L4Object( badge ) {}
	virtual ~L4Thread() override = default;

	virtual std::string type() override;
	virtual std::string description() override;

	void add_gate( std::shared_ptr<L4Gate> gate );
};

#endif // L4THREAD_H
