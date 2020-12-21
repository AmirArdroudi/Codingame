// ###################################################
//
//      Author: Amir Ardroudi
//      date : 21 Dec 2020
//      https://www.codingame.com/ide/puzzle/code-royale      
//
// ###################################################

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * structureType: -1 ->No structure  2 ->Barracks
 * owner:         -1 ->No structure  0 ->Friendly  1 ->Enemy
 **/
#define KNIGHT "KNIGHT"
#define ARCHER "ARCHER"


float CalDistance(int _x1, int _y1, int _x2, int _y2)
{
    float distance = sqrt(pow((_x1 - _x2),2) + pow((_y2 - _y1),2) * 1.0);
    return distance;
}

// calculate distance from queen
int FindNearestSite()
{
    int site_id;

    return  site_id;
}

void MoveToSite(int _x, int _y)
{
    printf("MOVE %d %d\n", _x, _y);
}

int Build(int _siteId, const char* _buildingType)
{
        printf("BUILD %d BARRACKS-%s\n", _siteId, _buildingType);    
        return _siteId;
}
void Train(int barracks[3])
{
    printf("TRAIN %d\n", barracks[0]);
}
void Wait()
{
    printf("WAIT\n");
}

struct s_site
{
    // id of the site
    int id;
    // coordination of site
    int x, y;
    // redius of site
    int radius;
    // distance from queen: should be updated each turn
    float distance;
    // -1:No structure 2:Barracks
    int structure_type;
    // -1:No structure 0:Friendly 1:Enemy
    int owner;
};

struct s_unit
{
    // Coordinates of the unit
    int x, y;
    
    // 0 = Friendly; 1 = Enemy
    int owner;

    // The unit type: -1 for Queen, 0 for KNIGHT, 1 for ARCHER
    int unitType;

    // Current HP of the unit
    int health;
};

int main()
{
    int num_sites;
    scanf("%d", &num_sites);
    
    struct s_site sites[num_sites];

    int queen_id = 0;
    int nearestSite_id = 0;
    
    int barracks[3] = {-1, -1, -1};
    
    for (int i = 0; i < num_sites; i++) {
        int site_id;
        int x;
        int y;
        int radius;
        scanf("%d%d%d%d", &site_id, &x, &y, &radius);
        
        // store sites in array of sites struct
        sites[i].id = site_id;
        sites[i].x = x;
        sites[i].y = y;
        sites[i].radius = radius;

    }
    

    // game loop
    while (1) 
    {
        // the current amount of gold available to be spent
        int gold;

        // touchedSite: the id of the site your Queen is touching, or -1 if none
        int touched_site;

        scanf("%d%d", &gold, &touched_site);

        for (int i = 0; i < num_sites; i++) 
        {
            int site_id;

            // used in future leagues
            int ignore_1;

            // used in future leagues
            int ignore_2;

            // -1 = No structure, 2 = Barracks
            int structure_type;

            // -1 = No structure, 0 = Friendly, 1 = Enemy
            int owner;

            int param_1;
            int param_2;

            scanf("%d%d%d%d%d%d%d", &site_id, &ignore_1, &ignore_2, &structure_type, &owner, &param_1, &param_2);
            sites[i].structure_type = structure_type;

        }

        int num_units;
        scanf("%d", &num_units);

        struct s_unit units[num_units];

        for (int i = 0; i < num_units; i++) 
        {
            int x;
            int y;
            int owner;

            // -1 = QUEEN, 0 = KNIGHT, 1 = ARCHER
            int unit_type;

            int health;
            scanf("%d%d%d%d%d", &x, &y, &owner, &unit_type, &health);

            units[i].x = x;
            units[i].y = y;
            units[i].owner = owner;
            units[i].unitType = unit_type;
            units[i].health = health;
        }

        // Find queen id
        for(int i = 0; i < num_units; i++)
        {
            //find queens
            if(units[i].unitType == -1 )
            {
                if(units[i].owner == 0)
                    queen_id = i;    
            }
        }
        // ------------------- Calculate Distance of owr Queen from sites -------------------
        for(int i = 0; i < num_sites; i++)
        {
            sites[i].distance = CalDistance(sites[i].x, sites[i].y, units[queen_id].x, units[queen_id].y);
        }
        //------------------------------------------------------------------------------------

        float Shortest_dis = sites[0].distance;
        for(int i = 0; i < num_sites; i++)
        {
            if(sites[i].distance < Shortest_dis)
            {
                Shortest_dis = sites[i].distance;
                nearestSite_id = i;
            }
            fprintf(stderr, "distance: %f from site id: %d \n", sites[i].distance, sites[i].id);
        }

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "%d %s Debug messages...\n", int, string);

        // First line: A valid queen action

        if(touched_site == -1)
        {
            MoveToSite(sites[nearestSite_id].x, sites[nearestSite_id].y);
        }
        else if(sites[touched_site].structure_type == -1)
        {
            barracks[0] = Build(touched_site, KNIGHT);
        }
        else
        {
            Wait();
        }
        // Second line: A set of training instructions
        Train(barracks);

    }

    return 0;
}