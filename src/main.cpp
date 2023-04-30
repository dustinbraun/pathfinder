#include <cstdint>
#include <vector>
#include <iostream>

constexpr int WINDOW_W = 800;
constexpr int WINDOW_H = 800;

#include <nav/query.hpp>

#include <SDL2/SDL.h>



void
save_mesh_data(
    const char * path,
    const std::vector<VertexData> & vertex_data,
    const std::vector<FaceData> & face_data
) {
    std::ofstream f(path, std::ios::binary);
    f.clear();
    VertexId num_vertices = vertex_data.size();
    f.write(reinterpret_cast<const char *>(&num_vertices), sizeof(VertexId));
    for (auto vertex : vertex_data) {
        f.write(reinterpret_cast<const char *>(&vertex), sizeof(VertexData));
    }
    FaceId num_faces = face_data.size();
    f.write(reinterpret_cast<const char *>(&num_faces), sizeof(FaceId));
    for (auto face : face_data) {
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[0]), sizeof(VertexId));
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[1]), sizeof(VertexId));
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[2]), sizeof(VertexId));
    }
}

void
load_mesh_data(
    const char * path,
    std::vector<VertexData> & vertex_data,
    std::vector<FaceData> & face_data
) {
    vertex_data.clear();
    face_data.clear();
    std::ifstream f(path, std::ios::binary);
    VertexId num_vertices;
    f.read(reinterpret_cast<char *>(&num_vertices), sizeof(VertexId));
    std::cout << num_vertices << std::endl;
    vertex_data.resize(num_vertices);
    for (VertexId vertex_id = 0; vertex_id < num_vertices; ++vertex_id) {
        f.read(reinterpret_cast<char *>(&vertex_data.data()[vertex_id]), sizeof(VertexData));
    }
    FaceId num_faces;
    f.read(reinterpret_cast<char *>(&num_faces), sizeof(FaceId));
    std::cout << num_faces << std::endl;
    face_data.resize(num_faces);
    for (FaceId face_id = 0; face_id < num_faces; ++face_id) {
        f.read(reinterpret_cast<char *>(&face_data.data()[face_id].m_vertex_ids[0]), sizeof(VertexId));
        f.read(reinterpret_cast<char *>(&face_data.data()[face_id].m_vertex_ids[1]), sizeof(VertexId));
        f.read(reinterpret_cast<char *>(&face_data.data()[face_id].m_vertex_ids[2]), sizeof(VertexId));
    }
}

void render_mesh(SDL_Renderer * renderer, const Mesh & mesh) {
    for (FaceId face_id = 0; face_id < mesh.get_num_faces(); ++face_id) {
        const Face & face = mesh.get_face_by_id(face_id);
        for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
            VertexId vertex_id_a;
            VertexId vertex_id_b;
            face.get_edge_vertex_ids(edge_index, vertex_id_a, vertex_id_b);
            const Vertex & vertex_a = mesh.get_vertex_by_id(vertex_id_a);
            const Vertex & vertex_b = mesh.get_vertex_by_id(vertex_id_b);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer,
                vertex_a.m_pos.m_x,
                vertex_a.m_pos.m_y,
                vertex_b.m_pos.m_x,
                vertex_b.m_pos.m_y
            );
        }
    }
}

void render_path(SDL_Renderer * renderer, const Mesh & mesh, const std::vector<FaceId> & path) {
    for (auto face_id : path) {
        //std::cout << face_id << std::endl;
        const Face & face = mesh.get_face_by_id(face_id);
        SDL_Rect rect;
        Point center_point = mesh.get_face_center_point(face);
        rect.x = center_point.m_x - 5.0f;
        rect.y = center_point.m_y - 5.0f;
        rect.w = 10;
        rect.h = 10;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void render_node_arena(SDL_Renderer * renderer, const Mesh & mesh, const NodeArena & node_arena) {
    for (size_t node_index = 0; node_index < node_arena.get_size(); ++node_index) {
        const Node & node = node_arena.get_nodes()[node_index];
        // SDL_Rect rect;
        // rect.x = node.m_state.m_pos.m_x - 3.0f;
        // rect.y = node.m_state.m_pos.m_y - 3.0f;
        // rect.w = 6;
        // rect.h = 6;
        // if (node.m_state.m_is_closed) {
        //     SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        // }
        // else {
        //     SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        // }
        // SDL_RenderDrawRect(renderer, &rect);

        // Point prev_face_pos = mesh.get_face_center_point(mesh.get_face_by_id(node.m_prev_face));
        // SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        // SDL_RenderDrawLine(
        //     renderer,
        //     node.m_state.m_pos.m_x,
        //     node.m_state.m_pos.m_y,
        //     prev_face_pos.m_x,
        //     prev_face_pos.m_y
        // );

        // Point next_face_pos = mesh.get_face_center_point(mesh.get_face_by_id(node.m_next_face));
        // SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        // SDL_RenderDrawLine(
        //     renderer,
        //     node.m_state.m_pos.m_x,
        //     node.m_state.m_pos.m_y,
        //     next_face_pos.m_x,
        //     next_face_pos.m_y
        // );

        if (node.m_state.m_parent_node != nullptr) {
            Point parent_pos = node.m_state.m_parent_node->m_state.m_pos;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(
                renderer,
                node.m_state.m_pos.m_x,
                node.m_state.m_pos.m_y,
                parent_pos.m_x,
                parent_pos.m_y
            );
        }
    }
}

void render_debug(SDL_Renderer * renderer, const Mesh & mesh, const NodeArena & node_arena) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    render_mesh(renderer, mesh);
    render_node_arena(renderer, mesh, node_arena);

    SDL_RenderPresent(renderer);
}


int main() {


    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_W,
        WINDOW_H,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    


    std::vector<VertexData> vertex_data;
    std::vector<FaceData> face_data;
    load_mesh_data("example.map", vertex_data, face_data);



    Mesh mesh;
    mesh.import(
        vertex_data.data(),
        vertex_data.size(),
        face_data.data(),
        face_data.size()
    );

    Point start_pos = Point(0.0f, 0.0f);
    FaceId start_face_id = 0;
    Point end_pos = Point(0.0f, 0.0f);
    FaceId end_face_id = 2;

    Query query([&] (const Mesh & mesh, const NodeArena & node_arena) {
        render_debug(renderer, mesh, node_arena);
        SDL_Delay(50);
    });

    // QueryEx query2(mesh, [&] (const Mesh & mesh, const NodeArenaEx & node_arena) {
    //     render_debug_ex(renderer, mesh, node_arena);
    //     SDL_Delay(50);
    // });
    // query2.search(start_face_id, start_pos, end_face_id, end_pos);

    std::vector<FaceId> path;

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                break;
            }
            
            // if (event.type == SDL_KEYDOWN) {
            //     if (event.key.keysym.sym == SDLK_s) {
            //         std::cout << "saved" << std::endl;
            //         mesh.save("example.map");
            //     }
            // }

            // if (event.type == SDL_KEYDOWN) {
            //     if (event.key.keysym.sym == SDLK_l) {
            //         std::cout << "loaded" << std::endl;
            //         mesh.load("example.map");
            //     }
            // }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    FaceId new_start_face_id = mesh.get_face_id_containing_point(Point(x, y));
                    if (new_start_face_id != FACE_ID_NONE) {
                        start_face_id = new_start_face_id;
                        start_pos = Point(x, y);
                        path = query.search(mesh, start_face_id, start_pos, end_face_id, end_pos);
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    FaceId new_end_face_id = mesh.get_face_id_containing_point(Point(x, y));
                    if (new_end_face_id != FACE_ID_NONE) {
                        end_face_id = new_end_face_id;
                        end_pos = Point(x, y);
                        path = query.search(mesh, start_face_id, start_pos, end_face_id, end_pos);

                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        render_mesh(renderer, mesh);
        render_path(renderer, mesh, path);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
