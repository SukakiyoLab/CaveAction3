#include "transform.h"

#include <cstdio>

namespace component{

	

	CAT_Transform::CAT_Transform(CAT_Transform::ComponentInitializer* cInit){

		this->set_position(cInit->position);
		this->set_rotation(cInit->rotation);
		this->set_scale(cInit->scale);

		printf("-Create Transform!\n");
	}

	CAT_Transform::~CAT_Transform(){
		printf("-Delete Transform!\n");
	}

}
