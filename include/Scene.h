#pragma once

/*
	Base class for every scene in the game. 
	A scene can be visualized as everything that needs to be done in a game loop (Event handling, updates, rendering). 
	Multiple scenes can be active at the same time, being updated from top to bottom and rendered from bottom to top.
	This behaviour can be used to create things like Pause Menus, Win/Lose animations and also can help separating the GUI into modular panels.
*/

#include <memory>

#include "Context.h"
#include "ResourceHolder.h"

class SceneManager;
class GameObject;


class Scene
{
	friend class SceneManager;

	SceneManager* _manager;

protected:
	Context* _context;
	TextureHolder _textures;
	FontHolder _fonts;
	SoundHolder _sounds;
	

public:
	Scene(Context* context);

	virtual ~Scene();

	/*
		A scene can control the behaviour of the SceneManager stack by returning true or false from these 4 functions.
		Scenes are updated from top to bottom. Returning false from any scene will result in the loop being terminated.
		Same thing happens with the render function, but it is looped from bottom to top.
		Use cases: Level scene returns true in it's render function so that you can still see the game faded out while in the pause menu. 
				   Pause menu can return false in the Update function so that the game remains still. (It doesn't currently since the pause menu implements 
				a slow motion effect)
	*/
	virtual bool HandleEvent(const sf::Event& ev);
	virtual bool Update(float dt);
	virtual bool FixedUpdate(float dt);
	virtual bool Render();

	virtual void Enable() { } //Called when pushing
	virtual void Disable() { } //Called when popping

	void RequestPush(std::shared_ptr<Scene> scene);
	void RequestPop();
	void RequestClear();

	//Is the scene on top of the stack? 
	bool OnTop() const;

	Context* GetContext() const { return _context; }
	TextureHolder& GetTextures() { return _textures; }
	FontHolder& GetFonts() { return _fonts; }
};