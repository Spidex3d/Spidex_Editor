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
    glm::vec2 texCoord = glm::vec2(0.0f); // Added this for textures
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
 
    int faceIndices[6][4] = { // this one is better
        {3, 2, 1, 0}, // back
        {2, 6, 5, 1}, // right
        {6, 7, 4, 5}, // front
        {7, 3, 0, 4}, // left
        {7, 6, 2, 3}, // top
        {1, 5, 4, 0}  // bottom
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

inline glm::vec3 computeFaceNormal(HEFace* face) {
    glm::vec3 normal(0.0f);
    glm::vec3 origin = face->edge->vertex->position;

    HEEdge* e = face->edge;
    do {
        glm::vec3 v1 = e->vertex->position - origin;
        glm::vec3 v2 = e->next->vertex->position - origin;
        normal += glm::cross(v1, v2);
        e = e->next;
    } while (e->next != face->edge);

    return glm::normalize(normal);
}


// === Renderable Wrapper for Editor Integration ===
class LoadHalfEdgeMesh : public BaseModel {
public:
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint highlightVAO, highlightVBO;
    std::vector<glm::vec3> renderVerts;
    std::vector<unsigned int> renderIndices;

    GLuint normalVAO = 0, normalVBO = 0;
    std::vector<glm::vec3> normalLines;
   
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

        // texture
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(renderVerts), (void*)offsetof(renderVerts.data(), texCoord)); // uv
        //glEnableVertexAttribArray(1);

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

        glUseProgram(0);
                
    }

    // new bit
    void rebuildRenderBuffers() {
        renderVerts.clear();
        renderIndices.clear();

        std::unordered_map<HEVertex*, unsigned int> vertMap;
        unsigned int i = 0;

        // Collect unique vertices
        for (HEVertex* v : mesh.vertices) {
            vertMap[v] = i++;
            renderVerts.push_back(v->position);
        }

        for (HEFace* face : mesh.faces) {
            HEEdge* e = face->edge;
            std::vector<unsigned int> faceIndices;

            HEEdge* start = e;
            do {
                faceIndices.push_back(vertMap[e->vertex]);
                e = e->next;
            } while (e != start);

            if (faceIndices.size() == 4) {
                // Use safe quad triangulation
                renderIndices.push_back(faceIndices[0]);
                renderIndices.push_back(faceIndices[1]);
                renderIndices.push_back(faceIndices[2]);

                renderIndices.push_back(faceIndices[0]);
                renderIndices.push_back(faceIndices[2]);
                renderIndices.push_back(faceIndices[3]);
            }
            else if (faceIndices.size() >= 3) {
                // Generic fan
                for (size_t j = 1; j + 1 < faceIndices.size(); ++j) {
                    renderIndices.push_back(faceIndices[0]);
                    renderIndices.push_back(faceIndices[j]);
                    renderIndices.push_back(faceIndices[j + 1]);
                }
            }
                        
        }
        
        for (HEFace* face : mesh.faces) {
            HEEdge* e = face->edge;
            int count = 0;
            HEEdge* start = e;
            do {
                count++;
                e = e->next;
            } while (e != start);

            std::cout << "Face has " << count << " vertices" << std::endl;
        }

        // Re-upload to GPU
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, renderVerts.size() * sizeof(glm::vec3), renderVerts.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderIndices.size() * sizeof(unsigned int), renderIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
    
     inline ~LoadHalfEdgeMesh() {
         glDeleteVertexArrays(1, &vao);
         glDeleteBuffers(1, &vbo);
         glDeleteBuffers(1, &ebo);
     }
     // new bit Normal
     void DrawNormalLines() {
         glBindVertexArray(normalVAO);
         glDrawArrays(GL_LINES, 0, normalLines.size());
         glBindVertexArray(0);

         glUseProgram(0);
         
     }
     

     void DrawHalfEdgeMesh() {
        
         glBindVertexArray(vao);
         //glDrawElements(GL_TRIANGLES, renderIndices.size(), GL_UNSIGNED_INT, 0);
          glDrawElements(GL_LINE_LOOP, renderIndices.size(), GL_UNSIGNED_INT, 0);
         //glDrawElements(GL_LINES, renderIndices.size(), GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);

         // new bit Draw normal lines
         glBindVertexArray(normalVAO);
         glDrawArrays(GL_LINES, 0, normalLines.size());
         glBindVertexArray(0);
         
         glUseProgram(0);
         
                          
     }
     // new bit Normals
     void updateFaceNormalLines(float length = 0.2f) {
         normalLines.clear();

         for (HEFace* face : mesh.faces) {
             glm::vec3 center(0.0f);
             int count = 0;
             HEEdge* e = face->edge;
             HEEdge* start = e;
             do {
                 center += e->vertex->position;
                 count++;
                 e = e->next;
             } while (e != start);

             center /= float(count);
             glm::vec3 normal = computeFaceNormal(face);

             normalLines.push_back(center);
             normalLines.push_back(center + normal * length);
         }

         if (normalVAO == 0) {
             glGenVertexArrays(1, &normalVAO);
             glGenBuffers(1, &normalVBO);
         }

         glBindVertexArray(normalVAO);
         glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
         glBufferData(GL_ARRAY_BUFFER, normalLines.size() * sizeof(glm::vec3), normalLines.data(), GL_DYNAMIC_DRAW);

         glEnableVertexAttribArray(0);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

         glBindVertexArray(0);
     }
                   
};

inline void HalfEdgeMesh::extrudeFace(int faceIndex, float distance) {
    if (faceIndex < 0 || faceIndex >= faces.size()) return;

    HEFace* face = faces[faceIndex];
    std::vector<HEVertex*> baseVerts;

    // 1. Gather base vertices (in order)
    HEEdge* start = face->edge;
    HEEdge* e = start;
    do {
        baseVerts.push_back(e->vertex);
        e = e->next;
    } while (e != start);
    int N = static_cast<int>(baseVerts.size());

    // 2. Compute face normal
    glm::vec3 normal = computeFaceNormal(face);

    // 3. Create extruded top vertices
    std::vector<HEVertex*> topVerts;
    for (HEVertex* v : baseVerts) {
        HEVertex* nv = new HEVertex();
        nv->position = v->position + normal * distance;
        vertices.push_back(nv);
        topVerts.push_back(nv);
    }

    // 4. Create top face with reversed winding (so normal matches original face)
    
    HEFace* topFace = new HEFace();
    std::vector<HEEdge*> topEdges(N);

    // Check if winding should be reversed
    bool reverseWinding = false;
    {
        glm::vec3 newNormal;
        glm::vec3 p0 = topVerts[0]->position;
        glm::vec3 p1 = topVerts[1]->position;
        glm::vec3 p2 = topVerts[2]->position;
        newNormal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

        // Dot with original face normal
        float alignment = glm::dot(newNormal, normal);
        reverseWinding = (alignment < 0.0f);  // If facing inward, reverse it
    }

    for (int i = 0; i < N; ++i) {
        int idx = reverseWinding ? (N - i) % N : i;
        topEdges[i] = new HEEdge();
        topEdges[i]->vertex = topVerts[idx];
        topEdges[i]->face = topFace;
    }
    for (int i = 0; i < N; ++i) {
        topEdges[i]->next = topEdges[(i + 1) % N];
    }
    topFace->edge = topEdges[0];
    faces.push_back(topFace);
    edges.insert(edges.end(), topEdges.begin(), topEdges.end());

    
    // 5. Create side faces (maintaining CCW winding for each quad)

    for (int i = 0; i < N; ++i) {
        HEVertex* b0 = baseVerts[i];
        HEVertex* b1 = baseVerts[(i + 1) % N];
        HEVertex* t1 = topVerts[(i + 1) % N];
        HEVertex* t0 = topVerts[i];

        // Default winding
        std::vector<HEVertex*> sideVerts = { b0, t0, t1, b1 };

        // Compute side face normal
        glm::vec3 v0 = b0->position;
        glm::vec3 v1 = t0->position;
        glm::vec3 v2 = t1->position;
        glm::vec3 sideNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        // Expected normal (cross between vertical and horizontal edge)
        glm::vec3 expected = glm::normalize(glm::cross(b1->position - b0->position, t0->position - b0->position));

        if (glm::dot(sideNormal, expected) < 0.0f) {
            // Flip winding
            sideVerts = { b0, b1, t1, t0 };
        }

        // Build side face
        HEFace* sideFace = new HEFace();
        std::vector<HEEdge*> sideEdges(4);
        for (int j = 0; j < 4; ++j) {
            sideEdges[j] = new HEEdge();
            sideEdges[j]->vertex = sideVerts[j];
            sideEdges[j]->face = sideFace;
        }
        for (int j = 0; j < 4; ++j)
            sideEdges[j]->next = sideEdges[(j + 1) % 4];

        sideFace->edge = sideEdges[0];
        faces.push_back(sideFace);
        edges.insert(edges.end(), sideEdges.begin(), sideEdges.end());
    }


    // 6. Delete the original face and its edges
    HEEdge* curr = face->edge;
    HEEdge* startEdge = curr;
    do {
        HEEdge* next = curr->next;
        auto it = std::find(edges.begin(), edges.end(), curr);
        if (it != edges.end()) edges.erase(it);
        delete curr;
        curr = next;
    } while (curr != startEdge);

    delete face;
    faces.erase(faces.begin() + faceIndex);

    // 7. Adjust selection if needed
    if (selectedFaceIndex == faceIndex)
        selectedFaceIndex = -1;
    else if (selectedFaceIndex > faceIndex)
        selectedFaceIndex--;
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

            static float extrusionDistance = 0.2f;

            ImGui::DragFloat("Extrude Distance", &extrusionDistance, 0.01f, 0.0f, 5.0f);

            if (ImGui::Button("Extrude")) {
                target->mesh.extrudeFace(selectedFaceIndex, extrusionDistance);
                target->rebuildRenderBuffers();
            }
            ImGui::SameLine();
            if (ImGui::Button("Texture")) {
                // open a dialog box to select an image
                
            }
            ImGui::SameLine();
            if (ImGui::Button("Close Editor")) {
                ShowHalfEdgeEditor = false;
            }

            ImGui::End();
        }
    }
};

