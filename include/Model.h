#include "global.h"
#include "Object.h"
#include <optional>

class Model : public Object
{
public:
	Model(const std::string &model_path) : Object(model_path){}
	virtual ~Model(){}

	//we can manage objects from the scene directly,but we lost some encapsulation
};
