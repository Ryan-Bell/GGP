#pragma once
class Object {
public:
	static int nextInstanceId;
	int instanceId;
private:
	Object();
	~Object();
	
	//TODO active and visible bools for early returns out of update and draw

	//TODO is const really needed here?
	int GetInstanceId() const;

	//TODO string name and ToString function would be nice

	// overloaded comparators that use check the instance ids
	bool operator!=(Object& other);
	bool operator==(Object& other);
};