#include "InstancedMeshFactory.h"


InstancedMeshFactory::InstancedMeshFactory()
{
	this->vertices = new std::vector<GLfloat>();
	this->texCoords = new std::vector<GLfloat>();
	this->instanceOffset = new std::vector<GLfloat>();
	this->instanceRotation = new std::vector<GLfloat>();
	this->instanceCount = 0;
}


InstancedMeshFactory::~InstancedMeshFactory()
{
	delete vertices;
	delete texCoords;
	delete instanceOffset;
	delete instanceRotation;
}

void InstancedMeshFactory::init(const char *texture, const char *material)
{
	this->texture = texture;
	this->material = material;
}

int InstancedMeshFactory::addInstance(glm::vec3 offset, glm::vec3 rotation)
{
	std::size_t instanceId = this->instanceCount;

	this->instanceOffset->push_back(offset.x);
	this->instanceOffset->push_back(offset.y);
	this->instanceOffset->push_back(offset.z);

	this->instanceRotation->push_back(rotation.x);
	this->instanceRotation->push_back(rotation.y);
	this->instanceRotation->push_back(rotation.z);

	this->instanceCount++;

	return instanceId;
}

std::size_t InstancedMeshFactory::vectorSize()
{
	return MESH_VECTOR_SIZE;
}

std::size_t InstancedMeshFactory::texDimens()
{
	return TEXCOORD_DIMENS;
}

std::size_t InstancedMeshFactory::vertexDataByteSize()
{
	return verticesByteSize() + texCoordByteSize();
}

std::size_t InstancedMeshFactory::instanceDataByteSize()
{
	return instanceOffsetByteSize() + instanceRotationByteSize();
}

std::size_t InstancedMeshFactory::verticesByteSize()
{
	return this->vertices->size() * sizeof(GLfloat);
}

std::size_t InstancedMeshFactory::texCoordByteSize()
{
	return this->texCoords->size() * sizeof(GLfloat);
}

std::size_t InstancedMeshFactory::instanceOffsetByteSize()
{
	return this->instanceOffset->size() * sizeof(GLfloat);
}

std::size_t InstancedMeshFactory::instanceRotationByteSize()
{
	return this->instanceRotation->size() * sizeof(GLfloat);
}

std::size_t InstancedMeshFactory::numInstances()
{
	return this->instanceCount;
}