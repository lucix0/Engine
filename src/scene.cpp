#include "scene.hpp"
#include <glm/ext/matrix_float4x4.hpp>

Scene::Scene() {
	struct MeshComponent {};
	struct TransformComponent {
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) { }

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () { return Transform; }
	};

	entt::entity entity = m_Registry.create();
	m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

	auto view = m_Registry.view<TransformComponent>();
	for (auto entity : view) {
		TransformComponent& transform = view.get<TransformComponent>(entity);
	}

	auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
}

Scene::~Scene() {

}