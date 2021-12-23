//
// jmneto 7/21/2009
//
// Asteroids.cpp 
//
// Demonstrates:
//
//		Class inheritance
//      Carray

/*


^
|
|
|	angle
|	VectorLenght
|   posX,posY
|   velX,velY
Y
 X -------------------->

*/

#define UNICODE
#include "stdafx.h"
#include "Asteroids.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// My Global Variables
HWND g_hWnd;									// GLobal Window Handle
BOOL continueInLoop = TRUE;						// Game Main Loop

// Screen Size Aprox 16:9
#define CANVAS_LEFT 0
#define CANVAS_TOP 0
#define CANVAS_WIDTH 900						
#define CANVAS_HEIGHT 500

// Max Object Acceleration
#define MAX_ACCELERATION 5

//Oject Types
#define NOTHING		0

#define SHIP		1
#define DOOMEDSHIP	2

#define BULLET		10

#define ASTEROID0	50	
#define ASTEROID1	51
#define ASTEROID2	52

#define GHOST		9999

// Vector Math
#define M_PI       3.14159265358979323846


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// My Classes 
// Base Space Object
class CSpaceObject : public CObject
{
public:
	CSpaceObject();
	CSpaceObject(int objectType);
	CSpaceObject(int posX, int posY, int objectType);
	CSpaceObject(int posX, int posY, int angle, int objectType);
	~CSpaceObject();
	virtual void		Draw(PPAINTSTRUCT pps);
	virtual void		Move();
	virtual void		FireThrusts();
	bool				CheckColision(CSpaceObject * object);


	// Properties
	void				set_velX(double value);
	double				get_velX();
	void				set_velY(double value);
	double				get_velY();
	void				set_posX(int value);
	int					get_posX();
	void				set_posY(int value);
	int					get_posY();
	void				set_objectType(int value);
	int					get_objectType();
	void				set_angle(int value);
	int					get_angle();
	void				set_vectorlength(double value);
	double				get_vectorlength();


private:
	void				Init();

protected:
	double m_velX;			// Acceleration X
	double m_velY;			// Acceleration Y
	int m_posX;				// Position X
	int m_posY;				// Position Y
	int m_sizeX;			// Object Size for impact detection
	int m_sizeY;			// Object Size for impact detection
	int m_objectType;		// object Type
	int m_angle;			// Object Vector Angle
	double m_vectorlength;	// Vector Lenght (Angular speed)

	__declspec(property(get = get_velX, put = set_velX)) double velX;
	__declspec(property(get = get_velY, put = set_velY)) double velY;
	__declspec(property(get = get_posX, put = set_posX)) int posX;
	__declspec(property(get = get_posY, put = set_posY)) int posY;
	__declspec(property(get = get_objectType, put = set_objectType)) int objectType;
	__declspec(property(get = get_angle, put = set_angle)) int angle;
	__declspec(property(get = get_vectorlength, put = set_vectorlength)) double vectorlength;

};

CSpaceObject::CSpaceObject() 
{
	Init(); 
}

CSpaceObject::CSpaceObject(int objectType) 
{
	Init();
	
	m_objectType = objectType;
}

CSpaceObject::CSpaceObject(int posX, int posY, int objectType) 
{
	Init();
	
	m_objectType = objectType;
	m_posX	= posX;
	m_posY	= posY;
}

CSpaceObject::CSpaceObject(int posX, int posY, int angle, int objectType) 
{
	Init();
	
	m_objectType = objectType;
	m_posX	= posX;
	m_posY	= posY;
	m_angle = angle;
}

CSpaceObject::~CSpaceObject() 
{
}
void CSpaceObject::Init()
{
	m_objectType = NOTHING;

	// Inititalize Acceleration and Position
	m_posX	= (int)(((rand() % (CANVAS_WIDTH - 200))  + 10));				// rnd Range 10 - Canvas Size
	m_posY	= (int)(((rand() % (CANVAS_HEIGHT - 200)) + 10));				// rnd Range 10 - Canvas Size
	
	// Object Angle
	m_angle = (int)(((rand() % 360) + 0)); 									// rnd Range 0 - 360 (positive only)
	
	//Speed Vector
	m_vectorlength = (int)(((rand() % 5) + 2)); 							// rnd Range 2 - 5 (positive only)

	// Initialize default size
	m_sizeX = m_sizeY = 20;
}

void CSpaceObject::set_velX(double value)
{
	m_velX = value;

	// Control Max Acceleration
	if (m_velX >= MAX_ACCELERATION)
		m_velX = MAX_ACCELERATION;
		
	if (m_velX * -1 >= MAX_ACCELERATION)
		m_velX = MAX_ACCELERATION * -1;

}

void CSpaceObject::set_velY(double value)
{
	m_velY = value;

	// Control Max Acceleration
	if (m_velY >= MAX_ACCELERATION)
		m_velY = MAX_ACCELERATION;

	if (m_velY * -1 >= MAX_ACCELERATION)
		m_velY = MAX_ACCELERATION * -1;
		
}

double CSpaceObject::get_velX()
{
	return m_velX;
}

double CSpaceObject::get_velY()
{
	return m_velY;
}

int CSpaceObject::get_posX()
{
	return m_posX;
}
int CSpaceObject::get_posY()
{
	return m_posY;
}

void CSpaceObject::set_posX(int value)
{
	m_posX = value;
}

void CSpaceObject::set_posY(int value)
{
	m_posY = value;
}


void CSpaceObject::set_objectType(int value)
{
	m_objectType = value;
}


int	CSpaceObject::get_objectType()
{
	return m_objectType;
}

void CSpaceObject::set_angle(int value)
{
	// cycle angle around
	if (value > 360) value	-= 360; 
	if (value < 0) value	+= 360;
	m_angle = value;
}


int	CSpaceObject::get_angle()
{
	return m_angle;
}

double	CSpaceObject::get_vectorlength()
{
	return m_vectorlength;
}

void CSpaceObject::set_vectorlength(double value)
{
	m_vectorlength = value;
}

void CSpaceObject::Draw(PPAINTSTRUCT pps)  // Paint Object
{
}

void CSpaceObject::Move()	// Move Object
{

	// Control off bounds behavior
	if (m_posX < CANVAS_LEFT - m_sizeX)		m_posX = CANVAS_WIDTH;
	if (m_posX > CANVAS_WIDTH)				m_posX = CANVAS_LEFT;
	if (m_posY < CANVAS_TOP - m_sizeY)		m_posY= CANVAS_HEIGHT;
	if (m_posY > CANVAS_HEIGHT)				m_posY = CANVAS_TOP;

	
	// Conver Angles to Radians
	double v = (m_angle - 90) * M_PI / 180;  //(-90 fixes object rotation divergenge)

	//Calculate object velocity based on angle (radians v) and vector lenght
	m_velX = m_vectorlength * cos (v);
	m_velY = m_vectorlength * sin (v);

	// Update position based on velocity
	m_posX+=m_velX;
	m_posY+=m_velY;
}

void CSpaceObject::FireThrusts()
{
}


bool CSpaceObject::CheckColision(CSpaceObject * object)
{
	// simple colision check, I plan to make this more acurate
	if (  ( (object->m_posX > m_posX && object->m_posX < m_posX + m_sizeX) && (object->m_posY > m_posY && object->m_posY < m_posY + m_sizeY) )  // Up Left Corner
			||
		  ( (object->m_posX > m_posX && object->m_posX <  m_posX + m_sizeX) && (object->m_posY + object->m_sizeY > m_posY && object->m_posY + object->m_sizeY < m_posY + m_sizeY) ) // Lower Left Corner
		    ||
		  ( (object->m_posX + object->m_sizeX > m_posX && object->m_posX + object->m_sizeX < m_posX + m_sizeX ) && (object->m_posY > m_posY && object->m_posY < m_posY + m_sizeY) ) // Up Right Corner
			||
		  ( (object->m_posX + object->m_sizeX > m_posX && object->m_posX + object->m_sizeX < m_posX + m_sizeX ) && (object->m_posY + object->m_sizeY > m_posY && object->m_posY + object->m_sizeY < m_posY + m_sizeY) ) // Lower Right Corner
		  )
		return TRUE;
	return FALSE;
}

// Asteroid : Specialization of CSpaceObject
class CAsteroid: public CSpaceObject
{
public:
	CAsteroid();
	CAsteroid(int objectType);
	CAsteroid(int posX, int posY, int objectType);
	virtual void Draw(PPAINTSTRUCT pps);

private:
	void Init();
};

CAsteroid::	CAsteroid() : CSpaceObject()
{
	Init();
}

CAsteroid::	CAsteroid(int objectType) : CSpaceObject(objectType) 
{
	Init();
}


CAsteroid::CAsteroid(int posX, int posY, int objectType) : CSpaceObject( posX,  posY,  objectType) 
{
	Init();
}

void CAsteroid::Init()
{
	switch (objectType)
	{
		case ASTEROID0:
			m_sizeX = 20;
			m_sizeY = 20;
			break;
		case ASTEROID1:
			m_sizeX = 50;
			m_sizeY = 50;
			break;
		case ASTEROID2:
			m_sizeX = 80;
			m_sizeY = 80;
			break;
	}
}

void CAsteroid::Draw(PPAINTSTRUCT pps)
{
	POINT asteroid0[8] = {m_posX,m_posY+10, m_posX+5,m_posY+5,  m_posX+15,m_posY,  m_posX+20,m_posY+10, m_posX+15,m_posY+20,  m_posX+10,m_posY+15,  m_posX+5,m_posY+20,  m_posX,m_posY+10};
	POINT asteroid1[11]= {m_posX,m_posY+10, m_posX+10,m_posY+10, m_posX+20,m_posY, m_posX+30,m_posY+20, m_posX+40,m_posY+10, m_posX+50,m_posY+30, m_posX+40,m_posY+50, m_posX+20,m_posY+40, m_posX,m_posY+50, m_posX+10,m_posY+30, m_posX,m_posY+10};
	POINT asteroid2[11]= {m_posX+20,m_posY+30,    m_posX+20,m_posY+10,    m_posX+40,m_posY,    m_posX+70,m_posY+10,    m_posX+80,m_posY+50,    m_posX+70,m_posY+70,    m_posX+50,m_posY+70,    m_posX+30,m_posY+80,    m_posX+20,m_posY+70,    m_posX+10,m_posY+50,   m_posX+20,m_posY+30};

	switch (m_objectType)
	{
		case ASTEROID0:
			Polyline(pps->hdc,asteroid0,8);
			break;
		case ASTEROID1:
			Polyline(pps->hdc,asteroid1,11);
			break;
		case ASTEROID2:
			Polyline(pps->hdc,asteroid2,11);
			break;
	}
}


// Ship : Derived from CSpaceObject
class CShip: public CSpaceObject
{
public:
	CShip(int objectType);
	virtual void Draw(PPAINTSTRUCT pps);
	virtual void Move();
	virtual void FireThrusts();

private:
	void Init();
	int firedThrusts;
};


CShip::CShip(int objectType) : CSpaceObject(objectType)
{
	Init();
}

void CShip::Init()
{
	m_sizeX = 10;
	m_sizeY = 20;
	m_velX = m_velY = 0;
	m_posX = CANVAS_WIDTH / 2;
	m_posY = CANVAS_HEIGHT / 2;
	firedThrusts = 0;
	m_vectorlength = 0;
	m_angle = 0;

}

void CShip::Draw(PPAINTSTRUCT pps)
{
	int numPoints = 4;
	if (firedThrusts-- > 0) numPoints = 10;

	//Figure vectors
	POINT vertices[10] = {-5,10,	
						0,-10,   
						5,10,  
						-5,10, 
						// Engine Fire
						0,10,
						-5,15,
						0,10,
						0,15,
						0,10,
						5,15};

	


	POINT rotatedvertice[10];
	
	// Convert Angles to Radians	
	double v = m_angle * M_PI / 180;
	
	//Rotate figure based on radians v
	double lcos = cos(v);
	double lsin = sin(v);
	for (int i = 0; i<numPoints; i++)
	{
		//Rotate object matrix based on radians v
		rotatedvertice[i].x = vertices[i].x * lcos - vertices[i].y * lsin;   
		rotatedvertice[i].y = vertices[i].x * lsin + vertices[i].y * lcos;

		//Apply rotated matrix to object cartesian position
		rotatedvertice[i].x += m_posX;
		rotatedvertice[i].y += m_posY;
	}

	// Draw figure
	Polyline(pps->hdc,rotatedvertice,numPoints);
}


void CShip::Move()	// Move Object
{
	// Control off bounds behavior
	if (m_posX < CANVAS_LEFT)	m_posX = CANVAS_WIDTH;
	if (m_posX > CANVAS_WIDTH)	m_posX = CANVAS_LEFT;
	if (m_posY < CANVAS_TOP)	m_posY = CANVAS_HEIGHT;
	if (m_posY > CANVAS_HEIGHT) m_posY = CANVAS_TOP;

	// Update position based on acceleration
	m_posX+=m_velX;
	m_posY+=m_velY;
}

void CShip::FireThrusts()
{
	firedThrusts = 3;
	m_vectorlength = 2.5;

	// Convert Angles to Radians	
	double v = (m_angle - 90) * M_PI / 180;  //(-90 fixes divergenge)
	
	//Move figure based on radians v and vector lenght
	m_velX += m_vectorlength * cos (v);
	m_velY += m_vectorlength * sin (v);
}


// Bullet : Derived from CSpaceObject
class CBullet: public CSpaceObject
{
public:
	CBullet(int posX, int posY, int angle, int objectType);
	//~CBullet();
	virtual void Draw(PPAINTSTRUCT pps);
	virtual void Move();

private:
	void Init();
};



CBullet::CBullet(int posX, int posY, int angle, int objectType) : CSpaceObject(posX,posY,angle,objectType)
{
	Init();
}

void CBullet::Init()
{
	m_sizeX = 2;
	m_sizeY = 2;

	m_vectorlength = 15;
	//Move figure based on radians v and vector lenght
	double v = (m_angle - 90) * M_PI / 180;  //(-90 fixes divergenge)
	m_velX = m_vectorlength * cos (v);
	m_velY = m_vectorlength * sin (v);

}

void CBullet::Draw(PPAINTSTRUCT pps)
{
	POINT polygon[8]= {m_posX,m_posY,	
					  m_posX-1,m_posY-1,  
					  m_posX,m_posY,
					  m_posX+1,m_posY+1,
					  m_posX,m_posY,
					  m_posX-1,m_posY+1,
					  m_posX,m_posY,
					  m_posX+1,m_posY-1};


	Polyline(pps->hdc,polygon,8);
}

void CBullet::Move()	// Move 
{
	// Control off bounds behavior
	if (m_posX < CANVAS_LEFT)		m_objectType = GHOST;
	if (m_posX > CANVAS_WIDTH)		m_objectType = GHOST;
	if (m_posY < CANVAS_TOP)		m_objectType = GHOST;
	if (m_posY > CANVAS_HEIGHT)		m_objectType = GHOST;

	// Update position based on acceleration
	m_posX+=m_velX;
	m_posY+=m_velY;
}



// Damaged Ship : Derived from CSpaceObject
class CDoomedShip: public CSpaceObject
{
public:
	CDoomedShip(int posX, int posY,int objectType);
	//~CShip();
	virtual void Draw(PPAINTSTRUCT pps);
	virtual void Move();

private:
	void Init();
};


CDoomedShip::CDoomedShip(int posX, int posY,int objectType) : CSpaceObject(posX,posY,DOOMEDSHIP)
{
	Init();
}

void CDoomedShip::Init()
{
	m_velX = 0;
	m_velY = +5;
}

void CDoomedShip::Draw(PPAINTSTRUCT pps)
{
	POINT triangle[4]= {m_posX,m_posY, m_posX+10,m_posY+20,  m_posX,m_posY+15, m_posX+5,m_posY};
	Polyline(pps->hdc,triangle,4);
}

void CDoomedShip::Move()	// Move 
{
	// Control off bounds behavior
	if (m_posX < CANVAS_LEFT)		m_objectType = GHOST;
	if (m_posX > CANVAS_WIDTH)		m_objectType = GHOST;
	if (m_posY < CANVAS_TOP)		m_objectType = GHOST;
	if (m_posY > CANVAS_HEIGHT)		m_objectType = GHOST;

	// Update position based on acceleration
	m_posX+=m_velX;
	m_posY+=m_velY;
}


// Control the Physics and Object interactions

class CUniverse
{
public:
	CUniverse();
	~CUniverse();
	void AccelerateShip();
	void DecelerateShip();
	void TurnShipLeft();
	void TurnShipRight();
	void ProcessMoves();
	void ProcessDrawings(PPAINTSTRUCT pps);
	void CheckColisions();
	void FireWeapon();

	
protected:
	CTypedPtrArray<CObArray ,CSpaceObject*> m_ptCSpaceObjects;

};

CUniverse::CUniverse()  // Ctor
{
	m_ptCSpaceObjects.Add(new CShip(SHIP));
	m_ptCSpaceObjects.Add(new CAsteroid(ASTEROID2));
	m_ptCSpaceObjects.Add(new CAsteroid(ASTEROID2));
	m_ptCSpaceObjects.Add(new CAsteroid(ASTEROID2));
	m_ptCSpaceObjects.Add(new CAsteroid(ASTEROID2));
	m_ptCSpaceObjects.Add(new CAsteroid(ASTEROID2));

}

CUniverse::~CUniverse()	// Dtor
{
}

void CUniverse::AccelerateShip()
{
	m_ptCSpaceObjects[0]->FireThrusts();
}

void CUniverse::DecelerateShip()
{

}

void CUniverse::TurnShipLeft()
{
	m_ptCSpaceObjects[0]->angle-=10;
}

void CUniverse::TurnShipRight()
{
	m_ptCSpaceObjects[0]->angle+=10;
}


void CUniverse::FireWeapon()
{
	m_ptCSpaceObjects.Add(new CBullet(m_ptCSpaceObjects[0]->posX,m_ptCSpaceObjects[0]->posY,m_ptCSpaceObjects[0]->angle,BULLET));
}

void CUniverse::ProcessDrawings(PPAINTSTRUCT pps)
{
	for (int i =0 ; i < m_ptCSpaceObjects.GetCount() ; i++)
		m_ptCSpaceObjects[i]->Draw(pps);

}

void CUniverse::ProcessMoves()
{
	// Process Moves
	for (int i =0 ; i < m_ptCSpaceObjects.GetCount() ; i++)
	{
		if( GHOST == m_ptCSpaceObjects[i]->objectType) 
			m_ptCSpaceObjects.RemoveAt(i--,1);// remove Ghosts
		if (i<0) i=0; // When ship is removed we go back to 0 not -1
		m_ptCSpaceObjects[i]->Move();
	}

	// Check Colisions
	CheckColisions();
}

void CUniverse::CheckColisions()
{
	BOOL colision ;
	int objCount= 0;
		
	for (int i = 0;  i < m_ptCSpaceObjects.GetCount() ; i++)
	{
	
		// tests bullet impact
		if (BULLET == m_ptCSpaceObjects[i]->objectType) 
		{

			objCount = m_ptCSpaceObjects.GetCount(); // do not consider newly created objs
 			for (int ii = 0;  ii < objCount ; ii++)
			{
				if (BULLET == m_ptCSpaceObjects[ii]->objectType || SHIP == m_ptCSpaceObjects[ii]->objectType || GHOST == m_ptCSpaceObjects[ii]->objectType) continue; // Bullets do not affect bullets or the ship
				
 				colision = m_ptCSpaceObjects[ii]->CheckColision(m_ptCSpaceObjects[i]); // Check inverted

				if (colision)
				{
					if (ASTEROID2 == m_ptCSpaceObjects[ii]->objectType ) // Create 2 new asteroids 1
					{
						m_ptCSpaceObjects.Add(new CAsteroid(m_ptCSpaceObjects[ii]->posX ,m_ptCSpaceObjects[ii]->posY,ASTEROID1));
						m_ptCSpaceObjects.Add(new CAsteroid(m_ptCSpaceObjects[ii]->posX ,m_ptCSpaceObjects[ii]->posY,ASTEROID1));
					}

					if (ASTEROID1 == m_ptCSpaceObjects[ii]->objectType ) // Create 2 new asteroids 0
					{
						m_ptCSpaceObjects.Add(new CAsteroid(m_ptCSpaceObjects[ii]->posX ,m_ptCSpaceObjects[ii]->posY,ASTEROID0));
						m_ptCSpaceObjects.Add(new CAsteroid(m_ptCSpaceObjects[ii]->posX ,m_ptCSpaceObjects[ii]->posY,ASTEROID0));
					}

					m_ptCSpaceObjects[i]->objectType = GHOST;
					m_ptCSpaceObjects[ii]->objectType = GHOST;
				}
			}
		}

		// tests ship impact
		if (SHIP == m_ptCSpaceObjects[i]->objectType) 
		{
 			for (int ii = 0;  ii < m_ptCSpaceObjects.GetCount() ; ii++)
			{
				if (BULLET == m_ptCSpaceObjects[ii]->objectType || SHIP == m_ptCSpaceObjects[ii]->objectType || GHOST == m_ptCSpaceObjects[ii]->objectType) continue; // Bullets do not affect bullets or the ship
				
				colision = m_ptCSpaceObjects[i]->CheckColision(m_ptCSpaceObjects[ii]);
				if (!colision) 
				{
 					colision = m_ptCSpaceObjects[ii]->CheckColision(m_ptCSpaceObjects[i]); // Check inverted
				}
				if (colision)
				{
					m_ptCSpaceObjects.Add(new CDoomedShip(m_ptCSpaceObjects[i]->posX ,m_ptCSpaceObjects[i]->posY,DOOMEDSHIP));
					m_ptCSpaceObjects[i]->objectType = GHOST;
				}
			}
		}

	}
}

CUniverse * pCUniverse = new CUniverse;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASTEROIDS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTEROIDS));

	int control = 0;

	// So we have a constant move/redraw time
	SetTimer(g_hWnd, // handle to main window 
    NULL,            // timer identifier 
    50,              // 50ms-second interval move objects
    (TIMERPROC) NULL);     

	// Main message loop:
	while (continueInLoop)
	{
		if (PeekMessage (&msg, NULL, 0, 0,PM_REMOVE))
		{
				DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTEROIDS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASTEROIDS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   //g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CANVAS_LEFT, CANVAS_TOP, CANVAS_WIDTH, CANVAS_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}




 
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//	WM_TIMER	- processes moves
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		// Process 
		pCUniverse->ProcessDrawings(&ps);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		continueInLoop = FALSE;
		break;

	case WM_TIMER:
		//
		pCUniverse->ProcessMoves();

		// Forces a redraw
		InvalidateRect(g_hWnd, NULL, TRUE);
		break;

	case WM_KEYDOWN: 
        switch (wParam) 
        { 
            case VK_LEFT: 
                
                // Process the LEFT ARROW key. 
				pCUniverse->TurnShipLeft();
               
                break; 

            case VK_RIGHT: 
                
                // Process the RIGHT ARROW key. 
				pCUniverse->TurnShipRight();
                 
                break; 

            case VK_UP: 
                
                // Process the UP ARROW key. 
				pCUniverse->AccelerateShip();
                 
                break; 

            case VK_DOWN: 
                
                // Process the DOWN ARROW key. 
				pCUniverse->DecelerateShip();
                 
                break; 

            case VK_SPACE: 
                
                // Process the VK_SPACE key. 
				pCUniverse->FireWeapon();
                
                break; 

            
            // Process other non-character keystrokes. 
             
            default: 
                break; 
        } 

		// Forces a redraw
		InvalidateRect(hWnd, NULL, TRUE);


	   break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
