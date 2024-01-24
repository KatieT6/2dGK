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

	void theEnd() {
		if (scoreP1 == 3) {
			std::cout << "Player 1 won!" << std::endl;
			printScore();

			//exit(0);
		}
		else if (scoreP2 == 3) {
			std::cout << "Player 2 won!" << std::endl;
			printScore();
			//exit(0);
		}

	}
	
	void printScore() {
		std::cout << "P1 ponts:" << scoreP1 << std::endl;
		std::cout << "P2 ponts:" << scoreP2 << std::endl;
	}

	void addPointTo(int player) {
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

	void nextMap() {
		currentMap++;
	}
};

