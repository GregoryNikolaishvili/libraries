#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(unsigned int newDataPointsCount, int newErrorValue)
{
	errorValue = newErrorValue;
	if (newDataPointsCount < MAX_DATA_POINTS)
		dataPointsCount = newDataPointsCount;
	else
		dataPointsCount = MAX_DATA_POINTS;

	sum = 0;
	readingCount = 0;
	totalErrors = 0;

	currentIdx = 0; //initialize so that we start to write at index 0

	for (int i = 0; i < dataPointsCount; i++)
	{
		values[i] = 0; // fill the array with 0's
	}
}

int MovingAverageFilter::process(int value)
{
	if (value == errorValue)
	{
		totalErrors++;
		if (totalErrors >= dataPointsCount)
		{
			reset();
			return errorValue;
		}
		if (readingCount > 0)
			return sum / readingCount;

		return errorValue;
	}

	totalErrors = 0;

	// add each new data point to the sum until the m_readings array is filled
	if (readingCount < dataPointsCount)
	{
		sum += value;
		readingCount++;
	}
	else
	{
		sum = sum - values[currentIdx] + value;
	}

	values[currentIdx] = value;

	currentIdx = (currentIdx + 1) % dataPointsCount;

	return sum / readingCount;
}

void MovingAverageFilter::reset()
{
	sum = 0;
	readingCount = 0;
	currentIdx = 0;
	totalErrors = 0;
}