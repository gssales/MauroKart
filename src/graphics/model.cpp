#include "graphics/model.h"

ObjModel::ObjModel(const char* filename, const char* basepath, bool triangulate)
{
    printf("Carregando modelo \"%s\"... ", filename);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

    if (!err.empty())
        fprintf(stderr, "\n%s\n", err.c_str());

    if (!ret)
        throw std::runtime_error("Erro ao carregar modelo.");

    this->ComputeNormals();

    printf("OK.\n");
}


void ObjModel::ComputeNormals() {
    if ( !this->attrib.normals.empty() )
        return;

    size_t num_vertices = this->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

    for (size_t shape = 0; shape < this->shapes.size(); ++shape)
    {
        size_t num_triangles = this->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(this->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4  vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = this->shapes[shape].mesh.indices[3*triangle + vertex];
                const float vx = this->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = this->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = this->attrib.vertices[3*idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
            }

            const glm::vec4  a = vertices[0];
            const glm::vec4  b = vertices[1];
            const glm::vec4  c = vertices[2];

            const glm::vec4  n = crossproduct(b-a,c-a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = this->shapes[shape].mesh.indices[3*triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                this->shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    this->attrib.normals.resize( 3*num_vertices );

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        this->attrib.normals[3*i + 0] = n.x;
        this->attrib.normals[3*i + 1] = n.y;
        this->attrib.normals[3*i + 2] = n.z;
    }
}
