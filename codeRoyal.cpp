#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define KNIGHT "KNIGHT"
#define TOWER  "TOWER"
#define MINE   "MINE"
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

    // -1:No structure  0:Goldmine  1:Tower  2:Barracks
    int structure_type;
    enum e_StructureTypes { None = -1, Mine, Tower, Barracks };

    // -1:No structure   0:Fri"\n"y   1:Enemy
    int owner;
    /*
    When no structure: -1
    When goldmine: the income rate ranging from 1 to 5 (or -1 if enemy)
    When tower: the remaining HP
    When barracks, the number of turns before a new set of creeps can be trained (if 0, then training may be started this turn)
    */
    int param1;
    /*
    When no structure: -1
    When goldmine: -1
    When tower: the attack radius measured from its center
    When barracks: the creep type: 0 for KNIGHT, 1 for ARCHER, 2 for GIANT
    */
    int param2;
    

    Site(){}

    Site(int _id, int _x, int _y, int _radius): id(_id), x(_x), y(_y), radius(_radius)
    {} 
    
    float CalDistance(int _xq, int _yq)
    {
        float distance = sqrt(pow((_xq - x), 2) + pow((_yq - y), 2));
        return distance;
    }
};

class Unit
{
public:
    // Coordinates of the unit
    int x, y;
    
    // 0 = Fri"\n"y; 1 = Enemy
    int owner;

    // Queen = -1 // knight = 0 // archer = 1
    int unitType;

    // Current HP of the unit
    int health;

    // Dummy constructor
    Unit() {}

    Unit(int _x, int _y, int _owner, int _unitType, int _health) 
            : x(_x), y(_y), owner(_owner), unitType(_unitType), health(_health)
    {}

};

class Commands
{
private:
    Commands();
public:
    static void MoveTo(Site _site)
    {
        cout << "MOVE " << _site.x <<" "<< _site.y <<"\n";
    }
    static void MoveTo(int _x, int _y)
    {
        cout << "MOVE " << _x <<" "<< _y <<"\n";
    }

    // TODO: Use enum for different type of buildings
    static int Build(const string &_type, int _siteId)
    {
        if(_type == KNIGHT)
            cout<< "BUILD "<< _siteId <<" BARRACKS-"<< _type << "\n";
        else if(_type == "TOWER")
            cout<< "BUILD "<< _siteId << " TOWER" << "\n";
        else
            cout<< "BUILD "<< _siteId <<" MINE"<< "\n";

        return _siteId;
    }

    static void Wait()
    {
        cout << "WAIT" << "\n";
    }
    static void Train(int _siteId, int _gold)
    {
        if(_gold >= 80)
            cout << "TRAIN " << _siteId << "\n";
        else
            printf("TRAIN\n");

    }
    static void Train(vector<Site>& v_sites, int _gold)
    {
        ostringstream os;
        for(auto site : v_sites)
        {
            // TODO: Remove destroyed barracks
            os << " " << site.id;
        }
        string ids(os.str());
        cout << "TRAIN" << ids <<"\n";
    }

    static int FindNearestSite(const vector<Site> &_sites)
    {
        int nearestId = 0;
        for(Site site : _sites)
        {
            if(site.distance < _sites[nearestId].distance && site.structure_type == -1 && site.owner != 0)
                nearestId = site.id;
        }
        return nearestId;
    }
    inline static bool Contains(const vector<Site>& sites, const Site& site)
    {
        if(find_if(sites.begin(), sites.end(), [&site](const Site& obj){return site.id == obj.id;}) != sites.end())
            return true;
        return false;
    }
};

int main()
{
    int numSites;
    cin >> numSites;
    vector<Site> v_sites;
    vector<Site> v_mySites;
    Unit* myQueen = nullptr;
    Unit* enemyQueen = nullptr;
    int towerId = 0;
    int barrackId = 0;
    int mine_num = 0;
    int tower_num = 0;
    cin.ignore();
    
    for (int i = 0; i < numSites; i++) 
    {
        int siteId;
        int x;
        int y;
        int radius;
        cin >> siteId >> x >> y >> radius; cin.ignore();

        v_sites.push_back({siteId, x, y, radius});
    }

    // game loop
    while (1) 
    {
        int gold;
        int touchedSite; // -1 if none
        int nearestSite_id;
        cin >> gold >> touchedSite; cin.ignore();
        
        // -------------------------------- Update Sites --------------------------------
        for (int i = 0; i < numSites; i++) 
        {
            int siteId;
            int gold; // used in future leagues
            int maxMineSize; // used in future leagues
            int structureType; // -1 = No structure, 2 = Barracks
            int owner; // -1 = No structure, 0 = Fri"\n"y, 1 = Enemy

            int param1;
      
            int param2; 

            cin >> siteId >> gold >> maxMineSize >> structureType >> owner >> param1 >> param2; cin.ignore();

            if(v_sites[i].id == siteId)
            {
                v_sites[i].owner = owner;
                v_sites[i].structure_type = structureType;
                v_sites[i].param1 = param1;
                v_sites[i].param2 = param2;
            }
            // TODO: Remove destroyed sites from v_mySites
            if(v_sites[i].owner != 0)
            {
                Site *currentSite = &v_sites[i];
                v_mySites.erase(remove_if(v_mySites.begin(), v_mySites.end(), [&currentSite, &mine_num](const Site& site)
                    {
                        return  currentSite->id == site.id;
                    }), v_mySites.end());

            }
            if(v_sites[i].owner == 0 && v_sites[i].structure_type != -1) // if site is mine
            {
                if(Commands::Contains(v_mySites, v_sites[i]) == false)
                    v_mySites.push_back(v_sites[i]);
            }


        }
       
        // -------------------------------- Update Units --------------------------------
        int numUnits;
        cin >> numUnits; cin.ignore();

        for (int i = 0; i < numUnits; i++) 
        {
            int x;
            int y;
            int owner;
            int unitType; // -1 = QUEEN, 0 = KNIGHT, 1 = ARCHER
            int health;
            cin >> x >> y >> owner >> unitType >> health; cin.ignore();

            // Get Queens
            if(unitType == -1)
            {
                if(owner == 0)
                    myQueen = new Unit(x, y, owner, unitType, health);
                else
                    enemyQueen = new Unit(x, y, owner, unitType, health);
            }   
        }

        // ---------------- Calculate distance of each site from the queen ----------------
        for(int i = 0; i < numSites; i++)
        {
            v_sites[i].distance = v_sites[i].CalDistance(myQueen->x, myQueen->y);
        }
        
        nearestSite_id = Commands::FindNearestSite(v_sites);
        
        if(v_mySites.size() < 8)
        {
            if(touchedSite == nearestSite_id && v_sites[touchedSite].structure_type == -1)
            {
                if(v_mySites.size() < 1)
                {
                    Commands::Build(KNIGHT, nearestSite_id);
                    barrackId = nearestSite_id;
                }
                else if(v_mySites.size() < 4 && )
                {
                    Commands::Build(MINE, nearestSite_id);
                }
                else
                {
                    Commands::Build(TOWER, nearestSite_id);
                    towerId = nearestSite_id;
                }

            }
            // if the site hasn't built yet
            else if(v_sites[nearestSite_id].structure_type == -1 || v_sites[nearestSite_id].owner != 0)
                Commands::MoveTo(v_sites[nearestSite_id]);
        }
        else
        {
            Commands::Build(TOWER, towerId);
        }
        Commands::Train(barrackId, gold);
    }
}