#include "Model.h"

struct V {
	unsigned int index_position, index_uv, index_normal;
	bool operator == (const V& v) const {
		return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
	}
	bool operator < (const V& v) const {
		return (index_position < v.index_position) || (index_position == v.index_position && index_uv < v.index_uv) || (index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
	}
};

// Constructor
Model::Model(const char* objPath, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, unsigned int txtr)
{
	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::map<V, unsigned int> temp_vertices;

	unsigned int num_vertices = 0;
	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	std::ifstream file;
	file.open(objPath);
	if (file.fail()) {
		std::cerr << "File failed to open" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser. Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					this->vertices.push_back(vertex);
					this->indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					unsigned int index = temp_vertices[vertices[i]];
					this->indices.push_back(index);
				}
			}
		} else {
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();
	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;

	glGenVertexArrays(1, &this->vaoID);
	glBindVertexArray(this->vaoID);
	glGenBuffers(1, &this->vboID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glGenBuffers(1, &this->eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	this->setPosition(pos);
	this->setRotation(rot);
	this->setScale(scl);
	this->setTexture(txtr);
}

// Draw function
void Model::draw()
{
	glBindVertexArray(this->vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
	glDrawElements(GL_TRIANGLES, GLsizei(this->indices.size()), GL_UNSIGNED_INT, 0);
}

// Get position
glm::vec3 Model::getPosition()
{
	return this->position;
}

// Get rotation
glm::vec3 Model::getRotation()
{
	return this->rotation;
}

// Get scale
glm::vec3 Model::getScale()
{
	return this->scale;
}

// Get texture
unsigned int Model::getTexture()
{
	return this->texture;
}

// Set position
void Model::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

// Set rotation
void Model::setRotation(glm::vec3 rot)
{
	this->rotation = rot;
}

// Set scale
void Model::setScale(glm::vec3 scl)
{
	this->scale = scl;
}

// Set texture
void Model::setTexture(unsigned int txtr) 
{
	this->texture = txtr;
}