#ifndef SUPERCOCO_RENDERABLE_HPP
#define SUPERCOCO_RENDERABLE_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <nlohmann/json_fwd.hpp>

struct SDL_FRect;

namespace Sce
{
	class Renderer;
	class WorldEditor;

	template<typename T> class Matrix;
	using Matrixf = Matrix<float>;

	class SUPER_COCO_API IRenderable
	{
	public:
		virtual ~IRenderable() = default;

		virtual void Render(Renderer& renderer, const Matrixf& transformMatrix) const = 0;

		virtual SDL_FRect GetBounds() const = 0;
		virtual int GetLayer() const = 0;

		virtual void PopulateInspector(WorldEditor& worldEditor) = 0;
		virtual nlohmann::json Serialize() const = 0;

	};
}

#endif