#include "proceso1.h"



t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("./cfg/proceso1.log", "CONSOLA", 1, LOG_LEVEL_INFO);

    	if (nuevo_logger == NULL) {
		printf("ERROR: No se pudo crear el Logger\n");
		exit(1);
	}

	return nuevo_logger;
}



t_config* iniciar_config(char *path_config, t_log* logger)
{
	t_config* nuevo_config = config_create(path_config);

    if (nuevo_config  == NULL){
		log_error(logger, "No se encontro el archivo en el path especificado\n");
		exit(2);
	}
 
 log_info(logger, "Se creo la estructura del config.");

	return nuevo_config;
}



void llenar_lista_de_instrucciones(FILE *archivo,t_list* valores){

 char cadena[25];
 char* cadena2;

     while (!feof(archivo)){
		  fgets(cadena, sizeof(cadena), archivo);

          cadena[strlen(cadena) - 1] = '\0';

          cadena2 = (char *) malloc (strlen (cadena) + 1);
          strcpy (cadena2,cadena);

		  if (cadena2 != NULL) { list_add(valores, cadena2); }
    }

    free(cadena2);
    fclose(archivo);

}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}


t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void llenar_Paquete(t_list* lista,t_paquete* paquete){

agregar_a_paquete2(paquete,list_size(lista));

int j =0;
       
	    while(j < list_size(lista)){

            agregar_a_paquete(paquete,list_get(lista,j),strlen(list_get(lista,j)) + 1); 

			j++; }

}

void agregar_a_paquete2(t_paquete* paquete,int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	paquete->buffer->size += sizeof(int);
}


void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}




void cargar_lista_de_segmentos(t_list* lista_Segmentos,char** segmentos) {

    int j =0;

	    while(j < 4){
            list_add(lista_Segmentos, segmentos[j]);
			j++; }

    }


int crear_conexion(t_log* logger, char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
			            server_info->ai_socktype,
				    server_info->ai_protocol);

    if(socket_cliente == -1) {
        log_error(logger, "No se pudo crear el socket.");
      freeaddrinfo(server_info);
        exit(3);
    }

	// Ahora que tenemos el socket, vamos a conectarlo
	
    if(connect(socket_cliente,server_info->ai_addr,server_info->ai_addrlen) == -1) {
           log_error(logger, "No se pudo conectar con el modulo Kernel.");
        freeaddrinfo(server_info);
        exit(4);
    } 

	freeaddrinfo(server_info);
     log_info(logger, "Se conecto con el modulo Kernel.");

	return socket_cliente;
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}



void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);

	config_destroy(config);

	liberar_conexion(conexion);
}


