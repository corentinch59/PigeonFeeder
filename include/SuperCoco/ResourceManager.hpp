#ifndef SUPERCOCO_RESOURCEMANAGER_HPP
#define SUPERCOCO_RESOURCEMANAGER_HPP

#include <SuperCoco/Export.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace Sce
{
	class Texture;
	class Renderer;
	class Surface;
	class Spritesheet;
	class Model;
	class Font;

	class SUPER_COCO_API ResourceManager
	{
		
		public:
			ResourceManager(Renderer* renderer);
			ResourceManager(const ResourceManager&) = delete;
			ResourceManager(const ResourceManager&&) = delete;
			~ResourceManager();

			const std::shared_ptr<Model>& GetModel(const std::string& modelPath);
			const std::shared_ptr<Texture>& GetTexture(const std::string& filepath);
			const std::shared_ptr<Spritesheet>& GetSpritesheet(const std::string& spritesheetPath);
			const std::shared_ptr<Font>& GetFont(const std::string& fontPath, uint8_t size);

			void Purge();

			static ResourceManager& Instance();

			ResourceManager& operator=(const ResourceManager&) = delete;
			ResourceManager& operator=(const ResourceManager&&) = delete;

		private:
			std::shared_ptr<Spritesheet> m_missingSpritesheet;
			std::shared_ptr<Texture> m_missingTexture;
			std::shared_ptr<Model> m_missingModel;
			std::unordered_map<std::string, std::shared_ptr<Font>> m_fonts;
			std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
			std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
			std::unordered_map<std::string, std::shared_ptr<Spritesheet>> m_spritesheets;

			Renderer* m_renderer;

			static ResourceManager* s_instance;
	};
}


#endif