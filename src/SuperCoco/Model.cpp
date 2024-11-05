#include <SuperCoco/Model.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/JsonSerializer.hpp>
#include <SuperCoco/BinarySerializer.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Renderer.hpp>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/std.h>
#include <imgui.h>
#include <lz4.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

namespace Sce
{
	constexpr unsigned int FileVersion = 1;

	Model::Model() :
	Asset("")
	{
	}

	Model::Model(std::shared_ptr<Texture> sharedTexture, std::vector<ModelVertex> vertices, std::vector<int> indices, std::string filepath) :
	Asset(std::move(filepath)),
	m_texture(std::move(sharedTexture)),
	m_vertices(std::move(vertices)),
	m_indices(std::move(indices))
	{
		Vector2f maxs(std::numeric_limits<float>::min()); // -Infinity
		Vector2f mins(std::numeric_limits<float>::max()); // +Infinity

		m_sdlVertices.resize(m_vertices.size());
		for (std::size_t i = 0; i < m_vertices.size(); ++i)
		{
			const ModelVertex& modelVertex = m_vertices[i];
			SDL_Vertex& sdlVertex = m_sdlVertices[i];

			// Calcul des coordonnées minimales et maximales
			maxs.x = std::max(maxs.x, modelVertex.pos.x);
			maxs.y = std::max(maxs.y, modelVertex.pos.y);
			mins.x = std::min(mins.x, modelVertex.pos.x);
			mins.y = std::min(mins.x, modelVertex.pos.y);

			// Conversion de nos structures vers les structures de la SDL
			sdlVertex.tex_coord = SDL_FPoint{ modelVertex.uv.x, modelVertex.uv.y };

			std::uint8_t r, g, b, a;
			modelVertex.color.ToRGBA8(r, g, b, a);

			sdlVertex.color = SDL_Color{ r, g, b, a };
		}

		// Maintenant que nous avons les coordonnées minimales et maximales nous pouvons facilement définir le rectangle
		m_bounds.x = mins.x;
		m_bounds.y = mins.y;
		m_bounds.w = maxs.x - mins.x; // longueur = max X moins min X
		m_bounds.h = maxs.y - mins.y;
	}

	void Model::Render(Renderer& renderer, const Matrixf& transformMatrix) const
	{
		assert(m_vertices.size() == m_sdlVertices.size());

		std::vector<SDL_Vertex> vertices = m_sdlVertices;

		for (std::size_t i = 0; i < m_vertices.size(); ++i)
		{
			const ModelVertex& modelVertex = m_vertices[i];
			SDL_Vertex& sdlVertex = m_sdlVertices[i];

			// tex_coord et color sont déjà gérés par le constructeur
			Vector2f transformedPos = (transformMatrix * Matrixf::MakeFromPosition(modelVertex.pos)).GetVector2() + Vector2f{ 1080 / 2, 769 / 2 };
			sdlVertex.position = SDL_FPoint{ transformedPos.x, transformedPos.y };
			sdlVertex.color.r = modelVertex.color.r;
			sdlVertex.color.g = modelVertex.color.g;
			sdlVertex.color.b = modelVertex.color.b;
			sdlVertex.color.a = modelVertex.color.a;
		}

		renderer.RenderGeometry(*m_texture, m_sdlVertices.data(), static_cast<int>(m_sdlVertices.size()), m_indices.data(), static_cast<int>(m_indices.size()));
	}

	bool Model::SaveToFileRegular(const std::string& filepath) const
	{
		// Ouverture d'un fichier en écriture
		std::ofstream outputFile(filepath);
		if (!outputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return false;
		}

		nlohmann::ordered_json doc = SaveToJSon();
		outputFile << doc.dump(4);

		// Pas besoin de fermer le fichier, le destructeur de std::ofstream s'en occupe (c'est bon les destructeurs, mangez-en !)
		return true;
	}

	bool Model::SaveToFileCompressed(const std::string& filepath) const
	{
		// Ouverture d'un fichier en écriture (et en mode binaire car nous ne stockons pas du texte)
		std::ofstream outputFile(filepath, std::ios::binary);
		if (!outputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return false;
		}

		nlohmann::ordered_json doc = SaveToJSon();

		std::string jsonStr = doc.dump();

		// Nous devons allouer un tableau d'une taille suffisante pour stocker la version compressée
		// La fonction LZ4_compressBound nous donne la taille compressée maximale possible avec une taille donnée
		int maxSize = LZ4_compressBound(static_cast<int>(jsonStr.size()));

		// Lors de la décompression en revanche, nous n'avons pas d'équivalent pour calculer la taille maximale décompressée
		// nous allons donc stocker la taille au début du fichier, en binaire.
		// note: il est important d'utiliser un type à taille fixe pour que ce soit lisible sur plusieurs machines
		// il faudrait également prendre l'endianness en compte mais cela dépasse le cadre de cet exercice
		std::uint32_t decompressedSize = static_cast<std::uint32_t>(jsonStr.size());
		outputFile.write(reinterpret_cast<const char*>(&decompressedSize), sizeof(std::uint32_t));

		// Nous pouvons ensuite allouer un tableau d'octets (char), std::vector<char> ferait l'affaire ici mais un unique_ptr suffit amplement
		std::unique_ptr<char[]> compressedStr = std::make_unique<char[]>(maxSize);

		// On peut ensuite compresser avec la fonction LZ4_compress_default
		// celle-ci prend :
		// 1) une suite d'octets à compresser en entrée, 
		// 2) un pointeur vers l'endroit où stocker le résultat
		// 3) ainsi que la taille des données à compresser
		// 4) la taille du buffer final, par sécurité
		int finalSize = LZ4_compress_default(jsonStr.data(), compressedStr.get(), static_cast<int>(jsonStr.size()), static_cast<int>(maxSize));
		if (finalSize == 0)
		{
			// La compression a échoué (ça ne devrait pas arriver)
			fmt::print(stderr, fg(fmt::color::red), "failed to compress model file\n");
			return false;
		}

		// En cas de succès, LZ4_compress_default renvoie la taille finale des données compressées, nous pouvons alors l'écrire directement
		// en mode binaire on utilisera alors plutôt .write
		outputFile.write(compressedStr.get(), finalSize);

		return true;
	}

	bool Model::SaveToFileBinary(const std::string& filepath) const
	{
		std::ofstream file(filepath, std::ios::out | std::ios::binary);
		if (!file.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return false;
		}

		std::vector<std::uint8_t> buffer;

		SerializeBinary<const std::string&>(buffer, m_texture->GetFilepath());

		SerializeBinary<std::size_t>(buffer, m_indices.size());
		for (int id : m_indices)
			SerializeBinary<int>(buffer, id);

		SerializeBinary<std::size_t>(buffer, m_vertices.size());
		for (const auto& vertice : m_vertices)
		{
			SerializeBinary<float>(buffer, vertice.pos.x);
			SerializeBinary<float>(buffer, vertice.pos.y);
			SerializeBinary<float>(buffer, vertice.uv.x);
			SerializeBinary<float>(buffer, vertice.uv.y);
			SerializeBinary<Uint8>(buffer, vertice.color.r);
			SerializeBinary<Uint8>(buffer, vertice.color.g);
			SerializeBinary<Uint8>(buffer, vertice.color.b);
			SerializeBinary<Uint8>(buffer, vertice.color.a);
		}

		
		file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		file.close();
		return true;
	}

	Model Model::LoadFromFileRegular(const std::string& filepath)
	{
		// Ouverture d'un fichier en lecture
		std::ifstream inputFile(filepath);
		if (!inputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return {}; //< on retourne un Model construit par défaut (on pourrait également lancer une exception)
		}

		return LoadFromJson(nlohmann::json::parse(inputFile), filepath);
	}

	Model Model::LoadFromFileCompressed(const std::string& filepath)
	{
		// Ouverture d'un fichier en lecture (en binaire)
		std::ifstream inputFile(filepath, std::ios::binary);
		if (!inputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return {}; //< on retourne un Model construit par défaut (on pourrait également lancer une exception)
		}

		// On lit tout le contenu dans un vector
		std::vector<char> content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

		// Nous devons allouer un tableau d'une taille suffisante pour stocker la version décompressée : problème, nous n'avons pas cette information
		// Nous l'avons donc stockée dans un std::uint32_t au début du fichier
		std::uint32_t decompressedSize;
		std::memcpy(&decompressedSize, &content[0], sizeof(std::uint32_t));

		// Petite sécurité pour éviter les données malveillantes : assurons-nous que la taille n'a pas une valeur délirante
		if (decompressedSize > 10'000'000) //< La taille dépasse 10MB? Évitons
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to load model file {}: decompressed size is too big ({}), is the file corrupt?\n", filepath, decompressedSize);
			return {};
		}

		// Nous pouvons ensuite allouer un tableau d'octets (char), std::vector<char> ferait l'affaire ici mais un unique_ptr suffit amplement
		std::unique_ptr<char[]> decompressedStr = std::make_unique<char[]>(decompressedSize);
		if (LZ4_decompress_safe(&content[sizeof(std::uint32_t)], decompressedStr.get(), static_cast<int>(content.size() - sizeof(std::uint32_t)), decompressedSize) <= 0)
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to load model file {}: corrupt file\n", filepath);
			return {};
		}

		return LoadFromJson(nlohmann::json::parse(decompressedStr.get()), filepath);
	}

	bool Model::SaveToFile(const std::string& filepath) const
	{
		if (filepath.ends_with(".model"))
			return SaveToFileRegular(filepath);
		else if (filepath.ends_with(".cmodel"))
			return SaveToFileCompressed(filepath);
		else if (filepath.ends_with(".bmodel"))
			return SaveToFileBinary(filepath);
		else
		{
			fmt::print(stderr, fg(fmt::color::red), "unknown extension {}\n", filepath.substr(filepath.find_last_of(".")));
			return false;
		}
	}

	nlohmann::json Model::SaveToJSon() const
	{
		nlohmann::ordered_json doc;

		// L'ajout d'un champ "version" permet de faire évoluer le format dans le futur
		doc["version"] = FileVersion;

		// Faisons référence à la texture via son chemin, si elle en a un
		if (m_texture)
		{
			const std::string& texturePath = m_texture->GetFilepath();
			if (!texturePath.empty())
				doc["texture"] = texturePath;
		}

		// On enregistre les indices si nous en avons
		if (!m_indices.empty())
			doc["indices"] = m_indices;

		nlohmann::ordered_json& vertices = doc["vertices"];
		for (const ModelVertex& modelVertex : m_vertices)
		{
			nlohmann::ordered_json& vertex = vertices.emplace_back();

			nlohmann::ordered_json& pos = vertex["pos"];
			pos["x"] = modelVertex.pos.x;
			pos["y"] = modelVertex.pos.y;

			nlohmann::ordered_json& uv = vertex["uv"];
			uv["u"] = modelVertex.uv.x;
			uv["v"] = modelVertex.uv.y;

			nlohmann::ordered_json& color = vertex["color"];
			color["r"] = modelVertex.color.r;
			color["g"] = modelVertex.color.g;
			color["b"] = modelVertex.color.b;

			// Le champ "a" (alpha) est optionnel et sa valeur sera de 1 si on ne l'enregistre pas
			// Cela permet d'économiser un peu de place
			if (modelVertex.color.a != 1.f)
				color["a"] = modelVertex.color.a;
		}

		return doc;
	}

	SDL_FRect Model::GetBounds() const
	{
		return m_bounds;
	}

	int Model::GetLayer() const
	{
		return -10;
	}

	const std::vector<ModelVertex>& Model::GetVertices() const
	{
		return m_vertices;
	}


	bool Model::IsValid() const
	{
		return !m_vertices.empty();
	}

	void Model::PopulateInspector(WorldEditor& worldEditor)
	{
	}

	nlohmann::json Model::Serialize() const
	{
		nlohmann::json renderableDoc;
		renderableDoc["Type"] = "Model";

		// On sauvegarde le chemin vers le modèle si possible, sinon le modèle entier
		if (const std::string& filepath = GetFilepath(); !filepath.empty())
			renderableDoc["Path"] = filepath;
		else
			renderableDoc["Model"] = SaveToJSon();

		return renderableDoc;
	}

	Model Model::LoadFromFileBinary(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("couldn't open file " + filepath);

		std::vector<std::uint8_t> buffer;

		file.seekg(0, std::ios::end);
		std::size_t len = file.tellg();
		file.seekg(0, std::ios::beg);

		buffer.resize(len);
		file.read(reinterpret_cast<char*>(buffer.data()), len);

		std::size_t offset = 0;

		std::string texpath = UnserializeBinary<std::string>(buffer, offset);
		std::shared_ptr<Texture> texture = ResourceManager::Instance().GetTexture(texpath);

		std::size_t indiceNum = UnserializeBinary<std::size_t>(buffer, offset);
		std::vector<int> indices;
		for (std::size_t i = 0; i < indiceNum; ++i)
		{
			int value = UnserializeBinary<int>(buffer, offset);
			indices.push_back(value);
		}

		std::size_t verticesNum = UnserializeBinary<std::size_t>(buffer, offset);
		std::vector<SDL_Vertex> vertices;
		for (std::size_t i = 0; i < verticesNum; ++i)
		{
			SDL_Vertex v;
			v.position.x = UnserializeBinary<float>(buffer, offset);
			v.position.y = UnserializeBinary<float>(buffer, offset);
			v.tex_coord.x = UnserializeBinary<float>(buffer, offset);
			v.tex_coord.y = UnserializeBinary<float>(buffer, offset);
			v.color.r = UnserializeBinary<char>(buffer, offset);
			v.color.g = UnserializeBinary<char>(buffer, offset);
			v.color.b = UnserializeBinary<char>(buffer, offset);
			v.color.a = UnserializeBinary<char>(buffer, offset);
			vertices.push_back(v);
		}

		std::vector<ModelVertex> modelVertex;
		modelVertex.resize(verticesNum);
		for (std::size_t i = 0; i < verticesNum; ++i)
		{
			modelVertex[i] = ModelVertex{
				vertices[i].position,
				vertices[i].tex_coord,
				{
				static_cast<float>(vertices[i].color.r) * 255,
				static_cast<float>(vertices[i].color.g) * 255,
				static_cast<float>(vertices[i].color.b) * 255,
				static_cast<float>(vertices[i].color.a) * 255
				}
			};
		}

		return Model{ std::move(texture), std::move(modelVertex), std::move(indices), texpath};
	}

	Model Model::LoadFromFile(const std::string& filepath)
	{
		if (filepath.ends_with(".model"))
			return LoadFromFileRegular(filepath);
		else if (filepath.ends_with(".cmodel"))
			return LoadFromFileCompressed(filepath);
		else if (filepath.ends_with(".bmodel"))
			return LoadFromFileBinary(filepath);
		else
		{
			fmt::print(stderr, fg(fmt::color::red), "unknown extension {}\n", filepath.substr(filepath.find_last_of(".")));
			return {};
		}
	}

	Model Model::LoadFromJson(const nlohmann::json& doc,std::string filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("couldn't open file " + filepath);

		std::shared_ptr<Texture> texture = ResourceManager::Instance().GetTexture(doc.at("texture"));

		std::vector<SDL_Vertex> vertices = doc.at("vertices");
		std::vector<ModelVertex> modelVertices;
		modelVertices.resize(vertices.size());

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			modelVertices[i].pos = vertices[i].position;
			modelVertices[i].uv = vertices[i].tex_coord;
			modelVertices[i].color = { 
				static_cast<float>(vertices[i].color.r),
				static_cast<float>(vertices[i].color.g),
				static_cast<float>(vertices[i].color.b),
				static_cast<float>(vertices[i].color.a)
			};
		}

		return Model{ std::move(texture), std::move(modelVertices), doc.at("indices"), filepath};
	}

	std::shared_ptr<Model> Model::Unserialize(const nlohmann::json& doc)
	{
		std::string filepath = doc.value("Path", "");
		if (!filepath.empty())
			return ResourceManager::Instance().GetModel(filepath);
		else
			return std::make_shared<Model>(LoadFromJson(doc["Model"]));
	}
}

