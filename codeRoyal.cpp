#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define KNIGHT "KNIGHT"
#define ARCHER "ARCHER"

class Unit
{
public:
    // Coordinates of the unit
    int x, y;
    
    // 0 = Friendly; 1 = Enemy
    int owner;

    // Queen = -1 // knight = 0 // archer = 1
    int unitType;

    // Current HP of the unit
    int health;

    // Dummy constructor
    Unit() {}

    Unit(int _x, int _y, int _owner, int _unitType, int _health) :
            x(_x), y(_y), owner(_owner), unitType(_unitType), health(_health)
    {}

};

class Site
{
public:
    // id of the site
    int id;
    // coordination of site
    int x, y;
    // redius of site
    int radius;
    // distance from queen: should be updated each turn
    float distance;

    // -1:No structure   2:Barracks
    int structure_type;

    // -1:No structure   0:Friendly   1:Enemy
    int owner;
    // -1 = No structure, 0= train this turn, ow= number of turns befor a new set of creeps can be trained 
    int TrainTurns;

    Site(){}

    Site(int _id, int _x, int _y, int _radius): id(_id), x(_x), y(_y), radius(_radius)
    {
    } 
    
    float CalDistance(int _xq, int _yq)
    {
        float distance = sqrt(pow((_xq - x), 2) + pow((_yq - y), 2));
        return distance;
    }
};

class Commands
{
private:
    Commands();
public:
    static void MoveTo(Site _site)
    {
        cout << "MOVE " << _site.x <<" "<< _site.y <<endl;
    }
    static void MoveTo(int _x, int _y)
    {
        cout << "MOVE " << _x <<" "<< _y <<endl;
    }

    static int Build(string type, int _siteId)
    {
        cout<< "BUILD "<< _siteId <<" BARRACKS-"<< type << endl;
        return _siteId;
    }

    static void Wait()
    {
        cout << "WAIT" << endl;

    }

    static void Train(vector<Site> &v_Barracks, int _gold)
    {
        ostringstream os;
        for(auto site : v_Barracks)
        {
            // TODO: Remove destroyed barracks
            if(site.owner != 0)
            {
                v_Barracks.pop_back();
            }
            if(_gold == 80)
            {
                if(site.structure_type == 2)
                os << " " << site.id;
                string ids(os.str());
                cout << "TRAIN" << ids <<endl;        
                return;
            }
            if(site.owner != -1)
                os <<" " << site.id;
        }
        string ids(os.str());
        cout << "TRAIN" << ids <<endl;
    }

};

int main()
{
    int numSites;
    cin >> numSites;
    Site* sites = new Site[numSites];
    vector<Site> barracks;
    barracks.clear();
    vector<Site> enemyBarracks;

    cin.ignore();
    
    for (int i = 0; i < numSites; i++) 
    {
        int siteId;
        int x;
        int y;
        int radius;
        cin >> siteId >> x >> y >> radius; cin.ignore();

        sites[i] = Site(siteId, x, y, radius);
        // cerr <<"id: " <<sites[i].id <<"   "<<sites[i].x <<","<< sites[i].y <<endl;

    }

    // game loop
    while (1) 
    {
        int gold;
        int touchedSite; // -1 if none
        int nearestSite_id;
        cin >> gold >> touchedSite; cin.ignore();

        for (int i = 0; i < numSites; i++) 
        {
            int siteId;
            int ignore1; // used in future leagues
            int ignore2; // used in future leagues
            int structureType; // -1 = No structure, 2 = Barracks
            int owner; // -1 = No structure, 0 = Friendly, 1 = Enemy
            int param1; // -1 = No structure, 0= train this turn, ow= number of turns befor a new set of creeps can be trained 
            int param2;

            cin >> siteId >> ignore1 >> ignore2 >> structureType >> owner >> param1 >> param2; cin.ignore();

            if(sites[i].id == siteId)
            {
                sites[i].owner = owner;
                sites[i].structure_type = structureType;
                sites[i].TrainTurns = param1;
                // cerr << sites[i] <<endl;
            }
           // TODO: get enemy barracks

        }

        // -------------------------------- Update Units --------------------------------
        int numUnits;
        cin >> numUnits; cin.ignore();
        Unit* myUnits = new Unit[numUnits]; 
        Unit myQueen;
        Unit enemyQueen;
        for (int i = 0; i < numUnits; i++) 
        {
            int x;
            int y;
            int owner;
            int unitType; // -1 = QUEEN, 0 = KNIGHT, 1 = ARCHER
            int health;
            cin >> x >> y >> owner >> unitType >> health; cin.ignore();
            // our units
            if(owner == 0)
            {
                myUnits[i] = Unit(x, y, owner, unitType, health);
                if(unitType == -1)
                {
                    myQueen = myUnits[i];
                }
            }
            else
            {
                // TODO: find enemy queen
            }

        }
        // --------------------------------------------------------------------------------

        // ---------------- Calculate distance of each site from the queen ----------------
        for(int i = 0; i < numSites; i++)
        {
            sites[i].distance = sites[i].CalDistance(myQueen.x, myQueen.y);
        }
        // --------------------------------------------------------------------------------
        // find nearest not built site
        nearestSite_id = 0;
        for(int i = 1; i < numSites; i++)
        {
            if(sites[i].distance < sites[nearestSite_id].distance && sites[i].structure_type == -1 && sites[i].owner != 0)
            {
                nearestSite_id = i;
            }
        }
        cerr << "nearest site id: " << nearestSite_id << endl; 

        if(barracks.size() < 2)
        {

            if(touchedSite == nearestSite_id )
            {
                if(barracks.size() < 1)
                    Commands::Build(ARCHER, nearestSite_id);
                else
                    Commands::Build(KNIGHT, nearestSite_id);

                sites[nearestSite_id].owner = 0;    
                barracks.push_back(sites[nearestSite_id]);
            }
            // if the site hasn't built yet
            else if(sites[nearestSite_id].structure_type != 2 || sites[nearestSite_id].owner != 0)
            {
                Commands::MoveTo(sites[nearestSite_id]);
            }    
        }
        else
        {
            // TODO: hide behind your barracks
            Commands::Wait();
        }       

        Commands::Train(barracks, gold);
    }
}