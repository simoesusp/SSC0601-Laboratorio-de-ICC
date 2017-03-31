#include <stdio.h>
#include <stdlib.h>
#include <windows.h>    // Para usar a funcao Sleep(1000)


int main()
{   int hora, minuto, segundo;    //Declara variaveis inteiras: hora, minuto e segundo
    hora=minuto=segundo=0;

    printf("\n\n\n\n\n\n\n\n");     //Imprime linhas em Branco!
    
    //Inicio do Loop
while(1)
    {
    printf("\r       Hora: %2dh %2dm %2ds", hora, minuto, segundo); 	//%2d  ->  Imprime o inteiro com 2 casa decimais!    
									//\r retorna o cursor para o inicio da linha
    segundo++;                              //Incrementa o segundo!
    if(segundo==60) {segundo=0; minuto++;}  //Se segundo = 60, zera segundo e incrementa minuto
    if(minuto==60) {minuto=0; hora++;}      //Se minuto = 60, zera minuto e incrementa hora
    if(hora==24) hora=0;                    //Se hora = 24, zera hora!

    Sleep(1000);            //Dorme por 1 segundo!

    if (kbhit())            //Se alguma tecla for pressionada!
   		{
		int  codigo_tecla = getch();    //Le codigo da tecla!
         	if (codigo_tecla=='f') break;   //Se codigo da tecla for "f", interrompe o While!
                }   //Fim do if!

    }   //Fim do while  ou  Loop!

    //Fim do Loop!;
    return 0;   //Retorna 0 para dizer que deu tudo certo!
}   //Final do Main!
