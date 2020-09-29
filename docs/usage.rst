Usage
=====

bimpp::plan2d::algorithm<>::computeRoomExs
------------------------------------------

.. code-block:: cpp

    // Create a house
    bimpp::plan2d::house<> bimpp_house;
    // ... add some nodes ...
    // ... add some walls ...
    // ... add some rooms ...

    // Create a `room_ex` vector for the output result
    bimpp::plan2d::algorithm<>::room_ex_vector bimpp_room_exs;
    // Compute all edges of rooms by all or a specialed room
    bimpp::plan2d::algorithm<>::computeRoomExs(bimpp_house, bimpp_room_exs/*, or set a room id */);
