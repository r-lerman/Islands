#include <iostream>
#include <list>
#include <set>

using coord = std::pair<int, int>;

class Island
{
public:

    Island(int x, int y) { add_land(x, y); }

    Island(const Island& isl, int x, int y) { add_land(isl, x, y); }

    bool operator!=(const Island &isl) { return !(m_land == isl.m_land); }
    bool operator==(const Island& isl) { return m_land == isl.m_land; }

    void add_land(int x, int y) { m_land.insert(coord(x, y)); }

    bool land_recorded(int x, int y) const
    { 
        return m_land.find(coord(x, y)) != m_land.end();
    }

    int size() const { return m_land.size(); }

    int set_land(int x, int y, bool land)
    {
        for (auto &it : m_land)
        {
            if (land)
            {
                if (it.first == x && it.second == y) return 2; // land already recorded

                if (it.first == x && it.second == y - 1 || it.first == x && it.second == y + 1 ||
                    it.first == x - 1 && it.second == y || it.first == x + 1 && it.second == y)
                {
                    return 1; // new land can be added to this island
                }
            }
            else
            {
                if (it.first == x && it.second == y)
                {
                    m_land.erase(it);
                    return 3; // land removed
                }
            }
        }
        
        return 0; // land cannot be added/removed to/from this island
    }

    void absorption(Island& isl)
    {
        for (auto &it : isl.m_land)
            m_land.insert(coord(it.first, it.second));

        isl.m_land.clear();
    }

private:

    void add_land(const Island& isl, int x, int y)
    {
        if (!isl.land_recorded(x, y) || land_recorded(x, y)) return;
        add_land(x, y);
        
        add_land(isl, x - 1, y);
        add_land(isl, x + 1, y);
        add_land(isl, x, y - 1);
        add_land(isl, x, y + 1);
    }

    std::set<coord> m_land;

};

class Ocean
{
public:

    Ocean() { }

    int set(int x, int y, int value)
    {
        int res = 0;
        std::list<Island>::iterator it = islands.begin();
        for (; it != islands.end(); it++)
            if (res = it->set_land(x, y, value)) break;

        if (res == 0)
        {
            // we cannot attach land to any existing islands so create a new island
            if (value)
                islands.push_back(Island(x, y));
        }
        else if (res == 1)
        {
            std::list<Island>::iterator base_i = it++; // first island where we can add a land

            // Absorb all other islands where we can add the same land too
            while (it != islands.end())
            {
                if (it->set_land(x, y, true) == 1)
                {
                    base_i->absorption(*it);
                    it = islands.erase(it);
                }
                else
                    it++;
            }

            base_i->add_land(x, y);
        }
        else if (res == 2)
        {
            // nothing to do here
            // land already recorded
        }
        else if (res == 3)
        {
            // separate islands 
            Island i1(*it, x - 1, y),
                   i2(*it, x + 1, y),
                   i3(*it, x, y - 1),
                   i4(*it, x, y + 1);

            if (i1.size())
                islands.push_back(i1);

            if (i2.size() && i2 != i1)
                islands.push_back(i2);

            if (i3.size() && i3 != i1 && i3 != i2)
                islands.push_back(i3);

            if (i4.size() && i4 != i1 && i4 != i2 && i4 != i3)
                islands.push_back(i4);

            islands.erase(it);
        }

        return islands.size();
    }

private:

    std::list<Island> islands;
};

int main()
{
    Ocean a;

    std::cout << a.set(3, 1, 1) << std::endl; // 1
    std::cout << a.set(3, 2, 1) << std::endl; // 1

    std::cout << a.set(3, 4, 1) << std::endl; // 2
    std::cout << a.set(3, 5, 1) << std::endl; // 2
    std::cout << a.set(3, 6, 1) << std::endl; // 2

    std::cout << a.set(1, 4, 1) << std::endl; // 3

    std::cout << a.set(5, 4, 1) << std::endl; // 4
    std::cout << a.set(6, 4, 1) << std::endl; // 4

    std::cout << a.set(4, 4, 1) << std::endl; // 3

    std::cout << a.set(2, 4, 1) << std::endl; // 2
    std::cout << a.set(3, 3, 1) << std::endl; // 1 

    std::cout << a.set(3, 4, 0) << std::endl; // 4 (global break)

    std::cout << a.set(3, 2, 0) << std::endl; // 5 
    std::cout << a.set(3, 3, 0) << std::endl; // 4
    std::cout << std::endl;

    // ------------------------------------------------------------

    Ocean b;

    std::cout << b.set(5, 0, 1) << std::endl; // 1
    std::cout << b.set(5, 2, 1) << std::endl; // 2
    std::cout << b.set(5, 1, 1) << std::endl; // 1

    std::cout << b.set(7, 0, 1) << std::endl; // 2
    std::cout << b.set(7, 2, 1) << std::endl; // 3
    std::cout << b.set(7, 1, 1) << std::endl; // 2

    std::cout << b.set(6, 0, 1) << std::endl; // 1
    std::cout << b.set(6, 2, 1) << std::endl; // 1
    std::cout << b.set(6, 1, 1) << std::endl; // 1

    std::cout << b.set(6, 1, 0) << std::endl; // 1 (lake)

    std::cout << b.set(9, 1, 0) << std::endl; // 1 (far from islands)
    std::cout << std::endl;

    std::cout << b.set(7, 3, 1) << std::endl; // 1
    std::cout << b.set(7, 4, 1) << std::endl; // 1
    std::cout << b.set(7, 5, 1) << std::endl; // 1

    std::cout << b.set(9, 3, 1) << std::endl; // 2
    std::cout << b.set(9, 4, 1) << std::endl; // 2
    std::cout << b.set(9, 5, 1) << std::endl; // 2
    std::cout << b.set(9, 5, 1) << std::endl; // 2 (the same)

    std::cout << b.set(8, 3, 1) << std::endl; // 1
    std::cout << b.set(8, 4, 1) << std::endl; // 1
    std::cout << b.set(8, 5, 1) << std::endl; // 1

    std::cout << b.set(7, 3, 0) << std::endl; // 2
    std::cout << std::endl;

    return 0;
}

