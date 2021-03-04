C++ Code Problem
"Islands in the ocean"

Create function for "Ocean" class which return the current number of islands in the ocean.
int set(int x, int y, int value)

Function shold register a land (value = 1) or water (value = 0) on the point with coordinates (x, y).

Points form islands via vertical and horizontal links with neighboring points. Diagonal links are not counted.
Islands should be merged if a new registered land joined them or should be separated if a new registered water disconnected them.
Adding water to inner part of the island just create a lake inside the island.

