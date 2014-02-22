
class Peti {

public:
	static const int RED = 0;
	static const int YELLOW = 1;
	static const int GREEN = 2;
	static const int RED_EXPLODE_TIME = 1;
	static const int YELLOW_EXPLODE_TIME = 2;
	static const int GREEN_EXPLODE_TIME = 4;
	int loc, type;
	
	Peti();
	Peti(int loc, int type);
	
};
