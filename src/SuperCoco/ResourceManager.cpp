#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Surface.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/Model.hpp>
#include <SuperCoco/Font.hpp>
#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <stdexcept>

namespace Sce
{
	ResourceManager* ResourceManager::s_instance = nullptr;

	ResourceManager::ResourceManager(Renderer* renderer) :
	m_renderer(renderer)
	{
		if (!s_instance)
			s_instance = this;
		else
			throw std::runtime_error("There is more than 1 resource manager object");

	}

	ResourceManager::~ResourceManager()
	{
		s_instance = nullptr;
	}

	ResourceManager& ResourceManager::Instance()
	{
		return *s_instance;
	}

	void ResourceManager::Purge()
	{
		for (auto i = m_textures.begin(); i != m_textures.end();)
		{
			if (i->second.use_count() <= 1)
			{
				fmt::print("Unused entry found, erasing entry...\n");
				i = m_textures.erase(i);
			}
			else
			{
				i++;
			}
			
		}
	}

	const std::shared_ptr<Model>& ResourceManager::GetModel(const std::string& modelPath)
	{
		// Avons-nous déjà ce modèle en stock ?
		auto it = m_models.find(modelPath);
		if (it != m_models.end())
			return it->second; // Oui, on peut le renvoyer

		// Non, essayons de le charger
		Model model = Model::LoadFromFile(modelPath);
		if (!model.IsValid())
		{
			// On a pas pu charger le modèle, utilisons un modèle "manquant"
			if (!m_missingModel)
				m_missingModel = std::make_shared<Model>();

			m_models.emplace(modelPath, m_missingModel);
			return m_missingModel;
		}

		it = m_models.emplace(modelPath, std::make_shared<Model>(std::move(model))).first;
		return it->second;
	}

	const std::shared_ptr<Texture>& ResourceManager::GetTexture(const std::string& filepath)
	{
		auto resource = m_textures.find(filepath);
		if (resource != m_textures.end())
			return resource->second;
		try 
		{
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture::LoadFromFile(*m_renderer, filepath));
			texture->m_path = filepath;
			resource = m_textures.emplace(filepath, std::move(texture)).first;
			return resource->second;
		}
		catch (const std::exception& e)
		{
			fmt::print(fg(fmt::color::red), "failed to load {0}: {1}\n", filepath, e.what());
			if (!m_missingTexture)
			{
				Surface missing = Surface::Create(64, 64);
				missing.FillRect(SDL_Rect{0, 0, 16, 16}, 254, 0, 254, 255);
				missing.FillRect(SDL_Rect{ 16, 0, 16, 16 }, 0, 0, 0, 255);
				missing.FillRect(SDL_Rect{ 0, 16, 16, 16 }, 0, 0, 0, 255);
				missing.FillRect(SDL_Rect{16, 16, 16, 16}, 254, 0, 254, 255);

				m_missingTexture = std::make_shared<Texture>(Texture::CreateFromSurface(*m_renderer, missing));
				return m_missingTexture;
			}
		}

	}

	const std::shared_ptr<Spritesheet>& ResourceManager::GetSpritesheet(const std::string& spritesheetPath)
	{
		auto it = m_spritesheets.find(spritesheetPath);
		if (it != m_spritesheets.end())
			return it->second;

		try
		{
			std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>(Spritesheet::LoadFromFile(spritesheetPath));
			it = m_spritesheets.emplace(spritesheetPath, std::move(spritesheet)).first;
			return it->second;
		}
		catch (const std::exception& e)
		{
			fmt::print(fg(fmt::color::red), "failed to load {0}: {1}", spritesheetPath, e.what());

			if (!m_missingSpritesheet)
				m_missingSpritesheet = std::make_shared<Spritesheet>();

			m_spritesheets.emplace(spritesheetPath, m_missingSpritesheet);
			return m_missingSpritesheet;
		}
	}

	const std::shared_ptr<Font>& ResourceManager::GetFont(const std::string& fontPath, uint8_t size)
	{
		std::string id = fontPath + ";" + std::to_string(size);
		std::unordered_map<std::string, std::shared_ptr<Font>>::iterator font = m_fonts.find(id);
		if (font != m_fonts.end())
			return font->second;

		std::shared_ptr<Font> newfont = std::make_shared<Font>(Font::OpenFont(*m_renderer, fontPath, size));
		m_fonts.emplace(id, newfont);
		return newfont;
	}

}
