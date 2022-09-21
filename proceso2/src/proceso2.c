#include "proceso2.h"


int main(void) {

	t_log* logger  = log_create("./cfg/proceso1.log", "PROCESO2", true, LOG_LEVEL_INFO);

	int server_fd = iniciar_servidor(logger,"127.0.0.1","8000");
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(logger,server_fd);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {


		case PAQUETE:
			lista = recibir_paquete(logger, cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	printf("%s\n", value);
}



int iniciar_servidor( t_log* logger, char* ip, char* puerto)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	// assert(!"no implementado!");

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
				servinfo->ai_socktype,
				servinfo->ai_protocol);
	// Asociamos el socket a un puerto
	bind(socket_servidor,servinfo->ai_addr,servinfo->ai_addrlen);
	// Escuchamos las conexiones entrantes
	listen(socket_servidor,SOMAXCONN);
	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(   t_log* logger, int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}




t_list* recibir_paquete(t_log* logger, int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	t_list* valores2 = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	// en size tengo el tamaño total del buffer

	int i, cantidad;

	desplazamiento += deserialize_int(&cantidad, buffer);

	for (i = 0; i < cantidad; i++) {

		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));// copio el primer numero del buffer(q son la cantidad de caracteres) en TAMANIO
		desplazamiento+=sizeof(int);                    // le sumo el tamaño de ese entero
		
		
		char* valor = malloc(tamanio);          // reservo memoria 
		memcpy(valor, buffer+desplazamiento, tamanio); // copio en valor 
		desplazamiento+=tamanio;

    printf("El valor es %s.\n", valor);

		list_add(valores, valor);

	}


	desplazamiento += deserialize_int(&cantidad, buffer + desplazamiento);


	for (i = 0; i < cantidad; i++) {

		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));// copio el primer numero del buffer(q son la cantidad de caracteres) en TAMANIO
		desplazamiento+=sizeof(int);                    // le sumo el tamaño de ese entero
		
		
		char* valor = malloc(tamanio);          // reservo memoria 
		memcpy(valor, buffer+desplazamiento, tamanio); // copio en valor 
		desplazamiento+=tamanio;


		list_add(valores2, valor);


	}

	free(buffer);

    //log_info(logger, "Me llegaron los siguientes valoresssssss:\n");
	//list_iterate(valores, (void*) iterator);

	//log_info(logger, "Me llegaron los siguientes valoressss:\n");
	//list_iterate(valores2, (void*) iterator);

	return valores2;
}


int deserialize_int(int* output, char* input) {
	int size = sizeof(int);
	memcpy(output, input, size);
	return size;
}

