//
//  Object.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 2/18/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum OBJ_TYPE
{
    OBJ_POINTS,
    OBJ_TRIANGLES
};

enum RENDER_TYPE
{
    RENDER_POINTS,
    RENDER_LINES,
    RENDER_TRIANGLES
};

class Object
{
public:
    struct Vertex
    {
        // Position
        glm::vec3 Position;
        // Normal
        glm::vec3 Normal;
        // TexCoords
        glm::vec2 TexCoords;
        // Tangent
        glm::vec3 Tangent;
        // BiTangent
        glm::vec3 BiTangent;
    };
    
    struct Vertex_Index
    {
        int pos_idx;
        int normal_idx;
        int texcoord_idx;
    };
    
    struct Face_Index
    {
        Vertex_Index vertex[3];
    };
    
    // vao and vbo vectors
    std::vector<Vertex> vao_vertices;
    std::vector<unsigned int> vbo_indices;
    
    // obj original data vector
    std::vector<glm::vec3> ori_positions;
    std::vector<glm::vec2> ori_texcoords;
    std::vector<glm::vec3> ori_normals;
    
    // obj face index vector
    std::vector<Face_Index> indexed_faces;
    
    glm::vec3 obj_center;
    
    glm::vec4 obj_color = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
//    nanogui::Color obj_color = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );
    GLfloat shininess = 32.0f;
    
    std::string m_obj_path;
    std::string obj_name;
    
    GLuint vao, vbo;
    OBJ_TYPE m_obj_type;
    RENDER_TYPE m_render_type;
    
public:
    void load_obj(std::string obj_path)
    {
        int path_str_length = (int)obj_path.size();
        std::string suffix = obj_path.substr(path_str_length - 3, path_str_length);
        
        if( suffix == "obj" )
        {
            this->vao_vertices.clear();
            this->vbo_indices.clear();
            this->indexed_faces.clear();
            
            this->ori_positions.clear();
            this->ori_normals.clear();
            this->ori_texcoords.clear();
            
            std::ifstream ifs;
            // Store original data vector
            try{
                ifs.open(obj_path);
                std::string one_line;
                while( getline( ifs, one_line ) )
                {
                    std::stringstream ss(one_line);
                    std::string type;
                    ss >> type;
                    if( type == "v" )
                    {
                        glm::vec3 vert_pos;
                        ss >> vert_pos[0] >> vert_pos[1] >> vert_pos[2];
                        this->ori_positions.push_back(vert_pos);
                    }
                    else if( type == "vt" )
                    {
                        glm::vec2 tex_coord;
                        ss >> tex_coord[0] >> tex_coord[1];
                        this->ori_texcoords.push_back(tex_coord);
                    }
                    else if( type == "vn" )
                    {
                        glm::vec3 vert_norm;
                        ss >> vert_norm[0] >> vert_norm[1] >> vert_norm[2];
                        this->ori_normals.push_back(vert_norm);
                    }
                    else if( type == "f" )
                    {
//                        Face_Index face_idx;
//                        // Here we only accept face number 3
//                        for(int i = 0; i < 3; i++)
//                        {
//                            std::string s_vertex;
//                            ss >> s_vertex;
//                            int pos_idx = -1;
//                            int tex_idx = -1;
//                            int norm_idx = -1;
//                            sscanf(s_vertex.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
//                            // We have to use index -1 because the obj index starts at 1
//                            // Incorrect input will be set as -1
//                            face_idx.vertex[i].pos_idx = pos_idx > 0 ? pos_idx-1 : -1;
//                            face_idx.vertex[i].texcoord_idx = pos_idx > 0 ? pos_idx-1 : -1;
//                            face_idx.vertex[i].normal_idx = pos_idx > 0 ? pos_idx-1 : -1;
//                        }
//
//                        indexed_faces.push_back(face_idx);
                        
                        std::string s_vertex_0, s_vertex_1, s_vertex_2;
                        ss >> s_vertex_0 >> s_vertex_1 >> s_vertex_2;
                        int pos_idx, tex_idx, norm_idx;
                        std::sscanf(s_vertex_0.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
                        // We have to use index - 1 because the obj index starts at 1
                        Vertex vertex_0;
                        vertex_0.Position = ori_positions[pos_idx - 1];
                        vertex_0.TexCoords = ori_texcoords[tex_idx - 1];
                        vertex_0.TexCoords.y *= -1;
                        vertex_0.Normal = ori_normals[norm_idx - 1];
                        sscanf(s_vertex_1.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);

                        Vertex vertex_1;
                        vertex_1.Position = ori_positions[pos_idx - 1];
                        vertex_1.TexCoords = ori_texcoords[tex_idx - 1];
                        vertex_1.TexCoords.y *= -1;
                        vertex_1.Normal = ori_normals[norm_idx - 1];
                        sscanf(s_vertex_2.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
                        
                        Vertex vertex_2;
                        vertex_2.Position = ori_positions[pos_idx - 1];
                        vertex_2.TexCoords = ori_texcoords[tex_idx - 1];
                        vertex_2.TexCoords.y *= -1;
                        vertex_2.Normal = ori_normals[norm_idx - 1];
                        
                        // Compute Triangle tangent/bi-tangent:
                        glm::vec2 UV1 = vertex_1.TexCoords - vertex_0.TexCoords;
                        glm::vec2 UV2 = vertex_2.TexCoords - vertex_0.TexCoords;
                        glm::vec3 V1 = vertex_1.Position - vertex_0.Position;
                        glm::vec3 V2 = vertex_2.Position - vertex_0.Position;
                        float d = 1.0f / (UV1.x * UV2.y - UV1.y * UV2.x);
                        glm::vec3 tangent;// = (V1 * UV2.y - V2 * UV1.y)*d;
                        tangent.x = d * (UV2.y * V1.x - UV1.y * V2.x);
                        tangent.y = d * (UV2.y * V1.y - UV1.y * V2.y);
                        tangent.z = d * (UV2.y * V1.z - UV1.y * V2.z);
                        tangent = glm::normalize(tangent);
                        
                        glm::vec3 bitangent;// = (V2 * UV1.x - V1 * UV2.x)*d;
                        bitangent.x = d * (-UV2.x * V1.x + UV1.x * V2.x);
                        bitangent.y = d * (-UV2.x * V1.y + UV1.x * V2.y);
                        bitangent.z = d * (-UV2.x * V1.z + UV1.x * V2.z);
                        bitangent = glm::normalize(bitangent);
                        
                        vertex_0.Tangent = tangent;
                        vertex_1.Tangent = tangent;
                        vertex_2.Tangent = tangent;
                        vertex_0.BiTangent = bitangent;
                        vertex_1.BiTangent = bitangent;
                        vertex_2.BiTangent = bitangent;
                        
                        vao_vertices.push_back(vertex_0);
                        vao_vertices.push_back(vertex_1);
                        vao_vertices.push_back(vertex_2);
                    }
                }
            }
            catch(const std::exception&)
            {
                std::cout << "Error:: Obj file cannot be read.\n";
            }
//            // Retrieve data from index and assign to vao and vbo
//            for(int i = 0; i < indexed_faces.size(); i++)
//            {
//                Face_Index cur_idx_face = indexed_faces[i];
//                // If no normal: recalculate for them
//                glm::vec3 v0 = ori_positions[cur_idx_face.vertex[0].pos_idx];
//                glm::vec3 v1 = ori_positions[cur_idx_face.vertex[1].pos_idx];
//                glm::vec3 v2 = ori_positions[cur_idx_face.vertex[2].pos_idx];
//                glm::vec3 new_normal = glm::cross( v1 - v0 , v2 - v0 );
//
//                glm::vec2 uv0 = ori_texcoords[cur_idx_face.vertex[0].texcoord_idx];
//                glm::vec2 uv1 = ori_texcoords[cur_idx_face.vertex[1].texcoord_idx];
//                glm::vec2 uv2 = ori_texcoords[cur_idx_face.vertex[2].texcoord_idx];
//
//                float d = 1.0f / (uv1.x * uv2.y - uv1.y * uv2.x);
//                glm::vec3 tangent;// = (V1 * UV2.y - V2 * UV1.y)*d;
//                tangent.x = d * (uv2.y * v1.x - uv1.y * v2.x);
//                tangent.y = d * (uv2.y * v1.y - uv1.y * v2.y);
//                tangent.z = d * (uv2.y * v1.z - uv1.y * v2.z);
//                tangent = glm::normalize(tangent);
//                glm::vec3 biTangent;// = (V2 * UV1.x - V1 * UV2.x)*d;
//                biTangent.x = d * (-uv2.x * v1.x + uv1.x * v2.x);
//                biTangent.y = d * (-uv2.x * v1.y + uv1.x * v2.y);
//                biTangent.z = d * (-uv2.x * v1.z + uv1.x * v2.z);
//                biTangent = glm::normalize(biTangent);
//
//                for(int j = 0; j < 3; j++)
//                {
//                    Vertex cur_vertex;
//                    Vertex_Index cur_idx_vertex = cur_idx_face.vertex[j];
//                    if( cur_idx_vertex.pos_idx >= 0 )
//                    {
//                        cur_vertex.Position = ori_positions[cur_idx_vertex.pos_idx];
//                    }
//                    if( cur_idx_vertex.normal_idx >= 0 )
//                    {
//                        cur_vertex.Normal = ori_normals[cur_idx_vertex.normal_idx];
//                    }
//                    else
//                    {
//                        cur_vertex.Normal = new_normal;
//                    }
//                    if( cur_idx_vertex.texcoord_idx >= 0 )
//                    {
//                        cur_vertex.TexCoords = ori_texcoords[cur_idx_vertex.texcoord_idx];
//                    }
//                    cur_vertex.Tangent = tangent;
//                    cur_vertex.BiTangent = biTangent;
//                    vao_vertices.push_back(cur_vertex);
//                    vbo_indices.push_back(i * 3 + j);
//                }
//            }
        }
    }
    
    void calculate_center()
    {
        glm::vec3 max_bound(INT_MIN);
        glm::vec3 min_bound(INT_MAX);
        for(auto vertex : this->vao_vertices)
        {
            max_bound[0] = std::max(vertex.Position[0], max_bound[0]);
            max_bound[1] = std::max(vertex.Position[1], max_bound[1]);
            max_bound[2] = std::max(vertex.Position[2], max_bound[2]);
            min_bound[0] = std::min(vertex.Position[0], min_bound[0]);
            min_bound[1] = std::min(vertex.Position[1], min_bound[1]);
            min_bound[2] = std::min(vertex.Position[2], min_bound[2]);
        }
        this->obj_center = (max_bound + min_bound) * 0.5f;
    }
    
    // Initialize object using obj file
    Object(std::string obj_path)
    {
        this->m_obj_path = obj_path;
        load_obj(this->m_obj_path);
        calculate_center();
        m_obj_type = OBJ_TRIANGLES;
        m_render_type = RENDER_TRIANGLES;
    }
    
    // Initialize object using point array, can used to draw
    Object(std::vector<glm::vec3> points)
    {
        this->vao_vertices.clear();
        this->vbo_indices.clear();
        this->indexed_faces.clear();
        
        this->ori_positions.clear();
        this->ori_normals.clear();
        this->ori_texcoords.clear();
        for(unsigned int i = 0; i < points.size(); i++)
        {
            glm::vec3 cur_point = points[i];
            this->ori_positions.push_back(cur_point);
            Vertex cur_vertex;
            cur_vertex.Position = cur_point;
            this->vao_vertices.push_back(cur_vertex);
        }
        calculate_center();
        m_obj_type = OBJ_POINTS;
        m_render_type = RENDER_POINTS;
    }
    
    ~Object() {}
};
