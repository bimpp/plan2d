#include <bimpp/plan2d.hpp>

int main(int argc, char* argv[])
{
    bimpp::plan2d::house<double> bimpp_house;

    for (size_t y = 0; y < 5; ++y)
    {
        for (size_t x = 0; x < 5; ++x)
        {
            bimpp_house.nodes.insert(std::make_pair<>(x * 10 + y, bimpp::plan2d::node<>(x, y)));
        }
    }

    for (size_t y = 0; y < 5; ++y)
    {
        for (size_t x = 0; x < 5; ++x)
        {
            if (x > 0)
            {
                bimpp_house.walls.insert(std::make_pair<>(1000 + y * 100 + (x - 1) * 10 + x, bimpp::plan2d::wall<>((x - 1) * 10 + y, x * 10 + y)));
            }
            if (y > 0)
            {
                bimpp_house.walls.insert(std::make_pair<>(2000 + x * 100 + (y - 1) * 10 + y, bimpp::plan2d::wall<>(x * 10 + (y - 1), x * 10 + y)));
            }
        }
    }

    bimpp::plan2d::room<> bimpp_room;
    bimpp_room.wall_ids = {
        1012, 1034, 1101, 1112, 1123, 1201, 1234, 1312, 1323, 1423, 1434,
        2001, 2012, 2023, 2034, 2101, 2112, 2123, 2134, 2201, 2223, 2234, 2301, 2334, 2401, 2412, 2423, 2434
    };
    bimpp_house.rooms.insert(std::make_pair<>(0, bimpp_room));

    bimpp::plan2d::algorithm<>::path_vector bimpp_paths;
    bimpp::plan2d::algorithm<>::calculatePaths(bimpp_house, bimpp_paths);
    return 0;
}
