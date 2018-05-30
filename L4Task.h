#ifndef L4TASK_H
#define L4TASK_H

#include "L4Object.h"

class L4Task : public L4Object
{
public:
	L4Task( int badge, const std::string& name ) : L4Object( badge, name ) {}
	virtual ~L4Task() override = default;

	virtual std::string type() override;
};

#endif // L4TASK_H
