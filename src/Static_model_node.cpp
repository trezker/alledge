#include "../alledge/Static_model_node.h"

Static_model_node::Static_model_node()
{
	model = NULL;
}

Static_model_node::~Static_model_node()
{
}

void Static_model_node::Set_model(shared_ptr<Static_model> m)
{
	model = m;
}

void Static_model_node::Render()
{
	model->Render();
}
