#include "stdafx.h"
#include "Tool.h"

Tool::Tool(int type) {
		this->type = type;
		state = STATE_UNUSED;
		if(type == TYPE_NORMAL){
			time = 4;
			price = 1;
		}
		else if(type == TYPE_2X){
			time = 2;
			price = 2;
		}
		else if(type == TYPE_4X){
			time = 1;
			price = 3;
		}
	}