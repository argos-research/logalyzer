#include "L4Object.h"

int L4Object::badge()
{
	return _badge;
}

const std::string& L4Object::name()
{
	return _name;
}

void L4Object::set_name(const std::string& name)
{
	_name = name;
}

bool L4Object::is_unmapped()
{
	return _unmapped;
}

void L4Object::set_unmapped()
{
	_unmapped = true;
}

void L4Object::mark_as_pager(const std::string& thread_badge)
{
	_additional_info += "<pager for " + thread_badge + '>';
}

std::string L4Object::type()
{
	return "L4Object";
}

std::string L4Object::description()
{
	std::stringstream ss;

	ss << std::hex << std::showbase << type() << " badge=" << _badge << " name='" << _name << "' " << _additional_info;

	return ss.str();
}
