#include "../Include/BaseModel.h"
#include "../Include/Scene.h"

using namespace glm;

BaseModel::BaseModel(){
}

BaseModel::BaseModel(char* modelFile,char* textFile){
	objName = modelFile;
	textName = textFile;
	Init();
}

BaseModel::~BaseModel(){

}

void BaseModel::Init(){
	program = new ShaderProgram();
	program->CreateProgram();
	Shader* vs = new Shader();
	Shader* fs = new Shader();
	vs->LoadShader("vertex.vs.glsl", GL_VERTEX_SHADER);
	fs->LoadShader("fragment.fs.glsl", GL_FRAGMENT_SHADER);
	program->AttachShader(vs->GetID());
	program->AttachShader(fs->GetID());
	program->LinkProgram();

	//init model matrix
	m_shape.model = mat4();
	translateMatrix = mat4();
	rotateMatrix = mat4();
	scaleMatrix = mat4();

	//Cache uniform variable id
	uniforms.proj_matrix = glGetUniformLocation(program->GetID(), "um4p");
	uniforms.mv_matrix = glGetUniformLocation(program->GetID(), "um4mv");

	program->UseProgram();
	///////////////////////////	

	//Load model to shader program
	LoadModel();
}

void BaseModel::LoadModel(){
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(shapes, materials, err, objName);
	if (err.size()>0)
	{
		printf("Load Models Fail! Please check the solution path");
		return;
	}

	printf("Load Models Success ! Shapes size %d Maerial size %d\n", shapes.size(), materials.size());

	/*
	*Bind model data
	*/
	for (int i = 0; i < shapes.size(); i++)
	{
		glGenVertexArrays(1, &m_shape.vao);
		glBindVertexArray(m_shape.vao);

		glGenBuffers(3, &m_shape.vbo);
		glGenBuffers(1, &m_shape.p_normal);
		glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float) + shapes[i].mesh.normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)(shapes[i].mesh.positions.size() * sizeof(float)));
		
		glBindBuffer(GL_ARRAY_BUFFER, m_shape.p_normal);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), shapes[i].mesh.normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_shape.vboTex);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.texcoords.size() * sizeof(float), shapes[i].mesh.texcoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shape.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
		m_shape.materialId = shapes[i].mesh.material_ids[0];
		m_shape.indexCount = shapes[i].mesh.indices.size();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	/*
	*Texture setting
	*/

	//Load texture data from file
	TextureData tdata = Common::Load_png(textName);

	//Generate empty texture
	glGenTextures(1, &m_shape.m_texture);
	glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);

	//Do texture setting
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void BaseModel::Update(float dt){

}
void BaseModel::Render(){
	//Update shaders' input variable
	///////////////////////////	
	glBindVertexArray(m_shape.vao);
	program->UseProgram();

	m_shape.model = translateMatrix * rotateMatrix * scaleMatrix;
	glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(Scene::GetCamera()->GetViewMatrix() * Scene::GetCamera()->GetModelMatrix() * m_shape.model));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(Scene::GetCamera()->GetProjectionMatrix()));

	glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
	///////////////////////////	
}

void BaseModel::Translate(glm::vec3 vec){
	translateMatrix = translate(translateMatrix, vec);
}

void BaseModel::Rotate(glm::vec3 axis,float angle){
	rotateMatrix = rotate(rotateMatrix, angle, axis);
}

void BaseModel::Scale(glm::vec3 vec){
	scaleMatrix = scale(scaleMatrix, vec);
}