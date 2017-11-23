template <class TYPE>
class p2Vec3 {

//Variables
//private: 
	

public:
	TYPE x, y, z;




//Methods
//private:
	

public:
	//Constructor
	p2Vec3(TYPE _x, TYPE _y, TYPE _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	//Destructor
	~p2Vec3() {};


	//Operators

	p2Vec3& operator +(const p2Vec3& other_vec) {

		Vec3 ret = {
			x + other_vec.x,
			y + other_vec.y,
			z + other_vec.z
		};

		return ret;
	}

	p2Vec3& operator +=(const p2Vec3& other_vec) {

		x += other_vec.x;
		y += other_vec.y;
		z += other_vec.z;

		return *this;
	}

	p2Vec3& operator -(const p2Vec3& other_vec) {

		Vec3 ret = {
			x - other_vec.x,
			y - other_vec.y,
			z - other_vec.z
		};

		return ret;
	}

	p2Vec3& operator -=(const p2Vec3& other_vec) {

			x -= other_vec.x;
			y -= other_vec.y;
			z -= other_vec.z;

		return *this;
	}

	bool operator ==(const p2Vec3& other_vec){

		return (x == other_vec.x && y == other_vec.y && z == other_vec.z);
	}


	// Utilities

	p2Vec3& Normalize() {

		float module = sqrt(x*x + y*y + z*z);
		x /= module;
		y /= module;
		z /= module;

		return *this;
	}

	p2Vec3 Normalized_Vec3() {

		float module = sqrt(x*x + y*y + z*z);
		p2Vec3<TYPE> ret = {
			x / module,
			y / module,
			z / module
		};

		return ret;
	}

	p2Vec3& Zero() {

		x = 0;
		y = 0;
		z = 0;

		return *this;
	}

	bool IsZero() {

		return (x == 0 && y == 0 && z == 0);
	}

	// Direct Distances
	float Distance(const p2Vec3& other_vec) {

		Vec3 ret = {
			x - other_vec.x,
			y - other_vec.y,
			z - other_vec.z
		};

		return sqrtf(ret.x*ret.x + ret.y*ret.y + ret.z*ret.z);
	}

	float DistanceNoSqrt(const p2Vec3& other_vec) {

		Vec3 ret = {
			x - other_vec.x,
			y - other_vec.y,
			z - other_vec.z
		};

		return (ret.x*ret.x + ret.y*ret.y + ret.z*ret.z);
	}

	float DistanceManhattan(const p2Vec3& other_vec) {

		return (abs(other_vec.x - x) + abs(other_vec.y - y) + abs(other_vec.z - z));
	}

};


typedef p2Vec3<int> iVec3;
typedef p2Vec3<float> fVec3;