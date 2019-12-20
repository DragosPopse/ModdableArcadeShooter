#include "Scenes/ErrorScene.h"


ErrorScene::ErrorScene(Context* context, const std::string& title, const std::string& message, std::shared_ptr<Scene> nextScene) :
	Scene(context),
	_gui(*_context->window),
	_nextScene(std::move(nextScene))
{
	_gui.loadWidgetsFromFile("assets/ui/ErrorPanel.txt");
	_fonts.Load("Default", "assets/fonts/pcsenior.ttf");
	_gui.setFont(_fonts["Default"]);
	_panel = _gui.get<tgui::Panel>("Panel");
	_message = _gui.get<tgui::TextBox>("Message");
	_title = _gui.get<tgui::Label>("Title");
	_confirmButton = _gui.get<tgui::Button>("ConfirmButton");

	_title->setText(title);
	_message->setText(message);

	_confirmButton->connect("pressed", 
		[this]()
		{
			if (_nextScene != nullptr)
			{
				RequestClear();
				RequestPush(std::move(_nextScene));
			}
			else
			{
				RequestPop();
			}
		});
}


bool ErrorScene::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);

	switch (ev.type)
	{
	case sf::Event::Closed:
		RequestClear();
		break;
	}

	return false;
}


bool ErrorScene::Render()
{
	_gui.draw();
	return false;
}