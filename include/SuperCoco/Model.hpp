#ifndef SUPERCOCO_MODEL_HPP
#define SUPERCOCO_MODEL_HPP

#pragma once

#include <SuperCoco/Asset.hpp>
#include <SuperCoco/Color.hpp>
#include <SuperCoco/Export.hpp>
#include <SuperCoco/Renderable.hpp>
#include <SuperCoco/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>

namespace Sce
{
	template<typename T> class Matrix;
	using Matrixf = Matrix<float>;

	class Renderer;
	class Texture;
	class WorldEditor;

	struct ModelVertex
	{
		Vector2f pos;
		Vector2f uv;
		Color color;
	};

	class SUPER_COCO_API Model : public Asset, public IRenderable
	{
		public:
			Model();
			Model(std::shared_ptr<Texture> sharedTexture, std::vector<ModelVertex> vertices, std::vector<int> indices, std::string filepath);
			Model(const Model&) = delete;
			Model(Model&&) = default;
			~Model() = default;


			virtual void Render(Renderer& renderer, const Matrixf& transformMatrix) const override;

			nlohmann::json SaveToJSon() const;

			SDL_FRect GetBounds() const override;
			int GetLayer() const override;
			const std::vector<ModelVertex>& GetVertices() const;

			bool IsValid() const;

			void PopulateInspector(WorldEditor& worldEditor) override;

			nlohmann::json Serialize() const override;

			bool SaveToFile(const std::string& filepath) const;

			static Model LoadFromFile(const std::string& filepath);
			static Model LoadFromJson(const nlohmann::json& doc, std::string filepath = {});
			static std::shared_ptr<Model> Unserialize(const nlohmann::json& doc);

		private:
			bool SaveToFileRegular(const std::string& filepath) const;
			bool SaveToFileCompressed(const std::string& filepath) const;
			bool SaveToFileBinary(const std::string& filepath) const;

			static Model LoadFromFileRegular(const std::string& filepath);
			static Model LoadFromFileCompressed(const std::string& filepath);
			static Model LoadFromFileBinary(const std::string& filepath);

			std::shared_ptr<Texture> m_texture;
			std::vector<ModelVertex> m_vertices;

			mutable std::vector<SDL_Vertex> m_sdlVertices;
			std::vector<int> m_indices;
			SDL_FRect m_bounds;
	};
}

#endif