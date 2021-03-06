#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cmdline.h"

static struct entrada_opcion tabla_opciones[] = {
  {"-?",		2, AYUDA},
  {"-ayuda",		2, AYUDA},
  {"-medida",		3, MEDIDA},
  {"GKOM",		3, GKOM},
  {"LKOM",		3, LKOM},
  {"LKHM",		3, LKHM},
  {"GKHM",		3, GKHM},
  {"LIHE",		2, LIHE},
  {"GIHE",		2, GIHE},
  {"-todas",		2, TODAS},
  {"-maximo",		3, MAX},
  {"-promedio", 	3, PROMEDIO},
  {"-minimo",		3, MIN},
  {"-inferior",		3, INF},
};

static int tamanio_tabla_opciones = sizeof(tabla_opciones) /
                                    sizeof(struct entrada_opcion);


void uso(char *progn)
{
  fprintf(stderr,
    "Uso: %s -medida <MEDIDA> [ Opcion-busqueda ] Fichero [ Fichero ... ]\n", progn);
  exit(1);
}

static int cadena_es_opcion(char *arg,
                            char *arg_valido,
                            int long_min_req)
{
  int long_arg, res;

  res = 0;
  long_arg = strlen(arg);
  if ((strncmp(arg, arg_valido, long_arg) == 0) && (long_arg>=long_min_req))
      res = 1;

  return res;
}


static int codigo_cadena(char *cadena)
{
  int i, res;

  res = -1;
  for (i = 0; i < tamanio_tabla_opciones; ++i) {
  if (cadena_es_opcion(cadena, tabla_opciones[i].opcion, tabla_opciones[i].long_min)) {
      res = tabla_opciones[i].identificador;
      break;
    }
  }
  return res;
}


void imprimir_comando(struct comando *comando)
{

  if ( ((comando->medida != NULL) && (strcmp(comando->medida, "-ayuda") == 0))
       || (comando->medida == NULL)) {
    uso("cmdline");
    return;
  }

  printf("Resumen del comando:\n");
  if (comando->medida)
    printf("Medida:  %s\n", comando->medida);
  if (comando->busqueda) {
    printf("Busqueda: %s\n", comando->busqueda);
    if (strcmp(comando->busqueda, "-inferior") == 0 || strcmp(comando->busqueda, "-superior") == 0)
      printf("Numero:  %.1f\n", comando->numero);
  }
}

void imprimir_ficherosin(int argi,
                         int argc,
                         char **argv)
{
   int i, numero;

   if (argc == argi)
      printf("No se han proporcionado ficheros de entrada\n");
   else
   {
      printf("Numero de ficheros de entrada: %d\n", numero);
      printf("Los ficheros de entrada son:\n");
      for (i = argi ; i < argc; )
        printf("   %s\n", argv[++i]);
   }
}


int procesar_opciones(int argc,
                      char **argv,
                      struct comando *comando)
{
  int i, res = 0, posicion = 0;

  comando->numero = 0.0;
  comando->medida = NULL;
  comando->busqueda = NULL; 

  for (i = 1; i < argc; ++i) {
    switch (codigo_cadena(argv[i])) {

    case AYUDA:
      comando->medida = "-ayuda";


    case MEDIDA:
      if (comando->medida != NULL) {
	fprintf(stderr, "Demasiadas medidas\n");
	res = -1;
      }
      else {
	if (++i >= argc) {
	  fprintf(stderr, "No hay medidas\n");
	  res = -1;
	}
	else {
	  switch (codigo_cadena(argv[i])) {
	  case GKOM: 
	    comando->medida = "GKOM";
	    break;
	  case LKOM:
	    comando->medida = "LKOM";
	    break;
	  case LKHM:
	    comando->medida = "LKHM";
	    break;
	  case GKHM:
	    comando->medida = "GKHM";
	    break;
	  case LIHE:
	    comando->medida = "LIHE";
	    break;
	  case GIHE:
	    comando->medida = "GIHE";
	    break;
	  default: 

	    res = -1;
	    break;
	  }
	} 
      }
      break;

    case TODAS:
      if (comando->busqueda != NULL) {
	fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else
	comando->busqueda = "-todas";
      break;

    case MAX:
      if (comando->busqueda != NULL) {
	fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else
	comando->busqueda = "-maximo";
      break;

    case PROMEDIO:
      if (comando->busqueda != NULL) {
	fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else
	comando->busqueda = "-promedio";
      break;

    case MIN:
      if (comando->busqueda != NULL) {
	fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else
	comando->busqueda = "-minimo";
      break;
    case INF:
      if (comando->busqueda != NULL) {
	fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else {
	comando->busqueda = "-inferior";
	++i;
	if (i >= argc || !isdigit(*argv[i])) {
	  fprintf(stderr, "No se ha proporcionado un numero\n");
	  res = -1;
	}
	else {
	  comando->numero = atof(argv[i]);
	}
      }
      break;

    case SUP:
      if (comando->busqueda != NULL) {
        fprintf(stderr, "Demasiadas opciones de busqueda\n");
	res = -1;
      }
      else {
	comando->busqueda = "-superior";
	++i;
	if (i >= argc || !isdigit(*argv[i])) {
	  fprintf(stderr, "No se ha proporcionado un numero\n");
	  res = -1;
	}
	else {
	  comando->numero = atof(argv[i]);
	}
      }
      break;

    default:
      posicion = i;
      break;
    }

    if ( (res < 0) || (posicion == i) )
       break;
  }

  if ( (res < 0) || (posicion == 0) )
    fprintf(stderr, "Los argumentos de la ocion son incorrectos\n");
  else if ( (posicion == 0) && (strcmp(comando->medida, "-ayuda") != 0) )
       fprintf(stderr, "No se han proporcionado ficheros de entrada\n");
  else
  {
    imprimir_comando(comando);
    imprimir_ficherosin(posicion, argc, argv);
  }
  return res;
}


















/**************************************************************
*************************************************************** 
 * Este es el programa principal.
 * No hay que crear datos de prueba para este, crear abstracciones, etc.
***************************************************************
 **************************************************************/
int main(int argc,
         char **argv)
{
  struct comando a;
  int res;

  if (argc == 1) {
    uso("cmdline");
    res = -1;
  }
  else
    res = procesar_opciones(argc, argv, &a);
    
  return res;
}
