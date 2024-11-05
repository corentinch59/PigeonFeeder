#include <PigeonFeeder/HealthComponent.hpp>
#include <SuperCoco/Maths.hpp>

namespace PigeonFeeder
{
	HealthComponent::HealthComponent(float initialHealth) :
		m_maxHealth(initialHealth),
		m_currentHealth(initialHealth)
	{
	}

	float HealthComponent::GetHealth() const
	{
		return m_currentHealth;
	}

	void HealthComponent::ModifyHealth(float value)
	{
		m_currentHealth = Sce::Maths::Clamp<float>(m_currentHealth + value, 0, m_maxHealth);
		if (m_currentHealth <= 0 && m_onDeath)
			m_onDeath();
	}

	void HealthComponent::SetOnDeathCallback(std::function<void()> callback)
	{
		m_onDeath = std::move(callback);
	}
}

