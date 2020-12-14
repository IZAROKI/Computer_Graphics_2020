#include <iostream>
#include <fstream>

float eps = 0.0001;

class ColorClass {
public:
	int red, green, blue;

	ColorClass(int tmpRed, int tmpGreen, int tmpBlue) {
		red = tmpRed;
		green = tmpGreen;
		blue = tmpBlue;
	}

	ColorClass() {
		red = 0;
		green = 0;
		blue = 0;
	}

	ColorClass(const ColorClass& tmpClass) {
		red = tmpClass.red;
		green = tmpClass.green;
		blue = tmpClass.blue;
	}

	const ColorClass operator+(const ColorClass& tmpClass) const {
		ColorClass add;
		add.red = red + tmpClass.red;
		add.green = green + tmpClass.green;
		add.blue = blue + tmpClass.blue;
		return add;
	}

	const ColorClass operator*(float tmpFloat) const {
		ColorClass mul;
		mul.red = red * tmpFloat;
		mul.green = green * tmpFloat;
		mul.blue = blue * tmpFloat;
		return mul;
	}

	~ColorClass() {};
};

class CoordClass {
public:
	float x, y, z;

	CoordClass(float tmpX, float tmpY, float tmpZ) {
		x = tmpX;
		y = tmpY;
		z = tmpZ;
	}

	CoordClass() {
		x = 0;
		y = 0;
		z = 0;
	}

	CoordClass(const CoordClass& tmpClass) {
		x = tmpClass.x;
		y = tmpClass.y;
		z = tmpClass.z;
	}

	void BuildNormal() {
		float norm = sqrt(x * x + y * y + z * z);
		x = x / norm;
		y = y / norm;
		z = z / norm;
	}

	const CoordClass operator+(const CoordClass& tmpClass) const {
		CoordClass add;
		add.x = x + tmpClass.x;
		add.y = y + tmpClass.y;
		add.z = z + tmpClass.z;
		return add;
	}

	const CoordClass operator-(const CoordClass& tmpClass) const {
		CoordClass sub;
		sub.x = x - tmpClass.x;
		sub.y = y - tmpClass.y;
		sub.z = z - tmpClass.z;
		return sub;
	}

	const CoordClass operator*(float tmpFloat) const {
		CoordClass mul;
		mul.x = x * tmpFloat;
		mul.y = y * tmpFloat;
		mul.z = z * tmpFloat;
		return mul;
	}

	~CoordClass() {};
};


float Global_GetScalar(const CoordClass tmpClass1, const CoordClass tmpClass2) {
	return tmpClass1.x * tmpClass2.x + tmpClass1.y * tmpClass2.y + tmpClass1.z * tmpClass2.z;
}

float Global_GetLength(const CoordClass tmpClass) {
	return sqrt(tmpClass.x * tmpClass.x + tmpClass.y * tmpClass.y + tmpClass.z * tmpClass.z);
}

CoordClass Global_GetCross(CoordClass tmpClass1, CoordClass tmpClass2) {
	CoordClass cross;
	cross.x = tmpClass1.y * tmpClass2.z - tmpClass1.z * tmpClass2.y;
	cross.y = tmpClass1.z * tmpClass2.x - tmpClass1.x * tmpClass2.z;
	cross.z = tmpClass1.x * tmpClass2.y - tmpClass1.y * tmpClass2.x;
	return cross;
}


class LightSourceClass {
public:
	CoordClass leftboarder, rightboarder;
	CoordClass* lightcoords;
	int segments = 30;
	float intense, partofintense;

	LightSourceClass(CoordClass tmpLeft, CoordClass tmpRight, float tmpIntens) {
		leftboarder = tmpLeft;
		rightboarder = tmpRight;
		intense = tmpIntens;
		partofintense = intense / ((float)(segments));
		lightcoords = new CoordClass[segments];
		float t = 0.0f;
		for (int i = 0; i < segments; i++) {
			lightcoords[i] = leftboarder + (rightboarder - leftboarder) * t;
			t += 0.033333;
		}
	}

	LightSourceClass(const LightSourceClass& tmpClass) {
		leftboarder = tmpClass.leftboarder;
		rightboarder = tmpClass.rightboarder;
		intense = tmpClass.intense;
		segments = tmpClass.segments;
		partofintense = tmpClass.partofintense;
		lightcoords = new CoordClass[segments];
		for (int i = 0; i < segments; i++) {
			lightcoords[i] = tmpClass.lightcoords[i];
		}
	}

	~LightSourceClass() { delete[] lightcoords; }

	//FUNCTION
	CoordClass GetCoords(int i) {
		return lightcoords[i];
	}

	//FUNCTION
	float GetIntense() {
		return partofintense;
	}
};

class ImageClass {
protected:
	int width, height;
	ColorClass* pixeldata;

public:
	ImageClass(int tmpWidth, int tmpHeight) {
		width = tmpWidth;
		height = tmpHeight;
		int sizeofbuffer = width * height;
		pixeldata = new ColorClass[sizeofbuffer];
		ColorClass white(255, 255, 255);
		for (int i = 0; i < sizeofbuffer; i++) {
			pixeldata[i] = white;
		}
	}

	ImageClass(const ImageClass& tmpClass) {
		width = tmpClass.width;
		height = tmpClass.height;
		int sizeofbuffer = width * height;
		pixeldata = new ColorClass[sizeofbuffer];
		for (int i = 0; i < sizeofbuffer; i++) {
			pixeldata[i] = tmpClass.pixeldata[i];
		}
	}

	~ImageClass() { delete[] pixeldata; }

	//FUNCTION
	int GetWidth() {
		return width;
	}

	//FUNCTION
	int GetHeight() {
		return height;
	}

	//FUNCTION
	void PutPixel(ColorClass color, int pointer) {
		pixeldata[pointer] = color;
	}

	//FUNCTION
	void PutPixel(int red, int green, int blue, int pointer) {
		pixeldata[pointer].red = red;
		pixeldata[pointer].green = green;
		pixeldata[pointer].blue = blue;
	}

	//FUNCTION
	void WriteInFile() {
		std::ofstream Render_File("RENDER.ppm");

		Render_File << "P3" << std::endl;
		Render_File << width << " " << height << std::endl;
		Render_File << "255" << std::endl;

		for (int i = 0; i < width * height; i++) {
			Render_File << pixeldata[i].red << " "
				<< pixeldata[i].green << " "
				<< pixeldata[i].blue << std::endl;
		}

		Render_File.close();
	}
};

class GraphObjectClass {
public:
	ColorClass color;
	int specular = 0;
	float reflective = 0;

	GraphObjectClass() {};

	virtual ~GraphObjectClass() {};

	virtual float CheckIntersection(CoordClass& camPos, CoordClass& ray) = 0;

	virtual CoordClass GetNormal(CoordClass& onSurf) const = 0;

	//FUNCTION
	ColorClass GetColor() {
		return color;
	}
	//FUNCTION
	int GetSpecular() {
		return specular;
	}
	//FUNCTION
	float GetReflective() {
		return reflective;
	}
};

class SphereClass : public GraphObjectClass {
public:
	float radius;
	CoordClass centercoords;

	SphereClass(float tmpRed, int tmpSpecular, ColorClass tmpColor, CoordClass tmpCenterPos, float tmpReflective) {
		radius = tmpRed;
		specular = tmpSpecular;
		color = tmpColor;
		centercoords = tmpCenterPos;
		reflective = tmpReflective;
	}

	SphereClass() {
		radius = 0;
		specular = 0;
		color.red = 0;
		color.green = 0;
		color.blue = 0;
		CoordClass centercoords(0, 0, 0);
		reflective = 0;
	}

	SphereClass(const SphereClass& tmpClass) {
		radius = tmpClass.radius;
		specular = tmpClass.specular;
		color = tmpClass.color;
		centercoords = tmpClass.centercoords;
		reflective = tmpClass.reflective;
	}

	~SphereClass() {};

	//FUNCTION
	float GetRadius() {
		return radius;
	}

	//FUNCTION
	CoordClass GetNormal(CoordClass& onSurf) const {
		return onSurf - centercoords;
	}

	//FUNCTION
	float CheckIntersection(CoordClass& camPos, CoordClass& ray) {
		CoordClass toCentre = camPos - centercoords;

		float a = Global_GetScalar(ray, ray);
		float b = (2 * Global_GetScalar(toCentre, ray));
		float c = Global_GetScalar(toCentre, toCentre) - radius * radius;
		float D = b * b - 4 * a * c;

		if (D < 0) {
			return -1;
		}

		float t = (-b + sqrt(D)) / (2 * a);
		if (t > (-b - sqrt(D)) / (2 * a)) {
			t = (-b - sqrt(D)) / (2 * a);
		}

		return t;
	}

};

class TriangleClass : public GraphObjectClass {
public:
	CoordClass* vertex;

	TriangleClass(CoordClass tmpVertex1, CoordClass tmpVertex2, CoordClass tmpVertex3, int tmpSpecular, ColorClass tmpColor, float tmpReflective) {
		vertex = new CoordClass[3];
		vertex[0] = tmpVertex1;
		vertex[1] = tmpVertex2;
		vertex[2] = tmpVertex3;
		specular = tmpSpecular;
		color = tmpColor;
		reflective = tmpReflective;
	}

	TriangleClass() {
		vertex = new CoordClass[3];
		specular = 0;
		ColorClass color;
		reflective = 0.0f;
	};

	TriangleClass(const TriangleClass& tmpClass) {
		vertex = new CoordClass[3];
		for (int i = 0; i < 3; i++) {
			vertex[i] = tmpClass.vertex[i];
		}
		specular = tmpClass.specular;
		color = tmpClass.color;
		reflective = tmpClass.reflective;
	}

	~TriangleClass() { delete[] vertex; }

	//FUNCTION
	CoordClass GetNormal(CoordClass& onSurf) const {
		CoordClass vectorAB = vertex[0] - vertex[1];
		CoordClass vectorBC = vertex[1] - vertex[2];
		return Global_GetCross(vectorAB, vectorBC);
	}

	//FUNCTION
	float CheckIntersect(CoordClass& camPos, CoordClass& ray) {
		CoordClass e1 = vertex[1] - vertex[0];
		CoordClass e2 = vertex[2] - vertex[0];
		CoordClass x = Global_GetCross(ray, e2);
		float det = Global_GetScalar(e1, x);

		if (det > -eps && det < eps) {
			return -1;
		}

		CoordClass s = camPos - vertex[0];
		float u = (1.0 / det) * Global_GetScalar(s, x);

		if (u < 0 || u > 1) {
			return -1;
		}

		CoordClass y = Global_GetCross(s, e1);
		float v = (1.0 / det) * Global_GetScalar(ray, y);

		if (v < 0 || v > 1 || u + v > 1) {
			return -1;
		}

		return (1.0 / det) * Global_GetScalar(e2, y);
	}
};

class PlaneClass : public GraphObjectClass {
protected:
	CoordClass normal;
	float distance;

public:
	PlaneClass(CoordClass tmpNormal, int tmpSpecular, ColorClass tmpColor, float tmpDistance, float tmpReflective) {
		normal = tmpNormal;
		specular = tmpSpecular;
		color = tmpColor;
		distance = tmpDistance;
		reflective = tmpReflective;
	}

	~PlaneClass() {};

	//FUNCTION
	CoordClass GetNormal(CoordClass& onSurf) const {
		return normal;
	}

	//FUNCTION
	float CheckIntersection(CoordClass& camPos, CoordClass& ray) {
		return -((distance + Global_GetScalar(camPos, normal)) / Global_GetScalar(ray, normal));
	}
};


//GLOBAL_FUNCTION
CoordClass Global_GetReflectRay(CoordClass ray, CoordClass normal) {
	return normal * (2 * Global_GetScalar(normal, ray)) - ray;
}

//GLOBAL_FUNCTION
GraphObjectClass* CheckForIntersect(float& t, GraphObjectClass** array, CoordClass camPos, CoordClass ray, float tMin, float tMax) {
	GraphObjectClass* closestObj = nullptr;
	float tempT;
	float closestT = 1000000;

	for (int i = 0; i < 3; i++) {
		tempT = array[i]->CheckIntersection(camPos, ray);
		if ((tempT - tMin > eps) && (tMax - tempT > eps) && (closestT - tempT > eps)) {
			closestT = tempT;
			closestObj = array[i];
		}
	}

	t = closestT;
	return closestObj;
}

//GLOBAL_FUNCTION
float ComputeLight(LightSourceClass& longLight, GraphObjectClass** array, CoordClass onSurf, CoordClass normal, CoordClass minusRay, GraphObjectClass* closestObj) {
	float n, r, intense = 0;
	float shadowT;
	CoordClass lightDirect, refection;
	CoordClass null;
	GraphObjectClass* shadowObj = nullptr;

	for (int i = 0; i < longLight.segments; i++) {
		lightDirect = longLight.GetCoords(i) - onSurf;
		lightDirect.BuildNormal();

		//shadow
		shadowObj = CheckForIntersect(shadowT, array, onSurf, lightDirect, 0.1, 1000000);
		if (shadowObj != nullptr) {
			continue;
		}

		//diffuse light
		n = Global_GetScalar(normal, lightDirect);
		if (n > eps) {
			intense += longLight.GetIntense() * n / (Global_GetLength(normal) * Global_GetLength(lightDirect));
		}

		//sparkling
		if ((closestObj->GetSpecular() != 0)) {
			CoordClass reflection = Global_GetReflectRay(lightDirect, normal);
			reflection.BuildNormal();
			r = Global_GetScalar(reflection, minusRay);
			if (r > eps) {
				intense += longLight.GetIntense() * pow((r / (Global_GetLength(reflection) * Global_GetLength(minusRay))), closestObj->GetSpecular());
			}
		}
	}

	if (intense > 0.8) {
		intense = 0.8;
	}

	return intense;
}

//GLOBAL_FUNCTION
ColorClass ColorToPut(float ambientLight, CoordClass& camPos, CoordClass& ray, GraphObjectClass** array, LightSourceClass& longLight, float depth) {
	float closestT;
	float i;
	ColorClass black;
	ColorClass tempColor, reflectedColor;
	CoordClass refRay;
	GraphObjectClass* closestObj;
	CoordClass null;

	closestObj = CheckForIntersect(closestT, array, camPos, ray, 0.0, 1000000);

	if (closestObj == nullptr) {
		return black;
	}

	CoordClass onSurf = camPos + ray * closestT;
	CoordClass normal = closestObj->GetNormal(onSurf);
	normal.BuildNormal();

	tempColor = closestObj->GetColor() * (ComputeLight(longLight, array, onSurf, normal, null - ray, closestObj) + ambientLight);

	if ((depth <= 0) || (closestObj->GetReflective() < 0)) {
		return tempColor;
	}

	refRay = Global_GetReflectRay(null - ray, normal);

	reflectedColor = ColorToPut(ambientLight, onSurf, refRay, array, longLight, depth - (float)(1));

	return tempColor * (1 - closestObj->GetReflective()) + reflectedColor * (closestObj->GetReflective());
}

//GLOBAL_FUNCTION
void SimpleRender(ImageClass& image, GraphObjectClass** array, LightSourceClass& longLight) {
	int pointer = 0;
	float anti = 0.5f;
	float ambientLight = 0.2f;
	ColorClass totalColor, tempColor;
	float r, g, b;
	CoordClass camPos;
	float depth = 5.0f;

	float distance = (float)(image.GetWidth() + image.GetHeight()) / 2;

	for (int y = -(image.GetHeight() / 2); y < image.GetHeight() / 2; y++) {
		for (int x = -(image.GetWidth() / 2); x < image.GetWidth() / 2; x++) {

			r = 0;
			g = 0;
			b = 0;

			for (float newY = (float)(y); newY < (float)(y + 1); newY = newY + anti) {
				for (float newX = (float)(x); newX < (float)(x + 1); newX = newX + anti) {

					CoordClass newRay(newX, -newY, distance);
					newRay.BuildNormal();
					tempColor = ColorToPut(ambientLight, camPos, newRay, array, longLight, depth);
					r += tempColor.red * anti * anti;
					g += tempColor.green * anti * anti;
					b += tempColor.blue * anti * anti;
				}
			}

			totalColor.red = (int)(r);
			totalColor.green = (int)(g);
			totalColor.blue = (int)(b);

			image.PutPixel(totalColor, pointer);

			pointer++;
		}
	}
}

int main()
{
	ImageClass image(1500, 1000);

	ColorClass color1(175, 240, 240);
	ColorClass color3(102, 205, 170);
	ColorClass color2(0, 0, 128);
	//ColorClass teal(0, 128, 128);

	CoordClass center1((float)(-400), (float)(200), (float)(2500));
	CoordClass center2((float)(400), (float)(200), (float)(2500));
	CoordClass normal(0, 1.0f, 0);
	CoordClass light1((float)(-300), (float)(500), (float)(1600));
	CoordClass light2(300.0, 500.0, 1600.0);

	SphereClass blueSphere((float)(300), 1000, color1, center1, (float)(1.0));
	SphereClass purpleSphere((float)(300), 1000, color2, center2, (float)(0.5));
	PlaneClass whitePlane(normal, 1000, color3, (float)(400), (float)(-1.0));


	GraphObjectClass** array = new GraphObjectClass * [3];
	array[0] = new SphereClass(blueSphere);
	array[1] = new SphereClass(purpleSphere);
	array[2] = new PlaneClass(whitePlane);

	LightSourceClass longLight(light1, light2, 0.5f);

	std::cout << "rendering...\n";

	SimpleRender(image, array, longLight);

	std::cout << "puting in file...\n";

	image.WriteInFile();

	std::cout << "done!\n";

	return 0;
}
