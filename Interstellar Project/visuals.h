#define RED 1
#define WHITE 2
#define YELLOW 3

struct vertice 
{
	float x; 
	float y; 
	float z; 
};
struct vernorm{
	float x;
	float y;
	float z;
};

struct face
{
	int a;
	int b;
	int c;
	int na;
	int nb;
	int nc;
};

struct model
{
	vertice *obj_vertices;
	vernorm *obj_vernorms;
	face *obj_faces;
	int vertices;
	int vernorms;
	int faces;
};


//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void ReadFile(model*);
//Function for reading a model file

void DisplayModel(model);
// Function for displaying a model

void Keyboard(unsigned char key,int x,int y);
// Function for handling keyboard events.

void rKeyboard(unsigned char key, int x, int y);
// Function for handling released-key keyboard events.

void Mouse(int button,int state,int x,int y); 
// Function for handling mouse events

void MenuSelect(int choice);
