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
	bool change_on = false;
	int triangle_index = 0;
	float rgb_val[3] = { 1.0f,1.0f,1.0f };//for color change

	std::vector<BaseModel*> models;
	static ViewManager* camera;
};

#endif  // Scene_H
