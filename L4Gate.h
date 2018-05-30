#ifndef L4GATE_H
#define L4GATE_H

#include "L4Object.h"

class L4Gate : public L4Object
{
private:
	int _thread_badge;

public:
	L4Gate( int badge, int thread_badge ) : L4Object( badge ), _thread_badge( thread_badge ) {}
	virtual ~L4Gate() override = default;

	virtual std::string type() override;
	virtual std::string description() override;

	void mark_as_thread_state( const std::string& thread_gate_cap ); // thread_gate_cap is the capability id of the thread's gate
};

#endif // L4GATE_H
