#include "../Include/Scene.h"

ViewManager* Scene::camera = new ViewManager();

Scene::Scene(){
	camera = new ViewManager();

	BaseModel* bottle = new BaseModel("Potion_bottle.obj", "bottle_mana.png");
	BaseModel* box = new BaseModel("box_realistic.obj", "box_texture_color.png");

	bottle->Scale(glm::vec3(0.01, 0.01, 0.01));
	bottle->Rotate(glm::vec3(1, 0, 0), 3.1415926 / 2 * 3);
	bottle->Translate(glm::vec3(0.0, 0.5, 0.0));

	models.push_back(bottle);
	models.push_back(box);
}

void Scene::MouseEvent(int button, int state, int x, int y){
	camera->mouseEvents(button, state, x, y);
}

void Scene::KeyBoardEvent(int key){

}

void Scene::KeyBoardEvent(unsigned char key){
	//camera->keyEvents(key);

	/*
	switch (key)
	{
	case 'z':
	case 'Z':
		models[1]->Rotate(glm::vec3(0,1,0),-0.1f);
		break;
	case 'x':
	case 'X':
		models[1]->Rotate(glm::vec3(0, 1, 0), 0.1f);
		break;
	case 'c':
	case 'C':
		models[1]->Translate(glm::vec3(-0.1, 0, 0));
		break;
	case 'v':
	case 'V':
		models[1]->Translate(glm::vec3(0.1, 0, 0));
		break;
	default:
		break;
	}
	*/
	if (change_on) {
		switch (key)
		{
		case 'a': {
			if (triangle_index == 0) {
				triangle_index = 2;
			}
			else {
				triangle_index--;
			}
		}break;
		case 'd': {
			if (triangle_index == 2) {
				triangle_index = 0;
			}
			else {
				triangle_index++;
			}
		}break;
		case 'w': {
			if (rgb_val[triangle_index] > 1.0f) {
				rgb_val[triangle_index] = 1.0f;
			}
			else {
				rgb_val[triangle_index] += 0.01f;
			}

		}break;
		case 's': {
			if (rgb_val[triangle_index] < 0.0f) {
				rgb_val[triangle_index] = 0.0f;
			}
			else {
				rgb_val[triangle_index] -= 0.01f;
			}
		}break;

		default:
			break;
		}
	}
}

void Scene::MenuEvent(int item){
	/*
	if (item == 1){
		models[1]->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	}
	else if (item == 2){
		models[1]->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	}*/
	if (item == 4) {
		change_on = !change_on;
	}
}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glColor3b(240,240,240);
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component

		glVertex2f(x + cx, y + cy);//output vertex

	}
	glEnd();
}


void Scene::Render(){
	//for (int i = 0; i < models.size(); i++){
	//	models[i]->Render();
	//}
	glUseProgram(0);
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0); // red
	glVertex2f(-0.7, 0.4);
	glColor3f(0, 1, 0); // green
	glVertex2f(0.7, 0.4);
	glColor3f(0, 0, 1); // blue
	glVertex2f(0, -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(rgb_val[0], 0, 0); // red
	glVertex2f(-0.8, -0.8);
	glColor3f(0, rgb_val[1], 0); // green
	glVertex2f(0.8, -0.8);
	glColor3f(0, 0, rgb_val[2]); // blue
	glVertex2f(0, 0.9);
	glEnd();
	if (change_on) {
		switch (triangle_index)
		{
		case 0:DrawCircle(-0.8, -0.8, 0.1, 100);break;
		case 1:DrawCircle(0.8, -0.8, 0.1, 100);break;
		case 2:DrawCircle(0, 0.9, 0.1, 100);break;
		default:
			break;
		}
	}
	
}



void Scene::Update(float dt){
	/*
	for (int i = 0; i < models.size(); i++){
		models[i]->Update(dt);
	}
	*/
}

std::vector<BaseModel*> Scene::GetModels(){
	return models;
}