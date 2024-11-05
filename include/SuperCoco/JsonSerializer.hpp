#ifndef SUPERCOCO_JSONSERIALIZER_HPP
#define SUPERCOCO_JSONSERIALIZER_HPP

#pragma once

#include <nlohmann/json.hpp>
#include <SDL2/SDL_render.h>
#include <SuperCoco/Vector2.hpp>
#include <SuperCoco/Transform.hpp>
#include <SDL2/SDL_rect.h>

inline void from_json(const nlohmann::json& j, SDL_Rect& rect)
{
	rect.x = j.at("x");
	rect.y = j.at("y");
	rect.w = j.at("w");
	rect.h = j.at("h");
}

inline void from_json(const nlohmann::ordered_json& j, SDL_Rect& rect)
{
	rect.x = j.at("x");
	rect.y = j.at("y");
	rect.w = j.at("w");
	rect.h = j.at("h");
}

inline void to_json(nlohmann::json& j, const SDL_Rect& rect)
{
	j["x"] = rect.x;
	j["y"] = rect.y;
	j["w"] = rect.w;
	j["h"] = rect.h;
}

inline void to_json(nlohmann::ordered_json& j, const SDL_Rect& rect)
{
	j["x"] = rect.x;
	j["y"] = rect.y;
	j["w"] = rect.w;
	j["h"] = rect.h;
}

namespace Sce
{
	template<typename T>
	inline void to_json(nlohmann::json& j, const Vector2<T>& vec)
	{
		j["x"] = vec.x;
		j["y"] = vec.y;
	}

	template<typename T>
	inline void to_json(nlohmann::ordered_json& j, const Vector2<T>& vec)
	{
		j["x"] = vec.x;
		j["y"] = vec.y;
	}

	template<typename T>
	inline void from_json(const nlohmann::json& j, Vector2<T>& vec)
	{
		vec.x = j.at("x");
		vec.y = j.at("y");
	}

	template<typename T>
	inline void from_json(const nlohmann::ordered_json& j, Vector2<T>& vec)
	{
		vec.x = j.at("x");
		vec.y = j.at("y");
	}
}

inline void to_json(nlohmann::json& j, const SDL_Color& color)
{
	j["r"] = color.r;
	j["g"] = color.g;
	j["b"] = color.b;
}

inline void to_json(nlohmann::ordered_json& j, const SDL_Color& color)
{
	j["r"] = color.r;
	j["g"] = color.g;
	j["b"] = color.b;
}

inline void from_json(const nlohmann::json& j, SDL_Color& color)
{
	color.r = j.at("r");
	color.g = j.at("g");
	color.b = j.at("b");
	color.a = 255;
}

inline void from_json(const nlohmann::ordered_json& j, SDL_Color& color)
{
	color.r = j.at("r");
	color.g = j.at("g");
	color.b = j.at("b");
	color.a = 255;
}

inline void to_json(nlohmann::json& j, const SDL_Vertex& vertex)
{
	j["pos"] = Sce::Vector2f(vertex.position);
	j["uv"] = Sce::Vector2f(vertex.tex_coord);
	j["color"] = vertex.color;
}

inline void to_json(nlohmann::ordered_json& j, const SDL_Vertex& vertex)
{
	j["pos"] = Sce::Vector2f(vertex.position);
	j["uv"] = Sce::Vector2f(vertex.tex_coord);
	j["color"] = vertex.color;
}

inline void from_json(const nlohmann::json& j, SDL_Vertex& vertex)
{
	Sce::Vector2f a = j.at("pos");
	Sce::Vector2f b = j.at("uv");
	vertex.position = a;
	vertex.tex_coord = b;
	vertex.color = j.at("color");
}

inline void from_json(const nlohmann::ordered_json& j, SDL_Vertex& vertex)
{
	Sce::Vector2f a = j.at("pos");
	Sce::Vector2f b = j.at("uv");
	vertex.position = a;
	vertex.tex_coord = b;
	vertex.color = j.at("color");
}

#endif