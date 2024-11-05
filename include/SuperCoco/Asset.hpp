#ifndef SUPERCOCO_ASSET_HPP
#define SUPERCOCO_ASSET_HPP

#include <SuperCoco/Export.hpp>
#include <string>

#pragma once
namespace Sce
{
	class SUPER_COCO_API Asset
	{
		public :
			Asset(const Asset&) = delete;
			Asset(Asset&& asset) noexcept;
			~Asset() = default;

			const std::string& GetFilepath() const;

			Asset& operator=(const Asset&) = delete;
			Asset& operator=(Asset&& asset) noexcept;

		protected :
			Asset() = default;
			explicit Asset(std::string filepath);

			std::string m_filepath;

	};
}


#endif