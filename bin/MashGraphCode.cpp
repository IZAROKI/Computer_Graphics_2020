#include <iostream>
#include <fstream>

using namespace std;

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

CoordClass Global_GetReflectRay(CoordClass raydircoords, CoordClass normalcoords) {
	return normalcoords * (2 * Global_GetScalar(normalcoords, raydircoords)) - raydircoords;
}

float RandFloat() {
	return rand() / (float)(RAND_MAX);
}

CoordClass RandomCoords() {
	CoordClass temp(RandFloat(), RandFloat(), RandFloat());
	temp.BuildNormal();
	return temp;
}

class LightSourceClass {
public:
	CoordClass leftcoords, rightcoords;
	CoordClass* lightcoords;
	int segments = 30;
	float intense, partofintense;

	LightSourceClass(CoordClass tmpLeftcoords, CoordClass tmpRightcoords, float tmpIntens) {
		leftcoords = tmpLeftcoords;
		rightcoords = tmpRightcoords;
		intense = tmpIntens;
		partofintense = intense / ((float)(segments));
		lightcoords = new CoordClass[segments];
		float t = 0.0f;
		for (int i = 0; i < segments; i++) {
			lightcoords[i] = leftcoords + (rightcoords - leftcoords) * t;
			t += 0.0333f; //при умножении на segments должна выйти 1
		}
	}

	LightSourceClass(const LightSourceClass& tmpClass) {
		leftcoords = tmpClass.leftcoords;
		rightcoords = tmpClass.rightcoords;
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
		ofstream Render_File("RENDER.ppm");

		Render_File << "P3" << endl;
		Render_File << width << " " << height << endl;
		Render_File << "255" << endl;

		for (int i = 0; i < width * height; i++) {
			Render_File << pixeldata[i].red << " " << pixeldata[i].green << " " << pixeldata[i].blue << endl;
		}

		Render_File.close();
	}
};

class GraphObjectClass {
public:
	ColorClass color;
	int specular = 0;
	float reflective = 0;
	float gloss = 0;

	GraphObjectClass() {};

	virtual ~GraphObjectClass() {};

	virtual float GetIntersection(CoordClass& raystartcoords, CoordClass& raydircoords) = 0;

	virtual CoordClass GetNormal(CoordClass& intersectioncoords) const = 0;

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
	//FUNCTION
	float GetGloss() {
		return gloss;
	}
};

class SphereClass : public GraphObjectClass {
protected:
	float radius;
	CoordClass centercoords;

public:
	SphereClass(ColorClass tmpColor, CoordClass tmpCentercoords, float tmpRadius, int tmpSpecular, float tmpReflective, float tmpGloss) {
		color = tmpColor;
		centercoords = tmpCentercoords;
		radius = tmpRadius;
		specular = tmpSpecular;
		reflective = tmpReflective;
		gloss = tmpGloss;
	}

	SphereClass() {
		color.red = 0;
		color.green = 0;
		color.blue = 0;
		radius = 0;
		specular = 0;
		CoordClass centercoords(0, 0, 0);
		reflective = 0;
		gloss = 0;
	}

	SphereClass(const SphereClass& tmpClass) {
		color = tmpClass.color;
		centercoords = tmpClass.centercoords;
		radius = tmpClass.radius;
		specular = tmpClass.specular;
		reflective = tmpClass.reflective;
		gloss = tmpClass.gloss;
	}

	~SphereClass() {};

	//FUNCTION
	float GetRadius() {
		return radius;
	}

	//FUNCTION
	CoordClass GetNormal(CoordClass& intersectioncoords) const {
		return intersectioncoords - centercoords;
	}

	//FUNCTION
	float GetIntersection(CoordClass& raystartcoords, CoordClass& raydircoords) {
		CoordClass centerdircoords = raystartcoords - centercoords;
		float a = Global_GetScalar(raydircoords, raydircoords);
		float b = (2 * Global_GetScalar(centerdircoords, raydircoords));
		float c = Global_GetScalar(centerdircoords, centerdircoords) - radius * radius;
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

	TriangleClass(ColorClass tmpColor, CoordClass tmpVertex1, CoordClass tmpVertex2, CoordClass tmpVertex3, int tmpSpecular, float tmpReflective, float tmpGloss) {
		color = tmpColor;
		vertex = new CoordClass[3];
		vertex[0] = tmpVertex1;
		vertex[1] = tmpVertex2;
		vertex[2] = tmpVertex3;
		specular = tmpSpecular;
		reflective = tmpReflective;
		gloss = tmpGloss;
	}

	TriangleClass() {
		ColorClass color;
		vertex = new CoordClass[3];
		specular = 0;
		reflective = 0;
		gloss = 0;
	};

	TriangleClass(const TriangleClass& tmpClass) {
		color = tmpClass.color;
		vertex = new CoordClass[3];
		for (int i = 0; i < 3; i++) {
			vertex[i] = tmpClass.vertex[i];
		}
		specular = tmpClass.specular;
		reflective = tmpClass.reflective;
		gloss = tmpClass.gloss;
	}

	~TriangleClass() { delete[] vertex; }

	//FUNCTION
	CoordClass GetNormal(CoordClass& intersectioncoords) const {
		CoordClass vectorAB = vertex[0] - vertex[1];
		CoordClass vectorBC = vertex[1] - vertex[2];
		return Global_GetCross(vectorAB, vectorBC);
	}

	//FUNCTION
	float GetIntersection(CoordClass& raystartcoords, CoordClass& raydircoords) {
		CoordClass e1 = vertex[1] - vertex[0];
		CoordClass e2 = vertex[2] - vertex[0];
		CoordClass x = Global_GetCross(raydircoords, e2);
		float det = Global_GetScalar(e1, x);
		float eps = 0.0001f;

		if (det > -eps && det < eps) {
			return -1;
		}

		CoordClass s = raystartcoords - vertex[0];
		float u = (1.0f / det) * Global_GetScalar(s, x);

		if (u < 0 || u > 1) {
			return -1;
		}

		CoordClass y = Global_GetCross(s, e1);
		float v = (1.0f / det) * Global_GetScalar(raydircoords, y);

		if (v < 0 || v > 1 || u + v > 1) {
			return -1;
		}

		return (1.0f / det) * Global_GetScalar(e2, y);
	}
};

class PlaneClass : public GraphObjectClass {
protected:
	float distance;
	CoordClass normalcoords;

public:
	PlaneClass(ColorClass tmpColor, CoordClass tmpNormalcoords, float tmpDistance, int tmpSpecular, float tmpReflective, float tmpGloss) {
		color = tmpColor;
		normalcoords = tmpNormalcoords;
		distance = tmpDistance;
		specular = tmpSpecular;
		reflective = tmpReflective;
		gloss = tmpGloss;
	}

	~PlaneClass() {};

	//FUNCTION
	CoordClass GetNormal(CoordClass& intersectioncoords) const {
		return normalcoords;
	}

	//FUNCTION
	float GetIntersection(CoordClass& raystartcoords, CoordClass& raydircoords) {
		return -((distance + Global_GetScalar(raystartcoords, normalcoords)) / Global_GetScalar(raydircoords, normalcoords));
	}
};


//GLOBAL_FUNCTION
GraphObjectClass* CheckIntersection(float& t, GraphObjectClass** array, CoordClass raystartcoords, CoordClass raydircoords, float tMin, float tMax, int NumberOfGraphObjects) {
	GraphObjectClass* nearestgraphobject = nullptr;
	float tTmp;
	float tNearest = 1000000;
	float eps = 0.0001f;

	for (int i = 0; i < NumberOfGraphObjects; i++) {
		tTmp = array[i]->GetIntersection(raystartcoords, raydircoords);
		if ((tTmp - tMin > eps) && (tMax - tTmp > eps) && (tNearest - tTmp > eps)) {
			tNearest = tTmp;
			nearestgraphobject = array[i];
		}
	}

	t = tNearest;
	return nearestgraphobject;
}

//GLOBAL_FUNCTION
float RenderLight(LightSourceClass& longlightsource, GraphObjectClass** array, CoordClass intersectioncoords, CoordClass normalcoords, CoordClass reverceraycoords, GraphObjectClass* nearestgraphobject, int NumberOfGraphObjects) {
	float n, r, tShadow;
	float intense = 0;
	float eps = 0.0001f;
	CoordClass lightsourcedircoords, reflection, null;
	GraphObjectClass* ShadowGraphObject = nullptr;

	for (int i = 0; i < longlightsource.segments; i++) {
		lightsourcedircoords = longlightsource.GetCoords(i) - intersectioncoords;
		lightsourcedircoords.BuildNormal();


		ShadowGraphObject = CheckIntersection(tShadow, array, intersectioncoords, lightsourcedircoords, 0.1f, 1000000.f, NumberOfGraphObjects);
		if (ShadowGraphObject != nullptr) {
			continue;
		}


		n = Global_GetScalar(normalcoords, lightsourcedircoords);
		if (n > eps) {
			intense += longlightsource.GetIntense() * n / (Global_GetLength(normalcoords) * Global_GetLength(lightsourcedircoords));
		}


		if ((nearestgraphobject->GetSpecular() != 0)) {
			CoordClass reflection = Global_GetReflectRay(lightsourcedircoords, normalcoords);
			reflection.BuildNormal();
			r = Global_GetScalar(reflection, reverceraycoords);
			if (r > eps) {
				intense += longlightsource.GetIntense() * pow((r / (Global_GetLength(reflection) * Global_GetLength(reverceraycoords))), nearestgraphobject->GetSpecular());
			}
		}
	}

	if (intense > 0.8f) { intense = 0.8f; }

	return intense;
}

//GLOBAL_FUNCTION
ColorClass RenderColor(float ambientlight, CoordClass& raystartcoords, CoordClass& raydircoords, GraphObjectClass** array, LightSourceClass& longlightsource, float depth, int NumberOfGraphObjects) {
	float tNearest;
	int rayspack = 3;
	ColorClass black, tmpcolor, reflectedcolor, totalreflectedcolor, totalcolor;
	CoordClass reflectedraycoords, null;
	GraphObjectClass* nearestgraphobject;

	nearestgraphobject = CheckIntersection(tNearest, array, raystartcoords, raydircoords, 0.0f, 1000000.f, NumberOfGraphObjects);

	if (nearestgraphobject == nullptr) {
		return black;
	}

	CoordClass intersectioncoords = raystartcoords + raydircoords * tNearest;
	CoordClass normalcoords = nearestgraphobject->GetNormal(intersectioncoords);
	normalcoords.BuildNormal();
	tmpcolor = nearestgraphobject->GetColor() * (RenderLight(longlightsource, array, intersectioncoords, normalcoords, null - raydircoords, nearestgraphobject, NumberOfGraphObjects) + ambientlight);

	if ((depth <= 0) || (nearestgraphobject->GetReflective() < 0)) {
		return tmpcolor;
	}

	if (nearestgraphobject->GetReflective() != 0.0f) {
		for (int i = 0; i < rayspack; i++) {
			reflectedraycoords = Global_GetReflectRay(null - raydircoords, normalcoords) + RandomCoords() * nearestgraphobject->GetGloss();
			reflectedcolor = RenderColor(ambientlight, intersectioncoords, reflectedraycoords, array, longlightsource, depth - 1.0f, NumberOfGraphObjects);
			totalreflectedcolor.red += (int)(reflectedcolor.red * 1 / ((float)(rayspack)));
			totalreflectedcolor.green += (int)(reflectedcolor.green * 1 / ((float)(rayspack)));
			totalreflectedcolor.blue += (int)(reflectedcolor.blue * 1 / ((float)(rayspack)));

		}
	}

	totalcolor = tmpcolor * (1 - nearestgraphobject->GetReflective()) + totalreflectedcolor * (nearestgraphobject->GetReflective());

	return totalcolor;
}

//GLOBAL_FUNCTION
void RenderSceneFunction(ImageClass& image, GraphObjectClass** array, LightSourceClass& longlightsource, int NumberOfGraphObjects) {
	int pointer = 0;
	float tmpred, tmpgreen, tmpblue;
	float antialiasing = 0.25f;
	float ambientlight = 0.2f; //минимальное освещение сцены (по умолчанию)
	float depth = 3.0f; //глубина (количество) отражений
	ColorClass tmpcolor, totalcolor;
	CoordClass raystartcoords;

	float distance = (float)(image.GetWidth() + image.GetHeight()) / 2;

	for (int y = -(image.GetHeight() / 2); y < image.GetHeight() / 2; y++) {
		for (int x = -(image.GetWidth() / 2); x < image.GetWidth() / 2; x++) {

			tmpred = 0;
			tmpgreen = 0;
			tmpblue = 0;

			for (float newY = (float)(y); newY < (float)(y + 1); newY = newY + antialiasing) {
				for (float newX = (float)(x); newX < (float)(x + 1); newX = newX + antialiasing) {

					CoordClass newraycoords(newX, -newY, distance);
					newraycoords.BuildNormal();
					tmpcolor = RenderColor(ambientlight, raystartcoords, newraycoords, array, longlightsource, depth, NumberOfGraphObjects);
					tmpred += tmpcolor.red * antialiasing * antialiasing;
					tmpgreen += tmpcolor.green * antialiasing * antialiasing;
					tmpblue += tmpcolor.blue * antialiasing * antialiasing;
				}
			}

			totalcolor.red = (int)(tmpred);
			totalcolor.green = (int)(tmpgreen);
			totalcolor.blue = (int)(tmpblue);

			image.PutPixel(totalcolor, pointer);

			pointer++;
		}
	}
}


int main()
{
	int NumberOfGraphObjects = 8;
	ImageClass Image(1500, 1000);
	CoordClass normalcoords(0.0f, 1.0f, 0.0f);
	ColorClass planecolor(255, 255, 238); //Seashell color
	PlaneClass plane(planecolor, normalcoords, 400.f, 1000, -1.0f, 0.f);

	CoordClass light1(-200.f, 500.f, 1300.f);
	CoordClass light2(-100.f, 500.f, 1300.f);
	LightSourceClass longlight(light1, light2, 0.8f);

	ColorClass color1(127, 238, 238); //Aquamarine color
	ColorClass color2(220, 20, 60); //Crimson color
	ColorClass color3(0, 191, 255); //DeepSkyBlue color
	ColorClass color4(255, 215, 0); //Cold color
	ColorClass color5(238, 130, 238); //Violet color
	ColorClass color6(0, 255, 0); //Lime color
	ColorClass color7(172, 255, 47); //GreenYellow color

	CoordClass center1(-200.f, -150.f, 2200.f);
	CoordClass center2(100.f, 100.f, 1625.f);
	CoordClass center3(300.f, 200.f, 1600.f);
	CoordClass center4(700.f, 200.f, 2200.f);
	CoordClass center5(400.f, 100.f, 1800.f);
	CoordClass center6(-700.f, -100.f, 2500.f);
	CoordClass center7(400.f, -300.f, 1800.f);

	//Color, Center, Radius, specular, reflective, gloss
	SphereClass sphere1(color1, center1, 250.f, 1000, 0.9f, 0.1f);
	SphereClass sphere2(color2, center2, 70.f, 800, 0.5f, 0.0f);
	SphereClass sphere3(color3, center3, 150.f, 1000, 0.0f, 0.0f);
	SphereClass sphere4(color4, center4, 200.f, 500, 0.0f, 0.1f);
	SphereClass sphere5(color5, center5, 225.f, 75, 0.3f, 0.0f);
	SphereClass sphere6(color6, center6, 300.f, 150, 1.0f, 0.0f);
	SphereClass sphere7(color7, center7, 100.f, 300, 0.0f, 0.1f);

	GraphObjectClass** array = new GraphObjectClass * [NumberOfGraphObjects];
	array[0] = new PlaneClass(plane);
	array[1] = new SphereClass(sphere1);
	array[2] = new SphereClass(sphere2);
	array[3] = new SphereClass(sphere3);
	array[4] = new SphereClass(sphere4);
	array[5] = new SphereClass(sphere5);
	array[6] = new SphereClass(sphere6);
	array[7] = new SphereClass(sphere7);


	cout << "RENDERING..." << endl;
	RenderSceneFunction(Image, array, longlight, NumberOfGraphObjects);
	cout << "WRITING IN FILE..." << endl;
	Image.WriteInFile();
	cout << "DONE!!!" << endl;

	return 0;
}
