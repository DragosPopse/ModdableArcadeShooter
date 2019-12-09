#pragma once

/*
	Interface for scenes that have a tgui::Gui and need to make it visible from other scenes
	Example for this usage is in the menu scenes where the settings panels are broke down into separate scenes to allow reuse.
*/

class GuiMenu
{
public:
	virtual void SetVisible(bool visible) = 0;
};