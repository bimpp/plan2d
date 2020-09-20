#include <bimpp/plan2d.hpp>

int main(int argc, char* argv[])
{
    bimpp::plan2d::house<double> bimpp_house;

    bimpp_house.nodes.insert(std::make_pair<>(0, bimpp::plan2d::node<>(0.0, 0.0)));
    bimpp_house.nodes.insert(std::make_pair<>(1, bimpp::plan2d::node<>(20.0, 0.0)));
    bimpp_house.nodes.insert(std::make_pair<>(2, bimpp::plan2d::node<>(20.0, 20.0)));
    bimpp_house.nodes.insert(std::make_pair<>(3, bimpp::plan2d::node<>(0.0, 20.0)));
    bimpp_house.nodes.insert(std::make_pair<>(4, bimpp::plan2d::node<>(10.0, 0.0)));
    bimpp_house.nodes.insert(std::make_pair<>(5, bimpp::plan2d::node<>(10.0, 10.0)));
    bimpp_house.nodes.insert(std::make_pair<>(6, bimpp::plan2d::node<>(10.0, -10.0)));
    bimpp_house.nodes.insert(std::make_pair<>(7, bimpp::plan2d::node<>(10.0, -20.0)));
    bimpp_house.nodes.insert(std::make_pair<>(8, bimpp::plan2d::node<>(0.0, -20.0)));
    bimpp_house.nodes.insert(std::make_pair<>(9, bimpp::plan2d::node<>(0.0, -10.0)));
    bimpp_house.nodes.insert(std::make_pair<>(10, bimpp::plan2d::node<>(-10.0, 0.0)));
    bimpp_house.nodes.insert(std::make_pair<>(11, bimpp::plan2d::node<>(-10.0, 10.0)));
    bimpp_house.walls.insert(std::make_pair<>(0, bimpp::plan2d::wall<>(0, 4)));
    bimpp_house.walls.insert(std::make_pair<>(1, bimpp::plan2d::wall<>(4, 5)));
    bimpp_house.walls.insert(std::make_pair<>(2, bimpp::plan2d::wall<>(4, 1)));
    bimpp_house.walls.insert(std::make_pair<>(3, bimpp::plan2d::wall<>(1, 2)));
    bimpp_house.walls.insert(std::make_pair<>(4, bimpp::plan2d::wall<>(2, 3)));
    bimpp_house.walls.insert(std::make_pair<>(5, bimpp::plan2d::wall<>(3, 0)));
    bimpp_house.walls.insert(std::make_pair<>(6, bimpp::plan2d::wall<>(4, 6)));
    bimpp_house.walls.insert(std::make_pair<>(7, bimpp::plan2d::wall<>(6, 7)));
    bimpp_house.walls.insert(std::make_pair<>(8, bimpp::plan2d::wall<>(7, 8)));
    bimpp_house.walls.insert(std::make_pair<>(9, bimpp::plan2d::wall<>(8, 9)));
    bimpp_house.walls.insert(std::make_pair<>(10, bimpp::plan2d::wall<>(9, 6)));
    bimpp_house.walls.insert(std::make_pair<>(11, bimpp::plan2d::wall<>(10, 11)));

    bimpp::plan2d::room<> bimpp_room;
    bimpp_room.wall_ids = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    bimpp_house.rooms.insert(std::make_pair<>(4, bimpp_room));

    bimpp::plan2d::algorithm<>::path_vector bimpp_paths;
    bimpp::plan2d::algorithm<>::calculatePaths(bimpp_house, 4, bimpp_paths);
    return 0;
}
