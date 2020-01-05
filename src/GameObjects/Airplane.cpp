#include "GameObjects/Airplane.h"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Animations.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Scenes/Level.h"

#include "GameObjects/TextObject.h"
#include "GameObjects/ParticleSystemObject.h"
#include "GameObjects/Pickup.h"
#include "GameObjects/Animation.h"

#include "Utility/Math.h"
#include "Utility/Sol.h"
#include "Utility/String.h"
#include "Utility/Animations.h"
#include "Random.h"


namespace
{
	GenericFadeAnimation<SpriteObject> _spriteFadeIn(
		1.f, 0.f, 
		&SpriteObject::SetColor,
		&SpriteObject::GetColor);
	GenericFadeAnimation<SpriteObject> _spriteFadeOut(
		0.f, 1.f,
		&SpriteObject::SetColor,
		&SpriteObject::GetColor);
	GenericFadeAnimation<CircleCooldown> _cooldownFadeIn(
		1.f, 0.f,
		&CircleCooldown::SetColor,
		&CircleCooldown::GetColor);
	GenericFadeAnimation<CircleCooldown> _cooldownFadeOut(
		0.f, 1.f,
		&CircleCooldown::SetColor,
		&CircleCooldown::GetColor);
	GenericFadeAnimation<TextObject> _textFadeIn(
		1.f, 0.f,
		&TextObject::SetColor,
		&TextObject::GetColor);
	GenericFadeAnimation<TextObject> _textFadeOut(
		0.f, 1.f,
		&TextObject::SetColor,
		&TextObject::GetColor);
}


Airplane::Airplane(AirplaneData* data) :
	_level(nullptr),
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
	_elapsedTime(0),
	_ammoDisplay(nullptr),
	_cooldownDisplay(nullptr),
	_currentPatternAngle(0.f),
	_currentPatternDistance(0.f),
	_weaponDisplay(nullptr),
	_texture(nullptr),
	_shader(nullptr),
	_healthText(nullptr),
	_weaponVisibilityElapsedTime(0.f),
	_fadingIn(true)
{
	for (int i = 0; i < static_cast<int>(_data->ammo.size()); i++)
	{
		_ammo.push_back(_data->ammo[i]);
	}
}


void Airplane::Start(Scene* scene)
{
	_level = static_cast<Level*>(scene);
	
	setScale(_data->scale, _data->scale);
	SetTexture(*_data->texture);
	SetTextureRect(_data->idleRect);

	_healthText = new TextObject();
	_healthText->SetFont(*_data->healthFont);
	_healthText->SetCharSize(_data->healthTextCharSize);
	if (_data->healthTextColor)
	{
		_healthText->SetColor(_data->healthTextColor.value());
	}

	auto bounds = GetBoundingRect();
	_healthText->setPosition(0, bounds.height / 2 + _data->healthTextCharSize / 2);
	if (!_playerControlled)
	{
		_healthText->setRotation(180);
		_cooldown = 100000.f; //Start firing instantly
	}
	else
	{
		_weaponDisplay = new SpriteObject();
		_ammoDisplay = new TextObject();
		_cooldownDisplay = new CircleCooldown();
		if (_data->ammoTextColor)
		{
			_ammoDisplay->SetColor(_data->ammoTextColor.value());
		}
		UpdateWeaponDisplay();
		
		_level->AddUiElement(_weaponDisplay);
		_level->AddUiElement(_cooldownDisplay);
		_level->AddUiElement(_ammoDisplay);
		
		_spriteFadeIn.SetLowestAlpha(125);
		_spriteFadeOut.SetLowestAlpha(125);
		_textFadeIn.SetLowestAlpha(125);
		_textFadeOut.SetLowestAlpha(125);
		_cooldownFadeIn.SetHighestAlpha(_cooldownDisplay->GetColor().a);
		_cooldownFadeOut.SetHighestAlpha(_cooldownDisplay->GetColor().a);
		_cooldownFadeIn.SetLowestAlpha(_cooldownDisplay->GetColor().a / 2);
		_cooldownFadeOut.SetLowestAlpha(_cooldownDisplay->GetColor().a / 2);
	}

	std::unique_ptr<TextObject> textPtr(_healthText);
	AddChild(std::move(textPtr));
	UpdateHealthDisplay();
	
	SetShader(_level->GetFlashShader());

	_luaObject = _data->start(this);

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
		if (!_ammo.empty()) //Check if the airplane has a gun 
		{
			Fire();
		}
		if (!_data->directions.empty())
		{
			if (_distanceMoved > _data->directions[_currentPatternIndex].distance)
			{
				_distanceMoved = 0;
				_currentPatternIndex++;
				if (_currentPatternIndex >= static_cast<int>(_data->directions.size()))
				{
					_currentPatternIndex = 0;
				}
			}
			float angle = _data->directions[_currentPatternIndex].angle;
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
		}
		else
		{
			move({0.f, _data->speed * dt});
			SetTextureRect(_data->idleRect);
		}
		

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
			sf::Vector2f velocity = Normalize(sf::Vector2f(static_cast<float>(_moveX), static_cast<float>(_moveY))) * _data->speed * dt;
			move(velocity);

		}
		_moveX = 0;
		_moveY = 0;
		_moved = false;
		UpdateCooldownVertices();

		UpdateWeaponVisibility(dt);
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
		int randN;
		Animation* explosion = new Animation();
		explosion->setPosition(GetWorldPosition());
		explosion->SetTexture(*_data->explosionsTexture);
		sf::IntRect firstRect;
		firstRect.width = _data->explosionSize.x;
		firstRect.height = _data->explosionSize.y;
		firstRect.left = 0;
		randN = RandInt(0, _data->numberOfExplosions - 1);
		firstRect.top = firstRect.height * randN;
		explosion->SetFirstRect(firstRect);
		explosion->SetDestroyOnFinish(true);
		explosion->SetLoopable(false);
		explosion->SetNumberOfFrames(_data->framesPerExplosion);
		explosion->SetTimePerFrame(_data->explosionFrameDuration);
		float randomScale = RandReal(_data->explosionMinScale, _data->explosionMaxScale);
		explosion->setScale(randomScale, randomScale);
		RandomizedSoundResult randSound = _data->explosionSounds[RandInt(0, (int)_data->explosionSounds.size() - 1)](_level->GetContext()->player->GetSfxVolume());
		_level->PlaySound(*randSound.buffer, randSound.volume, randSound.pitch);

		float randomRotation = RandReal(0.f, _data->explosionMaxRotation);
		explosion->setRotation(randomRotation);
		_level->AddExplosion(explosion);

		//Spawn pickup if RNG is in your favor
		randN = RandInt(1, 100);
		for (int i = 0; i < static_cast<int>(_data->drops.size()); i++)
		{
			if (randN <= _data->drops[i].dropRate)
			{
				Pickup* pickup = new Pickup(_data->drops[i].pickup);
				pickup->setPosition(GetWorldPosition());
				_level->AddPickup(pickup);
				break;
			}
		}

		if (!_playerControlled)
		{
			_level->AddDownedEnemy(_data->name);
		}

		if (_data->onDestroy.has_value())
		{
			Protect(_data->onDestroy.value()(this));
		}
	}
	else
	{
		_dmgAnimationActive = false;
		_startDmgAnimation = true;
		_elapsedTime = 0;
	}

	if (_data->onDamage)
	{
		Protect(_data->onDamage.value()(_luaObject, this));
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
		return static_cast<unsigned int>(Type::PlayerAirplane);
	}
	return static_cast<unsigned int>(Type::EnemyAirplane);
}


void Airplane::NextWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex++;
	if (_currentWeaponIndex >= static_cast<int>(_ammo.size()))
	{
		_currentWeaponIndex = 0;
	}
	if (_playerControlled)
	{
		UpdateWeaponDisplay();
		RandomizedSoundResult sound = _data->switchSound(_level->GetContext()->player->GetSfxVolume());
		_level->PlaySound(*sound.buffer, sound.volume, sound.pitch);
		UpdateCooldownDisplay();
	}
}


void Airplane::PreviousWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex--;
	if (_currentWeaponIndex < 0)
	{
		_currentWeaponIndex = static_cast<int>(_ammo.size()) - 1;
	}
	if (_playerControlled)
	{
		UpdateWeaponDisplay();
		RandomizedSoundResult sound = _data->switchSound(_level->GetContext()->player->GetSfxVolume());
		_level->PlaySound(*sound.buffer, sound.volume, sound.pitch);
		UpdateCooldownDisplay();
	}
}


void Airplane::Fire()
{
	if (_blockShooting)
	{
		return;
	}
	if ((_ammo[_currentWeaponIndex] > 0 || _ammo[_currentWeaponIndex] < 0) && _cooldown > _data->weapons[_currentWeaponIndex]->fireRate)
	{
		_cooldown = 0;
		_ammo[_currentWeaponIndex]--;

		ProjectileData* projectileData = _data->weapons[_currentWeaponIndex];
		Projectile* proj = new Projectile(projectileData);
		proj->setPosition(GetWorldPosition());

		if (_playerControlled)
		{
			proj->SetPlayerControlled(true);
			_level->AddPlayerProjectile(proj);
			proj->move(0, -GetBoundingRect().height / 2);
			UpdateCooldownDisplay();
			UpdateAmmoDisplay();
		}
		else
		{
			proj->setRotation(180);
			_level->AddEnemyProjectile(proj);
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
	_weaponDisplay->SetTexture(*_data->weapons[_currentWeaponIndex]->iconTexture);
	_weaponDisplay->SetTextureRect(_data->weapons[_currentWeaponIndex]->iconRect);
	sf::Vector2u ws = _level->GetContext()->window->getSize();
	float displayX = 0 - ((int)ws.x / 2) + (_data->weapons[_currentWeaponIndex]->iconRect.width / 2) * _data->weapons[_currentWeaponIndex]->iconScale;
	float displayY = (int)ws.y / 2 - (_data->weapons[_currentWeaponIndex]->iconRect.height / 2) * _data->weapons[_currentWeaponIndex]->iconScale;
	_weaponDisplay->setPosition(displayX, displayY);
	_weaponDisplay->setScale(_data->weapons[_currentWeaponIndex]->iconScale, _data->weapons[_currentWeaponIndex]->iconScale);
	
	UpdateAmmoDisplay();

	UpdateCooldownVertices();
}


void Airplane::UpdateAmmoDisplay()
{
	_ammoDisplay->SetFont(*_data->weapons[_currentWeaponIndex]->ammoFont);
	_ammoDisplay->SetCharSize(_data->weapons[_currentWeaponIndex]->ammoTextSize);

	if (_data->ammo[_currentWeaponIndex] < 0) //infinite ammo
	{
		std::string text("8");
		_ammoDisplay->SetString(text);
		int charSize = _data->weapons[_currentWeaponIndex]->ammoTextSize;

		float ammoX = _weaponDisplay->getPosition().x;
		float ammoY = _weaponDisplay->getPosition().y - _weaponDisplay->GetBoundingRect().height / 2.f - charSize / 2.f;

		_ammoDisplay->setRotation(90);
		_ammoDisplay->setPosition(ammoX, ammoY);

	}
	else
	{
		std::string text = std::to_string(_ammo[_currentWeaponIndex]);
		_ammoDisplay->SetString(text);
		int charSize = _data->weapons[_currentWeaponIndex]->ammoTextSize;

		float ammoX = _weaponDisplay->getPosition().x;
		float ammoY = _weaponDisplay->getPosition().y - _weaponDisplay->GetBoundingRect().height / 2.f - charSize / 2.f;

		_ammoDisplay->setRotation(0);
		_ammoDisplay->setPosition(ammoX, ammoY);
	}
}


void Airplane::UpdateCooldownVertices()
{
	sf::FloatRect rect = _weaponDisplay->GetBoundingRect();
	_cooldownDisplay->SetVertices(sf::Vector2f(rect.left, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top + rect.height),
		sf::Vector2f(rect.left, rect.top + rect.height),
		sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2));

}


void Airplane::SetShader(sf::Shader* shader)
{
	_shader = shader;
	_shader->setUniform("u_texture", *_texture);
	_shader->setUniform("u_flashColor", sf::Glsl::Vec4(1.f, 1.f, 1.f, 0.7f));
	_shader = shader;
}


void Airplane::UpdateCooldownDisplay()
{
	if (_data->weapons[_currentWeaponIndex]->fireRate > 0.1)
	{
		_cooldownDisplay->BeginAnimation(_data->weapons[_currentWeaponIndex]->fireRate);
	}
	else
	{
		_cooldownDisplay->StopAnimation();
	}
}


void Airplane::AddHealth(int n)
{
	_hitpoints += n;
	if (_data->onHeal)
	{
		Protect(_data->onHeal.value()(_luaObject, this));
	}
	UpdateHealthDisplay();
}


void Airplane::AddAmmo(const std::string& projectile, int n)
{
	for (int i = 0; i < static_cast<int>(_data->weapons.size()); i++)
	{
		if (projectile == _data->weapons[i]->name)
		{
			_ammo[i] += n;
			UpdateAmmoDisplay();
			break;
		}
	}
}


void Airplane::SetBlockShooting(bool block)
{
	_blockShooting = block;
}


void Airplane::UpdateWeaponVisibility(float dt)
{
	if (GetBoundingRect().intersects(_weaponDisplay->GetBoundingRect()))
	{
		if (_fadingIn)
		{
			_fadingIn = false;
			_weaponVisibilityElapsedTime = 0.f;
		}
		else
		{
			_weaponVisibilityElapsedTime += dt;
			float progress = _weaponVisibilityElapsedTime / 0.5f;
			if (progress < 1.f)
			{
				_spriteFadeOut(*_weaponDisplay, progress);
				_textFadeOut(*_ammoDisplay, progress);
				_cooldownFadeOut(*_cooldownDisplay, progress);

			}
			else
			{
				_spriteFadeOut(*_weaponDisplay, 1.f);
				_textFadeOut(*_ammoDisplay, 1.f);
				_cooldownFadeOut(*_cooldownDisplay, 1.f);
			}
		}
	}
	else
	{
		if (!_fadingIn)
		{
			_fadingIn = true;
			_weaponVisibilityElapsedTime = 0.f;
		}
		else
		{
			_weaponVisibilityElapsedTime += dt;
			float progress = _weaponVisibilityElapsedTime / 0.5f;
			if (progress < 1.f)
			{
				_spriteFadeIn(*_weaponDisplay, progress);
				_textFadeIn(*_ammoDisplay, progress);
				_cooldownFadeIn(*_cooldownDisplay, progress);
			}
			else
			{
				_spriteFadeIn(*_weaponDisplay, 1.f);
				_textFadeIn(*_ammoDisplay, 1.f);
				_cooldownFadeIn(*_cooldownDisplay, 1.f);
			}
		}
	}
}