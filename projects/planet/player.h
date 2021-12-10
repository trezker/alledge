class Player {
private:
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
	shared_ptr<Transformnode> transform;
public:
	Player(shared_ptr<Scenenode> p);
};