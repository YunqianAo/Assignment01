
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    Load(mapPath);

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if(mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayer; 
    mapLayer = mapData.layers.start;

    // L06: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    // iterates the layers in the map
    while (mapLayer != NULL) {
        //Check if the property Draw exist get the value, if it's true draw the lawyer
        if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value) {
            //iterate all tiles in a layer
            for (int i = 0; i < mapData.width; i++) {
                for (int j = 0; j < mapData.height; j++) {
                    //Get the gid from tile
                    int gid = mapLayer->data->Get(i, j);

                    //L08: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    //Get the Rect from the tileSetTexture;
                    TileSet* tileSet = GetTilesetFromTileId(gid);
                    SDL_Rect tileRect = tileSet->GetRect(gid);
                    //SDL_Rect tileRect = mapData.tilesets.start->data->GetRect(gid); // (!!) we are using always the first tileset in the list

                    //Get the screen coordinates from the tile coordinates
                    iPoint mapCoord = MapToWorld(i, j);

                    // L06: DONE 9: Complete the draw function
                    app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);

                }
            }
        }
        if (mapLayer->data->properties.GetProperty("Collider") != NULL && colLoaded == false) { // Colliders

            for (int x = 0; x < mapLayer->data->width; x++)
            {
                for (int y = 0; y < mapLayer->data->height; y++)
                {
                    // Complete the draw function
                    int gid = mapLayer->data->Get(x, y);

                    if (gid > 0) {
                        TileSet* tileset = GetTilesetFromTileId(gid);
                        SDL_Rect r = tileset->GetRect(gid);
                        iPoint pos = MapToWorld(x, y);
                        if (mapLayer->data->properties.GetProperty("Collider")->value == 1) {
                            PhysBody* c0 = app->physics->CreateRectangle(pos.x + 9, pos.y + 9,18, 18, STATIC); // pos + TilesetSize/2
                            c0->ctype = ColliderType::PLATFORM;
                        }
                        

                    }
                }
            }
        }
        mapLayer = mapLayer->next;
    }
    colLoaded = true;
    return ret;

}

// L08: DONE 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    ListItem<TileSet*>* tileSet;
    tileSet = mapData.tilesets.start;
    while (tileSet != NULL) {
        set = tileSet->data;
        if (gid >= tileSet->data->firstgid && gid < tileSet->data->tilecount) break;
        tileSet = tileSet->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L05: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;
    
    while (tileset != NULL) {
        RELEASE(tileset->data);
        tileset = tileset->next;
    }

    mapData.tilesets.Clear();

    // L06: DONE 2: clean up all layer data
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.layers.start;

    while (layerItem != NULL) {
        RELEASE(layerItem->data->tiles);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    // L05: DONE 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }
    else {

        //Fill mapData variable
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tilewidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileheight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L09: TODO 2: Define a property to store the MapType and Load it from the map

        // L05: DONE 4: Implement the LoadTileSet function to load the tileset properties
       // Iterate the Tileset
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

            TileSet* tileset = new TileSet();

            //Load Tileset attributes
            tileset->name = tilesetNode.attribute("name").as_string();
            tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
            tileset->margin = tilesetNode.attribute("margin").as_int();
            tileset->spacing = tilesetNode.attribute("spacing").as_int();
            tileset->tilewidth = tilesetNode.attribute("tilewidth").as_int();
            tileset->tileheight = tilesetNode.attribute("tileheight").as_int();
            tileset->columns = tilesetNode.attribute("columns").as_int();
            tileset->tilecount = tilesetNode.attribute("tilecount").as_int();

            //Load Tileset image
            SString mapTex = path;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = app->tex->Load(mapTex.GetString());

            mapData.tilesets.Add(tileset);

        }

        // L06: DONE 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L06: DONE 4: Implement a function that loads a single layer layer
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L08: DONE 6 Call Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Reserve the memory for the data 
            mapLayer->tiles = new uint[mapLayer->width * mapLayer->height];
            memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

            //Iterate over all the tiles and assign the values in the data array
            int i = 0;
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles[i] = tileNode.attribute("gid").as_uint();
                i++;
            }

            //add the layer to the map
            mapData.layers.Add(mapLayer);
        }


        // L07 DONE 3: Create colliders      
        // L07 DONE 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map
        //PhysBody* c1 = app->physics->CreateRectangle(0+14*18/2,11.5*18 , 18*14, 18, STATIC);
        //c1->ctype = ColliderType::PLATFORM;

        //PhysBody* c2 = app->physics->CreateRectangle(17.5*18+2*18/2, 11.5*18, 18*3, 18, STATIC);
        //c2->ctype = ColliderType::PLATFORM;

        //PhysBody* c3 = app->physics->CreateRectangle(21.5 * 18 + 2 * 18 / 2, 11.5 * 18, 18 * 3, 18, STATIC);
        //c3->ctype = ColliderType::PLATFORM;

        //PhysBody* c4 = app->physics->CreateRectangle(21.5 * 18+3*18 + 2 * 18 / 2, 11.5 * 18+18, 18 * 3, 18, STATIC);
        //c4->ctype = ColliderType::PLATFORM;

        //PhysBody* c5 = app->physics->CreateRectangle(9.5*18, 10.5*18, 18, 18, STATIC);
        //c5->ctype = ColliderType::PLATFORM;

        //PhysBody* c6 = app->physics->CreateRectangle(12.5 * 18, 9.5 * 18, 18*3, 18, STATIC);
        //c6->ctype = ColliderType::PLATFORM;

        //PhysBody* c7 = app->physics->CreateRectangle(23 * 18 + 3 * 18 + 2 * 18 / 2, 8.5 * 18 + 18, 18 * 2, 18, STATIC);
        //c7->ctype = ColliderType::PLATFORM;

        //PhysBody* c8 = app->physics->CreateRectangle(27 * 18 + 3 * 18 + 2 * 18 / 2, 13.5 * 18 + 18, 18 * 4, 18, STATIC);
        //c8->ctype = ColliderType::PLATFORM;

        //PhysBody* c9 = app->physics->CreateRectangle(33.5 * 18 + 3 * 18 + 2 * 18 / 2, 14.5 * 18 + 18, 18 * 3, 18, STATIC);
        //c9->ctype = ColliderType::PLATFORM;

        //PhysBody* c10 = app->physics->CreateRectangle(27.5 * 18 + 3 * 18 + 2 * 18 / 2, 5*18, 18*3 , 180, STATIC);
        //c10->ctype = ColliderType::PLATFORM;

        //PhysBody* c11 = app->physics->CreateRectangle(16.5 * 18 + 3 * 18 + 2 * 18 / 2, 8.5 * 18 + 18, 18 * 3, 18, STATIC);
        //c11->ctype = ColliderType::PLATFORM;

          // L05: DONE 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tilewidth, mapData.tileheight);

            LOG("Tilesets----");

            ListItem<TileSet*>* tileset;
            tileset = mapData.tilesets.start;
            while (tileset != NULL) {
                //iterate the tilesets
                LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
                LOG("tile width : %d tile height : %d", tileset->data->tilewidth, tileset->data->tileheight);
                LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
                tileset = tileset->next;
            }

            LOG("Layers----");

            ListItem<MapLayer*>* mapLayer;
            mapLayer = mapData.layers.start;

            while (mapLayer != NULL) {
                LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
                LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
                mapLayer = mapLayer->next;
            }
        }

        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tilewidth;
    ret.y = y * mapData.tileheight;

    return ret;
}

// L08: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.Add(p);
    }

    return ret;
}

// L08: DONE 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* property = propertyList.start;
    Property* p = NULL;

    while (property)
    {
        if (property->data->name == name) {
            p = property->data;
            break;
        }
        property = property->next;
    }

    return p;
}

