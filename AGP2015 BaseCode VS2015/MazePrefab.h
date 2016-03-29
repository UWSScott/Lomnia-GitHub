#ifndef MAZEPREFAB_H
#define MAZEPREFAB_H
#include "Prefab.h"

class MazePrefab : public Prefab
{
public:
	MazePrefab() {};
	MazePrefab(GLuint s_shaderProgram, GLuint s_meshObject, GLuint s_meshIndexCount, GLuint s_texture, glm::vec3 s_scale, glm::vec3 s_position);
	glm::vec3 testPosition;
	virtual void draw(glm::mat4 object, GLuint s_shaderProgram, int pass);
};
#endif