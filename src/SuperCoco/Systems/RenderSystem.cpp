#include <SuperCoco/Systems/RenderSystem.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/CameraComponent.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Matrix.hpp>
#include <tuple>
#include <algorithm>

namespace Sce
{
	RenderSystem::RenderSystem(entt::registry* registry, Renderer* renderer) :
	m_registry(registry),
	m_renderer(renderer)
	{
	}

	void RenderSystem::Render(float)
	{
		Transform camera = Transform();
		auto cameraView = m_registry->view<Transform, CameraComponent>();
		for (entt::entity entity : cameraView)
		{
			camera = cameraView.get<Transform>(entity);
		}

		auto view = m_registry->view<Transform, GraphicsComponent>();
		size_t viewSize = view.size_hint();

		std::vector<std::tuple<Transform*, GraphicsComponent*>> sortedView;
		sortedView.resize(viewSize);

		size_t i = 0;
		for (auto&& [entity, transform, graphics] : view.each())
		{
			sortedView[i++] = std::tuple<Transform*, GraphicsComponent*>(&transform, &graphics);
		}

		std::sort(sortedView.begin(), sortedView.end(), [](std::tuple<Transform*, GraphicsComponent*>& lhs, std::tuple<Transform*, GraphicsComponent*>& rhs)
			{
				GraphicsComponent* left = get<1>(lhs);
				GraphicsComponent* right = get<1>(rhs);

				if (!left || !right)
				{
					return false;
				}
				
				return left->m_renderable->GetLayer() < right->m_renderable->GetLayer();
			});

		for (auto&& [transform, graphic] : sortedView)
		{
			Matrixf transformMatrix =  camera.WorldToLocalMatrix() * transform->LocalToWorldMatrix();
			graphic->m_renderable->Render(*m_renderer, transformMatrix);
		}
	}
}