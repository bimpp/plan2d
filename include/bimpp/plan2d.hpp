/*
 * The MIT License (MIT)
 * Copyright © 2020 BIM++
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include <boost/polygon/polygon.hpp>

namespace bimpp
{
    namespace plan2d
    {
        template<typename T = double>
        class constant
        {
        public:
            typedef typename boost::polygon::point_data<T>      point_type;
            typedef typename boost::polygon::polygon_data<T>    polygon_type;

        public:
            static const point_type zero_point;
            static const point_type unit_point;
            static const size_t     none_id;

        public:
            static bool isValid(size_t _id)
            {
                return (_id != none_id);
            }
        };

        template<typename T>
        const typename constant<T>::point_type  constant<T>::zero_point(0, 0);
        template<typename T>
        const typename constant<T>::point_type  constant<T>::unit_point(1, 1);
        template<typename T>
        const size_t                            constant<T>::none_id(-1);

        template<typename T = double>
        class node
        {
        public:
            typedef typename constant<T>::point_type    point_type;

        public:
            node(T _x = 0, T _y = 0)
                : point(_x, _y)
            {}

            node(const point_type& _point = constant<T>::zero_point)
                : point(_point)
            {}

        public:
            point_type point;
        };

        template<typename T = double>
        class wall
        {
        public:
            wall(size_t _start_node_id = constant<T>::none_id
                , size_t _end_node_id = constant<T>::none_id
                , T _thickness = 0)
                : kind("")
                , start_node_id(_start_node_id)
                , end_node_id(_end_node_id)
                , thickness(_thickness)
            {}

        public:
            inline bool isValid() const
            {
                return (typename constant<T>::isValid(start_node_id)
                    && typename constant<T>::isValid(end_node_id)
                    && start_node_id != end_node_id
                    && thickness >= 0);
            }

        public:
            std::string kind;
            size_t start_node_id;
            size_t end_node_id;
            T thickness;
        };

        template<typename T = double>
        class hole
        {
        public:
            hole(size_t _wall_id = constant<T>::none_id
                , T _distance = 0
                , T _width = 0)
                : kind("")
                , direction("")
                , wall_id(_wall_id)
                , distance(_distance)
                , width(_width)
            {}

        public:
            inline bool isValid() const
            {
                return (typename constant<T>::isValid(wall_id)
                    && distance != 0
                    && width != 0);
            }

        public:
            std::string kind;
            std::string direction;
            size_t wall_id;
            T distance;
            T width;
        };

        template<typename T = double>
        class room
        {
        public:
            room()
                : kind("")
                , wall_ids()
            {}

        public:
            std::string kind;
            std::vector<size_t> wall_ids;
        };

        template<typename T = double>
        class house
        {
        public:
            typedef node<T>                     node_type;
            typedef std::map<size_t, node_type> node_map;
            typedef wall<T>                     wall_type;
            typedef std::map<size_t, wall_type> wall_map;
            typedef hole<T>                     hole_type;
            typedef std::map<size_t, hole_type> hole_map;
            typedef room<T>                     room_type;
            typedef std::map<size_t, room_type> room_map;

        public:
            house()
                : name("")
                , nodes()
                , walls()
                , holes()
                , rooms()
            {}

        public:
            inline void reset()
            {
                name = "";
                nodes.clear();
                walls.clear();
                holes.clear();
                rooms.clear();
            }

        public:
            std::string name;
            node_map nodes;
            wall_map walls;
            hole_map holes;
            room_map rooms;
        };

        template<typename T = double>
        class building
        {
        public:
            typedef house<T>                                    house_type;
            typedef std::map<size_t, house_type>                house_map;
            typedef typename boost::polygon::point_data<size_t> position_type;
            typedef std::map<position_type, size_t>             potision_map;

        public:
            house_map houses;
            potision_map positions;
        };

        template<typename T = double>
        class site
        {
        public:
            typedef building<T>                     building_type;
            typedef std::map<size_t, building_type> building_map;

        public:
            std::string name;
            building_map buildings;
        };

        template<typename T = double>
        class project
        {
        public:
            typedef site<T>                     site_type;
            typedef std::map<size_t, site_type> site_map;

        public:
            std::string name;
            site_map sites;
        };

        template<typename T = double>
        class algorithm
        {
        public:
            typedef house<T>                            house_type;
            typedef typename house_type::node_type      node_type;

        public:
            class wall_ex
            {
            public:
                wall_ex(size_t _id = constant<T>::none_id, bool _inversed = false)
                    : id(_id)
                    , inversed(_inversed)
                {}

            public:
                bool operator==(const wall_ex& _o)
                {
                    return (id == _o.id && inversed == _o.inversed);
                }

            public:
                size_t id;
                bool inversed;
            };

            class path
            {
            public:
                path()
                    : room_id(constant<T>::none_id)
                    , walls()
                    , inside(false)
                {}

            public:
                size_t room_id;
                std::vector<wall_ex> walls;
                bool inside;
            };

            class node_ex
            {
            public:
                node_ex(size_t _id = constant<T>::none_id
                    , bool _used = false)
                    : id(_id)
                    , used(_used)
                    , with_wall()
                {}

            public:
                size_t id;
                bool used;
                wall_ex with_wall;
            };

        public:
            typedef std::vector<path>               path_vector;

        public:
            static T calculateSinAngleEx(const node<T>& _o, const node<T>& _a, const node<T>& _b)
            {
                node<T> line_a(_a.point.x() - _o.point.x(), _a.point.y() - _o.point.y());
                T len_a = sqrt(line_a.point.x() * line_a.point.x() + line_a.point.y() * line_a.point.y());
                if (len_a != 0)
                {
                    line_a.point.x(line_a.point.x() / len_a);
                    line_a.point.y(line_a.point.y() / len_a);
                }
                node<T> line_b(_b.point.x() - _o.point.x(), _b.point.y() - _o.point.y());
                T len_b = sqrt(line_b.point.x() * line_b.point.x() + line_b.point.y() * line_b.point.y());
                if (len_b != 0)
                {
                    line_b.point.x(line_b.point.x() / len_b);
                    line_b.point.y(line_b.point.y() / len_b);
                }
                T sin_res = line_a.point.x() * line_b.point.y() - line_a.point.y() * line_b.point.x();
                T cos_res = line_a.point.x() * line_b.point.x() + line_a.point.y() * line_b.point.y();
                T res = sin_res;
                if (cos_res < static_cast<T>(0))
                {
                    if (sin_res >= static_cast<T>(0))
                    {
                        res = static_cast<T>(2) - res;
                    }
                    else
                    {
                        res = static_cast<T>(-2) + res;
                    }
                }
                if (res < static_cast<T>(0))
                {
                    res = static_cast<T>(4) + res;
                }
                return res;
            }

            static bool calculatePaths(const house_type& _house
                , size_t _room_id
                , path_vector& _paths)
            {
                const typename std::map<size_t, room<T>>::const_iterator cit_found_room = _house.rooms.find(_room_id);
                if (cit_found_room == _house.rooms.cend())
                {
                    return false;
                }
                std::map<size_t, std::vector<node_ex>> bim_node_2_next_nodes;

                const room<T>& bim_area = cit_found_room->second;
                for (const size_t wall_id : bim_area.wall_ids)
                {
                    const wall<T>& bim_wall = _house.walls.find(wall_id)->second;
                    node_ex bim_next_node;
                    bim_next_node.with_wall.id = wall_id;
                    bim_next_node.with_wall.inversed = false;
                    bim_next_node.id = bim_wall.end_node_id;
                    bim_node_2_next_nodes[bim_wall.start_node_id].push_back(bim_next_node);
                    bim_next_node.with_wall.inversed = true;
                    bim_next_node.id = bim_wall.start_node_id;
                    bim_node_2_next_nodes[bim_wall.end_node_id].push_back(bim_next_node);
                }

                if (bim_node_2_next_nodes.empty())
                {
                    return false;
                }

                /// compute the closed path
                while (!bim_node_2_next_nodes.empty())
                {
                    path bim_closed_path;
                    bim_closed_path.room_id = _room_id;
                    bool bim_path_is_closed = false;

                    size_t bim_start_node_id = bim_node_2_next_nodes.cbegin()->first;
                    size_t bim_last_node_id = constant<T>::none_id;
                    size_t bim_first_wall_start_node_id = constant<T>::none_id;
                    size_t bim_first_wall_end_node_id = constant<T>::none_id;
                    wall_ex bim_first_wall_ex;
                    while (true)
                    {
                        const node<T>& bim_start_node = _house.nodes.find(bim_start_node_id)->second;
                        std::vector<node_ex>& bim_next_nodes = bim_node_2_next_nodes.find(bim_start_node_id)->second;
                        if (bim_next_nodes.empty())
                        {
                            break;
                        }
                        if (bim_last_node_id == constant<T>::none_id)
                        {
                            bim_last_node_id = bim_start_node_id;
                            node_ex& bim_next_node = bim_next_nodes[0];
                            bim_start_node_id = bim_next_node.id;
                            bim_first_wall_ex = bim_next_node.with_wall;
                        }
                        else
                        {
                            const node<T>& bim_last_node = _house.nodes.find(bim_last_node_id)->second;
                            std::map<T, size_t> sin_angle_ex_2_index;
                            for (size_t i = 0; i < bim_next_nodes.size(); ++i)
                            {
                                const node_ex& bim_next_node = bim_next_nodes[i];
                                if (bim_next_node.used) continue;
                                const node<T>& bim_node = _house.nodes.find(bim_next_node.id)->second;
                                T sin_angle_ex = calculateSinAngleEx(bim_start_node, bim_last_node, bim_node);
                                sin_angle_ex_2_index.insert(std::make_pair<>(sin_angle_ex, i));
                            }
                            if (sin_angle_ex_2_index.empty())
                            {
                                break;
                            }

                            typename std::map<T, size_t>::const_iterator sin_angle_ex_2_index_last = sin_angle_ex_2_index.cend();
                            --sin_angle_ex_2_index_last;
                            node_ex& bim_next_node = bim_next_nodes[sin_angle_ex_2_index_last->second];
                            bim_next_node.used = true;

                            wall_ex bim_wall_ex;
                            bim_wall_ex = bim_next_node.with_wall;
                            bim_closed_path.walls.push_back(bim_wall_ex);

                            if (bim_first_wall_ex == bim_next_node.with_wall)
                            {
                                bim_path_is_closed = true;
                                break;
                            }

                            bim_last_node_id = bim_start_node_id;
                            bim_start_node_id = bim_next_node.id;

                            if (bim_start_node_id == bim_last_node_id)
                            {
                                break;
                            }
                        }
                    }

                    if (bim_path_is_closed)
                    {
                        _paths.push_back(bim_closed_path);
                    }

                    for (typename std::map<size_t, std::vector<node_ex>>::iterator it_m = bim_node_2_next_nodes.begin(); it_m != bim_node_2_next_nodes.end();)
                    {
                        std::vector<node_ex>& bim_next_nodes = it_m->second;
                        for (typename std::vector<node_ex>::iterator it_v = bim_next_nodes.begin(); it_v != bim_next_nodes.end();)
                        {
                            if (it_v->used)
                            {
                                it_v = bim_next_nodes.erase(it_v);
                            }
                            else
                            {
                                ++it_v;
                            }
                        }

                        if (it_m->second.empty())
                        {
                            it_m = bim_node_2_next_nodes.erase(it_m);
                        }
                        else
                        {
                            ++it_m;
                        }
                    }
                }

                /// decide wheather the closed path is inside or outside
                for (path& closed_path : _paths)
                {
                    //
                }

                return !_paths.empty();
            }
        };
    }
}
