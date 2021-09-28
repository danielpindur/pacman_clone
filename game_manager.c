#include "game_manager.h"
#include "graphics.h"
#include "audio_manager.h"
#include <unistd.h>

#define MAP_DATA_LOCATION "../map_data.bin"
#define POINT_VALUE 1 // How much each point is worth

#define NUMBER_OF_GHOSTS 4

#define HIGHSCORE_DATA_FILE "../highscore.bin"

// Converts 2D coords to 1D
int convert_to_1D(int width, int row, int column){
    return row * width + column;
}

// Load map data
void load_map_data(Map* map){        
    // Read data from file
    FILE* file = fopen(MAP_DATA_LOCATION, "rb");
    set_map_limits(map);
    map->map_data_size = map->x_limit * map->y_limit;
    int* input_data = (int*) malloc(map->map_data_size * sizeof(int));
    int read = fread(input_data, sizeof(int), map->map_data_size, file);
    fclose(file);

    // Put data into nodes
    map->map_nodes = (Node*) malloc(map->map_data_size * sizeof(Node));
    for (int i = 0; i < map->map_data_size; i++)
    {
        map->map_nodes[i].map_data = input_data[i];
        if (input_data[i] == 14){
            map->full_points++;
        }
    }
    
    free(input_data);
    assert(read == map->map_data_size);
}

// Reset map data
void reset_map_data(Map* map){
    FILE* file = fopen(MAP_DATA_LOCATION, "rb");
    int* input_data = (int*) malloc(map->map_data_size * sizeof(int));
    int read = fread(input_data, sizeof(int), map->map_data_size, file);
    fclose(file);

    for (int i = 0; i < map->map_data_size; i++)
    {
        map->map_nodes[i].map_data = input_data[i];
    }

    free(input_data);
    assert(read == map->map_data_size);
}


// Returns new coords for player based on direction of movement and player current coords
void get_coords_from_direction(int* new_x, int* new_y, const char direction, Player* player){
    switch (direction)
    {
    case 'U':
        *new_x = player->x_pos;
        *new_y = player->y_pos - 1; 
        break;
    case 'D':
        *new_x = player->x_pos;
        *new_y = player->y_pos + 1;
        break;
    case 'L':
        *new_x = player->x_pos - 1;
        *new_y = player->y_pos;
        break;
    case 'R':
        *new_x = player->x_pos + 1;
        *new_y = player->y_pos;
        break;
    }
}

// Can player move there?
bool is_correct_path(const int new_x, const int new_y, Map* map, Player* player){
    int map_position = convert_to_1D(map->x_limit, new_y, new_x);
    int field = map->map_nodes[map_position].map_data;
    if (new_x >= 0 && 
        new_x < map->x_limit &&
        new_y >= 0 &&
        new_y < map->y_limit &&
        (field == 0 || field == 14))
        {
        player->x_pos = new_x;
        player->y_pos = new_y;
        return true;
    }
    //Teleportations
    else if (new_x < 0){
        player->x_pos = map->x_limit;
        return true;
    }
    else if (new_x >= map->x_limit){
        player->x_pos = 0;
        return true;
    }
    return false;
}

// If there is point on field add to score and remove it
void check_for_poit(Player* player, Map* map, const int new_x, const int new_y, Audio* audio, Player_Prefs* player_prefs, Enemies* enemies){
    int field_pos = convert_to_1D(map->x_limit, new_y, new_x);

    // If point then add score and play sfx
    if (map->map_nodes[field_pos].map_data == 14){
        map->map_nodes[field_pos].map_data = 0;
        player->score += POINT_VALUE;
        player_prefs->remaining_points--;
        play_coin_sfx(audio);
        if (player->score > 9999){
            player->score = 9999;
        }       
    }
}

// Moves player based on user input
void move_player(Player* player, Map* map, const char new_direction, Audio* audio, Player_Prefs* player_prefs, Enemies* enemies){

    // Get new player position
    int new_x, new_y;
    get_coords_from_direction(&new_x, &new_y, new_direction, player);    

    // Can player move there?
    if (is_correct_path(new_x, new_y, map, player)){
        player->move_direction = new_direction;
        player->moving = true;
    }
    else{
        get_coords_from_direction(&new_x, &new_y, player->move_direction, player);
        if (!is_correct_path(new_x, new_y, map, player)){
            player->moving = false;
        }
        else
        {
            player->moving = true;
        }
        
    }

    // If there is point on field add to score and remove it
    check_for_poit(player, map, new_x, new_y, audio, player_prefs, enemies);
}

// Create ghosts and set their starting pos
void create_ghosts(Enemies* enemies){    
    enemies->ghosts = malloc (enemies->number_of_enemies * sizeof (Ghost));

    // Set ghosts starting position
    for (int i = 0; i < enemies->number_of_enemies; i++)
    {
        enemies->ghosts[i].x_pos = 12 + i;
        enemies->ghosts[i].y_pos = 14;
    }
}  

// Add connected node based on diff_x, diff_y if node is not wall
void add_connected_node(Map* map, int index, int diff_x, int diff_y){
    int index_to_add = index + convert_to_1D(map->x_limit, diff_y, diff_x);
    if (!map->map_nodes[index_to_add].is_wall){
        map->map_nodes[index].number_of_connected_nodes += 1;
        map->map_nodes[index].connected_nodes = (Node**) realloc(map->map_nodes[index].connected_nodes, sizeof(Node*) * map->map_nodes[index].number_of_connected_nodes);
        map->map_nodes[index].connected_nodes[map->map_nodes[index].number_of_connected_nodes - 1] = &map->map_nodes[index_to_add];
    }
}

// Set up nodes to default during startup
void setup_nodes(Map* map){
    for (int i = 0; i < map->map_data_size; i++)
    {
        // Setup is wall
        if (map->map_nodes[i].map_data == 0 || map->map_nodes[i].map_data == 14){
            map->map_nodes[i].is_wall = false;
        }
        else{
            map->map_nodes[i].is_wall = true;
        }

        // Setup x, y
        convert_to_2D(i, map->x_limit, &map->map_nodes[i].y, &map->map_nodes[i].x);

        // Setup number of connected nodes
        map->map_nodes[i].number_of_connected_nodes = 0;

        // Init connected nodes
        map->map_nodes[i].connected_nodes = NULL;
    }  

    for (int j = 0; j < map->map_data_size; j++)
    {
        // Setup connected nodes        
        // Look up if not first row
        if (j / map->x_limit != 0){
            add_connected_node(map, j, 0, -1);
        }

        // Look down if not last row
        if (j / map->x_limit != (map->y_limit - 1)){
            add_connected_node(map, j, 0, 1);
        }

        // Look left if not most left
        if (j % map->x_limit != 0){
            add_connected_node(map, j, -1, 0);
        }

        // Look right if not most right
        if (j % map->x_limit != (map->x_limit - 1)){
            add_connected_node(map, j, 1, 0);
        }
    }
    
}

// Reset nodes after pathfinding finished
void reset_nodes(Map* map){
    for (int i = 0; i < map->map_data_size; i++){
        // Reset global and local goal
        map->map_nodes[i].local_goal = INFINITY;
        map->map_nodes[i].global_goal = INFINITY;

        // Reset parent nodes
        map->map_nodes[i].parent_node = NULL;
        map->map_nodes[i].is_visited = false;
    } 
}

// Calculates the distance of a and b using pythagoras theorem
float distance(Node* start, Node* finish){
    int a = start->x - finish->x;
    int b = start->y - finish->y;
    return sqrtf(a * a + b * b);
}

float heuristic(Node* start, Node* finish){
    return distance(start, finish);
}

// Compare function for qsort
int compare_nodes_global_goal(const void* a, const void* b)
{
    const Node* node1 = *(Node**) a;
    const Node* node2 = *(Node**) b;
    float result = node1->global_goal - node2->global_goal;
    
    if (result < 0){
        return -1;
    }
    else if (result > 0){
        return 1;
    }
    else
    {
        return 0;
    }   
}

// Solves the A* path finding algorithm
void solve_Astar(Map* map, Ghost* ghost, Player* player, int* new_x, int* new_y){
    reset_nodes(map);

    // Setup start
    int starting_index = convert_to_1D(map->x_limit, ghost->y_pos, ghost->x_pos);
    Node* starting_node = &map->map_nodes[starting_index];

    // Setup end
    int finish_index = convert_to_1D(map->x_limit, player->y_pos, player->x_pos);
    Node* finish_node = &map->map_nodes[finish_index];

    // Set starting values 
    starting_node->local_goal = 0.0f;
    starting_node->global_goal = heuristic(starting_node, finish_node);

    // Aray of all nodes to test
    Node** not_tested_nodes = (Node**) malloc(sizeof(Node*));
    not_tested_nodes[0] = starting_node;
    int number_of_nodes_to_test = 1;

    bool testing = true;

    while (testing){
        // Sort the array
        if (number_of_nodes_to_test > 1){
            qsort(not_tested_nodes, number_of_nodes_to_test, sizeof(Node*), compare_nodes_global_goal);
        }        

        // Get first not-visited node
        Node* front_node = not_tested_nodes[0];
        if (front_node->is_visited && !(number_of_nodes_to_test > 1)){
            break;
        }
        else
        {
            while (number_of_nodes_to_test > 1 && front_node->is_visited)
            {
                number_of_nodes_to_test--;
                memmove(&not_tested_nodes[0], &not_tested_nodes[1], sizeof(Node*) * number_of_nodes_to_test);
                not_tested_nodes = realloc(not_tested_nodes, sizeof(Node*) * number_of_nodes_to_test);
                front_node = not_tested_nodes[0];
            }
            if (front_node->is_visited){
                break;
            }
        }
        
        // Set current node as visited
        front_node->is_visited = true;

        // Check all connected nodes
        for (int i = 0; i < front_node->number_of_connected_nodes; i++)
        {
            // If not already visitid add to not tested list
            if (!front_node->connected_nodes[i]->is_visited){
                number_of_nodes_to_test++;
                not_tested_nodes = realloc(not_tested_nodes, sizeof(Node*) * number_of_nodes_to_test);
                not_tested_nodes[number_of_nodes_to_test - 1] = front_node->connected_nodes[i];
            }

            // Check if path from this node is better
            float posibly_lower_goal = front_node->local_goal + distance(front_node, front_node->connected_nodes[i]);
            if (posibly_lower_goal < front_node->connected_nodes[i]->local_goal){
                front_node->connected_nodes[i]->parent_node = front_node;
                front_node->connected_nodes[i]->local_goal = posibly_lower_goal;
                front_node->connected_nodes[i]->global_goal = posibly_lower_goal + heuristic(front_node->connected_nodes[i], finish_node);
            }
        }   
    }

    if (finish_node->parent_node != NULL){
        // Get the node that ghost will move to
        while (finish_node->parent_node != starting_node){
        finish_node = finish_node->parent_node;
        }

        // Update ghost position
        *new_x = finish_node->x;
        *new_y = finish_node->y;
    }
    free(not_tested_nodes);
}

// Update position for all enemies
void move_enemies(Map* map, Enemies* enemies, Player* player){    
    for (int i = 0; i < enemies->number_of_enemies; i++)
    {
        int new_x = -1;
        int new_y = -1;
        solve_Astar(map, &enemies->ghosts[i], player, &new_x, &new_y);
        if (new_x != -1 && new_y != -1){
            //Check if it is free and ghost can move there
            bool is_ocupied = false;
            for (int j = 0; j < enemies->number_of_enemies; j++)
            {
                if (new_x == enemies->ghosts[j].x_pos && new_y == enemies->ghosts[j].y_pos){
                    is_ocupied = true;
                }
            }

            if (!is_ocupied){
                enemies->ghosts[i].x_pos = new_x;
                enemies->ghosts[i].y_pos = new_y;
            }
        }
    }    
}

// Reset player to starting position
void reset_player(Player* player){
    player->x_pos = player->starting_x;
    player->y_pos = player->starting_y;
    player->move_direction = '0';
    player->new_direction = '0';
    player->moving = false;
    player->open_mouth = false;
}

void reset_ghosts(Enemies* enemies){
    // Set ghosts starting position
    for (int i = 0; i < enemies->number_of_enemies; i++)
    {
        enemies->ghosts[i].x_pos = 12 + i;
        enemies->ghosts[i].y_pos = 14;
    }
}

// Reset game state after player died
void player_died(Player* player, Enemies* enemies, Map* map, Audio* audio, Player_Prefs* player_prefs){
    if (!audio->bg_muted){
        toggle_background_music(audio);
    }
    play_death_sfx(audio);

    // Reset state
    player->health--;
    next_level(map, enemies, player, player_prefs);

    // Wait 2s after death
    SDL_Delay(2000);
    if (!audio->bg_muted){
        toggle_background_music(audio);
    }
}

// Check for collision between ghosts and player
bool check_collision(Player* player, Enemies* enemies){
    bool result = false;
    for (int i = 0; i < enemies->number_of_enemies; i++)
    {
        if (player->x_pos == enemies->ghosts[i].x_pos && player->y_pos == enemies->ghosts[i].y_pos){
            result = true;
        }
    }
    return result;    
}

// Loads highscore from file, if no file -> create file and set highscore = 0
void load_highscore(Player_Prefs* player_prefs){
    if ( access (HIGHSCORE_DATA_FILE, F_OK) == 0){
        // File exists
        FILE* file = fopen(HIGHSCORE_DATA_FILE, "rb");
        int read = fread(&player_prefs->highscore, sizeof(int), 1, file);
        fclose(file);
        assert(read == 1);
    }
    else{
        // No file found
        player_prefs->highscore = 0;
        FILE* file = fopen(HIGHSCORE_DATA_FILE, "wb");
        int write = fwrite(&player_prefs->highscore, sizeof(int), 1, file);
        fclose(file);
        assert(write == 1);
    }
}

// Saves current score as new highscore and writes it as file
void try_to_save_highscore(Player_Prefs* player_prefs, Player* player){
    // If higher -> save
    if (player->score > player_prefs->highscore){
        player_prefs->highscore = player->score;
        FILE* file = fopen(HIGHSCORE_DATA_FILE, "wb");
        int write = fwrite(&player_prefs->highscore, sizeof(int), 1, file);
        fclose(file);
        assert(write == 1);
    }
}

// Heal player and set score to 0
void new_game(Player* player){
    player->health = 3;
    player->score = 0;
}

// Load "next" level
void next_level(Map* map, Enemies* enemies, Player* player, Player_Prefs* player_prefs){
    player_prefs->remaining_points = map->full_points;
    reset_map_data(map);
    reset_ghosts(enemies);
    reset_player(player);    
}

// Clear current level
void level_cleared(Audio* audio, Player_Prefs* player_prefs, Enemies* enemies, Player* player, Map* map){
    // Play level end sfx, wait, load next level
    if (!audio->bg_muted){
        toggle_background_music(audio);
    }
    play_level_finish_sfx(audio);
    SDL_Delay(2000);
    next_level(map, enemies, player, player_prefs);
    if (!audio->bg_muted){
        toggle_background_music(audio);
    }
}

            
                      
        