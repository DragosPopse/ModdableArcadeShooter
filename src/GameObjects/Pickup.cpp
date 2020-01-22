#include "GameObjects/Pickup.h"

#include "GameObjects/Animation.h"
#include "GameObjects/Airplane.h"

#include "Scenes/Level.h"

#include "Utility/Sol.h"


Pickup::Pickup(PickupData* data) :
	_data(data),
	_playingDestroyAnimation(false),
	_animation(nullptr),
	_level(nullptr)
{ 
}


void Pickup::Start(Scene* scene)
{
	_level = static_cast<Level*>(scene);
	_animation = new Animation();
	std::unique_ptr<Animation> animation(_animation);

	_animation->SetTexture(*_data->texture);
	_animation->SetFirstRect(_data->firstRect);
	_animation->SetNumberOfFrames(_data->frames);
	_animation->SetTimePerFrame(_data->frameDuration);
	_animation->SetDestroyOnFinish(false);
	_animation->SetLoopable(true);
	_animation->setScale(_data->scale, _data->scale);

	AddChild(std::move(animation));
	
	GameObject::Start(scene);
}


void Pickup::Update(float dt)
{
	if (_animation->IsFinished())
	{
		MarkForDestroy();
	}

	GameObject::Update(dt);
}


void Pickup::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	//target.draw(_sprite, states);

	GameObject::Draw(target, states);

}

void Pickup::OnPickup(Airplane& player)
{
	RandomizedSoundResult sound = _data->onPickupSound(_level->GetContext()->player->GetSfxVolume());
	_level->PlaySound(*sound.buffer, sound.volume, sound.pitch);
	Protect(_data->onPickup(player));
	
	_animation->SetTexture(*_data->destroyTexture);
	_animation->SetFirstRect(_data->destroyFirstRect);
	_animation->SetNumberOfFrames(_data->destroyFrames);
	_animation->SetTimePerFrame(_data->destroyFrameDuration);
	_animation->SetDestroyOnFinish(false);
	_animation->SetLoopable(false);
	_animation->SetIndex(0);
	_animation->setScale(_data->destroyScale, _data->destroyScale);
	_playingDestroyAnimation = true;

}