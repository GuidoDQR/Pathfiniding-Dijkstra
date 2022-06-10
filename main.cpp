#include "raylib.h"
#include <math.h>
#include <iostream>
#include <queue>
#include <assert.h>    // For error-checking purposes

using namespace std;

// La multiplicacion entre estos 2 no debe ser mayor a 500 
const int tilesX = 16;
const int tilesY = 15;

class tile{
private:
	int id;
	float posX = 0, posY = 0;
	float width = 32, height = 32;
	float totalWidth = posX + width, totalHeight = posY + height;

	Rectangle shape;
	Color colorShape = RAYWHITE;
	Color colorLine = GRAY;

	int movement = 1; // Si tiene 0 es un objeto solido o donde no se puede mover

	bool Press = false;
    bool pressDown = false;
public:
	tile(){ shape = {posX, posY,  width, height}; }

	void setId(int newId){ id = newId; }
	int getId(){ return id; }

	void setPosition(int newX, int newY ){ posX = newX; posY = newY; shape = {posX, posY, width, height}; totalWidth = posX + width; totalHeight = posY + height;}
	void setPosition32(int newX, int newY ){ posX = newX * 32; posY = newY * 32; shape = {posX, posY, width, height}; totalWidth = posX + width; totalHeight = posY + height;}

	void setColorShape(Color newColor){ colorShape = newColor; }
	Color getColorShape(){ return colorShape; }

	void setColorLine(Color newColor){ colorLine = newColor; }

	void Draw(){ DrawRectangleRec(shape, colorShape); DrawRectangleLinesEx(shape, 1, colorLine);}

	void setMovement(int newMove){ movement = newMove; }
	int getMovement(){ return movement; } // Sirve para saber si se puede pasar o cuanto cuesta

	bool MouseOver(){
	    if(GetMouseX() >= shape.x && GetMouseX() <= totalWidth && GetMouseY() >= shape.y && GetMouseY() <= totalHeight){
	        return true;
	    }
	    return false;
	}

	bool isPressed(int button){
		Press = false;

	    if(MouseOver() == true && IsMouseButtonPressed(button)){     
	        Press = true; 
	    }
	    	      
    	return Press;
	}

	bool isPressedDown(int button){
		pressDown = false;
    	if((MouseOver() == true && IsMouseButtonDown(button)) ){     
	        pressDown = true; 
	    }
	    
	    return pressDown;
	}

	bool released(int button){
    if(IsMouseButtonPressed(button) == false && IsMouseButtonDown(button) == false){
        return true;
    }
    return false;
	}

	bool getStatePressDown(){ return pressDown; }


  	queue <int> adyacentID(){
  		queue <int> adyacentTiles;
        
        // Ingresar id del lado arriba
  		// Si no esta en la primera fila 
  		if(id >= tilesX){
  			adyacentTiles.push(id-tilesX);
  		}
  		
  		// Ingresar id del lado izquierda
  		// Si no esta en la primera columna, lado izquierdo
  		if(id % tilesX != 0){
  			adyacentTiles.push(id-1);
  		}
  		
  		// Ingresar id del lado abajo
  		// Si no esta en la ultima fila 
  		if(id < (tilesX*tilesY)-tilesY ){
  			adyacentTiles.push(id+tilesX);
  		}

  		// Ingresar id del lado derecho
  		// Si no esta en la ultima columna, lado derecho 
  		if((id+1) % tilesX != 0 ){
  			adyacentTiles.push(id+1);
  		}
  		
  		
  		
  		return adyacentTiles;
  	}
  		
};

template <class T> 

class pathPriority {
protected:
	T **Data;  // The actual Data array
	const int MAX_NUM;   // The actual spaces will be one more than this
    int       Beginning, // Numbered location of the start and end
              End;

    // Instead of calculating the number of elements, using this variable
    // is much more convenient.
    int       ElemCount;          	

public:
    
    pathPriority(int MaxSize = 500);
    pathPriority(const pathPriority<T> &OtherPath);
	~pathPriority(void);

    void push(const T &newid, const T &cost);
    T pop(void); // Returns Item from Queue
    inline int size(void);  // Returns Number of Elements
   
};

template <class T>
pathPriority<T>::pathPriority(int MaxSize): 
	MAX_NUM( MaxSize )
{
    // This extra space added will allow us to distinguish between
    // the Beginning and the End locations.
    Data      = new T*[MAX_NUM + 1];

    for(int i=0; i < MAX_NUM+1; i++){
   		Data[i] = new T[2];
    }
    
    Beginning = 0;
    End       = 0;
    ElemCount = 0;//cout << "pase" << endl; //cout << "data[50][0]: "<< Data[End][0] << endl;
}

template <class T>
pathPriority<T>::pathPriority(const pathPriority<T> &OtherPath):
	MAX_NUM( OtherPath.MAX_NUM )
{
	Beginning = OtherPath.Beginning;
    End       = OtherPath.End;
    ElemCount = OtherPath.ElemCount;
 
    Data      = new T*[MAX_NUM + 1];

    for(int i=0; i< MAX_NUM+1; i++){
    	Data[i] = new T[2];
    }

    for (int i = 0; i < MAX_NUM; i++){
    	//cout << "pase 1"<< endl;
    	Data[i][0] = OtherPath.Data[i][0];
    	Data[i][1] = OtherPath.Data[i][1];
    }
    
} 


template <class T>
pathPriority<T>::~pathPriority(void){
	for(int i=0; i < MAX_NUM ;i++){
		delete[] Data[i];
	}
	
	delete[] Data;
}

// Push() function
template <class T>
void pathPriority<T>::push(const T &newid, const T &cost){
	// Error Check: Make sure we aren't exceeding our maximum storage space
    assert( ElemCount < MAX_NUM );
    
    Data[ End   ][0] = newid; // El esta el id del tile
    Data[ End++ ][1] = cost;  // el esta el costo

    ++ElemCount;
       
    // Check for wrap-around
    if (End > MAX_NUM)
      End -= (MAX_NUM + 1);

    for(int i = End-1; i > Beginning; i--){
    	if(Data[i][1] < Data[i-1][1] && i != 0){ // Si el costo de ahora es menor al siguiente Y no es la posicion 0
    		int auxId = Data[i-1][0];
   			int auxCost = Data[i-1][1];

   			Data[i-1][0] = Data[i][0];  
   			Data[i-1][1] = Data[i][1]; 

   			Data[i][0] = auxId;   
   			Data[i][1] = auxCost;    			 
    	}else{
   			break;
   		}
    }
}

// Pop() function
template <class T>
T pathPriority<T>::pop(void){
	// Error Check: Make sure we aren't dequeueing from an empty queue
    assert( ElemCount > 0 );
 
    T ReturnValue = Data[ Beginning++ ][0]; // Devuelve el id del tile
    --ElemCount;
 
    // Check for wrap-around
    if (Beginning > MAX_NUM)
      Beginning -= (MAX_NUM + 1);
 
    return ReturnValue;
}

// Size() function
template <class T>
inline int pathPriority<T>::size(void)
{
  return ElemCount;
}


Vector2 Coordinates(int);
bool MapMouseOver(tile (&mapa)[tilesY][tilesX],int,int);
bool MapPressedBoton(tile (&mapa)[tilesY][tilesX],int,int,int);
bool MapPressedBotonDown(tile (&mapa)[tilesY][tilesX],int,int,int);

int main(int argc, char const *argv[])
{
	const int screenWidth = tilesX*32; 
    const int screenHeight = tilesY*32; 

    InitWindow(screenWidth, screenHeight, "Juego de Memoria");

    SetTargetFPS(60);      

    tile mapa[tilesY][tilesX];

    int id = 0;
    int move =1;
    bool pathFound = false;
    int k = 0;
    
    for(int i = 0; i < tilesY; i++){
    	for(int j=0; j < tilesX; j++){
    		mapa[i][j].setPosition32(j,i);
    		mapa[i][j].setId(k);
    		k++;
    	}
    }

    int initialPosX = 8;
    int initialPosY = 2;
    int initialID;
    bool dragInitial = false;

    mapa[initialPosX][initialPosY].setColorShape(RED);
    mapa[initialPosX][initialPosY].setMovement(0);

    int targetPosX = 6;
    int targetPosY = 13;
    int targetID;
    bool dragTarget = false;

    mapa[targetPosX][targetPosY].setColorShape(GREEN);
    mapa[targetPosX][targetPosY].setMovement(0);
    
    pathPriority <int> frontier; // Se ingresa los id por donde puede pasar 

    int came_from[tilesX*tilesY] = {-1}; // Guarda el ID padre o el ID de donde proviene
    int cost_so_far[tilesX*tilesY] = {0};
    queue <int> path;

    queue <int> adyacent; // Obtiene los 4 tiles adyacentes del tile del momento
    int current;
    int next;
    int newCost;
    int distance = 1;

    Vector2 tileAdyacent;
    Vector2 nextTile;
    Color nextTileColor[2] = {RAYWHITE, RAYWHITE}; 
    int nextTileMove[2] = {1,1}; 
    // MOVIMIENTOS DE COSTO MAYOR
    							mapa[2][6].setMovement(10);  mapa[2][7].setMovement(10);  mapa[2][8].setMovement(10);  mapa[2][9].setMovement(10);
    						    mapa[3][6].setMovement(10);  mapa[3][7].setMovement(10);  mapa[3][8].setMovement(10);  mapa[3][9].setMovement(10);
    mapa[4][5].setMovement(10);  mapa[4][6].setMovement(10);  mapa[4][7].setMovement(10);  mapa[4][8].setMovement(10);  mapa[4][9].setMovement(10);  mapa[4][10].setMovement(10);
    mapa[5][5].setMovement(10);  mapa[5][6].setMovement(10);  mapa[5][7].setMovement(10);  mapa[5][8].setMovement(10);  mapa[5][9].setMovement(10);  mapa[5][10].setMovement(10);
    mapa[6][5].setMovement(10);  mapa[6][6].setMovement(10);  mapa[6][7].setMovement(10);  mapa[6][8].setMovement(10);								mapa[6][10].setMovement(10);
    mapa[7][5].setMovement(10);  mapa[7][6].setMovement(10);  							mapa[7][8].setMovement(10);  mapa[7][9].setMovement(10);  mapa[7][10].setMovement(10);
    mapa[8][5].setMovement(10);  mapa[8][6].setMovement(10);  mapa[8][7].setMovement(10);  mapa[8][8].setMovement(10);                              mapa[8][10].setMovement(10);
    mapa[9][5].setMovement(10);  mapa[9][6].setMovement(10);                              mapa[9][8].setMovement(10);  mapa[9][9].setMovement(10);  mapa[9][10].setMovement(10);
    mapa[10][5].setMovement(10); mapa[10][6].setMovement(10); mapa[10][7].setMovement(10); mapa[10][8].setMovement(10); mapa[10][9].setMovement(10); mapa[10][10].setMovement(10);
    							mapa[11][6].setMovement(10); mapa[11][7].setMovement(10); mapa[11][8].setMovement(10); mapa[11][9].setMovement(10);
    							mapa[12][6].setMovement(10); mapa[12][7].setMovement(10); mapa[12][8].setMovement(10); mapa[12][9].setMovement(10);
    
	while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        pathFound = false; // Sirve para mostrar el camino, en caso de que se pueda

    	initialID = mapa[initialPosX][initialPosY].getId();
    	targetID = mapa[targetPosX][targetPosY].getId();

    	cost_so_far[initialID] = 0;

    	BeginDrawing();
            ClearBackground(BLACK);
            
            // Pathfinding

            for(int i = 0; i < tilesY; i++){
		    	for(int j=0; j < tilesX; j++){
		    		if(mapa[i][j].getMovement() > 0){
		    			mapa[i][j].setColorLine(GRAY);
		    			mapa[i][j].setColorShape(RAYWHITE);
		    		}

		    		if(mapa[i][j].getMovement() == 10){
		    			mapa[i][j].setColorShape(LIME);
		    		}		  		
		    	}
		    }

            frontier.push(initialID,0); // Se ingresa como posibilidad al inicio donde buscar
            
            for(int i=0;i<tilesX*tilesY;i++){
            	came_from[i] = -1; // Se inicializa a todos los array a -1
                cost_so_far[i] = 0;
            }
            
            while(frontier.size() != 0){
                
                // Se obtiene el siguiente id del tile a buscar 
            	current = frontier.pop();
                 
                // Se obtiene las coordenadas del tile por medio del ID

                tileAdyacent = Coordinates(current);
                
                if(current == targetID){
                	//break; 
                	pathFound = true;
                	
                	while(frontier.size() != 0){
                		frontier.pop();
                	}
                	
                }else{
	            	// adyacente obtiene los 4 tiles cercanos, del tile actual
	            	adyacent = mapa[(int)tileAdyacent.x][(int)tileAdyacent.y].adyacentID();
	             
		            int size = adyacent.size();

				    for(int i=0; i < size; i++){

				    	// Obtiene el tile del frente, el orden sera:
				    	// derecha, arriba, izquierda, abajo

				        next = adyacent.front();
		                        
		                nextTile = Coordinates(next);


		                newCost = cost_so_far[current] + mapa[(int)nextTile.x][(int)nextTile.y].getMovement();        
		                // Si el tile no se le fue ingresado de donde proviene        
		                if(cost_so_far[next] == 0 || newCost < cost_so_far[next]){ 
		                	
		                	cost_so_far[next] = newCost;

	                        // Si el tile no es un muro o es el punto final, se considera como un lugar para moverse
	                        if(mapa[(int)nextTile.x][(int)nextTile.y].getMovement() != 0 || ((int)nextTile.x == targetPosX && (int)nextTile.y == targetPosY)){

			                	frontier.push(next, newCost); // Se ingresa el siguiente id del tile a buscar y se pone la prioridad
			        
			                	came_from[next] = current; // al tile de los costados se le ingresa el tile de donde proviene

			                	if(nextTile.x != targetPosX || nextTile.y != targetPosY){
			                		mapa[(int)nextTile.x][(int)nextTile.y].setColorLine(ORANGE);
			                	}
								
	                        }                           
		                }	                
	                    
	                    // Se saca el tile cercano que esta al frente de las lista
				        adyacent.pop();					        	                
				    }
				    distance++;
				}
            }

            // Obtengo el camino desde el punto final hasta el inicial

            if(pathFound == true){
            	// Ingreso la posicion final
	            current = targetID;
	           	

	            // Mientras el current no sea la posicion inicial, sigue

	            while(current != initialID){

	            	//cout << current << endl;

	            	// Se ingresa en el camino el siguiente punto a recorrer
	            	path.push(current);       

					// current obtiene el tile del que proviene el tile pintado 
					current = came_from[current];
							
	            }

	            path.push(initialID);

	            //Dibujar camino
	            while(!path.empty()){
	            	current = path.front();

	            	nextTile = Coordinates(current);

					// Mientras no sea el punto final y el punto inicial, no lo pinta completo
					if((nextTile.y != targetPosY || nextTile.x != targetPosX) && (nextTile.x != initialPosX || nextTile.y != initialPosY )){
						mapa[(int)nextTile.x][(int)nextTile.y].setColorShape(BLUE);	
					}else{
						mapa[(int)nextTile.x][(int)nextTile.y].setColorLine(YELLOW);
					}
					
	            	path.pop();
	            }
            }
            

            for(int i = 0; i < tilesY; i++){
    			for(int j=0; j < tilesX; j++){

    				// Arrastrar el punto de inicio
    				if(dragInitial == true && mapa[i][j].getMovement() != 0 && MapMouseOver(mapa, i, j)){

    					
    					// En la posicion inicial, lo deja como estaba
    					mapa[initialPosX][initialPosY].setColorShape(nextTileColor[0]);
    					mapa[initialPosX][initialPosY].setMovement(nextTileMove[0]);

    					//Obtengo sus parametros para devolverselos despues cuando salga
    					nextTileColor[0] = mapa[i][j].getColorShape();
    					nextTileMove[0] = mapa[i][j].getMovement();

    					initialPosX = i; // Obtiene la posicion donde esta actualmente la posicion inicial
    					initialPosY = j;

    					mapa[initialPosX][initialPosY].setColorShape(RED);
    					mapa[initialPosX][initialPosY].setMovement(0);
    				}

    				// Arrastrar el punto de final
    				if(dragTarget == true && mapa[i][j].getMovement() != 0 && MapMouseOver(mapa, i, j)){


    					mapa[targetPosX][targetPosY].setColorShape(nextTileColor[1]);
    					mapa[targetPosX][targetPosY].setMovement(nextTileMove[1]);

    					//Obtengo sus parametros para devolverselos despues cuando salga
    					nextTileColor[1] = mapa[i][j].getColorShape();
    					nextTileMove[1] = mapa[i][j].getMovement();

    					targetPosX = i;
    					targetPosY = j;

    					mapa[targetPosX][targetPosY].setColorShape(GREEN);
    					mapa[targetPosX][targetPosY].setMovement(0);
    				}
 				
    				// Permite arrastar el punto inicial
    				if(initialPosX == i && initialPosY == j && dragInitial == true && dragTarget == false && mapa[initialPosX][initialPosY].released(0) == false){
    					dragInitial = true;
    				}else
    				if(initialPosX == i && initialPosY == j && MapPressedBotonDown(mapa,i,j, 0)){
    					dragInitial = true;
    				}else if(initialPosX == i && initialPosY == j){
    					dragInitial = false;
    				}

    				// Permite arrastar el punto final
    				if(targetPosX == i && targetPosY == j && dragTarget == true && dragInitial == false && mapa[targetPosX][targetPosY].released(0) == false){
    					dragTarget = true;
    				}else
    				if(targetPosX == i && targetPosY == j && MapPressedBotonDown(mapa,i,j, 0)){
    					dragTarget = true;
    				}else if(targetPosX == i && targetPosY == j){
    					dragTarget = false;
    				}

    				if(mapa[i][j].MouseOver() && mapa[i][j-1].MouseOver() == false && mapa[i][j+1].MouseOver() == false
    				&& mapa[i-1][j].MouseOver() == false && mapa[i+1][j].MouseOver() == false){
					
    					id = mapa[i][j].getId();
    					move = mapa[i][j].getMovement(); 
    				}

    				// Muros
    				// Baja el movimiento

    				if(MapPressedBoton(mapa,i,j,0) && (i != initialPosX || j != initialPosY ) && (i != targetPosX || j != targetPosY )){
    					if(mapa[i][j].getMovement() > 0){
    						mapa[i][j].setMovement(mapa[i][j].getMovement()-1);
    					}

    					if(mapa[i][j].getMovement() == 0){
    						mapa[i][j].setColorShape(BROWN); 
    					}
                        
                       
    				}

    				// Incrementa el movimiento
    				if(MapPressedBoton(mapa,i,j,1) && (i != initialPosX || j != initialPosY ) && (i != targetPosX || j != targetPosY )){   					
    					
    					mapa[i][j].setMovement(mapa[i][j].getMovement()+1); 

    					move = mapa[i][j].getMovement(); 	

    					if(mapa[i][j].getMovement() == 10){
    						mapa[i][j].setColorShape(LIME); 
    					}		
    				}
                   
    				
    					mapa[i][j].Draw();
    				
    				
    			}
    		}        

    		DrawText(TextFormat("id: %d", id), 10, 360, 20, VIOLET);

            DrawText(TextFormat("Cost Move: %d", move), 10, 380, 20, VIOLET);
                       
		EndDrawing();
    }

    CloseWindow();

	return 0;
}

// Se obtiene las coordenadas del tile por medio del ID
Vector2 Coordinates(int id){
	Vector2 newPos;
    float posXAdyacent = id/tilesX;
	float posYAdyacent = id;

	while(posYAdyacent >= tilesX ){
		posYAdyacent -= tilesX;
	}

	newPos = {posXAdyacent, posYAdyacent};
	return newPos;
}

bool MapMouseOver(tile (&mapa)[tilesY][tilesX] ,int i, int j){
	bool MouseOver = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

	if( mapa[i][j].MouseOver() ){
        
        // Lado Izquierdo
        if(mapa[i][j-1].MouseOver() == false || j == 0){
    		left = true;
    	}
        
        // Lado Derecho
        if(mapa[i][j+1].MouseOver() == false || j == tilesY-1){
    		right = true;
    	}

        // Lado de arriba
    	if(mapa[i-1][j].MouseOver() == false || i == 0){
    		up = true;
    	}
        
        // Lado de abajo
        if(mapa[i+1][j].MouseOver() == false || i == tilesX-1){
    		down = true;
    	}

    	if(left && right && up && down){
    		MouseOver = true;
    	}
		
	}

	return MouseOver;
}

bool MapPressedBoton(tile (&mapa)[tilesY][tilesX] ,int i, int j, int button){
	bool PressedBoton = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

	if( mapa[i][j].isPressed(button)){
        
        // Lado Izquierdo
        if(mapa[i][j-1].isPressed(button) == false || j == 0){
    		left = true;
    	}
        
        // Lado Derecho
        if(j != tilesY-1){
        	if(mapa[i][j+1].isPressed(button) == false){
        		right = true;
        	}
    		
    	}else{
    		right = true;
    	}

        // Lado de arriba
    	if(i != 0){
    		if(mapa[i-1][j].isPressed(button) == false){
    			up = true;
    		}  		
    	}else{
    		up = true;
    	}
        
        if(i != tilesX-1){
        	if(mapa[i+1][j].isPressed(button) == false){
        		down = true;
        	}
    	}else{
    		down = true;
    	}

    	if(left && right && up && down){
    		PressedBoton = true;
    	}
		
	}

	return PressedBoton;
}

bool MapPressedBotonDown(tile (&mapa)[tilesY][tilesX] ,int i, int j, int button){
	bool PressedBoton = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

	if( mapa[i][j].isPressed(button)){
        
        // Lado Izquierdo
        if(mapa[i][j-1].isPressed(button) == false || j == 0){
    		left = true;
    	}
        
        // Lado Derecho
        if(j != tilesY-1){
        	if(mapa[i][j+1].isPressed(button) == false){
        		right = true;
        	}
    		
    	}else{
    		right = true;
    	}

        // Lado de arriba
    	if(i != 0){
    		if(mapa[i-1][j].isPressed(button) == false){
    			up = true;
    		}  		
    	}else{
    		up = true;
    	}
        
        if(i != tilesX-1){
        	if(mapa[i+1][j].isPressed(button) == false){
        		down = true;
        	}
    	}else{
    		down = true;
    	}

    	if(left && right && up && down){
    		PressedBoton = true;
    	}
		
	}

	return PressedBoton;
}