/*
	Algoritmo evolutivo para uma matriz de cores aleatória

	Comando para instalar a OpenGL no Ubuntu: 
			sudo apt-get install freeglut3-dev 
	Comando para compilar o código (sendo "codeName.c" o nome do código e "executableName" o nome desejado para o executável): 
			g++ codeName.c -lGL -lGLU -lglut -lm -o executableName
	Rodar o executável:
			./executableName

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>

//VARIÁVEIS GLOBAIS
int n_bestColors = 50; //Número de melhores pixeis selecionados por chamada da função avalueColors()
int n_mutations = 3; //Número de pixeis "mutantes"
float neighborC = 0.60; //Taxa de crescimento da cor selecionada
float neighborM = 0.21; //Taxa de crescimento da "cor mutante"


static int win(0); //Função da OpenGL

void orderVector(int vector[][3], int indice); //Função acessória para ordenar vetores

//FUNÇÕES DO ALGORITMO EVOLUTIVO
void beginColors(float color[][500][3]); //Função que preenche a matriz de cores com valores aleatórios

void avalueColors(float color[][500][3], int points[][3], float PerfectRed, float PerfectGreen, float PerfectBlue); //Cada pixel ganhará um pontuação de acordo com o quanto ele está perto da cor desejada pelo usuário e então os melhores são guardados no vetor points

void crossoverColors(float color[][500][3], int points[][3]); //Os melhores "cruzam" com seus vizinhos

void mutationColors(float color[][500][3], float red, float green, float blue); //Mutações de uma cor aleatória fixa surgem na matriz e também "cruzam" com os vizinhos



int main(int argc, char* argv[])
{
	time_t t; srand((unsigned) time(&t)); //Renova a semente da função rand() para evitar repetição dos números sorteados
	float Color[500][500][3]; //Matriz que contem os parâmetros RGB de cada pixel da imagem
	int Points[n_bestColors][3]; //Matriz que guarda os melhores pixeis (suas pontuações e suas coordenadas (i,j))
	float Mutation[n_mutations][3]; //Matriz que guarda os "mutantes" (seus valores RGB)

	//Funções e procedimentos da OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500,500);
	glutCreateWindow("Color Dissemination");
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0.0, 500.0, 500.0, 0.0);

	
	beginColors(Color);
	
	//Variáveis para selecionar uma mutação aleatória
	int i_mutation = rand()%500;
	int j_mutation = rand()%500;
	float redM = Color[i_mutation][j_mutation][0];
	float greenM = Color[i_mutation][j_mutation][1];
	float blueM = Color[i_mutation][j_mutation][2];

	int contador, i, j;
	for(contador=0; contador<500; contador++)
	{
		avalueColors(Color, Points, 0.8, 0.1, 0.4);
		
		crossoverColors(Color, Points);

		mutationColors(Color, redM, greenM, blueM);

		//Laço para imprimir as cores
		for(i=0;i<500;i++)
		{
			for(j=0;j<500;j++)
			{
				// Função da OpenGL para colorir um pixel da janela criada
				glBegin(GL_POINTS);
					glColor3f(Color[i][j][0],Color[i][j][1],Color[i][j][2]); // R, G e B desejado
					glVertex2i(i,j); // local a ser impresso na janela (coordenada)
				glEnd();
			}
		}

		//sleep(1);
		glFlush();
	}

	glutMainLoop();	
}






void orderVector(int vector[][3], int indice)
{
	int n, m; 
	int aux, i_aux, j_aux;
	int contador=0;

    for( n=0; (n<indice) && (contador==0) ; n++ )
	{
		for( m=0; m<indice; m++ ) if( vector[n][0] <= vector[m][0] ) contador++;
			
		if(contador>=indice)
		{
			aux = vector[0][0];
			i_aux = vector[0][1];
			j_aux = vector[0][2];

			vector[0][0]=vector[n][0];
			vector[0][1]=vector[n][1];
			vector[0][2]=vector[n][2];

			vector[n][0]=aux;
			vector[n][1]=i_aux;
			vector[n][2]=j_aux;
		}
		else contador=0;
	}
}




void beginColors(float color[][500][3])
{
	int i, j; //Índices da matriz de pixeis(determina a coordenada dos pixeis)
	int k; //Índice da dimensão que contém as características de cada pixel (R=Red, G=green, B=blue)

	for(i=0;i<500;i++)
	{
		for(j=0;j<500;j++)
		{
			for(k=0;k<3;k++) color[i][j][k]=(float)((rand()%1000)/1000.0);

			glBegin(GL_POINTS);
				glColor3f(color[i][j][0],color[i][j][1],color[i][j][2]);
				glVertex2i(i,j);
			glEnd();
		}
	}
}


void avalueColors(float color[][500][3], int points[][3], float PerfectRed, float PerfectGreen, float PerfectBlue)
{
	int n;
	for(n=0;n<n_bestColors;n++) points[n][0]=0; //zera a pontuação de todos os 20 para evitar que lixo seja comparado
	
	int i, j;
	float Pr, Pg, Pb;
	int P;

	for(i=0;i<500;i++)
	{
		for(j=0;j<500;j++)
		{
			if( color[i][j][0] <= PerfectRed )  Pr=(color[i][j][0]/PerfectRed);
			else Pr=( (1-color[i][j][0])/(1-PerfectRed));

			if( color[i][j][1] <= PerfectGreen )  Pr=(color[i][j][1]/PerfectGreen);
			else Pg=( (1-color[i][j][1])/(1-PerfectGreen));

			if( color[i][j][2] <= PerfectBlue )  Pr=(color[i][j][2]/PerfectBlue);
			else Pb=( (1-color[i][j][2])/(1-PerfectBlue));

			P = (int) ((Pr+Pg+Pb)*1000);

			for(n=0; n<n_bestColors ;n++)
			{
				if(P>points[n][0])
				{
					points[n][0]=P;
					points[n][1]=i;
					points[n][2]=j;
					break;
				}
			}
			
			orderVector(points,n_bestColors);
		}
	}
}


void crossoverColors(float color[][500][3], int points[][3])
{
	int n, p, q, k;
	for(n=0; n<n_bestColors; n++) // Seleciona um dos melhores pixeis guardados
	{
		for(p=-neighborC;p<=neighborC;p++) 
		{
			for(q=-neighborC;q<=neighborC;q++)
			{
				if( ((points[n][1]+p)>=0) && ((points[n][1]+p)<500) && ((points[n][2]+q)>=0) && ((points[n][2]+q)<500) && ( pow(p,2) + pow(q,2) <= pow(neighborC,2) ) )
				{
					for(k=0;(k<3);k++)  
						color[points[n][1]+p][points[n][2]+q][k]= ((3*(color[points[n][1]+p][points[n][2]+q][k])+color[points[n][1]][points[n][2]][k])/4) ;
				}
			}
		} 
	}
	neighborC=neighborC+0.40;
}


void mutationColors(float color[][500][3], float red, float green, float blue)
{
	int c, p, q, k;
	for(c=0; c<n_mutations;c++)
	{
		int i_aleatory = rand()%500;
		int j_aleatory = rand()%500;

		color[i_aleatory][j_aleatory][0]=red;
		color[i_aleatory][j_aleatory][1]=green;
		color[i_aleatory][j_aleatory][2]=blue;

		for(p=-neighborM; p<=neighborM; p++) 
			{
				for(q=-neighborM;q<=neighborM;q++)
				{
					if(((i_aleatory+p)>=0) && ((i_aleatory+p)<500) && ((j_aleatory+q)>=0) && ((j_aleatory+q)<500) && ( pow(p,2) + pow(q,2) <= pow(neighborM,2) ))
					{
						for(k=0;(k<3);k++)  
							color[i_aleatory+p][j_aleatory+q][k]= ((color[i_aleatory+p][j_aleatory+q][k]+color[i_aleatory][j_aleatory][k])/2) ;
					}
				}
			} 
	}

	neighborM=neighborM+0.2;
}



