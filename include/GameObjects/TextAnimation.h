#pragma once

#include "GameObject.h"

#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "GameObjects/TextObject.h"

class TextAnimation :
	public GameObject
{
	sf::Text _text;
	float _elapsedTime;
	std::string _finalStr;
	std::stringstream _currentStr;
	float _timeBeforeDestroy;
	float _timePerLetter;
	bool _destroyOnFinish;
	bool _finished;
	int _currentLetter;
	int _strSize;
	std::string _font;

public:
	TextAnimation(const std::string& finalStr);

	void Start(Scene* scene) override;
	void Update(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetTimePerLetter(float time);
	void SetDestroyOnFinish(float timeBeforeDestroy);
	void SetFont(const std::string& font);
	bool IsFinished() const;
	void SetCharSize(unsigned int n);

	void SetFinalString(const std::string& finalStr);
	void Skip();

private:
	void SetString(const std::string& str);
};