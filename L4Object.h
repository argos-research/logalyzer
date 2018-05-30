#ifndef L4OBJECT_H
#define L4OBJECT_H

#include <string>
#include <sstream>

class L4Object
{
protected:
	int _badge;
	std::string _name;
	std::string _additional_info;
	bool _unmapped = false;

public:
	L4Object( int badge ) : _badge( badge ) {}
	L4Object( int badge, const std::string& name ) : _badge( badge ), _name( name ) {}
	virtual ~L4Object() = default;

	int badge();
	const std::string& name();
	void set_name( const std::string& name );

	bool is_unmapped();
	void set_unmapped();

	void mark_as_pager( const std::string& thread_badge );

	virtual std::string type();
	virtual std::string description();
};

#endif // L4OBJECT_H
