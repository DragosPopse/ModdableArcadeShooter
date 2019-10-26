#include "GameObjects/Airplane.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "GameObjects/TextObject.h"
#include "GameObjects/ParticleSystemObject.h"
#include <Thor/Math.hpp>
#include <Thor/Animations.hpp>


Airplane::Airplane(AirplaneData* data) :
	_currentScene(nullptr),
	_data(data),
	_hitpoints(data->hitpoints),
	_currentWeaponIndex(0),
	_playerControlled(false),
	_cooldown(0.f),
	_moveX(0),
	_moveY(0),
	_moved(false),
	_distanceMoved(0),
	_currentPatternIndex(0),
	_startDmgAnimation(false),
	_dmgAnimationActive(false),
	_elapsedTime(0)
{
	for (int i = 0; i < _data->ammo.size(); i++)
	{
		_ammo.push_back(_data->ammo[i]);
	}
}


void Airplane::Start(Scene* scene)
{
	_currentScene = static_cast<Level*>(scene);
	auto& textures = _currentScene->GetTextures();
	
	setScale(_data->scale, _data->scale);
	SetTexture(textures[_data->texture]);
	SetTextureRect(_data->idleRect);

	_healthText = new TextObject();
	_healthText->SetFont(_currentScene->GetFonts()[_data->healthFont]);
	_healthText->SetCharSize(_data->healthTextCharSize);

	auto bounds = GetBoundingRect();
	_healthText->setPosition(0, bounds.height / 2 + _data->healthTextCharSize / 2);
	if (!_playerControlled)
	{
		_healthText->setRotation(180);

	}
	else
	{
		_weaponDisplay = new SpriteObject();
		_cooldownDisplay = new CircleCooldown();
		UpdateWeaponDisplay();
		
		_currentScene->AddUiElement(_weaponDisplay);
		_currentScene->AddUiElement(_cooldownDisplay);
		
	}

	std::unique_ptr<TextObject> textPtr(_healthText);
	AddChild(std::move(textPtr));
	UpdateHealthDisplay();
	
	SetShader(_currentScene->GetFlashShader());

	GameObject::Start(scene);
}


void Airplane::Update(float dt)
{
	_cooldown += dt;
	if (_startDmgAnimation)
	{
		_dmgAnimationActive = true;
		_startDmgAnimation = false;
	}
	if (_dmgAnimationActive)
	{
		_elapsedTime += dt;
		if (_elapsedTime > 0.06)
		{
			_dmgAnimationActive = false;
		}
	}
	GameObject::Update(dt);
}


void Airplane::FixedUpdate(float dt)
{
	if (!_playerControlled)
	{
		if (_distanceMoved > _data->directions[_currentWeaponIndex].distance)
		{
			_distanceMoved = 0;
			_currentWeaponIndex++;
			if (_currentWeaponIndex >= _data->directions.size())
			{
				_currentWeaponIndex = 0;
			}
		}
		float angle = _data->directions[_currentWeaponIndex].angle;
		float radian = ToRadian(angle + 90);
		float x = _data->speed * cos(radian);
		float y = _data->speed * sin(radian);
		if (x > 0.01)
		{
			SetTextureRect(_data->leftRect);
		}
		else if (x < -0.01)
		{
			SetTextureRect(_data->rightRect);
		}
		else
		{
			SetTextureRect(_data->idleRect);
		}
		sf::Vector2f direction(x, y);
		direction = Normalize(direction);
		sf::Vector2f velocity = direction * _data->speed * dt;
		_distanceMoved += Magnitude(velocity);
		move(velocity);
		//std::cout << Magnitude(velocity) << ' ' << _data->directions[_currentWeaponIndex].distance << '\n';
	}
	else
	{
		if (_moveX == 0)
		{
			SetTextureRect(_data->idleRect);
		}
		else if (_moveX > 0)
		{
			SetTextureRect(_data->rightRect);
		}
		else
		{
			SetTextureRect(_data->leftRect);
		}

		if (_moved)
		{
			sf::Vector2f velocity = Normalize(sf::Vector2f(_moveX, _moveY)) * _data->speed * dt;
			move(velocity);

		}
		_moveX = 0;
		_moveY = 0;
		_moved = false;
		UpdateCooldownVertices();
	}
	GameObject::FixedUpdate(dt);
}


void Airplane::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	const sf::Shader* last = states.shader;
	if (_dmgAnimationActive)
	{
		states.shader = _shader;
	}
	target.draw(_sprite, states);
	states.shader = last;

	GameObject::Draw(target, states);
}

 
void Airplane::Damage(int hp)
{
	_hitpoints -= hp;
	if (_hitpoints <= 0)
	{
		MarkForDestroy();
		Animation* explosion = new Animation();
		explosion->setPosition(GetWorldPosition());
		explosion->SetTexture(_currentScene->GetTextures()[_data->explosionsTexture]);
		sf::IntRect firstRect;
		firstRect.width = _data->explosionSize.x;
		firstRect.height = _data->explosionSize.y;
		firstRect.left = 0;
		int randN = rand() % _data->numberOfExplosions;
		firstRect.top = firstRect.height * randN;
		explosion->SetFirstRect(firstRect);
		explosion->SetDestroyOnFinish(true);
		explosion->SetLoopable(false);
		explosion->SetNumberOfFrames(_data->framesPerExplosion);
		explosion->SetTimePerFrame(_data->explosionFrameDuration);
		float randomScale = _data->generator(_data->rng);
		explosion->setScale(randomScale, randomScale);
		_currentScene->AddExplosion(explosion);
	}
	else
	{
		_dmgAnimationActive = false;
		_startDmgAnimation = true;
		_elapsedTime = 0;
	}
	UpdateHealthDisplay();
}


void Airplane::Repair(int hp)
{
	_hitpoints += hp;
	UpdateHealthDisplay();
}


void Airplane::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
	_texture = &texture;
	CenterOrigin(_sprite);
}


void Airplane::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
	CenterOrigin(_sprite);
}


void Airplane::SetPlayerControlled(bool b)
{
	_playerControlled = b;
}


unsigned int Airplane::GetCategory() const
{
	if (_playerControlled)
	{
		return Type::PlayerAirplane;
	}
	return Type::EnemyAirplane;
}


void Airplane::NextWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex++;
	if (_currentWeaponIndex >= _ammo.size())
	{
		_currentWeaponIndex = 0;
	}
	if (_playerControlled)
	{
		UpdateWeaponDisplay();
		UpdateCooldownDisplay();
	}
}


void Airplane::PreviousWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex--;
	if (_currentWeaponIndex < 0)
	{
		_currentWeaponIndex = _ammo.size() - 1;
	}
	if (_playerControlled)
	{
		UpdateWeaponDisplay();
		UpdateCooldownDisplay();
	}
}


void Airplane::Fire()
{
	if (_ammo[_currentWeaponIndex] > 0 && _cooldown > _data->weapons[_currentWeaponIndex]->fireRate)
	{
		_cooldown = 0;
		_ammo[_currentWeaponIndex]--;

		ProjectileData* projectileData = _data->weapons[_currentWeaponIndex];
		Projectile* proj = new Projectile(projectileData);
		proj->setPosition(GetWorldPosition());

		if (_playerControlled)
		{
			proj->SetPlayerControlled(true);
			_currentScene->AddPlayerProjectile(proj);
			proj->move(0, -GetBoundingRect().height / 2);
			UpdateCooldownDisplay();
		}
		else
		{
			proj->setRotation(180);
			_currentScene->AddEnemyProjectile(proj);
			proj->move(0, GetBoundingRect().height / 2);
		}
	}
}


void Airplane::MoveX(int x)
{
	_moveX = x;
	_moved = true;
}


void Airplane::MoveY(int y)
{
	_moveY = y;
	_moved = true;
}


sf::FloatRect Airplane::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(_sprite.getGlobalBounds());
}


void Airplane::UpdateHealthDisplay()
{
	std::string str = BuildString(_hitpoints);
	_healthText->SetString(str);
}


void Airplane::UpdateWeaponDisplay()
{
	_weaponDisplay->SetTexture(_currentScene->GetTextures()[_data->weapons[_currentWeaponIndex]->iconTexture]);
	_weaponDisplay->SetTextureRect(_data->weapons[_currentWeaponIndex]->iconRect);
	sf::Vector2u ws = _currentScene->GetContext()->window->getSize();
	float displayX = 0 - ((int)ws.x / 2);
	float displayY = (int)ws.y / 2 - _data->weapons[_currentWeaponIndex]->iconRect.height * _data->weapons[_currentWeaponIndex]->iconScale;
	_weaponDisplay->setPosition(displayX, displayY);
	_weaponDisplay->setScale(_data->weapons[_currentWeaponIndex]->iconScale, _data->weapons[_currentWeaponIndex]->iconScale);
	
	UpdateCooldownVertices();
}


void Airplane::UpdateCooldownVertices()
{
	sf::FloatRect rect = _weaponDisplay->GetBoundingRect();
	//std::cout << "RECT: " << rect.left << ' ' << rect.top << ' ' << rect.width << ' ' << rect.height << '\n';
	_cooldownDisplay->SetVertices(sf::Vector2f(rect.left, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top + rect.height),
		sf::Vector2f(rect.left, rect.top + rect.height),
		sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2));

}


void Airplane::SetShader(sf::Shader* shader)
{
	_shader = shader;
	_shader->setUniform("texture", *_texture);
	_shader->setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 0.7));
	_shader = shader;
}


void Airplane::UpdateCooldownDisplay()
{
	_cooldownDisplay->BeginAnimation(_data->weapons[_currentWeaponIndex]->fireRate);
}