#ifndef SUPERCOCO_COMPONENTSERIALIZER_HPP
#define SUPERCOCO_COMPONENTSERIALIZER_HPP

#pragma once

#include <SuperCoco/Export.hpp>

class SUPER_COCO_API ComponentSerializer
{
public:
	ComponentSerializer() = default;
	~ComponentSerializer() = default;
	ComponentSerializer(ComponentSerializer&) = delete;
	ComponentSerializer(ComponentSerializer&&) noexcept = delete;

	ComponentSerializer& operator=(ComponentSerializer&) = delete;
	ComponentSerializer& operator=(ComponentSerializer&&) noexcept = delete;

	
};

#endif