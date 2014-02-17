
class Verteks {

public:
	static const int STATE_DEFAULT = 0;
	static const int STATE_ACTIVATED = 1;
	static const int STATE_TAKEN = 2;
	static const int STATE_EXPLODE = 3;
	static const int CONTAIN_RED = 0;
	static const int CONTAIN_YELLOW = 1;
	static const int CONTAIN_GREEN = 2;
	static const int CONTAIN_NOTHING = 3;
	static int count;
	static int nRed;
	static int nYellow;
	static int nGreen;
	int type;
	int time;
	int state;
	int *lengths;
	
	Verteks();

	Verteks(int nVerteks);
	
	void setUp(int type);

	int getType();

	int getNum();

	int getX();

	void setX(int x);

	int getY();

	void setY(int y);

private:
	int x, y,num;
};
