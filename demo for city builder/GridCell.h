#pragma once


enum class TerrainType
{
	Water,
	Plains,
	Mountain,
	Snow,
	None
};

// the basis for all gridMap occupiers. implement a basic is full and is destoryed fuction to be used.
struct GridCell
{
	GridCell() : Height(0.0), Type(TerrainType::None), isOccupied(false), isDestroyed(false)//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{

	}
	GridCell(double height) : Height(height), isOccupied(false), isDestroyed(false)//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{
		if (Height < 75.0)      Type = TerrainType::Water;
		else if (Height < 150.0) Type = TerrainType::Plains;
		else if (Height < 205.0) Type = TerrainType::Mountain;
		else                    Type = TerrainType::Snow;
	}
	// normal gridcells hold information about the terrian it is situated on, if it is being used by some building or person and etc
	// public functions > getter and setter functions to be called when creating or changing the gridcell.

	bool isWalkable() const
	{
		return Type != TerrainType::Water;
	}

	float getMovementCost() const
	{
		switch (Type)
		{
		case TerrainType::Water:    return 5.0f;
		case TerrainType::Plains:   return 1.0f;
		case TerrainType::Mountain: return 1.5f;
		case TerrainType::Snow:     return 2.5f;
		default:                    return 999.9f;
		}
	}


	void setHeight(double height) { Height = height; }
	double getHeight() const { return Height; }
	void setTemperature(double temperature) { Temperature = temperature; }
	double getTemperature() const { return Temperature; }
	void setOccupied(bool occupied) { isOccupied = occupied; }
	bool getOccupied() const { return isOccupied; }
	void setDestroyed(bool destroyed) { isDestroyed = destroyed; }
	bool getDestroyed() const { return isDestroyed; }
	void setWater(bool water) { isWater = water; }
	bool getWater() const { return isWater; }

	TerrainType getType() const { return Type; }



private:
	double Height = 0.0; 
	TerrainType Type;
	double Temperature = 0.0;
	int resources = 0;

	bool isOccupied = false; // if the cell is occupied by a building or person
	bool isDestroyed = false; // if the cell is destroyed by a disaster or something else. if true, the cell cannot be used until it is repaired.
	bool isWater = false; // if the cell under water, it cannot be used for building or walking. only for boats and bridges.
};  