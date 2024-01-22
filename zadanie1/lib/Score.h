#pragma once
class Score
{
public:
	unsigned int scoreP1;
	unsigned int scoreP2;

	unsigned int currentMap;
	unsigned int maxMap;

	void resetScore() {
		scoreP1 = 0;
		scoreP2 = 0;
	}


	Score() {
		scoreP1 = 0;
		scoreP2 = 0;
		currentMap = 0;
		maxMap = 2;
	}

	
	void addScore(int player) {
		if (player == 1) {
			scoreP1++;
		}
		else if (player == 2) {
			scoreP2++;
		}
	}

	int getCurrentMap() {
		return currentMap;
	}
};

