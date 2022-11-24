#pragma once
enum Direction { Up, Down, Left, Right, None };

static std::string ToString(Direction d)
{
	switch (d)
	{
	case Up:return "Up";
	case Down:return "Down";
	case Left:return "Left";
	case Right:return "Right";
	}
	return "None";
}

static Direction OppositeDirection(Direction d)
{
    switch (d)
    {
    case Up:
        return Down;
    case Down:
        return Up;
    case Left:
        return Right;
    case Right:
        return Left;
    default:
        return None;
    }
}

// Given a position and a direction, calculate the new destination
static std::pair<int, int> CalculateCoordinate(std::pair<int, int> position, Direction d)
{
    switch (d)
    {
    case Up:
        return std::pair<int, int>(position.first, position.second - 1);
    case Down:
        return std::pair<int, int>(position.first, position.second + 1);
    case Left:
        return std::pair<int, int>(position.first - 1, position.second);
    case Right:
        return std::pair<int, int>(position.first + 1, position.second);
    default:
        return position;
    }
}