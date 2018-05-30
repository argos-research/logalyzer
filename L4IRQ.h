#ifndef L4IRQ_H
#define L4IRQ_H

#include <list>
#include <memory>

#include "L4Object.h"
#include "L4Thread.h"

class L4IRQ : public L4Object
{
private:
	std::list<std::shared_ptr<L4Thread>> _threads;

public:
	L4IRQ( int badge );
	virtual ~L4IRQ() override = default;

	virtual std::string type() override;
	virtual std::string description() override;

	void attach( std::shared_ptr<L4Thread> thread );
	void detach(); // detaches all threads

	void mark_as_semaphore_irq( const std::string& thread_badge );
};

#endif // L4IRQ_H
