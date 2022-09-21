#include "proceso1.h"
//hay que pasar los parametros de la siguiente manera ./consola bytes cfg/instrucciones.txt




int main(int argc, char** argv){

/*
	if(argc < 3) {
		printf("ERROR: Se esperan 3 argumentos\n");
		return EXIT_FAILURE;
	}

*/

	t_log* logger = iniciar_logger();

    char *path_config = "./cfg/cliente.config";
    //char *path_config = argv[1];

	t_config* config = iniciar_config(path_config,logger);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip_kernel', 'puerto_kernel' y 'segmentos'

	//char *ip_kernel = config_get_string_value(config,"IP_KERNEL");
	char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
	char **segmentos = config_get_array_value(config,"SEGMENTOS");

    // char *path_pseudocodigo = argv[2];
    char *path_pseudocodigo = "./cfg/prueba.txt";


    FILE *archivo = fopen(path_pseudocodigo,"r");
    if(archivo==NULL){
        log_error(logger, "El archivo no se pudo abrir.");
        return EXIT_FAILURE;
    }


    t_list* valores = list_create();

    llenar_lista_de_instrucciones(archivo,valores);

	log_info(logger,"Se cargo la lista de instrucciones.");

    t_paquete* paquete = crear_paquete();

    llenar_Paquete(valores,paquete);

    t_list* lista_Segmentos = list_create();

    cargar_lista_de_segmentos(lista_Segmentos,segmentos);

	log_info(logger,"Se cargo la lista de segmentos.");

    llenar_Paquete(lista_Segmentos,paquete);


	// Creamos una conexión hacia el modulo Kernel
	int conexion = crear_conexion(logger,"127.0.0.1", puerto_kernel);

    enviar_paquete(paquete,conexion);

    log_info(logger, "Esperando mensaje del Kernel.");

/*
	
FALTA IMPLEMENTAR ESTO

una vez recibida la confirmación de recepción la consola quedará a la espera de nuevos mensajes del Kernel los cuales pueden ser:

*Imprimir un valor por pantalla.
*Solicitar que se ingrese un valor por teclado (siempre serán valores numéricos).



*/










	list_destroy(valores);
	list_destroy(lista_Segmentos);
	eliminar_paquete(paquete);

	terminar_programa(conexion, logger, config);



}
















/*
typedef struct Proceso {
    uint32_t tamProceso;
    char *instrucciones;
} Proceso;

int main(int argc, char** argv){


         if (argc < 3) {
               printf("ERROR: Tenes que poner 3 argumentos.");
               
               return EXIT_FAILURE; 
         }

         char *ip_kernel, *puerto_kernel;
         int conexion;
         t_log *logger;
         Proceso *proceso = malloc(sizeof(Proceso));

         char *path = (char *) malloc(strlen(argv[1]) + 1);
         strcpy(path, argv[1]);

         char *path2 = (char *) malloc(strlen(argv[2]) + 1);
         strcpy(path, argv[2]);

         char *instrucciones = archivo_leer(path);
         free(path);
         free(path2);




}



        //t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
        //log_info(logger, "Soy el proceso 1! %s", mi_funcion_compartida());
        //log_destroy(logger);




#include <stdio.h>
#include <string.h>
int main() {
    
    char* c[]={"1","2","3","4","5","6"};

    int length = sizeof(c)/sizeof(c[0]);
    
    //int numero=0;
    //while (*(c+numero)!= NULL) {

      // printf("%d\n",numero);
      //  numero++; }
    
    printf("total %d",length);
    return 0;
}

        //printf("%s\n",c[numero]);


*/