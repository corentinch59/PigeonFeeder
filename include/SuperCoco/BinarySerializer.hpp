#ifndef SUPERCOCO_BSERIALIZER_HPP
#define SUPERCOCO_BSERIALIZER_HPP

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

namespace Sce
{
	template<typename T>
	void SerializeBinary(std::vector<uint8_t>& byteArray, T value)
	{
		std::size_t offset = byteArray.size();
		byteArray.resize(offset + sizeof(value));
		std::memcpy(&byteArray[offset], &value, sizeof(value));
	}

	template<typename T>
	void SerializeBinary(std::vector<uint8_t>& byteArray, const std::string& value)
	{
		SerializeBinary<std::uint16_t>(byteArray, value.size());

		std::size_t offset = byteArray.size();
		byteArray.resize(offset + value.size());
		std::memcpy(&byteArray[offset], &value[0], value.size());
	}

	template<typename T>
	T UnserializeBinary(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
	{
		T value;
		std::memcpy(&value, &byteArray[offset], sizeof(T));
		offset += sizeof(T);

		return value;
	}

	template<>
	std::string UnserializeBinary(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
	{
		std::uint16_t strLength = UnserializeBinary<std::uint16_t>(byteArray, offset);

		std::string value(strLength, '\0');
		std::memcpy(&value[0], &byteArray[offset], strLength);
		offset += strLength;

		return value;
	}

}


#endif