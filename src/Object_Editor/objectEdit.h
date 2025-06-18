#pragma once
// Minimal Half-Edge Mesh with Cube Builder
#include <gl/GL.h>
#include <unordered_map>
#include "../Headers/Config.h"

#include "../Headers/GlobalVars.h"

struct HEVertex;
struct HEEdge;
struct HEFace;

struct HEVertex {
    glm::vec3 position;
    HEEdge* edge = nullptr;
};

struct HEFace {
    HEEdge* edge = nullptr;
};

struct HEEdge {
    HEVertex* vertex = nullptr; // vertex at start of edge
    HEEdge* twin = nullptr;
    HEEdge* next = nullptr;
    HEFace* face = nullptr;
};

class HalfEdgeMesh {
public:
    std::vector<HEVertex*> vertices;
    std::vector<HEEdge*> edges;
    std::vector<HEFace*> faces;

    inline ~HalfEdgeMesh();
    inline void buildCube();

    inline void extrudeFace(int faceIndex, float distance);
};

HalfEdgeMesh::~HalfEdgeMesh() {
    for (auto v : vertices) delete v;
    for (auto e : edges) delete e;
    for (auto f : faces) delete f;
}

namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& k) const {
            return std::hash<int>()(k.first) ^ (std::hash<int>()(k.second) << 1);
        }
    };
}

//glm::vec3 computeFaceNormal(HEFace* face) {
//    HEEdge* e = face->edge;
//    glm::vec3 p0 = e->vertex->position;
//    glm::vec3 p1 = e->next->vertex->position;
//    glm::vec3 p2 = e->next->next->vertex->position;
//
//    return glm::normalize(glm::cross(p1 - p0, p2 - p0));
//}

inline void HalfEdgeMesh::buildCube() {
    // Clear existing data
    for (auto v : vertices) delete v;
    for (auto e : edges) delete e;
    for (auto f : faces) delete f;
    vertices.clear();
    edges.clear();
    faces.clear();

    // Define cube vertex positions
    glm::vec3 cubeVerts[8] = {
        { -1, -1, -1 }, // 0
        {  1, -1, -1 }, // 1
        {  1,  1, -1 }, // 2
        { -1,  1, -1 }, // 3
        { -1, -1,  1 }, // 4
        {  1, -1,  1 }, // 5
        {  1,  1,  1 }, // 6
        { -1,  1,  1 }  // 7
    };

    for (int i = 0; i < 8; ++i) {
        HEVertex* v = new HEVertex();
        v->position = cubeVerts[i];
        vertices.push_back(v);
    }

    // Each face has 4 vertices in counter-clockwise order
    int faceIndices[6][4] = {
        {0, 1, 2, 3}, // back
        {1, 5, 6, 2}, // right
        {5, 4, 7, 6}, // front
        {4, 0, 3, 7}, // left
        {3, 2, 6, 7}, // top
        {0, 4, 5, 1}  // bottom
    };

     std::unordered_map<std::pair<int, int>, HEEdge*> edgeMap;

    // For each face, create 4 edges and a face
    for (int f = 0; f < 6; ++f) {
        HEFace* face = new HEFace();
        HEEdge* e[4];

        for (int i = 0; i < 4; ++i) e[i] = new HEEdge();

        for (int i = 0; i < 4; ++i) {
            int curr = faceIndices[f][i];
            int next = faceIndices[f][(i + 1) % 4];

            e[i]->vertex = vertices[curr];
            e[i]->next = e[(i + 1) % 4];
            e[i]->face = face;

            edgeMap[{curr, next}] = e[i];
        }

        face->edge = e[0];

        for (int i = 0; i < 4; ++i) {
            edges.push_back(e[i]);
        }
        faces.push_back(face);
    }

    // Link twin edges
    for (const auto& entry : edgeMap) {
        int from = entry.first.first;
        int to = entry.first.second;

        auto it = edgeMap.find({ to, from });
        if (it != edgeMap.end()) {
            entry.second->twin = it->second;
        }
    }
        
}


// === Renderable Wrapper for Editor Integration ===
class LoadHalfEdgeMesh : public BaseModel {
public:
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint highlightVAO, highlightVBO;
    std::vector<glm::vec3> renderVerts;
    std::vector<unsigned int> renderIndices;
   
    //int selectedFaceIndex = -1;  Moved to GlobalVars.h
    HalfEdgeMesh mesh;

    LoadHalfEdgeMesh(int idx, const std::string& name, int HalfEdgeIdx) {
        index = idx;
        objectName = name;
        objectIndex = HalfEdgeIdx;
        objectTypeID = HALF_EDGE_MESH;

        mesh.buildCube();

        std::unordered_map<HEVertex*, unsigned int> vertMap;
        unsigned int i = 0;
        for (HEVertex* v : mesh.vertices) {
            vertMap[v] = i++;
            renderVerts.push_back(v->position);
        }
        for (HEFace* face : mesh.faces) {
            HEEdge* e = face->edge;
            do {
                renderIndices.push_back(vertMap[e->vertex]);
                e = e->next;
            } while (e != face->edge);
        }

        // Upload to OpenGL
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, renderVerts.size() * sizeof(glm::vec3), renderVerts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderIndices.size() * sizeof(unsigned int), renderIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void DrawSelectedFace() {
        if (selectedFaceIndex < 0 || selectedFaceIndex >= mesh.faces.size())
            return;

        HEFace* face = mesh.faces[selectedFaceIndex];
        std::vector<glm::vec3> faceVerts;

        HEEdge* e = face->edge;
        do {
            faceVerts.push_back(e->vertex->position);
            e = e->next;
        } while (e != face->edge);

        // Upload faceVerts to GPU
        if (highlightVAO == 0) {
            glGenVertexArrays(1, &highlightVAO);
            glGenBuffers(1, &highlightVBO);
        }

        glBindVertexArray(highlightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, highlightVBO);
        glBufferData(GL_ARRAY_BUFFER, faceVerts.size() * sizeof(glm::vec3), faceVerts.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindVertexArray(highlightVAO);

        glDrawArrays(GL_TRIANGLE_FAN, 0, faceVerts.size());

        glBindVertexArray(0);
    }
    

    inline ~LoadHalfEdgeMesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

     void DrawHalfEdgeMesh() {
        glBindVertexArray(vao);
        glDrawElements(GL_LINE_LOOP, renderIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
     }

         
};

inline glm::vec3 computeFaceNormal(HEFace* face) {
    HEEdge* e = face->edge;
    glm::vec3 p0 = e->vertex->position;
    glm::vec3 p1 = e->next->vertex->position;
    glm::vec3 p2 = e->next->next->vertex->position;

    return glm::normalize(glm::cross(p1 - p0, p2 - p0));
}

inline void HalfEdgeMesh::extrudeFace(int faceIndex, float distance) {
    //if (faceIndex < 0 || faceIndex >= mesh.faces.size()) return;
    if (faceIndex < 0 || faceIndex >= faces.size()) return;

    //HEFace* face = mesh.faces[faceIndex];
    HEFace* face = faces[faceIndex];

    std::vector<HEVertex*> originalVerts;

    // Step 1: Collect original vertices (looping once)
    HEEdge* start = face->edge;
    HEEdge* e = start;
    do {
        originalVerts.push_back(e->vertex);
        e = e->next;
    } while (e != start);

    // Step 2: Compute face normal
    glm::vec3 normal = computeFaceNormal(face);

    // Step 3: Create extruded vertices
    std::vector<HEVertex*> newVerts;
    for (HEVertex* v : originalVerts) {
        HEVertex* newV = new HEVertex();
        newV->position = v->position + normal * distance;
       // mesh.vertices.push_back(newV);
        vertices.push_back(newV);
        newVerts.push_back(newV);
    }

    // Next step: Create top face and side faces...
    // (We'll do this in the next update)
}

class MeshEdit :public BaseModel {
public:

    void RenderMesh(LoadHalfEdgeMesh* target) {
        if (ShowHalfEdgeEditor) { // if this is true open the editor window
            ImGui::GetStyle().WindowRounding = 12.0f;
            ImGui::Begin("Mesh Editor", &ShowHalfEdgeEditor);
            ImGui::InputText("Mesh Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
            ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " Mesh Editor");
            ImGui::SeparatorText("Mesh Editor");

            for (int i = 0; i < target->mesh.faces.size(); ++i) {
                std::string label = "Face " + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), selectedFaceIndex == i)) {
                    selectedFaceIndex = i;
                }
            }

            if (selectedFaceIndex != -1) {
                ImGui::Separator();
                ImGui::Text("Selected Face: %d", selectedFaceIndex);
            }

            if (ImGui::Button("Extrude")) {

                target->mesh.extrudeFace(selectedFaceIndex, 0.2f);
            }

            if (ImGui::Button("Close Editor")) {
                ShowHalfEdgeEditor = false;
            }

            ImGui::End();
        }
    }
};

