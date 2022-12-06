#pragma once
#include <map>
#include <list>
#include <glm.hpp>
#include <vector>
#include <iostream>
#include <climits>
#include <cstring>
#include "Direction.h"

class SearchAgent
{
private:
    template <typename T>
    static bool contains(std::vector<T> list, T element)
    {
        for (int i = 0; i < list.size(); i++)
        {
            if (list.at(i) == element)
                return true;
        }
        return false;
    }

    static bool InBoundary(std::pair<int, int> position, std::vector<std::vector<bool>>* searchSpace)
    {
        if (position.first < 0 || position.first >= searchSpace->size())
            return false;
        if (position.second < 0 || position.second >= searchSpace[0].size())
            return false;
        return true;
    }

public:
    // Accepts 2 positions within a grid
    static Direction GetDirectionOfShortestPath(glm::vec2 start, glm::vec2 end, std::vector<std::vector<bool>>* searchSpace)
    {
        std::pair<int, int> p1 = std::make_pair(start.x, start.y);
        std::pair<int, int> p2 = std::make_pair(end.x, end.y);
        return GetDirectionOfShortestPath(p1, p2, searchSpace);
    }


    static Direction GetDirectionOfShortestPath(std::pair<int,int> start, std::pair<int,int> end, std::vector<std::vector<bool>>* searchSpace)
    {
        // Base case, ensure that the start is not the same as the end
        if (start == end)
            return None;

        std::vector<std::pair<int, int>> visited = std::vector<std::pair<int, int>>();
        std::map<std::pair<int, int>, Direction> paths = std::map<std::pair<int, int>, Direction>();
        std::list<std::pair<int, int>> queue = std::list<std::pair<int, int>>();
        std::pair<int, int> curr;

        queue.push_back(start);
        visited.push_back(start);
        while (!queue.empty())
        {
            curr = queue.front();
            queue.pop_front();

            // Found our node, return the original direction we took to get here
            if (curr == end)
            {
                Direction d;
                while (curr != start)
                {
                    d = paths[curr];
                    curr = CalculateCoordinate(curr, OppositeDirection(d));
                }
                return d;
            }

            std::pair<int, int> up = std::pair<int, int>(curr.first, curr.second - 1);
            std::pair<int, int> down = std::pair<int, int>(curr.first, curr.second + 1);
            std::pair<int, int> left = std::pair<int, int>(curr.first - 1, curr.second);
            std::pair<int, int> right = std::pair<int, int>(curr.first + 1, curr.second);

            //search space is in [row][column] format meaning we must 
            // consider the second piece of the pair as the row coordinate 
            // and the first as the column
            if (InBoundary(up, searchSpace) && searchSpace->at(up.second).at(up.first) && !contains(visited, up))
            {
                paths[up] = Up;
                visited.push_back(up);
                queue.push_back(up);
            }
            if (InBoundary(down, searchSpace) && searchSpace->at(down.second).at(down.first) && !contains(visited, down))
            {
                paths[down] = Down;
                visited.push_back(down);
                queue.push_back(down);
            }
            if (InBoundary(left, searchSpace) && searchSpace->at(left.second).at(left.first) && !contains(visited, left))
            {
                paths[left] = Left;
                visited.push_back(left);
                queue.push_back(left);
            }
            if (InBoundary(right, searchSpace) && searchSpace->at(right.second).at(right.first) && !contains(visited, right))
            {
                paths[right] = Right;
                visited.push_back(right);
                queue.push_back(right);
            }
        }

        // Should never reach here, but return none if the target is not found.
        return None;
    }

};