#pragma once

// the basis for all gridMap occupiers. implement a basic is full and is destoryed fuction to be used.
struct GridCell
{
	GridCell()//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{

	}
	GridCell(double height)//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{
		Height = height;
	}
	// normal gridcells hold information about the terrian it is situated on, if it is being used by some building or person and etc
	// public functions > getter and setter functions to be called when creating or changing the gridcell.

	bool isWalkable() const
	{
		// 나중에 water도 추가
		return Height >= 30.0;
	}

	float getMovementCost() const // 높이에 따른 g cost 
	{
		if (Height < 75) return 2.0f;   // Water 
		if (Height < 150) return 1.0f;  // Plains
		if (Height < 205) return 1.5f;  // Hills 
		return 2.5f;                    // Mountains 
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


private:
	double Height = 0.0; 
	double Temperature = 0.0;
	int resources = 0;

	bool isOccupied = false; // if the cell is occupied by a building or person
	bool isDestroyed = false; // if the cell is destroyed by a disaster or something else. if true, the cell cannot be used until it is repaired.
	bool isWater = false; // if the cell under water, it cannot be used for building or walking. only for boats and bridges.
};  