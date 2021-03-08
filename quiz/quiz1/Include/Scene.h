#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "ViewManager.h"
#include "BaseModel.h"

class Scene
{
public:
	Scene();
	void MouseEvent(int button,int state,int x,int y);
	void KeyBoardEvent(int key);
	void KeyBoardEvent(unsigned char key);
	void MenuEvent(int item);

	void Render();
	void Update(float dt);

	static ViewManager* GetCamera(){ return camera; }
	std::vector<BaseModel*> GetModels();

private:

	std::vector<BaseModel*> models;
	static ViewManager* camera;
};

#endif  // Scene_H
