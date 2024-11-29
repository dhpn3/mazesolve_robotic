#ifndef MAIN_H
#define MAIN_H

/****************************************************************************************************************
 *  BIBLIOTECAS
 ****************************************************************************************************************/

#include "Arduino.h"
#include <Wire.h>

/****************************************************************************************************************
 *  DEFINES E ENUMS
 ****************************************************************************************************************/

/****************************************************************************************************************
 *  TYPEDEFS
 ****************************************************************************************************************/

/**
 *  @brief  Lista de direções operadas pelo robô
 */
typedef enum 
{
    ROBO_PARADO = 0,
    ROBO_VIRA_ESQUERDA,
    ROBO_VIRA_DIREITA,
    ROBO_SEGUE_RETO,
    ROBO_MEIA_VOLTA,
}direcaoRobo_t;

/**
 *  @brief  Lista de velocidades do motor
 */
typedef enum 
{
    BAIXA_VELOCIDADE = 0,
    MEDIA_VELOCIDADE,
    RAPIDA_VELOCIDADE,
}velocidadesRobo_t;

/**
 *  @brief  Lista de caminhos alternativos
 *  @note   Caminho alternativo será analisado no processo do robô andando em linha reta
 */
typedef enum 
{
    NENHUM_LADO_LIVRE = 0,
    LIVRE_LADO_DIREITO,
    LIVRE_LADO_ESQUERDO,
}caminhoAlternativo_t;

typedef struct 
{
    direcaoRobo_t direcaoRobo;
    velocidadesRobo_t velocidadeRobo;
    caminhoAlternativo_t caminhoAlternativo; 
}operacaoRobo_t;

/****************************************************************************************************************
 *  PROTÓTIPOS DE FUNÇÃO
 ****************************************************************************************************************/

#endif //MAIN_H