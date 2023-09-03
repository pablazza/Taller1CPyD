#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

using namespace std; 

void leerArchivo();
int infoC(int);
int metaCerca();
void MuestraLaberinto();
int evitaParedesDer(int);
int evitaParedesIzq(int);
void borraRastro(int,int,int);
void mueveJugador(int);
void dimensionaMatriz();
void menu();
bool backtracking(char, int, int);

int **m;             
int f,c;           
int fp,cp; 
int fs,cs;
int ifp,icp; 
const int ARRIBA=0, DERECHA=1, IZQUIERDA=2, ABAJO=3;
int direccion=ARRIBA; 
int movs; 
int espacios; 
bool pasoApaso=false; 
bool vision=true; 
int metodo=1;
int pasosReales; 
char ruta[20]="Laberinto.txt"; 
bool ya;
int fila, columna;

void generarLaberinto(int filas, int columnas, float densidad) {
	int **matriz;
	matriz = new int *[filas];

	int FParedes = densidad * 8; 
	densidad = filas*columnas*densidad / 4;

    ofstream archivo;
    archivo.open("Laberinto.txt", ios::out);

	for (int i = 0; i < filas; i++) {
		matriz[i] = new int[columnas];
	}
	//valores
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			if (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1) {
				matriz[i][j] = 1;
			}
			else {
				matriz[i][j] = 0; 
			}
            if(i==1&&j==columnas -1){
                matriz[1][1+rand()%columnas-1]= 2;
            }
            if(i== filas - 2 && j== columnas - 1){
                matriz[i][1+rand()%columnas]=3;
            }
	    }
    }



	//laberinto 
	srand(time(NULL));
	for (int i = 0; i < densidad; i++) {
		int x = rand() % (columnas - 4) + 2; // 2 18 
		x = (x / 2) * 2; 
		int y = rand()% (filas - 4) + 2;
		y = (y / 2) * 2;
		matriz[y][x] = 1; 
		for (int j = 0; j < FParedes; j++) {
			int mx[4] = { x,  x,  x + 2, x - 2 };
			int my[4] = { y + 2,y - 2, y ,  y };
			int r = rand() % 4;
			if (matriz[my[r]][mx[r]] == 0) {
				matriz[my[r]][mx[r]] = 1; 
				matriz[my[r] +( y - my[r])/2][mx[r]+(x- mx[r])/2] = 1;
			}
				
		}
	}
	//pintar
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			if (matriz[i][j] == 1) {
				cout<< char(219);
			}
			if(matriz[i][j] == 0) {
				cout << "  ";
			}
            archivo<<matriz[i][j];
		}
		cout << "\n"; 
        archivo<<endl;
	}
    archivo.close();
}

bool backtracking(int i, int j){

   if(m[i][j]==3){
       MuestraLaberinto();
       fp=i; cp=j;
      return true;
   }

   
   m[i][j]=4; 
   
   if (pasoApaso) {
      fp=i; cp=j;
      MuestraLaberinto();
      cout<<"Movimientos "<<movs<<"pasos reales "<<pasosReales<<".";
      system("pause");
   }
  
   movs++;
   pasosReales++; 

   
   if(i-1>=0 && i-1<f && (m[i-1][j]==0 || m[i-1][j]==3))  if(backtracking(i-1, j)) return true;    
      
    if(j+1>=0 && j+1<c && (m[i][j+1]==0 || m[i][j+1]==3))  if(backtracking(i, j+1)) return true;     
      
    if(i+1>=0 && i+1<f && (m[i+1][j]==0 ||  m[i+1][j]==3)) if(backtracking(i+1, j)) return true;      
    
    if(j-1>=0 && j-1<c && (m[i][j-1]==0 ||  m[i][j-1]==3)) if(backtracking(i, j-1)) return true;     
      
    m[i][j]=0;
    movs--;
    if (pasoApaso) {
      fp=i; cp=j;
      MuestraLaberinto();
      cout<<"Movimientos "<<movs<<"pasos reales "<<pasosReales<<".";
      pasosReales++;
      system("pause");
   }
    return false;
}


void menu() {
     int opcion;
     
     while (opcion!=1) {
         system("cls");
         cout<<"------------------------  M E N U ------------------------\n\n";
         cout<<"Barra de estado|";
         if (pasoApaso) cout<<"Paso a paso|";
         else           cout<< "...........|";
         if (vision) cout<<"Ver meta|";
         else        cout<<"........|";
         if (metodo==0) cout<<"Mano izquierda|";
         else if  (metodo==1) cout<<"Mano derecha|";
         else cout<<"Backtracking|";
         cout<<"Archivo: "<<ruta<<"|";
         cout<<"\n\n\n";  
         
         cout<<"1) Resolver.\n\n";
         cout<<"2) Metodo de la mano derecha.\n\n";
         cout<<"3) Metodo de la mano izquierda.\n\n";
         cout<<"4) Mostrar paso a paso.\n\n";  
         cout<<"5) Ver meta.\n\n";
         cout<<"6) Metodo de Backtracking.\n\n";
         cout<<"7) Salir.\n\n";
         
         cout<<"Opcion:";
         cin>>opcion;        
         switch (opcion) {
                case 2: metodo=1; break;
                case 3: metodo=0; break;
                case 4:
                     if (pasoApaso) pasoApaso=false;
                     else pasoApaso=true; break;
                case 5: if (vision) vision=false;
                     else vision=true; break;
                case 6: metodo=2; break;
                case 7: 
                     exit(EXIT_SUCCESS); break;
         }
     }                      
}

void dimensionaMatriz() {
     bool flag=true; 
    FILE *archivo;
    char caracter;
    int codigo; 
    
    archivo=fopen(ruta,"rt"); 
    if (archivo==NULL) { 
       cout<<"Error en lectura de archivo\n";
       system("pause");
       exit (EXIT_FAILURE);
    }
    f=1;c=1;
    while(!feof(archivo)) { 
            caracter=fgetc(archivo);
            codigo = caracter; 
            
            if (codigo==10) { 
               f++; 
               if (flag) flag=false; 
            } 
            else if (flag) c++;              
    }
    fclose(archivo); 
    
    
    m = (int **)malloc (f*sizeof(int *));        
    for (int j=0;j<f;j++) m[j] = (int *) malloc (c*sizeof(int));      
}

void leerArchivo() {
    int ff,cc,codigo;
    FILE *archivo;
    char caracter;      
    
    archivo=fopen(ruta,"rt"); 
    if (archivo==NULL) { 
       cout<<"Error en lectura de archivo\n";
       system("pause");
       exit (EXIT_FAILURE);
    }
    
    for (int i=0;i<f-1;i++){
       for (int ii=0;ii<c;ii++) {
           caracter=fgetc(archivo);
           codigo = caracter;               
           if (codigo!=10) {
               m[i][ii]=codigo-48; 
               if (m[i][ii]==2){ifp=i;icp=ii;}
               if (m[i][ii]==3){fs=i;cs=ii;}
               if (m[i][ii]==0) espacios++; 
           }
               
       }
    }        
    fclose(archivo);      
}

int infoC(int dir) {
    switch (dir) {
       case ARRIBA: return m[fp-1][cp];break;
       case ABAJO: return m[fp+1][cp]; break;
       case IZQUIERDA: return m[fp][cp-1]; break;
       case DERECHA: return m[fp][cp+1];break;
    }
    return 0;
}

int infoC(int dir,int dist) {
    switch (dir) {
       case ARRIBA: return m[fp-dist][cp];break;
       case ABAJO: return m[fp+dist][cp]; break;
       case IZQUIERDA: return m[fp][cp-dist]; break;
       case DERECHA: return m[fp][cp+dist];break;
    }
    return 0;
}

int metaCerca() { 
    
       int lejos=1,aux=0,dir=-1;
       
       while (aux!=1 && aux!=3) {
           aux=infoC(ABAJO,lejos);
           if (aux==3) dir=ABAJO;
           lejos++;
       } lejos=1;aux=0;
       if (dir==-1) {
           while (aux!=1 && aux!=3) {
               aux=infoC(DERECHA,lejos);
               if (aux==3) dir=DERECHA;
               lejos++;
           } lejos=1;aux=0;
       }
       if (dir==-1) {
       while (aux!=1 && aux!=3) {
           aux=infoC(ARRIBA,lejos);
           if (aux==3) dir=ARRIBA;
           lejos++;
       } lejos=1;aux=0;
       }
       if (dir==-1) {
          while (aux!=1 && aux!=3) {
               aux=infoC(IZQUIERDA,lejos);
               if (aux==3) {dir=IZQUIERDA;}
               lejos++;
           } lejos=1;
       }
      return dir;  
}

void MuestraLaberinto() {
     
    system("cls");
    for (int i=0;i<f;i++) {
        for (int ii=0;ii<c;ii++) {
           if (m[i][ii]==1) cout<<char(219); 
           else if (i==fs && ii==cs) cout<<"M";
           else if (i==fp && ii==cp) cout<<"J";
           else if (m[i][ii]==4) cout<<"."; 
           else cout<<" ";
        }
    cout<<"\n";
    }
}
int evitaParedesDer(int dir) {
    switch (dir) {
    case ARRIBA: 
        if (infoC(DERECHA)==1 && infoC(ARRIBA)!=1) {}
        else if (infoC(DERECHA)!=1) dir=DERECHA;       
        else if (infoC(IZQUIERDA)!=1) dir=IZQUIERDA;
        else dir=ABAJO;          
    break;    
    case DERECHA:
        if (infoC(ABAJO)==1 && infoC(DERECHA)!=1) {}      
        else if (infoC(ABAJO)!=1) dir=ABAJO;
        else if (infoC(ARRIBA)!=1) dir=ARRIBA;
        else dir=IZQUIERDA;     
    break;                
    case ABAJO:
        if (infoC(IZQUIERDA)==1 && infoC(ABAJO)!=1) {}     
        else if (infoC(IZQUIERDA)!=1) dir=IZQUIERDA;
        else if (infoC(DERECHA)!=1) dir=DERECHA;
        else dir=ARRIBA;
    break;
     case IZQUIERDA:
        if (infoC(ARRIBA)==1 && infoC(IZQUIERDA)!=1) {}           
        else if (infoC(ARRIBA)!=1) dir=ARRIBA;
        else if (infoC(ABAJO)!=1) dir=ABAJO;
        else dir=DERECHA;
    }
    return dir;           
}

int evitaParedesIzq(int dir) {
    switch (dir) {
    case ARRIBA: 
         if (infoC(IZQUIERDA)==1 && infoC(ARRIBA)!=1) {}
        else if (infoC(IZQUIERDA)!=1) dir=IZQUIERDA;
        else if (infoC(DERECHA)!=1) dir=DERECHA;
        else dir=ABAJO;           
    break;    
    case DERECHA:
         if (infoC(ARRIBA)==1 && infoC(DERECHA)!=1) {} 
        else if (infoC(ARRIBA)!=1) dir=ARRIBA;
        else if (infoC(ABAJO)!=1) dir=ABAJO;
        else dir=IZQUIERDA;
    break;                
    case ABAJO:
          if (infoC(DERECHA)==1 && infoC(ABAJO)!=1) {}
        else if (infoC(DERECHA)!=1) dir=DERECHA;
        else if (infoC(IZQUIERDA)!=1) dir=IZQUIERDA;
        else dir=ARRIBA;
    break;
     case IZQUIERDA:
            if (infoC(ABAJO)==1 && infoC(IZQUIERDA)!=1) {}            
        else if (infoC(ABAJO)!=1) dir=ABAJO;
        else if (infoC(ARRIBA)!=1) dir=ARRIBA;
        else dir=DERECHA;
    break;
    }
    return dir;           
}

void Rastro(int dir,int f,int c) {
     switch(dir){ 
        case ARRIBA: if (infoC(ARRIBA)==4) ya=true;
        case DERECHA: if (infoC(DERECHA)==4) ya=true;           
        case IZQUIERDA: if (infoC(IZQUIERDA)==4) ya=true;
        case ABAJO: if (infoC(ABAJO)==4) ya=true;            
     }
     m[f][c]=4;      
}
void mueveJugador(int dir) {
     switch(dir){ 
        case ARRIBA: m[fp-1][cp]=2;             
            fp--;         
        break;            
        case DERECHA: m[fp][cp+1]=2;            
             cp++;            
        break;            
        case IZQUIERDA: m[fp][cp-1]=2;            
            cp--;            
        break; 
        case ABAJO: m[fp+1][cp]=2;           
            fp++;            
        break;            
     }     
}    

void guardarSolucion(int **matriz, int filas, int columnas) {
    ofstream archivo;
    archivo.open("SolucionLaberinto.txt", ios::out);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar la solución." << endl;
        return;
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            
            if (matriz[i][j] == 1) {
                archivo << char(219) << char(219); // Pared
            } else if (matriz[i][j] == 0 || matriz[i][j] == 4) {
                archivo << "  "; // Espacio o camino
            } else if (matriz[i][j] == 2) {
                archivo << "J "; // Jugador
            } else if (matriz[i][j] == 3) {
                archivo << "M "; // Meta
            }
        }
        archivo << "\n";
    }

    archivo.close();
    
}

void primerLaberinto(int **matriz, int filas, int columnas) {
    ofstream archivo;
    archivo.open("LaberintoInicio.txt", ios::out);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar la solución." << endl;
        return;
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            
            if (matriz[i][j] == 1) {
                archivo << char(219) << char(219); // Pared
            } else if (matriz[i][j] == 0 || matriz[i][j] == 4) {
                archivo << "  "; // Espacio o camino
            } else if (matriz[i][j] == 2) {
                archivo << "J "; // Jugador
            } else if (matriz[i][j] == 3) {
                archivo << "M "; // Meta
            }
        }
        archivo << "\n";
    }

    archivo.close();
    
}

int main() { 
    int old_fp,old_cp;  
    int meta=-1; 
    int codAscii;
    char tcl;
    int choice;
          
    cout<<"Filas: ";
    cin>>fila;
    cout<<"Columnas: ";
    cin>>columna;
    generarLaberinto(fila,columna,1);

    while (tcl!=27) {
        system("cls");
        pasosReales=0;
        
        
        dimensionaMatriz(); 
        leerArchivo();
        primerLaberinto(m,fila,columna);
        menu();
        movs=0; fp=ifp;cp=icp;
        while (fp!=fs || cp!=cs) {
            old_fp=fp; old_cp=cp; 

            if (pasoApaso && metodo!=2) {
             MuestraLaberinto();
             cout<<"Movimientos "<<movs<<".";
             system("pause");                    
            }
            
            if (vision) meta=metaCerca(); 
            if (meta!=-1) direccion=meta;  
            else  if (metodo==0) direccion=evitaParedesIzq(direccion);
                else if (metodo==1)direccion=evitaParedesDer(direccion);
                else  {
                      cout<<"Calculando...";
                      backtracking(fp, cp);
                }
            
            if (metodo!=2) {
                mueveJugador(direccion);
                Rastro(direccion,old_fp, old_cp);
                guardarSolucion(m,f,c);
                movs++;
                if (espacios*2-movs<0) break;
            }
            
        }
        MuestraLaberinto();
        cout << "La solución del laberinto se ha guardado en SolucionLaberinto.txt" << endl;
        
        if (fp==fs && cp==cs) {
            if (metodo==0) cout<<" "<<movs<<" movimientos con el metodo de la mano izquierda.\n";
            else if (metodo==1) cout<<" "<<movs<<" movimientos con el metodo de la mano derecha.\n";
            else cout<<" "<<movs<<" movimientos con el metodo de Backtracking.\n";
        }
        else if (metodo!=2) cout<<"El laberinto no tiene solucion.";
        cout<<"Nuevo Laberinto? Si= 1, No=0\n";
        cin>>choice;
        if(choice==1){
            cout<<"Filas: ";
            cin>>fila;
            cout<<"Columnas: ";
            cin>>columna;
            generarLaberinto(fila,columna,1);
        }
        cout<<"Quieres probar otro metodo? <esc> para salir, cualquier otra tecla para continuar";
        tcl = getch();
    }
    return 0;
}