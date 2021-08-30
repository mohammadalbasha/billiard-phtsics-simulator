#include "3DTexture.cpp"
#include "Model_3DS.cpp"
#include "texture.cpp"


// skybox
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN;

GLTexture treem1, treem2, team1, team2, library1, library2, kioskm1, kioskm2, kioskm3, kioskm4, kioskm5, kioskm6, sunshade1, sunshade2, bench1, bench2;
Model_3DS tank;
Model_3DS tree;
Model_3DS tea, kiosk, library, sunshade, bench;
// Skybox
int back, front, left, right, top;

int asphalt, terrain, medgr, path;
//TGAImage grass;
int wall, brick, t, envi1, envi2, envi3, envi4, envi5, envi6, medwindow, meddoor, it, itdoor, itsquare, itsquarewall;


void loadTextures() {
	glEnable(GL_TEXTURE_2D);

	SKYFRONT = LoadTexture("data\\front.bmp", 255);
	SKYBACK = LoadTexture("data\\back.bmp", 255);
	SKYLEFT = LoadTexture("data\\left.bmp", 255);
	SKYRIGHT = LoadTexture("data\\right.bmp", 255);
	SKYUP = LoadTexture("data\\up.bmp", 255);
	SKYDOWN = LoadTexture("data\\down.bmp", 255);
	terrain = LoadTexture("data\\terrain.bmp",255);
	path = LoadTexture("data\\path.bmp",255);
	asphalt = LoadTexture("data\\asphalt.bmp",255);
	medgr = LoadTexture("data\\medicalterrain.bmp",255);
	itsquare = LoadTexture("data\\itsquare.bmp",255);
	itsquarewall = LoadTexture("data\\itsquarewall.bmp",255);
	it = LoadTexture("data\\itTEX.bmp",255);
	itdoor = LoadTexture("data\\itdoor.bmp",255);
	medwindow = LoadTexture("data\\medwindow.bmp",255);
	meddoor = LoadTexture("data\\meddoor.bmp",255);
	wall = LoadTexture("data\\walll.bmp", 255);

	// Model textures
	treem2.LoadBMP("data\\woodlog.bmp");
	treem1.LoadBMP("data\\treepaper1.bmp");
	team1.LoadBMP("data\\treewood.bmp");
	kioskm1.LoadBMP("data\\kioskm1.bmp");
	kioskm2.LoadBMP("data\\kioskm2.bmp");
	kioskm3.LoadBMP("data\\kioskm3.bmp");
	kioskm4.LoadBMP("data\\kioskm4.bmp");
	kioskm5.LoadBMP("data\\kioskm5.bmp");
	kioskm6.LoadBMP("data\\kioskm6.bmp");
	library1.LoadBMP("data\\wood.bmp");
	library2.LoadBMP("data\\wd.bmp");
	sunshade1.LoadBMP("data\\wood.bmp");
	sunshade2.LoadBMP("data\\wd.bmp");
	bench1.LoadBMP("data\\wood.bmp");
	bench2.LoadBMP("data\\wd.bmp");
}

void loadModels() {
	tank = Model_3DS();
	tank.Load("data\\tank.3ds");

	tea = Model_3DS();
	tea.Load("data\\tea.3ds");
	tea.Materials[0].tex = team1;

	tree = Model_3DS();
	tree.Load("data\\tree1.3ds");
	tree.Materials[0].tex = treem2;
	tree.Materials[1].tex = treem2;
	tree.Materials[2].tex = treem2;
	tree.Materials[3].tex = treem2;
	tree.Materials[4].tex = treem1;
	tree.Materials[5].tex = treem1;

	library = Model_3DS();
	library.Load("data\\kisoque.3ds");
	library.Materials[0].tex = library1;
	library.Materials[1].tex = library2;
	library.Materials[2].tex = library2;
	library.Materials[3].tex = library2;
	library.Materials[4].tex = library2;

	kiosk = Model_3DS();
	kiosk.Load("data\\Kiosk.3ds");
	kiosk.Materials[0].tex = kioskm1;
	kiosk.Materials[1].tex = kioskm2;
	kiosk.Materials[2].tex = kioskm3;
	kiosk.Materials[3].tex = kioskm4;
	kiosk.Materials[4].tex = kioskm5;
	kiosk.Materials[5].tex = kioskm6;

	sunshade = Model_3DS();
	sunshade.Load("data\\ssunshade.3ds");
	sunshade.Materials[0].tex = sunshade1;

	bench = Model_3DS();
	bench.Load("data\\Bench.3ds");
	bench.Materials[0].tex = bench1;
	bench.Materials[1].tex = bench2;
}