
class Tool {

public:
	static const int STATE_UNUSED = 0;
	static const int STATE_USED = 1;
	static const int TYPE_NORMAL = 0;
	static const int TYPE_2X = 1;
	static const int TYPE_4X = 2;
	int state;
	
	Tool(int type);
	int type, time, price;
	
};
