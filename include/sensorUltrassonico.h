#ifndef SENSOR_ULTRASSONICO_H
#define SENSOR_ULTRASSONICO_H

/****************************************************************************************************************
 *  BIBLIOTECAS
 ****************************************************************************************************************/

#include <Ultrasonic.h>

#include "main.h"

/****************************************************************************************************************
 *  DEFINES E ENUMS
 ****************************************************************************************************************/

/****************************************************************************************************************
 *  TYPEDEFS
 ****************************************************************************************************************/



/**
 *  @brief  Lista dos estados da máquina de estados no qual analisa os sensores ultrassônicos
 */
typedef enum 
{
    EST0 = 0,
    EST1,
    EST2,
    EST3,
    EST4,
}estadosMovimentoRobo_t;

/****************************************************************************************************************
 *  PROTÓTIPOS DE FUNÇÃO
 ****************************************************************************************************************/

void obtem_direcao_robo(operacaoRobo_t * operacaoRobo);
int distancia_sensor_frontal(void);
int distancia_sensor_direito(void);
int distancia_sensor_esquerdo(void);

#endif //SENSOR_ULTRASSONICO_H